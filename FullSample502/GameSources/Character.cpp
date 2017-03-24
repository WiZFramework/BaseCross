/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	///	Staticキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticChara::StaticChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	StaticChara::~StaticChara() {}
	//初期化
	void StaticChara::OnCreate() {
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Matrix4X4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"MODEL_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"MODEL_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);


	}

	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	BoneChara::~BoneChara() {}

	//初期化
	void BoneChara::OnCreate() {
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Matrix4X4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();

		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"BONE_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTBoneModelDraw>();
		PtrDraw->SetFogEnabled(true);
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"BONE_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);
		PtrDraw->AddAnimation(L"Default", 0, 40, true, 10.0f);
		PtrDraw->ChangeCurrentAnimation(L"Default");
	}

	//更新
	void BoneChara::OnUpdate() {
		//アニメーションを更新する
		auto PtrDraw = GetComponent<BcPNTBoneModelDraw>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		PtrDraw->UpdateAnimation(ElapsedTime);
	}


}
//end basecross
