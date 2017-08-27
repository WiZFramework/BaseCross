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

	void AttackBall::Weakup(const Vec3& Position, const Vec3& Velocity) {
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

		//CollisionSphere衝突判定を付ける
		auto PtrColl = AddComponent<CollisionSphere>();


		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
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
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}

	//初期化
	void Player::OnCreate() {

		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);

		//Rigidbodyをつける
		auto PtrRedid = AddComponent<Rigidbody>();

		//CollisionSphere衝突判定を付ける
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetIsHitAction(IsHitAction::Auto);


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

		//Aボタンはアタック
		m_PlayerAction = PlayerAction::Attack;

		//ステートマシンの構築
		m_StateMachine.reset(new LayeredStateMachine<Player>(GetThis<Player>()));
		//最初のステートをPlayerDefaultにリセット
		m_StateMachine->Reset(PlayerDefaultState::Instance());
	}

	//更新
	void Player::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.PushHandle(GetThis<Player>());
		//ステートマシン更新
		m_StateMachine->Update();
		//文字列の表示
		DrawStrings();
	}


	//Aボタン
	void  Player::OnPushA() {
		if (GetStateMachine()->GetTopState() == PlayerDefaultState::Instance()) {
				GetStateMachine()->Push(PlayerAttackState::Instance());
		}
	}

	//文字列の表示
	void Player::DrawStrings() {

		//文字列表示

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

		wstring str = FPS + PositionStr + RididStr;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}


	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------

	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefaultState)

	void PlayerDefaultState::Enter(const shared_ptr<Player>& Obj) {
		//何もしない
	}

	void PlayerDefaultState::Execute(const shared_ptr<Player>& Obj) {
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->MovePlayer();
	}

	void PlayerDefaultState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	アタックステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(PlayerAttackState)

	void PlayerAttackState::Enter(const shared_ptr<Player>& Obj) {
		auto PtrBehavior = Obj->GetBehavior<PlayerBehavior>();
		PtrBehavior->FireAttackBall();
	}

	void PlayerAttackState::Execute(const shared_ptr<Player>& Obj) {
		//すぐにステートを戻す
		Obj->GetStateMachine()->Pop();
	}

	void PlayerAttackState::Exit(const shared_ptr<Player>& Obj) {
		//何もしない
	}


}
//end basecross

