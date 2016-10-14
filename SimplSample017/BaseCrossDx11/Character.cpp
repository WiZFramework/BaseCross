/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class StaticModel : public GameObject;
	//	用途: 固定のモデル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticModel::StaticModel(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	StaticModel::~StaticModel() {}

	//初期化
	void StaticModel::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		Matrix4X4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f)
		);


		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"Chara_Rst_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		auto PtrDraw = AddComponent<PNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"Chara_Rst_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

	}


}
//end basecross
