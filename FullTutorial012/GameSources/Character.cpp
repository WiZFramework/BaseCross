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
		param.m_HalfSize = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsBox = GetStage()->GetPhysicsManager()->AddSingleBox(param);

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
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsBox::~ActivePsBox() {}
	//初期化
	void ActivePsBox::OnCreate() {

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
		//basecrossのスケーリングは各辺基準なので、ハーフサイズ基準にする
		param.m_HalfSize = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsBox = GetStage()->GetPhysicsManager()->AddSingleBox(param);

	}

	void ActivePsBox::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsBox->GetIndex(), Status);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(Status.m_Orientation);
		PtrTransform->SetPosition(Status.m_Position);
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
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//初期化
	void ActivePsSphere::OnCreate() {

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
		//basecrossのスケーリングは直径基準なので、半径基準にする
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSingleSphere(param);

	}

	void ActivePsSphere::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Status.m_Orientation);
		PtrTransform->SetPosition(Status.m_Position);
	}

	//--------------------------------------------------------------------------------------
	///	物理計算する発射する球体
	//--------------------------------------------------------------------------------------
	FirePsSphere::FirePsSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity):
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity)
	{}
	FirePsSphere::~FirePsSphere() {}
	//初期化
	void FirePsSphere::OnCreate() {
		m_IndexKey = L"FirePsSphere";
		//共有オブジェクトにセット
		GetStage()->SetSharedGameObject(L"FirePsSphere",GetThis<FirePsSphere>());

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(0.25f));
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
		//basecrossのスケーリングは直径基準なので、半径基準にする
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = m_Emitter;
		param.m_Velocity = m_Velocity;
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSingleSphere(param, m_IndexKey);


	}
	//更新
	void FirePsSphere::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(Vec3(0.25f));
		PtrTransform->SetQuaternion(Status.m_Orientation);
		PtrTransform->SetPosition(Status.m_Position);

	}

	void FirePsSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		//現在のインデックスを得る
		auto Index = m_PhysicsSphere->GetIndex();
		PsSphereParam param;
		//basecrossのスケーリングは直径基準なので、半径基準にする
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = Emitter;
		param.m_Velocity = Velocity;
		//同じインデックスで再構築
		m_PhysicsSphere = ObjectFactory::Create<PhysicsSphere>(param, Index);
	}






}
//end basecross
