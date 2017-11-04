/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	Tree::Tree(const shared_ptr<Stage>& StagePtr, const Vec3& Pos):
		GameObject(StagePtr),
		m_Pos(Pos)
	{}


	void Tree::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_Pos);
		PtrTransform->SetScale(0.5f, 1.0f, 0.5f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//capsuleの衝突判定をつける
		auto PtrColl = AddComponent<CollisionCapsule>();
		PtrColl->SetFixed(true);
//		PtrColl->SetDrawActive(true);

		shared_ptr<MeshResource> TreeMesh, LeafMesh1, LeafMesh2;
		if (!App::GetApp()->CheckResource<MeshResource>(L"TREE_MESH")) {
			TreeMesh =MeshResource::CreateCylinder(1.0f, 1.0f, 18, false);
			Mat4x4 TransToMat;
			TransToMat.affineTransformation(
				Vec3(1.0f,1.0f,1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
				);
			TreeMesh->SetMeshToTransformMatrix(TransToMat);
			TreeMesh->SetUseMeshToTransformMatrix(true);
			App::GetApp()->RegisterResource(L"TREE_MESH", TreeMesh);
		}
		else {
			TreeMesh = App::GetApp()->GetResource<MeshResource>(L"TREE_MESH");
		}
		//このメッシュ特有のテクスチャとして登録
		TreeMesh->SetTextureResource(L"BROWN_TX");


		if (!App::GetApp()->CheckResource<MeshResource>(L"LEAF_MESH_1")) {
			LeafMesh1 = MeshResource::CreateCone(1.0f, 1.0f,18,false);
			Mat4x4 TransToMat;
			TransToMat.affineTransformation(
				Vec3(3.0f, 0.5f, 3.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.4f, 0.0f)
			);
			LeafMesh1->SetMeshToTransformMatrix(TransToMat);
			LeafMesh1->SetUseMeshToTransformMatrix(true);
			App::GetApp()->RegisterResource(L"LEAF_MESH_1", LeafMesh1);
		}
		else {
			LeafMesh1 = App::GetApp()->GetResource<MeshResource>(L"LEAF_MESH_1");
		}
		//このメッシュ特有のテクスチャとして登録
		LeafMesh1->SetTextureResource(L"GREEN_TX");

		if (!App::GetApp()->CheckResource<MeshResource>(L"LEAF_MESH_2")) {
			LeafMesh2 = MeshResource::CreateCone(1.0f, 1.0f, 18, false);
			Mat4x4 TransToMat;
			TransToMat.affineTransformation(
				Vec3(2.0f, 0.5f, 2.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.7f, 0.0f)
			);
			LeafMesh2->SetMeshToTransformMatrix(TransToMat);
			LeafMesh2->SetUseMeshToTransformMatrix(true);
			App::GetApp()->RegisterResource(L"LEAF_MESH_2", LeafMesh2);
		}
		else {
			LeafMesh2 = App::GetApp()->GetResource<MeshResource>(L"LEAF_MESH_2");
		}
		//このメッシュ特有のテクスチャとして登録
		LeafMesh2->SetTextureResource(L"GREEN_TX");

		//このオブジェクトはマルチメッシュを使う
		m_MultiMeshResource = ObjectFactory::Create<MultiMeshResource>();
		m_MultiMeshResource->AddMesh(TreeMesh);
		m_MultiMeshResource->AddMesh(LeafMesh1);
		m_MultiMeshResource->AddMesh(LeafMesh2);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMultiMeshResource(m_MultiMeshResource);

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するマルチメッシュを設定
		PtrDraw->SetMultiMeshResource(m_MultiMeshResource);

	}



	//--------------------------------------------------------------------------------------
	///	敵１
	//--------------------------------------------------------------------------------------
	Enemy1::Enemy1(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos):
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
		//操舵系の行動をつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<GameObject>());
		//分離行動をつける
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//Obbの衝突判定をつける
		auto PtrColl = AddComponent<CollisionObb>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::Auto);

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
		//ステートによって変わらない行動を実行
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void Enemy1::OnUpdate2() {
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(0.2f);
	}


	//--------------------------------------------------------------------------------------
	///	Enemy1のFarステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy1FarState)

	void Enemy1FarState::Enter(const shared_ptr<Enemy1>& Obj) {
	}

	void Enemy1FarState::Execute(const shared_ptr<Enemy1>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		float f = PtrSeek->Execute(L"Player");
		if (f < Obj->GetNearFarChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy1NearState::Instance());
		}
	}

	void Enemy1FarState::Exit(const shared_ptr<Enemy1>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	Enemy1のNearステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy1NearState)

	void Enemy1NearState::Enter(const shared_ptr<Enemy1>& Obj) {
	}

	void Enemy1NearState::Execute(const shared_ptr<Enemy1>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetNearFarChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy1FarState::Instance());
		}
	}

	void Enemy1NearState::Exit(const shared_ptr<Enemy1>& Obj) {
	}


	//--------------------------------------------------------------------------------------
	///	敵2
	//--------------------------------------------------------------------------------------
	Enemy2::Enemy2(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
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
		//操舵系の行動をつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<GameObject>());
		//分離行動をつける
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);

		auto PtrColl = AddComponent<CollisionSphere>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::Auto);

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
		//ステートによって変わらない行動を実行
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void Enemy2::OnUpdate2() {
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(0.1f);
	}

	//衝突時
	void Enemy2::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		if (m_StateMachine->GetCurrentState() == Enemy2MediumState::Instance()) {
			auto PtrGrav = GetBehavior<Gravity>();
			PtrGrav->StartJump(Vec3(0, 4.0f, 0));
		}
	}



	//--------------------------------------------------------------------------------------
	///	Enemy2のlongステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2longState)

	void Enemy2longState::Enter(const shared_ptr<Enemy2>& Obj) {
	}

	void Enemy2longState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrWait = Obj->GetBehavior<WaitBehavior>();
		float f = PtrWait->Execute(L"Player");
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
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->StartJump(Vec3(0, 4.0f, 0));
	}

	void Enemy2MediumState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		float f = PtrSeek->Execute(L"Player");
		if (f < Obj->GetMediumShortChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2ShortState::Instance());
		}
		else if (f >= Obj->GetLongMediumChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2longState::Instance());
		}
	}

	void Enemy2MediumState::Exit(const shared_ptr<Enemy2>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	Enemy2のShortステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy2ShortState)

	void Enemy2ShortState::Enter(const shared_ptr<Enemy2>& Obj) {
	}

	void Enemy2ShortState::Execute(const shared_ptr<Enemy2>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetMediumShortChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy2MediumState::Instance());
		}
	}

	void Enemy2ShortState::Exit(const shared_ptr<Enemy2>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	敵３
	//--------------------------------------------------------------------------------------
	Enemy3::Enemy3(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
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
		//操舵系の行動をつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"EnemyGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<GameObject>());
		//分離行動をつける
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);

		auto PtrColl = AddComponent<CollisionSphere>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::Auto);

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
		//ステートによって変わらない行動を実行
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void Enemy3::OnUpdate2() {
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(0.1f);
	}


	//--------------------------------------------------------------------------------------
	///	Enemy3のDefaultステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(Enemy3DefaultState)

	void Enemy3DefaultState::Enter(const shared_ptr<Enemy3>& Obj) {
		//何もしない
	}

	void Enemy3DefaultState::Execute(const shared_ptr<Enemy3>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		float f = PtrSeek->Execute(L"Player");
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
		auto PtrGrav = Obj->GetBehavior<Gravity>();
		PtrGrav->StartJump(Vec3(0, 4.0f, 0));
	}

	void Enemy3NearState::Execute(const shared_ptr<Enemy3>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetDefaultNearChange()) {
			Obj->GetStateMachine()->ChangeState(Enemy3DefaultState::Instance());
		}
	}

	void Enemy3NearState::Exit(const shared_ptr<Enemy3>& Obj) {
		//何もしない
	}


}
//end basecross
