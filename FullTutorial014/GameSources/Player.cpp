/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	//構築
	Player::Player(const shared_ptr<Stage>& StagePtr, const Vec3& StatPos) :
		GameObject(StagePtr),
		m_StartPos(StatPos),
		m_Scale(0.25f)
	{}

	Vec3 Player::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		//キーボードの取得
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		float MoveLength = 0;	//動いた時のスピード
		auto PtrTransform = GetComponent<Transform>();
		auto PtrCamera = OnGetDrawCamera();
		//進行方向の向きを計算
		Vec3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
		Front.y = 0;
		Front.normalize();
		//進行方向向きからの角度を算出
		float FrontAngle = atan2(Front.z, Front.x);
		float MoveX = 0.0f;
		float MoveZ = 0.0f;
		if (KeyState.m_bPushKeyTbl['W']) {
			//前
			MoveZ = 1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['A']) {
			//左
			MoveX = -1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['S']) {
			//後ろ
			MoveZ = -1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['D']) {
			//右
			MoveX = 1.0f;
		}
		if (MoveX == 0.0f && MoveZ == 0.0f) {
			return Angle;
		}

		float KeyAngle = atan2(-MoveX, MoveZ);
		//トータルの角度を算出
		float TotalAngle = FrontAngle + KeyAngle;
		//角度からベクトルを作成
		Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
		//正規化する
		Angle.normalize();
		//Y軸は変化させない
		Angle.y = 0;
		return Angle;
	}


	//初期化
	void Player::OnCreate() {
		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(Vec3(m_Scale));	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//衝突判定をつける
		auto PtrCol = AddComponent<CollisionSphere>();
		//判定するだけなのでアクションはNone
		PtrCol->SetIsHitAction(IsHitAction::None);


		PsSphereParam param;
		//basecrossのスケーリングは直径基準なので、半径基準にする
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		//プレイヤーなのでスリープしない
		param.m_UseSleep = false;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = m_StartPos;
		param.m_LinearVelocity = Vec3(0);
		auto PsPtr = AddComponent<PsSphereBody>(param);
		PsPtr->SetAutoTransform(false);
		PsPtr->SetDrawActive(true);

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"TRACE_TX");

		//透明処理
		SetAlphaActive(true);
		//カメラを得る
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(OnGetDrawCamera());
		if (PtrCamera) {
			//LookAtCameraである
			//LookAtCameraに注目するオブジェクト（プレイヤー）の設定
			PtrCamera->SetTargetObject(GetThis<GameObject>());
			PtrCamera->SetTargetToAt(Vec3(0, 0.25f, 0));
		}
	}

	//更新
	void Player::OnUpdate() {

		auto Vec = GetMoveVector();
		auto PtrPs = GetComponent<PsSphereBody>();
		auto Velo = PtrPs->GetLinearVelocity();
		Velo.x = Vec.x * 5.0f;
		Velo.z = Vec.z * 5.0f;
		PtrPs->SetLinearVelocity(Velo);

		//キーボードの取得
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPressedKeyTbl['X']) {
			OnPushX();
		}
		else if (KeyState.m_bPressedKeyTbl['Z']) {
			OnPushA();
		}
	}

	//後更新
	void Player::OnUpdate2() {
		auto PtrPs = GetComponent<PsSphereBody>();
		auto Ptr = GetComponent<Transform>();
		Ptr->SetPosition(PtrPs->GetPosition());
		//回転の計算
		Vec3 Angle = GetMoveVector();
		if (Angle.length() > 0.0f) {
			auto UtilPtr = GetBehavior<UtilBehavior>();
			//補間処理を行わない回転。補間処理するには以下1.0を0.1などにする
			UtilPtr->RotToHead(Angle, 1.0f);
		}
		//文字列の表示
		DrawStrings();
	}

	//Aボタンハンドラ
	void  Player::OnPushA() {
		auto Ptr = GetComponent<Transform>();
		if (Ptr->GetPosition().y > 0.125f) {
			return;
		}
		auto PtrPs = GetComponent<PsSphereBody>();
		auto Velo = PtrPs->GetLinearVelocity();
		Velo += Vec3(0, 4.0f, 0.0);
		PtrPs->SetLinearVelocity(Velo);
	}


	//Xボタンハンドラ
	void Player::OnPushX() {
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Start(L"cursor");

		auto Ptr = GetComponent<Transform>();
		Vec3 Pos = Ptr->GetPosition();
		Pos.y += 0.3f;
		Quat Qt = Ptr->GetQuaternion();
		Vec3 Rot = Qt.toRotVec();
		float RotY = Rot.y;
		Vec3 velo(sin(RotY), 0.05f, cos(RotY));
		velo.normalize();
		velo *= 20.0f;
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
		for (size_t i = 0; i < Group->size(); i++) {
			auto shptr = dynamic_pointer_cast<ShellSphere>(Group->at(i));
			if (shptr && !shptr->IsUpdateActive()) {
				//空きが見つかった
				shptr->Reset(Pos, velo);
				return;
			}
		}
		//ここまで来てれば空きがない
		GetStage()->AddGameObject<ShellSphere>(Pos, velo);
	}


	//文字列の表示
	void Player::DrawStrings() {

		//文字列表示
		wstring Mess(L"Aでジャンプ、Xボタンで発射、Bでステージ切替\n");
		//オブジェクト数
		auto ObjCount = GetStage()->GetGameObjectVec().size();
		wstring OBJ_COUNT(L"OBJ_COUNT: ");
		OBJ_COUNT += Util::UintToWStr(ObjCount);
		OBJ_COUNT += L"\n";
		//物理オブジェクト数
		wstring PS_OBJ_COUNT(L"PS_OBJ_COUNT: ");
		PS_OBJ_COUNT += Util::UintToWStr(GetStage()->GetBasePhysics().GetNumBodies());
		PS_OBJ_COUNT += L"\n";
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		FPS += Util::FloatToWStr(ElapsedTime);
		FPS += L"\n";

		auto Pos = GetComponent<Transform>()->GetPosition();
		wstring PositionStr(L"Position:\t");
		PositionStr += L"X=" + Util::FloatToWStr(Pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Y=" + Util::FloatToWStr(Pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Z=" + Util::FloatToWStr(Pos.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring MousePosStr(L"MousePos:\t");
		auto MousePos = GetTypeStage<GameStage>()->GetMousePoint();
		MousePosStr += L"X=" + Util::IntToWStr(MousePos.x) + L",\t";
		MousePosStr += L"Y=" + Util::IntToWStr(MousePos.y) + L"\n";

		Vec3 Near; Vec3 Far;
		GetTypeStage<GameStage>()->GetMouseRay(Near, Far);

		wstring MouseRayNearStr(L"MouseRayNear:\t");
		MouseRayNearStr += L"X=" + Util::FloatToWStr(Near.x, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayNearStr += L"Y=" + Util::FloatToWStr(Near.y, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayNearStr += L"Z=" + Util::FloatToWStr(Near.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring MouseRayFarStr(L"MouseRayFar:\t");
		MouseRayFarStr += L"X=" + Util::FloatToWStr(Far.x, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayFarStr += L"Y=" + Util::FloatToWStr(Far.y, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayFarStr += L"Z=" + Util::FloatToWStr(Far.z, 6, Util::FloatModify::Fixed) + L"\n";



		wstring str = Mess + OBJ_COUNT + PS_OBJ_COUNT + FPS + PositionStr + MousePosStr + MouseRayNearStr + MouseRayFarStr;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}

}
//end basecross

