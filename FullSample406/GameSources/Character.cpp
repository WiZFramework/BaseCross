/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	RollingBoxグループ(インスタンス描画のためだけのオブジェクト)
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RollingBoxGroup::RollingBoxGroup(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	RollingBoxGroup::~RollingBoxGroup() {}

	//初期化
	void RollingBoxGroup::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(Vector3(0, 0, 0));
		//影をつける(自己影用のシェーダを使うための理由)
		auto ShadowPtr = AddComponent<Shadowmap>();
		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticInstanceDraw>();
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		//毎ターン毎に行列をクリアする
		PtrDraw->SetAutoClearMatrixVec(true);
	}


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定の回転ボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RollingBox::RollingBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position,
		float RotParam
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position),
		m_RotParam(RotParam)
	{
	}
	RollingBox::~RollingBox() {}

	//初期化
	void RollingBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//衝突判定
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto GroupPtr = GetStage()->GetSharedGameObject<RollingBoxGroup>(L"RollingBoxGroup");
		auto DrawPtr = GroupPtr->GetComponent<PNTStaticInstanceDraw>();
		DrawPtr->AddMatrix(PtrTransform->GetWorldMatrix());

		AddTag(L"MoveBox");

	}

	void RollingBox::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Quaternion QtSpan(Vector3(0, 1, 0), ElapsedTime * m_RotParam);
		Qt *= QtSpan;
		PtrTransform->SetQuaternion(Qt);
		auto GroupPtr = GetStage()->GetSharedGameObject<RollingBoxGroup>(L"RollingBoxGroup");
		auto DrawPtr = GroupPtr->GetComponent<PNTStaticInstanceDraw>();
		DrawPtr->AddMatrix(PtrTransform->GetWorldMatrix());
	}




}
//end basecross
