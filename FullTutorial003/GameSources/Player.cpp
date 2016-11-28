/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class AttackBall : public GameObject;
	//	用途: 飛んでいくボール
	//--------------------------------------------------------------------------------------
	//構築と破棄
	AttackBall::AttackBall(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr) {}

	AttackBall::~AttackBall() {}

	void AttackBall::Weakup(const Vector3& Position, const Vector3& Velocity) {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->ResetPosition(Position);
		auto PtrRedid = GetComponent<Rigidbody>();
		PtrRedid->SetVelocity(Velocity);
		SetDrawActive(true);
		SetUpdateActive(true);
	}


	//初期化
	void AttackBall::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(0, 0, 0);

		//Rigidbodyをつける
		auto PtrRedid = AddComponent<Rigidbody>();


		//衝突判定をつける
		auto PtrCol = AddComponent<CollisionSphere>();
		PtrCol->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		auto Group = GetStage()->GetSharedObjectGroup(L"AttackBall");
		Group->IntoGroup(GetThis<AttackBall>());

		//最初は無効にしておく
		SetDrawActive(false);
		SetUpdateActive(false);

	}

	void AttackBall::OnUpdate() {
		Rect2D<float> rect(-25.0f, -25.0f, 25.0f, 25.0f);
		Point2D<float> point;
		auto PtrTransform = GetComponent<Transform>();
		point.x = PtrTransform->GetPosition().x;
		point.y = PtrTransform->GetPosition().z;
		if (!rect.PtInRect(point) || abs(PtrTransform->GetPosition().y) > 10.0f) {
			PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
			PtrTransform->SetRotation(0, 0, 0);
			PtrTransform->SetPosition(0, 0, 0);
			SetDrawActive(false);
			SetUpdateActive(false);
		}
	}




	//--------------------------------------------------------------------------------------
	/// アクションコマンド（Aボタン）
	//--------------------------------------------------------------------------------------
	void ActionCommand::Excute(const shared_ptr<Player>& Obj) {
		//アクションスタート
		if (Obj->GetStateMachine()->GetTopState() == PlayerDefault::Instance()) {
			Obj->GetStateMachine()->Push(PlayerAction::Instance());
		}
	}

	//--------------------------------------------------------------------------------------
	///	行動切り替えコマンド
	//--------------------------------------------------------------------------------------
	void BehaviorChangeCommand::Excute(const shared_ptr<Player>& Obj) {
		//行動切り替え
		Obj->ChangePlayerBehavior();
	}



	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_MaxSpeed(30.0f),	//最高速度
		m_Decel(0.95f),	//減速値
		m_Mass(1.0f)	//質量
	{}

	//行動の切り替え
	void Player::ChangePlayerBehavior() {
		//次の行動をチェンジする（仮想関数呼び出しで切り替える）
		m_PlayerBehavior = m_PlayerBehavior->ChangeNextBehavior(GetThis<Player>());
	}

	//初期化
	void Player::OnCreate() {
		//コマンドの登録
		//Aボタンにアクションコマンドを登録
		m_Handler.SetButtonA(&m_Action);
		//Bボタンに行動切り替えコマンドを登録
		m_Handler.SetButtonB(&m_BehaviorChange);


		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);

		//Rigidbodyをつける
		auto PtrRedid = AddComponent<Rigidbody>();
		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//衝突判定をつける
		auto PtrCol = AddComponent<CollisionSphere>();
		PtrCol->SetIsHitAction(IsHitAction::Slide);
		PtrCol->SetDrawActive(true);

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"TRACE_TX");

		//透明処理
		SetAlphaActive(true);
		//0番目のビューのカメラを得る
		//LookAtCameraである
		
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			//LookAtCameraに注目するオブジェクト（プレイヤー）の設定
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
		//行動クラスの構築(デフォルト行動)
		m_PlayerBehavior = DefaultPlayerBehavior::Instance();
		//ステートマシンの構築
		m_StateMachine = make_shared< LayeredStateMachine<Player> >(GetThis<Player>());
		//最初のステートをPlayerDefaultに設定
		m_StateMachine->Reset(PlayerDefault::Instance());
	}

	//更新
	void Player::OnUpdate() {
		//コントローラチェック
		InputCheck();
		//ステートマシン更新
		m_StateMachine->Update();
	}

	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
	}

	//ターンの最終更新時
	void Player::OnLastUpdate() {
		//文字列表示
		//行動
		wstring BEHAVIOR;

		if (m_PlayerBehavior == DefaultPlayerBehavior::Instance()) {
			BEHAVIOR = L"DEFAULT行動: Aボタンでジャンプ。Bボタンで行動切り替え\n";
		}
		else {
			BEHAVIOR = L"ATTACK行動: Aボタンでアタック。Bボタンで行動切り替え\n";
		}


		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\n";


		auto Pos = GetComponent<Transform>()->GetPosition();
		wstring PositionStr(L"Position:\t");
		PositionStr += L"X=" + Util::FloatToWStr(Pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Y=" + Util::FloatToWStr(Pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Z=" + Util::FloatToWStr(Pos.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring RididStr(L"Velocity:\t");
		auto Velocity = GetComponent<Rigidbody>()->GetVelocity();
		RididStr += L"X=" + Util::FloatToWStr(Velocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Y=" + Util::FloatToWStr(Velocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Z=" + Util::FloatToWStr(Velocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring GravStr(L"Gravity:\t");

		auto Grav = GetComponent<Gravity>()->GetGravity();
		GravStr += L"X=" + Util::FloatToWStr(Grav.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Y=" + Util::FloatToWStr(Grav.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Z=" + Util::FloatToWStr(Grav.z, 6, Util::FloatModify::Fixed) + L"\n";


		wstring GravityStr(L"GravityVelocity:\t");
		auto GravityVelocity = GetComponent<Gravity>()->GetGravityVelocity();
		GravityStr += L"X=" + Util::FloatToWStr(GravityVelocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Y=" + Util::FloatToWStr(GravityVelocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Z=" + Util::FloatToWStr(GravityVelocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring HitObjectStr(L"HitObject: ");
		if (GetComponent<Collision>()->GetHitObjectVec().size() > 0) {
			for (auto&v : GetComponent<Collision>()->GetHitObjectVec()) {
				HitObjectStr += Util::UintToWStr((UINT)v.get()) + L",";
			}
			HitObjectStr += L"\n";
		}
		else {
			HitObjectStr += L"NULL\n";
		}
		wstring str = BEHAVIOR + FPS + PositionStr + RididStr + GravStr + GravityStr + HitObjectStr ;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}


	//--------------------------------------------------------------------------------------
	///	プレイヤー行動親
	//--------------------------------------------------------------------------------------
	Vector3 PlayerBehavior::GetAngle(const shared_ptr<Player>& Obj) {
		Vector3 Angle(0, 0, 0);
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//動いた時のスピード
				auto PtrTransform = Obj->GetComponent<Transform>();
				auto PtrCamera = Obj->GetStage()->GetView()->GetTargetCamera();
				//進行方向の向きを計算
				Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.Normalize();
				//進行方向向きからの角度を算出
				float FrontAngle = atan2(Front.z, Front.x);
				//コントローラの向き計算
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				//コントローラの向きから角度を計算
				float CntlAngle = atan2(-MoveX, MoveZ);
				//トータルの角度を算出
				float TotalAngle = FrontAngle + CntlAngle;
				//角度からベクトルを作成
				Angle = Vector3(cos(TotalAngle), 0, sin(TotalAngle));
				//正規化する
				Angle.Normalize();
				//Y軸は変化させない
				Angle.y = 0;
			}
		}
		return Angle;
	}

	void PlayerBehavior::Move(const shared_ptr<Player>& Obj) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetAngle(Obj);
		//Transform
		auto PtrTransform = Obj->GetComponent<Transform>();
		//Rigidbodyを取り出す
		auto PtrRedit = Obj->GetComponent<Rigidbody>();
		//現在の速度を取り出す
		auto Velo = PtrRedit->GetVelocity();
		//目的地を最高速度を掛けて求める
		auto Target = Angle * Obj->GetMaxSpeed();
		//目的地に向かうために力のかける方向を計算する
		//Forceはフォースである
		auto Force = Target - Velo;
		//yは0にする
		Force.y = 0;
		//加速度を求める
		auto Accel = Force / Obj->GetMass();
		//ターン時間を掛けたものを速度に加算する
		Velo += (Accel * ElapsedTime);
		//減速する
		Velo *= Obj->GetDecel();
		//速度を設定する
		PtrRedit->SetVelocity(Velo);
		//回転の計算
		float YRot = PtrTransform->GetRotation().y;
		Quaternion Qt;
		Qt.Identity();
		if (Angle.Length() > 0.0f) {
			//ベクトルをY軸回転に変換
			float PlayerAngle = atan2(Angle.x, Angle.z);
			Qt.RotationRollPitchYaw(0, PlayerAngle, 0);
			Qt.Normalize();
		}
		else {
			Qt.RotationRollPitchYaw(0, YRot, 0);
			Qt.Normalize();
		}
		//Transform
		PtrTransform->SetQuaternion(Qt);
	}


	//--------------------------------------------------------------------------------------
	///	デフォルトプレイヤー行動
	//--------------------------------------------------------------------------------------
	//インスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(DefaultPlayerBehavior)

	shared_ptr<PlayerBehavior> DefaultPlayerBehavior::ChangeNextBehavior(const shared_ptr<Player>& Obj) {
		//攻撃行動を返す
		return AttackPlayerBehavior::Instance();
	}


	void DefaultPlayerBehavior::StartAction(const shared_ptr<Player>& Obj) {
		//重力
		auto PtrGravity = Obj->GetComponent<Gravity>();
		//ジャンプスタート
		Vector3 JumpVec(0.0f, 4.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}
	void DefaultPlayerBehavior::ExcuteAction(const shared_ptr<Player>& Obj) {
		//重力
		auto PtrGravity = Obj->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			//ジャンプ終了
			Obj->GetStateMachine()->Pop();
		}
	}

	//--------------------------------------------------------------------------------------
	///	攻撃プレイヤー行動
	//--------------------------------------------------------------------------------------
	//インスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(AttackPlayerBehavior)

	shared_ptr<PlayerBehavior> AttackPlayerBehavior::ChangeNextBehavior(const shared_ptr<Player>& Obj) {
		//通常行動を返す
		return DefaultPlayerBehavior::Instance();
	}

	void AttackPlayerBehavior::StartAction(const shared_ptr<Player>& Obj) {
		auto PtrCamera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto PtrTrans = Obj->GetComponent<Transform>();
		auto Group = Obj->GetStage()->GetSharedObjectGroup(L"AttackBall");
		for (auto& v : Group->GetGroupVector()) {
			auto shptr = v.lock();
			if (shptr) {
				auto AttackPtr = dynamic_pointer_cast<AttackBall>(shptr);
				if (AttackPtr && !AttackPtr->IsUpdateActive()) {

					//回転の計算
					auto RotY = PtrTrans->GetRotation().y;
					auto Angle = Vector3(sin(RotY), 0, cos(RotY));
					Angle.Normalize();

					auto Span = Angle * 0.5f;



					AttackPtr->Weakup(PtrTrans->GetPosition() + Span, Angle * 10.0f);
					return;
				}
			}
		}
	}
	void AttackPlayerBehavior::ExcuteAction(const shared_ptr<Player>& Obj) {
		//アタック終了
		Obj->GetStateMachine()->Pop();
	}



	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefault)

	//ステート実行中に毎ターン呼ばれる関数
	void PlayerDefault::Execute(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->Move(Obj);
	}

	//--------------------------------------------------------------------------------------
	///	アクションステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(PlayerAction)

	//ステートに入ったときに呼ばれる関数
	void PlayerAction::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->StartAction(Obj);
	}

	//ステート実行中に毎Updateに呼ばれる関数
	void PlayerAction::Execute(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->Move(Obj);
		Obj->GetPlayerBehavior()->ExcuteAction(Obj);
	}





}
//end basecross

