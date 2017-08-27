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
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_MaxHP(100.0f),
		m_HP(100.0f),
		m_StartPosition(0, 0.5f, -7.5f)
	{}

	//初期化
	void Player::OnCreate() {

		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPosition);
		AddTag(L"Player");

		//Rigidbodyをつける
		auto PtrRedid = AddComponent<Rigidbody>();
		//衝突判定をつける
		auto PtrCol = AddComponent<CollisionCapsule>();
		PtrCol->SetIsHitAction(IsHitAction::Slide);
//		PtrCol->SetDrawActive(true);
		PtrCol->AddExcludeCollisionTag(L"PlayerSword");

		//MovetToアクションを実装（倒れた時対応）
		auto PtrMove = AddComponent<MoveTo>();
		//無効にしておく
		PtrMove->SetUpdateActive(false);



		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");
		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
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
		//戦いの行動の構築
		m_FightBehavior.reset(new FightBehavior<Player>(GetThis<Player>()));
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//最初のステートをPlayerDefaultに設定
		m_StateMachine->ChangeState(PlayerDefaultState::Instance());
	}

	//更新
	void Player::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.PushHandle(GetThis<Player>());
		//ステートマシン更新
		m_StateMachine->Update();
	}

	//後更新
	void Player::OnUpdate2() {
		//ステートマシン更新2
		m_StateMachine->Update2();

		//文字列の表示
		DrawStrings();
	}

	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		//プレイヤーが何かに当たった
		if (GetStateMachine()->GetCurrentState() == PlayerJumpState::Instance()) {
			//現在がジャンプステートならPlayerDefaultに設定
			GetStateMachine()->ChangeState(PlayerDefaultState::Instance());
			return;
		}
		//それ以外でプレイヤーが何かに当たった
		auto SrcColl = GetComponent<CollisionCapsule>();
		for (auto&v : OtherVec) {
			if (v->FindTag(L"EnemySword")) {
				//剣に当たった
				auto DestColl = v->GetComponent<CollisionCapsule>();
				//剣のコリジョンを一時的に無効にする
				DestColl->SetUpdateActive(false);
				//スパークの放出
				auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark", false);
				if (PtrSpark) {
					Vec3 Pos1, Pos2;
					HitTest::CAPSULE_CAPSULE(SrcColl->GetCapsule(), DestColl->GetCapsule(), Pos1, Pos2);
					PtrSpark->InsertSpark(Pos1);
				}
				//HPを下げる
				m_HP -= 5.0f;
				if (m_HP <= 0.0f) {
					m_HP = 0.0f;
					//ファイアの放出
					auto PtriFire = GetStage()->GetSharedGameObject<MultiFire>(L"MultiFire", false);
					if (PtriFire) {
						PtriFire->InsertFire(GetComponent<Transform>()->GetPosition());
					}
					//倒れる
					GetStateMachine()->ChangeState(PlayerDieState::Instance());
				}
				else {
					auto HaveSordPtr = v->GetComponent<Transform>()->GetParent();
					m_FightBehavior->SetHitBase(HaveSordPtr->GetComponent<Transform>()->GetPosition());
					GetStateMachine()->ChangeState(PlayerSwordHit::Instance());
				}
				return;
			}
		}



	}

	//Aボタンハンドラ
	void  Player::OnPushA() {
		if (GetStateMachine()->GetCurrentState() == PlayerDefaultState::Instance()) {
			//通常ステートならジャンプステートに移行
			GetStateMachine()->ChangeState(PlayerJumpState::Instance());
		}
	}

	//Xボタンハンドラ
	void  Player::OnPushX() {
		if (GetStateMachine()->GetCurrentState() == PlayerDefaultState::Instance()){
			//通常ステートなら剣を振るステートに移行
			GetStateMachine()->ChangeState(PlayerSwordState::Instance());
		}
		else if (GetStateMachine()->GetCurrentState() == PlayerJumpState::Instance()) {
			//ジャンプステートなら剣を突くステートに移行
			GetStateMachine()->ChangeState(PlayerSwordPokeState::Instance());
		}

	}

	//文字列の表示
	void Player::DrawStrings() {

		//文字列表示
		//行動
		wstring BEHAVIOR;


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

		wstring RididStr(L"Velocity:\t");
		auto Velocity = GetComponent<Rigidbody>()->GetVelocity();
		RididStr += L"X=" + Util::FloatToWStr(Velocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Y=" + Util::FloatToWStr(Velocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Z=" + Util::FloatToWStr(Velocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring str = BEHAVIOR + FPS + PositionStr + RididStr ;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}


	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefaultState)

	void PlayerDefaultState::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetFightBehavior()->HaveSword(L"PlayerSword");
	}

	void PlayerDefaultState::Execute(const shared_ptr<Player>& Obj) {
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->MovePlayer();
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->Execute();
	}


	void PlayerDefaultState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	ジャンプステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerJumpState)

	void PlayerJumpState::Enter(const shared_ptr<Player>& Obj) {
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->StartJump(Vec3(0, 4.0f, 0));
	}

	void PlayerJumpState::Execute(const shared_ptr<Player>& Obj) {
		//ジャンプ中も方向変更可能
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->MovePlayer();
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->Execute();
	}

	void PlayerJumpState::Execute2(const shared_ptr<Player>& Obj) {
	}

	void PlayerJumpState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	剣を振るステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerSwordState)

	void PlayerSwordState::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetFightBehavior()->StartShakeSword();
	}

	void PlayerSwordState::Execute(const shared_ptr<Player>& Obj) {
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->Execute();
		if (Obj->GetFightBehavior()->RotationShakeSword()) {
			//半周回転させたらデフォルトステートに移行
			Obj->GetStateMachine()->ChangeState(PlayerDefaultState::Instance());
		}
	}

	void PlayerSwordState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	剣を突くステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerSwordPokeState)

	void PlayerSwordPokeState::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetFightBehavior()->StartPokeSword();

	}

	void PlayerSwordPokeState::Execute(const shared_ptr<Player>& Obj) {
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->Execute();
		if (Obj->GetFightBehavior()->PokeSword()) {
			//半周回転させたらデフォルトステートに移行
			Obj->GetStateMachine()->ChangeState(PlayerDefaultState::Instance());
		}
	}

	void PlayerSwordPokeState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}


	//--------------------------------------------------------------------------------------
	///	敵の剣に当たったステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(PlayerSwordHit)

	void PlayerSwordHit::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetFightBehavior()->HitSwordBehavior();
	}

	void PlayerSwordHit::Execute(const shared_ptr<Player>& Obj) {
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->Execute();
		if (Obj->GetFightBehavior()->HitAfterdBehavior()) {
			Obj->GetStateMachine()->ChangeState(PlayerDefaultState::Instance());
		}
	}

	void PlayerSwordHit::Exit(const shared_ptr<Player>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	倒されたステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(PlayerDieState)

	void PlayerDieState::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetFightBehavior()->DieStartBehavior();
	}

	void PlayerDieState::Execute(const shared_ptr<Player>& Obj) {
		if (Obj->GetFightBehavior()->DieBehavior()) {
			Obj->GetStateMachine()->ChangeState(PlayerDefaultState::Instance());
		}
	}

	void PlayerDieState::Exit(const shared_ptr<Player>& Obj) {
		//HPをもとに戻す
		Obj->SetHP(Obj->GetMaxHP());
	}





}
//end basecross

