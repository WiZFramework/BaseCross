/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	物理計算する固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	):
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox(){}
	//初期化
	void FixedPsBox::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//物理計算ボックス
		PsBoxParam param;
		//DEFAULT_CUBEのスケーリングは各辺基準なので、ハーフサイズにする
		param.m_HalfSize = m_Scale * 0.5f;
		//固定なので質量はいらない
		param.m_Mass = 0.0f;
		//慣性テンソルもデフォルトで良い
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsBoxBody>(param);
		PsPtr->SetDrawActive(true);
	}

	//--------------------------------------------------------------------------------------
	///	追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_StateChangeSize(3.0f),
		m_MaxForce(30.0f),
		m_MaxSpeed(10.0f),
		m_Decl(3.0f)
	{
	}
	SeekObject::~SeekObject() {}

	//初期化
	void SeekObject::OnCreate() {
		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"SeekObjectGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<SeekObject>());
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//最初のステートをSeekFarStateに設定
		m_StateMachine->ChangeState(FarState::Instance());
	}

	//操作
	void SeekObject::OnUpdate() {
		//フォースの初期化
		m_Force = Vec3(0);
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	float SeekObject::GetTargetToLen() const {
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto TargetTrans = GetStage()->GetSharedObject(L"Player")->GetComponent<Transform>();
		auto TargetPos = TargetTrans->GetPosition();
		return length(TargetPos - Pos);
	}

	void SeekObject::SeekBehavior() {
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto TargetTrans = GetStage()->GetSharedObject(L"Player")->GetComponent<Transform>();
		auto TargetPos = TargetTrans->GetPosition();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Seek(GetVelocity(), TargetPos, Pos, m_MaxSpeed);
		Steering::AccumulateForce(m_Force, WorkForce, m_MaxForce);
	}

	void SeekObject::ArriveBehavior() {
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto TargetTrans = GetStage()->GetSharedObject(L"Player")->GetComponent<Transform>();
		auto TargetPos = TargetTrans->GetPosition();
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Arrive(GetVelocity(), TargetPos,
			Pos, m_MaxSpeed, m_Decl);
		Steering::AccumulateForce(m_Force, WorkForce, m_MaxForce);
	}


	void SeekObject::SeparationBehavior() {
		auto Group = GetStage()->GetSharedObjectGroup(L"SeekObjectGroup");
		bsm::Vec3 WorkForce;
		WorkForce = Steering::Separation(Group, GetThis<GameObject>());
		Steering::AccumulateForce(m_Force, WorkForce, m_MaxForce);
	}



	//--------------------------------------------------------------------------------------
	///	プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->SeparationBehavior();
		Obj->SeekBehavior();
		if (Obj->GetTargetToLen() < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->SeparationBehavior();
		Obj->ArriveBehavior();
		if (Obj->GetTargetToLen() >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな凸面オブジェクト
	//--------------------------------------------------------------------------------------
	//static変数の初期化
	shared_ptr<MeshResource> ActivePsConvex::m_ConvexMesh = nullptr;
	shared_ptr<PsConvexMeshResource> ActivePsConvex::m_PsConvexMesh = nullptr;

	ActivePsConvex::ActivePsConvex(const shared_ptr<Stage>& StagePtr,
		const Vec3& Position
	) :
		SeekObject(StagePtr),
		m_Position(Position)
	{}
	ActivePsConvex::~ActivePsConvex() {}

	void ActivePsConvex::OnCreate() {
		if (!m_ConvexMesh || !m_PsConvexMesh) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateDodecahedron(0.5, vertices, indices);
			m_ConvexMesh = MeshResource::CreateMeshResource(vertices, indices, false);
			m_PsConvexMesh = PsConvexMeshResource::CreateMeshResource(vertices, indices);
		}

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(1.0f));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Position);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(m_ConvexMesh);

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(m_ConvexMesh);
		PtrDraw->SetTextureResource(L"WALL_TX");

		//物理計算凸面
		PsConvexParam param;
		param.m_ConvexMeshResource = m_PsConvexMesh;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算(球と同じにする)
		param.m_Inertia = BasePhysics::CalcInertiaSphere(0.5f, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = Quat();
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsConvexBody>(param);
		PsPtr->SetDrawActive(true);
		//親クラスのOnCreateを呼ぶ
		SeekObject::OnCreate();
	}

	Vec3 ActivePsConvex::GetVelocity()const {
		auto PtrPs = GetComponent<PsConvexBody>();
		return PtrPs->GetLinearVelocity();
	}

	//更新
	void ActivePsConvex::OnUpdate() {
		//親クラスのOnUpdateを呼ぶ
		SeekObject::OnUpdate();
		auto PtrPs = GetComponent<PsConvexBody>();
		//現在のフォースを設定
		PtrPs->ApplyForce(GetForce());
	}




	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな球体
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActivePsSphere::ActivePsSphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Vec3& Position
	) :
		SeekObject(StagePtr),
		m_Scale(Scale),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//初期化
	void ActivePsSphere::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Position);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"WALL_TX");

		//物理計算球体
		PsSphereParam param;
		//DEFAULT_SPHEREのスケーリングは直径基準なので、半径にする
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = Quat();
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsSphereBody>(param);
		PsPtr->SetDrawActive(true);
		//親クラスのOnCreateを呼ぶ
		SeekObject::OnCreate();
	}

	Vec3 ActivePsSphere::GetVelocity()const {
		auto PtrPs = GetComponent<PsSphereBody>();
		return PtrPs->GetLinearVelocity();
	}

	//更新
	void ActivePsSphere::OnUpdate() {
		//親クラスのOnUpdateを呼ぶ
		SeekObject::OnUpdate();
		auto PtrPs = GetComponent<PsSphereBody>();
		//現在のフォースを設定
		PtrPs->ApplyForce(GetForce());
	}


	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな合成オブジェクト
	//--------------------------------------------------------------------------------------
	ActivePsCombinedObject::ActivePsCombinedObject(const shared_ptr<Stage>& StagePtr,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsCombinedObject::~ActivePsCombinedObject() {}

	void ActivePsCombinedObject::CreateDrawComp(const PsCombinedParam& param) {
		//表示用のカプセルメッシュの作成
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		bsm::Vec3 PointA(0, 0, 0);
		bsm::Vec3 PointB(0, 0, 0);
		PointA -= bsm::Vec3(0, param.m_Primitives[2].m_HalfLen, 0);
		PointB += bsm::Vec3(0, param.m_Primitives[2].m_HalfLen, 0);
		MeshUtill::CreateCapsule(param.m_Primitives[2].m_Radius * 2.0f,
			PointA, PointB, 18, vertices, indices, true);
		m_CapsuleMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//このオブジェクトはマルチメッシュを使う
		m_MultiMeshResource = ObjectFactory::Create<MultiMeshResource>();
		auto index = m_MultiMeshResource->AddMesh(L"DEFAULT_CUBE");
		//メッシュごとのテクスチャの設定
		m_MultiMeshResource->SetTextureResource(L"WALL_TX", index);
		index = m_MultiMeshResource->AddMesh(L"DEFAULT_CUBE");
		m_MultiMeshResource->SetTextureResource(L"WALL_TX", index);
		index = m_MultiMeshResource->AddMesh(m_CapsuleMesh);
		m_MultiMeshResource->SetTextureResource(L"SKY_TX", index);
		//マルチメッシュを表示用に使うために設定
		for (size_t i = 0; i < param.m_Primitives.size();i++) {
			auto& v = param.m_Primitives[i];
			Mat4x4 m;
			if (i == 2) {
				//カプセルは拡大率1.0
				m.affineTransformation(
					Vec3(1.0f),
					Vec3(0, 0, 0),
					v.m_OffsetOrientation,
					v.m_OffsetPosition
				);
			}
			else {
				//ボックスは拡大率指定
				m.affineTransformation(
					v.m_HalfSize * 2.0f,
					Vec3(0, 0, 0),
					v.m_OffsetOrientation,
					v.m_OffsetPosition
				);
			}
			//Transformとメッシュの差分行列の設定
			m_MultiMeshResource->SetUseMeshToTransformMatrix(true, i);
			m_MultiMeshResource->SetMeshToTransformMatrix(m, i);
		}
		//描画コンポネント
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		//マルチメッシュリソースを設定
		PtrDraw->SetMultiMeshResource(m_MultiMeshResource);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMultiMeshResource(m_MultiMeshResource);
	}


	void ActivePsCombinedObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(1.0f));
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);
		//合成オブジェクトの準備
		PsCombinedParam param;
		//質量は重くする
		param.m_Mass = 3.0f;
		//Box用の慣性（慣性テンソル）を計算
		param.m_Inertia = BasePhysics::CalcInertiaBox(Vec3(2.5f, 1.0f, 1.0f), param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		//合成されるプリミティブ（0番目、ボックス）
		PsCombinedPrimitive primitive;
		primitive.reset();
		primitive.m_CombinedType = PsCombinedType::TypeBox;
		primitive.m_HalfSize = Vec3(0.5f, 0.5f, 1.5f);
		primitive.m_OffsetPosition = Vec3(-2.0f, 0.0f, 0.0f);
		//合成オブジェクトに追加
		param.AddPrim(primitive);
		//合成されるプリミティブ（1番目、ボックス）
		primitive.reset();
		primitive.m_CombinedType = PsCombinedType::TypeBox;
		primitive.m_HalfSize = Vec3(0.5f, 1.5f, 0.5f);
		primitive.m_OffsetPosition = Vec3(2.0f, 0.0f, 0.0f);
		//合成オブジェクトに追加
		param.AddPrim(primitive);
		//合成されるプリミティブ（2番目、カプセル）
		primitive.reset();
		primitive.m_CombinedType = PsCombinedType::TypeCapsule;
		primitive.m_HalfLen = 1.5f;
		primitive.m_Radius = 0.5f;
		primitive.m_OffsetPosition = Vec3(0.0f, 0.0f, 0.0f);
		//合成オブジェクトに追加
		param.AddPrim(primitive);
		//物理コンポーネント（合成）
		auto PsPtr = AddComponent<PsCombinedBody>(param);
		PsPtr->SetDrawActive(true);
		//物理コンポーネントに合わせて描画コンポーネント（影も）を作成
		CreateDrawComp(param);
	}




	//--------------------------------------------------------------------------------------
	///	物理計算する発射する球体
	//--------------------------------------------------------------------------------------
	FirePsSphere::FirePsSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity):
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f)
	{}
	FirePsSphere::~FirePsSphere() {}

	void FirePsSphere::CreateDefParam(PsSphereParam& param) {
		//DEFAULT_SPHEREのスケーリングは直径基準なので、半径にする
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		//スリープしない
		param.m_UseSleep = false;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
	}

	//初期化
	void FirePsSphere::OnCreate() {
		//共有オブジェクトにセット
		GetStage()->SetSharedGameObject(L"FirePsSphere",GetThis<FirePsSphere>());

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");

		PsSphereParam param;
		CreateDefParam(param);
		param.m_Pos = m_Emitter;
		param.m_LinearVelocity = m_Velocity;
		auto PsPtr = AddComponent<PsSphereBody>(param);
		PsPtr->SetDrawActive(true);
	}


	void FirePsSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		auto PsPtr = GetComponent<PsSphereBody>();
		PsSphereParam param;
		CreateDefParam(param);
		param.m_Pos = Emitter;
		param.m_LinearVelocity = Velocity;
		PsPtr->Reset(param, PsPtr->GetIndex());
	}






}
//end basecross
