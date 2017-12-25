/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ライン
	//--------------------------------------------------------------------------------------
	ActionLine::ActionLine(const shared_ptr<Stage>& StagePtr, const shared_ptr<GameObject>& StartObj,
		const shared_ptr<GameObject>& EndObj) :
		GameObject(StagePtr),
		m_StartObj(StartObj),
		m_EndObj(EndObj)
	{}




	//初期化
	void ActionLine::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(Vec3(0.0f, 0.0f, 0.0f));

		//描画コンポーネント
		auto PtrDraw = AddComponent<PCStaticDraw>();

		auto StartPos = m_StartObj.lock()->GetComponent<Transform>()->GetWorldPosition();
		auto EndPos = m_EndObj.lock()->GetComponent<Transform>()->GetWorldPosition();

		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(StartPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) },
			{ VertexPositionColor(EndPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) }

		};
		vector<uint16_t> indices = {
			0, 1
		};

		auto MeshRes = MeshResource::CreateMeshResource(vertices, indices, true);
		MeshRes->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		PtrDraw->SetOriginalMeshResource(MeshRes);
		PtrDraw->SetOriginalMeshUse(true);

	}

	void ActionLine::OnUpdate() {
		auto StartPos = m_StartObj.lock()->GetComponent<Transform>()->GetWorldPosition();
		auto EndPos = m_EndObj.lock()->GetComponent<Transform>()->GetWorldPosition();

		auto PtrDraw = GetComponent<PCStaticDraw>();
		const vector<VertexPositionColor>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<VertexPositionColor>();
		vector<VertexPositionColor> new_vec;
		VertexPositionColor new_v;
		new_v = BackupVec[0];
		new_v.position = StartPos;
		new_vec.push_back(new_v);

		new_v = BackupVec[1];
		new_v.position = EndPos;
		new_vec.push_back(new_v);

		PtrDraw->UpdateVertices(new_vec);

	}



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
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsSingleBoxBody>(param);
		PsPtr->SetDrawActive(true);

	}

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなオブジェクトの親
	//--------------------------------------------------------------------------------------
	//初期化
	void ActivePsObject::OnCreate() {
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<ActivePsObject>(GetThis<ActivePsObject>()));
		//最初のステートをPlayerDefaultに設定
		m_StateMachine->ChangeState(ActivePsDefaultState::Instance());
	}

	void ActivePsObject::SetHold(bool b) {
		if (b) {
			if (m_StateMachine->GetCurrentState() == ActivePsDefaultState::Instance()) {
				m_StateMachine->ChangeState(ActivePsHoldState::Instance());
				//プレイヤーに自分がホールドされていることを伝える
				auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player", false);
				if (PlayerPtr) {
					PlayerPtr->SetHoldObject(GetThis<ActivePsObject>());
				}
			}
		}
		else {
			if (m_StateMachine->GetCurrentState() == ActivePsHoldState::Instance()) {
				m_StateMachine->ChangeState(ActivePsDefaultState::Instance());
			}
		}
	}


	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(ActivePsDefaultState)
	void ActivePsDefaultState::Enter(const shared_ptr<ActivePsObject>& Obj) {
		Obj->SetHoldBehavior(false);
	}

	void ActivePsDefaultState::Execute(const shared_ptr<ActivePsObject>& Obj) {
	}

	void ActivePsDefaultState::Exit(const shared_ptr<ActivePsObject>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	選択ステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(ActivePsHoldState)

	void ActivePsHoldState::Enter(const shared_ptr<ActivePsObject>& Obj) {
		Obj->SetHoldBehavior(true);
	}

	void ActivePsHoldState::Execute(const shared_ptr<ActivePsObject>& Obj) {
	}

	void ActivePsHoldState::Exit(const shared_ptr<ActivePsObject>& Obj) {
		//何もしない
	}


	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActivePsBox::ActivePsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		ActivePsObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsBox::~ActivePsBox() {}
	//初期化
	void ActivePsBox::OnCreate() {
		//親クラスのOnCreateを呼ぶ
		ActivePsObject::OnCreate();

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
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsSingleBoxBody>(param);
		PsPtr->SetDrawActive(true);
	}

	void ActivePsBox::SetHoldBehavior(bool b) {
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		if (b) {
			PtrDraw->SetEmissive(Col4(1.0f, 1.0f, 0, 0));

		}
		else {
			PtrDraw->SetEmissive(Col4(0.0f, 0.0f, 0, 0));
		}
	}



	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな球体
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActivePsSphere::ActivePsSphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		ActivePsObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//初期化
	void ActivePsSphere::OnCreate() {
		//親クラスのOnCreateを呼ぶ
		ActivePsObject::OnCreate();

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//物理計算球体
		PsSphereParam param;
		//DEFAULT_SPHEREのスケーリングは直径基準なので、半径にする
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsSingleSphereBody>(param);
		PsPtr->SetDrawActive(true);
	}

	void ActivePsSphere::SetHoldBehavior(bool b) {
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		if (b) {
			PtrDraw->SetEmissive(Col4(1.0f, 1.0f, 0, 0));

		}
		else {
			PtrDraw->SetEmissive(Col4(0.0f, 0.0f, 0, 0));
		}
	}


	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなカプセル
	//--------------------------------------------------------------------------------------
	ActivePsCapsule::ActivePsCapsule(const shared_ptr<Stage>& StagePtr,
		float Len,
		float Diameter,
		const Quat& Qt,
		const Vec3& Position
	):
		ActivePsObject(StagePtr),
		m_Len(Len),
		m_Diameter(Diameter),
		m_Qt(Qt),
		m_Position(Position)
	{}
	ActivePsCapsule::~ActivePsCapsule(){}

	//初期化
	void ActivePsCapsule::OnCreate() {
		//親クラスのOnCreateを呼ぶ
		ActivePsObject::OnCreate();

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		bsm::Vec3 PointA(0, 0, 0);
		bsm::Vec3 PointB(0, 0, 0);
		PointA -= bsm::Vec3(0,m_Len * 0.5f, 0);
		PointB += bsm::Vec3(0, m_Len * 0.5f, 0);
		MeshUtill::CreateCapsule(m_Diameter,
			PointA, PointB, 18, vertices, indices,true);
		m_CapsuleMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		auto PtrTransform = GetComponent<Transform>();
		//スケーリングは1.0f
		PtrTransform->SetScale(Vec3(1.0f));
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(m_CapsuleMesh);

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(m_CapsuleMesh);
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//物理計算カプセル
		PsCapsuleParam param;
		//半径にする
		param.m_HalfLen = m_Len * 0.5f;
		param.m_Radius = m_Diameter * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsSingleCapsuleBody>(param);
		PsPtr->SetDrawActive(true);
	}

	void ActivePsCapsule::SetHoldBehavior(bool b) {
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		if (b) {
			PtrDraw->SetEmissive(Col4(1.0f, 1.0f, 0, 0));

		}
		else {
			PtrDraw->SetEmissive(Col4(0.0f, 0.0f, 0, 0));
		}
	}

	//--------------------------------------------------------------------------------------
	///	物理計算しない発射する球体
	//--------------------------------------------------------------------------------------
	FireSphere::FireSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity
	):
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f)
	{}
	FireSphere::~FireSphere() {}

	void FireSphere::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);
		//コリジョンを付ける（ボリューム取得のため）
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetIsHitAction(IsHitAction::None);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");

		GetStage()->SetSharedGameObject(L"FireSphere", GetThis<GameObject>());
	}

	void FireSphere::OnUpdate() {
		auto PtrTransform = GetComponent<Transform>();
		if (PtrTransform->GetPosition().y > -20.0f) {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			Vec3 Ac = Vec3(0, -9.8f, 0) * 1.0f;
			m_Velocity += Ac * ElapsedTime;
			auto Pos = PtrTransform->GetPosition();
			Pos += m_Velocity* ElapsedTime;
			PtrTransform->SetPosition(Pos);
		}
		else {
			//じっとしている
			PtrTransform->SetPosition(Vec3(0,-20.0f,0));
		}
		auto Coll = GetComponent<CollisionSphere>();
		//物理オブジェクトを持つ配列の取得
		vector<shared_ptr<PsBodyComponent>> PsComptVec;
		GetStage()->GetUsedDynamicCompoentVec<PsBodyComponent>(PsComptVec);
		for (auto& v : PsComptVec) {
			auto g_ptr = dynamic_pointer_cast<ActivePsObject>(v->GetGameObject());
			if (g_ptr) {
				if (v->CollisionTest(Coll->GetSphere())) {
					auto h_ptr = m_HoldObject.lock();
					if (h_ptr) {
						h_ptr->SetHold(false);
					}
					m_HoldObject = g_ptr;
					g_ptr->SetHold(true);
					PtrTransform->SetPosition(Vec3(0, -20, 0));
					break;
				}
			}
		}
	}

	void FireSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(Emitter);
		m_Velocity = Velocity;

	}

}
//end basecross
