/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	物理計算するボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PhysicsBox::PhysicsBox(const shared_ptr<Stage>& StagePtr,
		PsMotionType MotionType,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_MotionType(MotionType),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{
	}
	PhysicsBox::~PhysicsBox() {}

	//初期化
	void PhysicsBox::OnCreate() {
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

		auto PSPtr = GetStage()->GetPhysicsManager();
		m_PsTransIndex = PSPtr->AddSingleShape(PsShapeType::ShapeTypeBox, m_MotionType,
			m_Scale, m_Qt, m_Position);

	}

	void PhysicsBox::OnUpdate() {
		auto PSPtr = GetStage()->GetPhysicsManager();
		Quat Qt;
		Vec3 Pos;

		PSPtr->GetShapeQuatPos(m_PsTransIndex, Qt, Pos);
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(Qt);
		PtrTransform->SetPosition(Pos);
	}

}
//end basecross
