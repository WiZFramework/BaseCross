/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	敵１
	//--------------------------------------------------------------------------------------
	Enemy1::Enemy1(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos):
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_NearFarChange(5.0f)
	{}

	//初期化
	void Enemy1::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek操舵
		auto PtrSeek = AddComponent<SeekSteering>();
		//Seekは無効にしておく
		PtrSeek->SetUpdateActive(false);
		//Arrive操舵
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arriveは無効にしておく
		PtrArrive->SetUpdateActive(false);

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<GameObject>());
		//分離行動をつける
		AddComponent<SeparationSteering>(Group);
		//Obbの衝突判定をつける
		auto PtrColl = AddComponent<CollisionObb>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParentRepel);

		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理
		SetAlphaActive(true);


		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<Enemy1>(GetThis<Enemy1>()));
		//最初のステートをEnemy1FarStateに設定
		m_StateMachine->ChangeState(Enemy1FarState::Instance());
	}

	void Enemy1::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}



	//--------------------------------------------------------------------------------------
	///	Enemy1のFarステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy1FarState)

	void Enemy1FarState::Enter(const shared_ptr<Enemy1>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		PtrSeek->Enter(L"Player");
	}

	void Enemy1FarState::Execute(const shared_ptr<Enemy1>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		float f = PtrSeek->Execute(L"Player",true,0.1f);
		if (f < Obj->GetNearFarChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy1NearState::Instance());
		}
	}

	void Enemy1FarState::Exit(const shared_ptr<Enemy1>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		PtrSeek->Exit();
	}

	//--------------------------------------------------------------------------------------
	///	Enemy1のNearステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy1NearState)

	void Enemy1NearState::Enter(const shared_ptr<Enemy1>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		PtrArrive->Enter(L"Player");
	}

	void Enemy1NearState::Execute(const shared_ptr<Enemy1>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		if (PtrArrive->Execute(L"Player",true,0.1f) >= Obj->GetNearFarChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy1FarState::Instance());
		}
	}

	void Enemy1NearState::Exit(const shared_ptr<Enemy1>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		PtrArrive->Exit();
	}


	//--------------------------------------------------------------------------------------
	///	敵2
	//--------------------------------------------------------------------------------------
	Enemy2::Enemy2(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_LongMediumChange(10.0f),
		m_MediumShortChange(5.0f)
	{}

	//初期化
	void Enemy2::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek操舵
		auto PtrSeek = AddComponent<SeekSteering>();
		//Seekは無効にしておく
		PtrSeek->SetUpdateActive(false);
		//Arrive操舵
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arriveは無効にしておく
		PtrArrive->SetUpdateActive(false);

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<GameObject>());
		//分離行動をつける
		AddComponent<SeparationSteering>(Group);
		auto PtrColl = AddComponent<CollisionSphere>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParentRepel);

		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//透明処理
		SetAlphaActive(true);


		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<Enemy2>(GetThis<Enemy2>()));
		//最初のステートをEnemy2longStateに設定
		m_StateMachine->ChangeState(Enemy2longState::Instance());
	}

	void Enemy2::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	//--------------------------------------------------------------------------------------
	///	Enemy2のlongステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2longState)

	void Enemy2longState::Enter(const shared_ptr<Enemy2>& Obj) {
		auto PtrWait = Obj->GetBehavior<WaitBehavior>();
		PtrWait->Enter();
	}

	void Enemy2longState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrWait = Obj->GetBehavior<WaitBehavior>();
		float f = PtrWait->Execute();
		if (f < Obj->GetLongMediumChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2MediumState::Instance());
		}
	}

	void Enemy2longState::Exit(const shared_ptr<Enemy2>& Obj) {
		//何もしない
	}



	//--------------------------------------------------------------------------------------
	///	Enemy2のMediumステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2MediumState)

	void Enemy2MediumState::Enter(const shared_ptr<Enemy2>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		PtrSeek->Enter(L"Player");
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		PtrJump->StartJump(Vector3(0, 4.0f, 0));
	}

	void Enemy2MediumState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrUtil = Obj->GetBehavior<UtilBehavior>();
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		if (PtrJump->Execute()) {
			PtrJump->StartJump(Vector3(0, 4.0f, 0));
		}
		float f = PtrSeek->Execute(L"Player",true,0.1f);
		if (f < Obj->GetMediumShortChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2ShortState::Instance());
		}
		else if (f >= Obj->GetLongMediumChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2longState::Instance());
		}
	}

	void Enemy2MediumState::Exit(const shared_ptr<Enemy2>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		PtrSeek->Exit();
	}

	//--------------------------------------------------------------------------------------
	///	Enemy2のShortステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2ShortState)

	void Enemy2ShortState::Enter(const shared_ptr<Enemy2>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		PtrArrive->Enter(L"Player");
	}

	void Enemy2ShortState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		if (PtrArrive->Execute(L"Player",true,0.1f) >= Obj->GetMediumShortChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2MediumState::Instance());
		}
	}

	void Enemy2ShortState::Exit(const shared_ptr<Enemy2>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveBehavior>();
		PtrArrive->Exit();
	}

	//--------------------------------------------------------------------------------------
	///	敵３
	//--------------------------------------------------------------------------------------
	Enemy3::Enemy3(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_DefaultNearChange(7.0f)
	{}

	//初期化
	void Enemy3::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek操舵
		auto PtrSeek = AddComponent<SeekSteering>();

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<GameObject>());
		//分離行動をつける
		AddComponent<SeparationSteering>(Group);
		auto PtrColl = AddComponent<CollisionSphere>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParentRepel);

		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"WALL_TX");
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<Enemy3>(GetThis<Enemy3>()));
		//最初のステートをEnemy3DefaultStateに設定
		m_StateMachine->ChangeState(Enemy3DefaultState::Instance());
	}

	void Enemy3::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	//--------------------------------------------------------------------------------------
	///	Enemy3のDefaultステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy3DefaultState)

	void Enemy3DefaultState::Enter(const shared_ptr<Enemy3>& Obj) {
		//何もしない
	}

	void Enemy3DefaultState::Execute(const shared_ptr<Enemy3>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		float f = PtrSeek->Execute(L"Player", true, 0.1f);
		if (f < Obj->GetDefaultNearChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy3NearState::Instance());
		}
	}

	void Enemy3DefaultState::Exit(const shared_ptr<Enemy3>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	Enemy3のNearステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy3NearState)

	void Enemy3NearState::Enter(const shared_ptr<Enemy3>& Obj) {
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		PtrJump->StartJump(Vector3(0, 3.0f, 0));
	}

	void Enemy3NearState::Execute(const shared_ptr<Enemy3>& Obj) {
		auto PtrJump = Obj->GetBehavior<JumpBehavior>();
		if (PtrJump->Execute()) {
			PtrJump->StartJump(Vector3(0, 3.0f, 0));
		}
		auto PtrSeek = Obj->GetBehavior<SeekBehavior>();
		float f = PtrSeek->Execute(L"Player", true, 0.1f);
		if (f >= Obj->GetDefaultNearChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy3DefaultState::Instance());
		}
	}

	void Enemy3NearState::Exit(const shared_ptr<Enemy3>& Obj) {
		//何もしない
	}



}
//end basecross
