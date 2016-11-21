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
	//構築と破棄
	Player::Player(const shared_ptr<Stage>& StagePtr,
		const Vector3 StartPos, const Vector3 StartVelocity) :
		GameObject(StagePtr),
		m_MaxSpeed(10.0f),	//最高速度
		m_Decel(0.95f),	//減速値
		m_Mass(1.0f),	//質量
		m_IsThrough(false),
		m_StartPos(StartPos),
		m_StartVelocity(StartVelocity)
	{}

	//初期化
	void Player::OnCreate() {
		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);
		//Rigidbodyをつける
		auto PtrRedid = AddComponent<Rigidbody>();
		//反発係数は0.5
		PtrRedid->SetReflection(0.5f);
		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.125f);
		//衝突判定
		auto PtrColl = AddComponent<CollisionSphere>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

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

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetFont(L"ＭＳゴシック", 32);
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//透明処理
		SetAlphaActive(true);
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			//LookAtCameraに注目するオブジェクト（プレイヤー）の設定
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<Player> >(GetThis<Player>());
		//最初のステートをDefaultStateに設定
		m_StateMachine->ChangeState(DefaultState::Instance());
	}

	//移動の向きを得る
	Vector3 Player::GetAngle() {
		Vector3 Angle(0, 0, 0);
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//動いた時のスピード
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
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


	//更新
	void Player::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの更新が行われる(Execute()関数が呼ばれる)
		m_StateMachine->Update();
	}

	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		for (auto &v : OtherVec) {
			auto shenemy = dynamic_pointer_cast<EnemyInterface>(v);
			auto shCylinder = dynamic_pointer_cast<FixedCylinder>(v);
			auto shWindow = dynamic_pointer_cast<RoomWindow>(v);
			auto shDoor = dynamic_pointer_cast<RoomDoor>(v);
			auto sh = dynamic_pointer_cast<GameObject>(v);
			if (shenemy) {
				auto PtrRedid = GetComponent<Rigidbody>();
				auto Pos = GetComponent<Transform>()->GetPosition();
				auto EnemyPos = sh->GetComponent<Transform>()->GetPosition();

				auto Coll = GetComponent<CollisionSphere>();
				auto Sp = Coll->GetSphere();
				auto EnColl = v->GetComponent<CollisionSphere>();
				auto EnSp = EnColl->GetSphere();
				float Esc = Sp.m_Radius + EnSp.m_Radius;
				auto Span = Pos - EnemyPos;
				Span.y = 0;
				Span.Normalize();
				Span *= Esc;
				Pos = EnemyPos + Span;
				GetComponent<Transform>()->SetPosition(Pos);
				GetComponent<Transform>()->SetToBefore();

				auto Velo = Pos - EnemyPos;
				Velo *= 10.0f;
				PtrRedid->SetVelocity(Velo);
				//ライフを減らす
				App::GetApp()->GetScene<Scene>()->GetGameParamaters().m_PlayerLife--;
				if (App::GetApp()->GetScene<Scene>()->GetGameParamaters().m_PlayerLife <= 0) {
					auto ScenePtr = App::GetApp()->GetScene<Scene>();
					PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ToGameOver");
				}
				return;
			}
			if (shCylinder) {
				auto NowStage = dynamic_pointer_cast<NowScienceRoom>(GetStage());
				if (NowStage) {
					shCylinder->BalloonObOff();
					return;
				}
			}
			if (shWindow) {
				auto PastStage = dynamic_pointer_cast<PastScienceRoom>(GetStage());
				if (PastStage) {
					shWindow->WindowClear();
					return;
				}
			}
			if (shDoor) {
				auto& param = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
				if (param.m_IsHeadTaskClear) {
					auto NowStage = dynamic_pointer_cast<NowScienceRoom>(GetStage());
					if (NowStage) {
						auto ScenePtr = App::GetApp()->GetScene<Scene>();
						PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ToClear");
						return;
					}
				}
			}
		}

		if (GetStateMachine()->GetCurrentState() == JumpState::Instance()) {
			GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//ターンの最終更新時
	void Player::OnLastUpdate() {

		auto PtrTransform = GetComponent<Transform>();
		auto Coll = GetComponent<CollisionSphere>();
		auto Sp = Coll->GetSphere();
		//ゴールとの交差を判定
		auto ShGoal = GetStage()->GetSharedGameObject<TileWall>(L"Goal");
		if (ShGoal && !m_IsThrough) {
			auto PtrCylinder = GetStage()->GetSharedGameObject<FixedCylinder>(L"FixedCylinder");
			//バルーンタスクをクリアしたかどうか
			if (App::GetApp()->GetScene<Scene>()->GetGameParamaters().m_BalloonOpenTask) {
				Vector3 RetVec, Normal;
				if (ShGoal->HitTestSphere(Sp, RetVec, Normal)) {
					m_IsThrough = true;
					return;
				}
			}
		}
		//平面との交差を判定
		if (!m_IsThrough) {
			auto Group = GetStage()->GetSharedObjectGroup(L"WallGroup");
			for (auto Ptr : Group->GetGroupVector()) {
				if (!Ptr.expired()) {
					auto SquarePtr = dynamic_pointer_cast< TileWall>(Ptr.lock());
					if (SquarePtr) {
						Vector3 RetVec, Normal;
						if (SquarePtr->HitTestSphere(Sp, RetVec, Normal)) {
							//ヒットしたので位置をヒット位置に修正する
							Vector3 SetPos = RetVec + -Normal * Sp.m_Radius;
							PtrTransform->SetPosition(SetPos);
							auto PtrGrav = GetComponent<Gravity>();
							//蓄積した落下の加速度を0にする
							PtrGrav->SetGravityVelocityZero();
							auto PtrRidit = GetComponent<Rigidbody>();
							//反発
							Vector3 Refalct = Vector3EX::Reflect(PtrRidit->GetVelocity(), Normal);
							Refalct *= 0.5f;
							PtrRidit->SetVelocity(Refalct);

						}
					}
				}
			}
		}

		//ゲームステージ範囲からの離脱チェック
		Rect2D<float> StageRect(-5.05f, -5.05f, 5.05f, 5.05f);
		auto Pos = PtrTransform->GetPosition();
		Point2D<float> PlayerPoint(Pos.x, Pos.z);
		if (!StageRect.PtInRect(PlayerPoint)) {
			auto ScenePtr = App::GetApp()->GetScene<Scene>();
			PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ChangeScienceRoom");
			return;

		}


		//文字列表示
		wstring STAGE_NAME(L"現在の理科室");
		auto PastStagePtr = dynamic_pointer_cast<PastScienceRoom>(GetStage());
		if (PastStagePtr) {
			STAGE_NAME = L"いにしえの理科室";
		}
		STAGE_NAME += L"\n";


		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\n";

		wstring LIFE(L"LIFE: ");
		LIFE += Util::IntToWStr(App::GetApp()->GetScene<Scene>()->GetGameParamaters().m_PlayerLife);
		LIFE += L"\n";
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}

		wstring str = STAGE_NAME + FPS + LIFE;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
		
	}

	//モーションを実装する関数群
	//移動して向きを移動方向にする
	void Player::MoveRotationMotion() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetAngle();
		//Transform
		auto PtrTransform = GetComponent<Transform>();
		//Rigidbodyを取り出す
		auto PtrRedit = GetComponent<Rigidbody>();
		//現在の速度を取り出す
		auto Velo = PtrRedit->GetVelocity();
		//目的地を最高速度を掛けて求める
		auto Target = Angle * m_MaxSpeed;
		//目的地に向かうために力のかける方向を計算する
		//Forceはフォースである
		auto Force = Target - Velo;
		//yは0にする
		Force.y = 0;
		//加速度を求める
		auto Accel = Force / m_Mass;
		//ターン時間を掛けたものを速度に加算する
		Velo += (Accel * ElapsedTime);
		//減速する
		Velo *= m_Decel;
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


	//Aボタンでジャンプするどうかを得る
	bool Player::IsJumpMotion() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Aボタンが押された瞬間ならジャンプ
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				return true;
			}
		}
		return false;
	}
	//Aボタンでジャンプする瞬間の処理
	void Player::JumpMotion() {
		auto PtrTrans = GetComponent<Transform>();
		//重力
		auto PtrGravity = GetComponent<Gravity>();
		//ジャンプスタート
		Vector3 JumpVec(0.0f, 4.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}
	//Aボタンでジャンプしている間の処理
	//ジャンプ終了したらtrueを返す
	bool Player::JumpMoveMotion() {
		auto PtrTransform = GetComponent<Transform>();
		//重力
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			return true;
		}
		return false;
	}


	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	用途: 通常移動
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<DefaultState> DefaultState::Instance() {
		static shared_ptr<DefaultState> instance;
		if (!instance) {
			instance = shared_ptr<DefaultState>(new DefaultState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void DefaultState::Enter(const shared_ptr<Player>& Obj) {
		//何もしない
	}
	//ステート実行中に毎ターン呼ばれる関数
	void DefaultState::Execute(const shared_ptr<Player>& Obj) {
		Obj->MoveRotationMotion();
		if (Obj->IsJumpMotion()) {
			//Jumpボタンでステート変更
			Obj->GetStateMachine()->ChangeState(JumpState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void DefaultState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}


	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	用途: ジャンプ状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<JumpState> JumpState::Instance() {
		static shared_ptr<JumpState> instance;
		if (!instance) {
			instance = shared_ptr<JumpState>(new JumpState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void JumpState::Enter(const shared_ptr<Player>& Obj) {
		//ジャンプ中も移動可能とする
		Obj->MoveRotationMotion();
		Obj->JumpMotion();
	}
	//ステート実行中に毎ターン呼ばれる関数
	void JumpState::Execute(const shared_ptr<Player>& Obj) {
		//ジャンプ中も移動可能とする
		Obj->MoveRotationMotion();
		if (Obj->JumpMoveMotion()) {
			//通常状態に戻る
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void JumpState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}


}
//end basecross

