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
	StaticChara::StaticChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	StaticChara::~StaticChara() {}
	//初期化
	void StaticChara::OnCreate() {
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Mat4x4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"MODEL_MESH_WITH_TAN");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		auto PtrDraw = AddComponent<BcPNTnTStaticModelDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"MODEL_MESH_WITH_TAN");
		PtrDraw->SetNormalMapTextureResource(L"MODEL_NORMAL_TX");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);


	}

	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	BoneChara::~BoneChara() {}

	//初期化
	void BoneChara::OnCreate() {
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Mat4x4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();

		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"BONE_MESH_WITH_TAN");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTnTBoneModelDraw>();
		PtrDraw->SetFogEnabled(true);
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"BONE_MESH_WITH_TAN");
		PtrDraw->SetNormalMapTextureResource(L"MODEL_NORMAL_TX");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);
		PtrDraw->AddAnimation(L"Default", 0, 30, true, 20.0f);
		PtrDraw->ChangeCurrentAnimation(L"Default");
	}

	//更新
	void BoneChara::OnUpdate() {
		//アニメーションを更新する
		auto PtrDraw = GetComponent<BcPNTnTBoneModelDraw>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		PtrDraw->UpdateAnimation(ElapsedTime);
	}


}
//end basecross
