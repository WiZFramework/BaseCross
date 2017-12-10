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
		Quat Qt;
		Vec3 Pos;
		GetStage()->GetPhysicsManager()->GetBodyWorldQuatPos(m_PhysicsBox->GetIndex(),Qt,Pos);

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(Qt);
		PtrTransform->SetPosition(Pos);
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
		Quat Qt;
		Vec3 Pos;
		GetStage()->GetPhysicsManager()->GetBodyWorldQuatPos(m_PhysicsSphere->GetIndex(), Qt, Pos);

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Qt);
		PtrTransform->SetPosition(Pos);
	}






}
//end basecross
