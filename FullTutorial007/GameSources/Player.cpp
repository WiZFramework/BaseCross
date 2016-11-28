/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class Player :  public SS5ssae;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Player::Player(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir) :
		SS5ssae(StagePtr, BaseDir, L"character_template_3head.ssae", L"wait"),
		m_MaxSpeed(40.0f),	//最高速度
		m_Decel(0.95f),	//減速値
		m_Mass(1.0f),	//質量
		m_ZRailIndex(0),
		m_ZRailBeforIndex(0)
	{
		m_ZRail = { 0, 2.0f, 4.0f, 6.0f, 8.0f };
		m_ToAnimeMatrixLeft.DefTransformation(
			Vector3(0.1f, 0.1f, 0.1f),
			Vector3(0, 0, 0),
			Vector3(0, -0.55f, 0.0f)
		);
		m_ToAnimeMatrixRight.DefTransformation(
			Vector3(-0.1f, 0.1f, 0.1f),
			Vector3(0, 0, 0),
			Vector3(0, -0.55f, 0.0f)
		);

	}

	//初期化
	void Player::OnCreate() {
		//丸影（小さ目に作成する）
		//頂点は変更できなくてよい
		m_ShadowResource = MeshResource::CreateSphere(0.8f, 18);

		//元となるオブジェクトからアニメーションオブジェクトへの行列の設定
		SetToAnimeMatrix(m_ToAnimeMatrixLeft);

		auto PtrT = GetComponent<Transform>();
		PtrT->SetScale(1.0f,1.0f,1.0f);
		PtrT->SetPosition(Vector3(0, 0.5f, m_ZRail[m_ZRailIndex]));
		//親クラスのクリエイトを呼ぶ
		SS5ssae::OnCreate();
		//値は秒あたりのフレーム数
		SetFps(30.0f);

		//Rigidbodyをつける
		auto PtrRedit = AddComponent<Rigidbody>();
		//あまり反発しなくする
		PtrRedit->SetReflection(0.2f);
		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//衝突判定をつける(OBB)
		//auto PtrCol = AddComponent<CollisionObb>();
		//CollisionSphereを使う場合は、OBBを無効にして以下を有効にする
		auto PtrCol = AddComponent<CollisionSphere>();
		//コリジョンを表示する場合は以下を設定
		PtrCol->SetDrawActive(true);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(m_ShadowResource);

		//Actionをつける
		//レイヤー変更用
		auto PtrAction = AddComponent<Action>();
		//アクションは無効にしておく
		PtrAction->SetUpdateActive(false);


		//0番目のビューのカメラを得る
		//MyCameraである
		auto PtrCamera = dynamic_pointer_cast<MyCamera>(GetStage()->GetView()->GetTargetCamera());
		//LookAtCameraの場合は以下コメントを外す
		//auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			//Cameraに注目するオブジェクト（プレイヤー）の設定
			PtrCamera->SetTargetObject(GetThis<GameObject>());
			PtrCamera->SetToTargetLerp(0.1f);
		}

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<Player> >(GetThis<Player>());
		//最初のステートをWaitStateに設定
		m_StateMachine->SetCurrentState(WaitState::Instance());
		//WaitStateの初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<Player>());
	}

	//移動の方向を得る
	float Player::GetMoveX() {
		float MoveX = 0;
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0) {
				//コントローラの向き計算
				MoveX = CntlVec[0].fThumbLX;
			}
		}
		return MoveX;
	}


	//更新
	void Player::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	//衝突時
	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		auto ColPtr = GetComponent<Collision>();
		//現在のステートがRailChangeStateだったら
		if (GetStateMachine()->GetCurrentState() == RailChangeState::Instance()) {
			//何かと衝突した、強制的に戻される
			RailChangeBeforStartMotion();
			return;
		}
		if (GetStateMachine()->GetCurrentState() == JumpState::Instance()) {
			GetStateMachine()->ChangeState(WaitState::Instance());
		}
	}


	void Player::OnLastUpdate() {
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		wstring str = FPS;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}


	//モーションを実装する関数群
	//移動して向きを移動方向にする
	//移動距離を返す
	float  Player::MoveRotationMotion() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto MoveX = GetMoveX();
		//Transform
		auto PtrTransform = GetComponent<Transform>();
		//Rigidbodyを取り出す
		auto PtrRedit = GetComponent<Rigidbody>();
		//現在の速度を取り出す
		auto Velo = PtrRedit->GetVelocity().x;
		//目的地を最高速度を掛けて求める
		auto Target = MoveX * m_MaxSpeed;
		//目的地に向かうために力のかける方向を計算する
		//Forceはフォースである
		auto Force = Target - Velo;
		//加速度を求める
		auto Accel = Force / m_Mass;
		//ターン時間を掛けたものを速度に加算する
		Velo += (Accel * ElapsedTime);
		//減速する
		Velo *= m_Decel;
		//速度を設定する
		Vector3 VecVelo(Velo, 0, 0);
		PtrRedit->SetVelocity(VecVelo);
		////回転の計算
		//元となるオブジェクトからアニメーションオブジェクトへの行列の設定
		//Transformのスケーリングを-1にすると衝突判定がうまくいかないので
		//SpriteStdioの部分だけ変更する
		if (MoveX >= 0.0f) {
			SetToAnimeMatrix(m_ToAnimeMatrixRight);
		}
		else {
			SetToAnimeMatrix(m_ToAnimeMatrixLeft);
		}
		//MoveXを返す
		return MoveX;
	}

	void Player::AnimeChangeMotion(const wstring& key, bool looped) {
		ChangeAnimation(key);
		SetLooped(looped);
	}

	void Player::LoopedAnimeUpdateMotion() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//アニメーションを更新する
		UpdateAnimeTime(ElapsedTime);
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
	void Player::JumpStartMotion() {
		auto PtrTrans = GetComponent<Transform>();
		//重力
		auto PtrGravity = GetComponent<Gravity>();
		//ジャンプスタート
		Vector3 JumpVec(0.0f, 4.0f, 0);
		PtrGravity->StartJump(JumpVec, 0.05f);
		auto PtrCol = GetComponent<Collision>();
		//親があったら切り離す
		//		PtrTrans->SetParent(nullptr);
	}
	//Aボタンでジャンプしている間の処理
	//ジャンプ終了したらtrueを返す
	bool Player::JumpMotion() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//アニメーションを更新する
		//ジャンプは地面に着くまでアニメーション変更はしない
		UpdateAnimeTime(ElapsedTime);
		auto PtrTransform = GetComponent<Transform>();
		//重力
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			return true;
		}
		return false;
	}


	//Bボタンでアタックするどうかを得る
	bool Player::IsAttackMotion() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Bボタンが押された瞬間なら砲弾発射
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				return true;
			}
		}
		return false;
	}
	//Bボタンでアタックする処理
	bool Player::AttackMotion() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//アニメーションを更新する
		UpdateAnimeTime(ElapsedTime);
		if (IsAnimeEnd()) {
			return true;
		}
		return false;
	}

	//左スティックでZレールを変更するかどうか
	bool Player::IsRailChangeMotion() {
		auto PtrTrans = GetComponent<Transform>();
		auto PtrGra = GetComponent<Gravity>();
		if (PtrTrans->GetPosition().y > 0.5f) {
			//何かの上に乗ってる時は変更できない
			return false;
		}
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (abs(CntlVec[0].fThumbLY) > 0.9f) {
				//コントローラの向きY
				if (CntlVec[0].fThumbLY > 0) {
					//現在のレールーが一番奥なら
					//0を返す
					if (m_ZRail.size() <= m_ZRailIndex + 1) {
						return false;
					}
					else {
						m_ZRailBeforIndex = m_ZRailIndex;
						m_ZRailIndex++;
					}
					return true;
				}
				else if (CntlVec[0].fThumbLY < 0) {
					//現在のレールが一番手前なら
					//0を返す
					if (m_ZRailIndex == 0) {
						return false;
					}
					else {
						m_ZRailBeforIndex = m_ZRailIndex;
						m_ZRailIndex--;
					}
					return true;
				}
			}
		}
		return false;
	}

	//Zレール変更を開始する
	void Player::RailChangeStartMotion() {
		//Rigidbodyを取得
		auto PtrRedit = GetComponent<Rigidbody>();
		//無効にする
		PtrRedit->SetUpdateActive(false);
		//重力を取得
		auto PtrGravity = GetComponent<Gravity>();
		//無効にする
		PtrGravity->SetUpdateActive(false);
		//衝突判定を取得
		//auto PtrCol = GetComponent<Collision>();

		auto PtrTrans = GetComponent<Transform>();
		auto NowPos = PtrTrans->GetPosition();

		//Actionを取得する
		auto PtrAction = AddComponent<Action>();
		PtrAction->SetUpdateActive(true);
		//登録されているアクションをクリアする
		PtrAction->AllActionClear();
		PtrAction->AddMoveTo(1.0f, Vector3(NowPos.x, NowPos.y, m_ZRail[m_ZRailIndex]));
		//ループしない
		PtrAction->SetLooped(false);
		PtrAction->Run();
	}

	//Zレール変更を更新する
	bool Player::RailChangeMotion() {
		//Actionは勝手に実行されるので、処理が終わったかどうかだけチェックする
		//Actionを取得する
		auto PtrAction = AddComponent<Action>();
		if (PtrAction->IsArrived()) {
			return true;
		}
		return false;
	}

	//Zレールを強制的に戻す
	void Player::RailChangeBeforStartMotion() {
		auto PtrTrans = GetComponent<Transform>();
		auto NowPos = PtrTrans->GetPosition();
		//Actionを取得する
		auto PtrAction = AddComponent<Action>();
		PtrAction->SetUpdateActive(true);
		//登録されているアクションをクリアする
		PtrAction->AllActionClear();
		auto temp = m_ZRailIndex;
		m_ZRailIndex = m_ZRailBeforIndex;
		m_ZRailBeforIndex = temp;
		PtrAction->AddMoveTo(0.5f, Vector3(NowPos.x, NowPos.y, m_ZRail[m_ZRailIndex]));
		//ループしない
		PtrAction->SetLooped(false);
		PtrAction->Run();
	}


	//Zレール変更を終了する
	void Player::RailChangeEndMotion() {
		//Actionを取得する
		auto PtrAction = AddComponent<Action>();
		//無効にする
		PtrAction->SetUpdateActive(false);
		//Rigidbodyを取得
		auto PtrRedit = GetComponent<Rigidbody>();
		PtrRedit->SetVelocity(0, 0, 0);
		//有効にする
		PtrRedit->SetUpdateActive(true);
		//重力を取得
		auto PtrGravity = GetComponent<Gravity>();
		//有効にする
		PtrGravity->SetUpdateActive(true);
		//衝突判定を取得
		auto PtrCol = GetComponent<Collision>();
		//有効にする
		PtrCol->SetUpdateActive(true);

	}





	//--------------------------------------------------------------------------------------
	//	class WaitState : public ObjState<Player>;
	//	用途: 待機状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<WaitState> WaitState::Instance() {
		static shared_ptr<WaitState> instance;
		if (!instance) {
			instance = shared_ptr<WaitState>(new WaitState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void WaitState::Enter(const shared_ptr<Player>& Obj) {
		Obj->AnimeChangeMotion(L"wait", true);
	}
	//ステート実行中に毎ターン呼ばれる関数
	void WaitState::Execute(const shared_ptr<Player>& Obj) {
		//アニメーション更新
		Obj->LoopedAnimeUpdateMotion();

		if (Obj->IsRailChangeMotion()) {
			//Zレール変更
			Obj->GetStateMachine()->ChangeState(RailChangeState::Instance());
			//変更後は以降の処理は行わない
			return;
		}

		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (AbsMoveX > 0.9f) {
			//一定以上のスピードでステート変更
			Obj->GetStateMachine()->ChangeState(RunState::Instance());
			//変更後は以降の処理は行わない
			return;
		}
		else if (AbsMoveX > 0.0f) {
			//一定以上のスピードでステート変更
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			//変更後は以降の処理は行わない
			return;
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void WaitState::Exit(const shared_ptr<Player>& Obj) {
	}


	//--------------------------------------------------------------------------------------
	//	class RailChangeState : public ObjState<Player>;
	//	用途: Zレール変更状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<RailChangeState> RailChangeState::Instance() {
		static shared_ptr<RailChangeState> instance;
		if (!instance) {
			instance = shared_ptr<RailChangeState>(new RailChangeState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void RailChangeState::Enter(const shared_ptr<Player>& Obj) {
		//Zレール変更はアニメ
		Obj->AnimeChangeMotion(L"charge", true);
		Obj->RailChangeStartMotion();

	}
	//ステート実行中に毎ターン呼ばれる関数
	void RailChangeState::Execute(const shared_ptr<Player>& Obj) {
		//アニメーション更新
		Obj->LoopedAnimeUpdateMotion();
		if (Obj->RailChangeMotion()) {
			//処理が終わったらステート変更
			Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			//変更後は以降の処理は行わない
			return;
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void RailChangeState::Exit(const shared_ptr<Player>& Obj) {
		Obj->RailChangeEndMotion();
	}



	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	用途: 通常移動
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<WalkState> WalkState::Instance() {
		static shared_ptr<WalkState> instance;
		if (!instance) {
			instance = shared_ptr<WalkState>(new WalkState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void WalkState::Enter(const shared_ptr<Player>& Obj) {
		Obj->AnimeChangeMotion(L"walk", true);
	}
	//ステート実行中に毎ターン呼ばれる関数
	void WalkState::Execute(const shared_ptr<Player>& Obj) {
		//アニメーション更新
		Obj->LoopedAnimeUpdateMotion();

		if (Obj->IsRailChangeMotion()) {
			//Zレール変更
			Obj->GetStateMachine()->ChangeState(RailChangeState::Instance());
			//変更後は以降の処理は行わない
			return;
		}


		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (AbsMoveX > 0.9f) {
			//一定以上のスピードでステート変更
			Obj->GetStateMachine()->ChangeState(RunState::Instance());
			//変更後は以降の処理は行わない
			return;
		}
		else if (AbsMoveX == 0.0f) {
			//一定以上のスピードでステート変更
			Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			//変更後は以降の処理は行わない
			return;
		}
		if (Obj->IsJumpMotion()) {
			//Jumpボタンでステート変更
			Obj->GetStateMachine()->ChangeState(JumpState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void WalkState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}





	//--------------------------------------------------------------------------------------
	//	class RunState : public ObjState<Player>;
	//	用途: 走るアニメーション
	//--------------------------------------------------------------------------------------
	shared_ptr<RunState> RunState::Instance() {
		static shared_ptr<RunState> instance;
		if (!instance) {
			instance = shared_ptr<RunState>(new RunState);
		}
		return instance;
	}
	void RunState::Enter(const shared_ptr<Player>& Obj) {
		Obj->AnimeChangeMotion(L"run", true);
	}
	void  RunState::Execute(const shared_ptr<Player>& Obj) {
		//アニメーション更新
		Obj->LoopedAnimeUpdateMotion();
		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (AbsMoveX <= 0.9f) {
			//一定以上のスピードでステート変更
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			//変更後は以降の処理は行わない
			return;
		}
		else if (AbsMoveX == 0.0f) {
			//一定以上のスピードでステート変更
			Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			//変更後は以降の処理は行わない
			return;
		}
		if (Obj->IsAttackMotion()) {
			Obj->GetStateMachine()->ChangeState(AttackState::Instance());
			//変更後は以降の処理は行わない
			return;
		}
		if (Obj->IsJumpMotion()) {
			//Jumpボタンでステート変更
			Obj->GetStateMachine()->ChangeState(JumpState::Instance());
		}
	}
	void  RunState::Exit(const shared_ptr<Player>& Obj) {
		//ステート終了処理は何もしない
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
		Obj->AnimeChangeMotion(L"jump_all", false);
		Obj->JumpStartMotion();
	}
	//ステート実行中に毎ターン呼ばれる関数
	void JumpState::Execute(const shared_ptr<Player>& Obj) {
		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (Obj->JumpMotion()) {
			if (AbsMoveX > 0.9f) {
				//走る状態に戻る
				Obj->GetStateMachine()->ChangeState(RunState::Instance());
			}
			if (AbsMoveX == 0.0f) {
				//待ち状態に戻る
				Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			}
			else {
				//歩き状態に戻る
				Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			}
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void JumpState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}


	//--------------------------------------------------------------------------------------
	//	class AttackState : public ObjState<Player>;
	//	用途: アタックアニメーション
	//--------------------------------------------------------------------------------------
	shared_ptr<AttackState> AttackState::Instance() {
		static shared_ptr<AttackState> instance;
		if (!instance) {
			instance = shared_ptr<AttackState>(new AttackState);
		}
		return instance;
	}
	void AttackState::Enter(const shared_ptr<Player>& Obj) {
		Obj->AnimeChangeMotion(L"attack2", false);
		Obj->SetFps(40.0f);
	}
	void  AttackState::Execute(const shared_ptr<Player>& Obj) {
		auto AbsMoveX = abs(Obj->MoveRotationMotion());
		if (Obj->AttackMotion()) {
			if (AbsMoveX > 0.9f) {
				//走る状態に戻る
				Obj->GetStateMachine()->ChangeState(RunState::Instance());
			}
			if (AbsMoveX == 0.0f) {
				//待ち状態に戻る
				Obj->GetStateMachine()->ChangeState(WaitState::Instance());
			}
			else {
				//歩き状態に戻る
				Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			}
		}
	}
	void  AttackState::Exit(const shared_ptr<Player>& Obj) {
		Obj->SetFps(30.0f);
	}



}
//end basecross

