/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	プレイヤー
	//--------------------------------------------------------------------------------------
	//構築
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(0.25f)
	{}

	Vec3 Player::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//動いた時のスピード
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = OnGetDrawCamera();
				//進行方向の向きを計算
				Vec3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.normalize();
				//進行方向向きからの角度を算出
				float FrontAngle = atan2(Front.z, Front.x);
				//コントローラの向き計算
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = MoveVec.length();
				//コントローラの向きから角度を計算
				float CntlAngle = atan2(-MoveX, MoveZ);
				//トータルの角度を算出
				float TotalAngle = FrontAngle + CntlAngle;
				//角度からベクトルを作成
				Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
				//正規化する
				Angle.normalize();
				//移動サイズを設定。
				Angle *= MoveSize;
				//Y軸は変化させない
				Angle.y = 0;
			}
		}
		return Angle;
	}


	//初期化
	void Player::OnCreate() {
		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(Vec3(m_Scale));	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		auto bkCamera = App::GetApp()->GetScene<Scene>()->GetBackupCamera();
		Vec3 FirstPos;
		if (!bkCamera) {
			FirstPos = Vec3(0, m_Scale * 0.5f, 0);
		}
		else {
			FirstPos = App::GetApp()->GetScene<Scene>()->GetBackupPlayerPos();
		}
		Ptr->SetPosition(FirstPos);

		PsSphereParam param;
		//basecrossのスケーリングは直径基準なので、半径基準にする
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		//プレイヤーなのでスリープしない
		param.m_UseSleep = false;
		param.m_Quat.identity();
		param.m_Pos = FirstPos;
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
		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.ButtonHandle(GetThis<Player>());

		auto Vec = GetMoveVector();
		auto PtrPs = GetComponent<PsSphereBody>();
		auto Velo = PtrPs->GetLinearVelocity();
		Velo.x = Vec.x * 5.0f;
		Velo.z = Vec.z * 5.0f;
		PtrPs->SetLinearVelocity(Velo);
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

	//Bボタンハンドラ
	void  Player::OnPushB() {
		//ゲームステージ再読み込み
		App::GetApp()->GetScene<Scene>()->SetBackupCamera(dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera()));
		App::GetApp()->GetScene<Scene>()->SetBackupPlayerPos(GetComponent<Transform>()->GetPosition());
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
	}

	//Xボタンハンドラ
	void Player::OnPushX() {
		auto Ptr = GetComponent<Transform>();
		Vec3 Pos = Ptr->GetPosition();
		Pos.y += 0.5f;
		Quat Qt = Ptr->GetQuaternion();
		Vec3 Rot = Qt.toRotVec();
		float RotY = Rot.y;
		Vec3 velo(sin(RotY), 0.4f, cos(RotY));
		velo.normalize();
		velo *= 15.0f;

		auto ShPtr = GetStage()->GetSharedGameObject<FireSphere>(L"FireSphere", false);
		if (ShPtr) {
			ShPtr->Reset(Pos, velo);
		}
		else {
			GetStage()->AddGameObject<FireSphere>(Pos, velo);
		}
	}

	//Yボタンハンドラ(押した瞬間)
	void Player::OnPushY() {
		//ホールドしたオブジェクトがなければ何もしない
		auto HoldPtr = m_HoldObject.lock();
		if (!HoldPtr) {
			return;
		}
		auto ActionLinePtr = m_ActionLine.lock();
		if (ActionLinePtr) {
			auto Check = ActionLinePtr->GetEndObj();
			auto CheckHold = dynamic_pointer_cast<GameObject>(HoldPtr);
			if (Check != CheckHold) {
				ActionLinePtr->SetEndObj(HoldPtr);
			}
			ActionLinePtr->SetDrawActive(true);
		}
		else {
			//ラインの作成
			auto LinePtr = GetStage()->AddGameObject<ActionLine>(GetThis<GameObject>(), HoldPtr);
			LinePtr->SetDrawActive(true);
			m_ActionLine = LinePtr;
		}
	}


	//Yボタンハンドラ(押し続け)
	void Player::OnPressY() {
		auto Ptr = GetComponent<Transform>();
		auto PlayerPos = Ptr->GetPosition();
		auto HoldPtr = m_HoldObject.lock();
		if (HoldPtr) {
			auto PsPtr = HoldPtr->GetDynamicComponent<PsBodyComponent>(false);
			if (PsPtr) {
				auto PsPos = PsPtr->GetPosition();
				float ToY = 2.0f;
				if (PsPos.y > 5.0f) {
					ToY = 0.0f;
				}
				PsPos.y = 0;
				PlayerPos.y = 0;
				Vec3 ToPlayerVec = PlayerPos - PsPos;
				PsPtr->WakeUp();
				PsPtr->SetLinearVelocity(Vec3(ToPlayerVec.x, ToY, ToPlayerVec.z));
			}
		}
	}

	//Yボタンハンドラ(離した瞬間)
	void Player::OnReleaseY() {
		auto ActionLinePtr = m_ActionLine.lock();
		if (ActionLinePtr) {
			ActionLinePtr->SetDrawActive(false);
		}
	}

	//文字列の表示
	void Player::DrawStrings() {
		//文字列表示
		wstring Mess(L"Bボタンで再読み込み\nXボタンで発射\n");
		//オブジェクト数
		auto ObjCount = GetStage()->GetGameObjectVec().size();
		wstring OBJ_COUNT(L"OBJ_COUNT: ");
		OBJ_COUNT += Util::UintToWStr(ObjCount);
		OBJ_COUNT += L"\n";
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

		wstring str = Mess + OBJ_COUNT + FPS + PositionStr;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}

}
//end basecross

