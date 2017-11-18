/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	PC球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PcSphere::PcSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PcSphere::~PcSphere() {}

	//初期化
	void PcSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//描画コンポーネント
		auto PtrDraw = AddComponent<BcPCStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColor> new_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f,18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColor new_v;
			new_v.position = vertices[i].position;
			new_v.color = Col4(
				new_v.position.x * 2.0f, 
				new_v.position.y * 2.0f, 
				new_v.position.z * 2.0f, 
				1.0f);
			new_vertices.push_back(new_v);

		}
		PtrDraw->CreateOriginalMesh(new_vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(PtrDraw->GetOriginalMeshResource());
	}

	void PcSphere::OnUpdate() {
		auto Beh = GetBehavior<VertexBehavior>();
		Beh->ExpandAndContract<VertexPositionColor, BcPCStaticDraw>();
	}

	//--------------------------------------------------------------------------------------
	///	PT球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PtSphere::PtSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PtSphere::~PtSphere() {}

	//初期化
	void PtSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//描画コンポーネント
		auto PtrDraw = AddComponent<BcPTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionTexture> new_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionTexture new_v;
			new_v.position = vertices[i].position;
			new_v.textureCoordinate = vertices[i].textureCoordinate;
			new_vertices.push_back(new_v);
		}
		PtrDraw->CreateOriginalMesh(new_vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(PtrDraw->GetOriginalMeshResource());
	}
	void PtSphere::OnUpdate() {
		auto Beh = GetBehavior<VertexBehavior>();
		Beh->ExpandAndContract<VertexPositionTexture, BcPTStaticDraw>();
	}


	//--------------------------------------------------------------------------------------
	///	PCT球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PctSphere::PctSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PctSphere::~PctSphere() {}

	//初期化
	void PctSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//描画コンポーネント
		auto PtrDraw = AddComponent<BcPCTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColorTexture> new_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColorTexture new_v;
			new_v.position = vertices[i].position;
			new_v.textureCoordinate = vertices[i].textureCoordinate;
			new_v.color = Col4(
				new_v.position.x * 2.0f,
				new_v.position.y * 2.0f,
				new_v.position.z * 2.0f,
				1.0f);
			new_vertices.push_back(new_v);
		}
		PtrDraw->CreateOriginalMesh(new_vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(PtrDraw->GetOriginalMeshResource());
	}

	void PctSphere::OnUpdate() {
		auto Beh = GetBehavior<VertexBehavior>();
		Beh->ExpandAndContract<VertexPositionColorTexture, BcPCTStaticDraw>();
	}



	//--------------------------------------------------------------------------------------
	///	Pnt球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PntSphere::PntSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TextureUse(TextureUse)
	{
	}
	PntSphere::~PntSphere() {}

	//初期化
	void PntSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//描画コンポーネント
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		if (m_TextureUse) {
			PtrDraw->SetTextureResource(L"SKY_TX");
		}
	}

	//--------------------------------------------------------------------------------------
	///	Pntスペキュラー球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PntSpSphere::PntSpSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TextureUse(TextureUse)
	{
	}
	PntSpSphere::~PntSpSphere() {}
	//初期化
	void PntSpSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネント
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetSpecularColorAndPower(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		if (m_TextureUse) {
			PtrDraw->SetTextureResource(L"SKY_TX");
		}
	}


	//--------------------------------------------------------------------------------------
	///	Staticキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticChara::StaticChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
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
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"MODEL_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		if (m_TamgentUse) {
			auto PtrDraw = AddComponent<BcPNTnTStaticModelDraw>();
			PtrDraw->SetFogEnabled(true);
			PtrDraw->SetMeshResource(L"MODEL_MESH_WITH_TAN");
			PtrDraw->SetNormalMapTextureResource(L"MODEL_NORMAL_TX");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);
		}
		else {
			auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
			PtrDraw->SetFogEnabled(true);
			PtrDraw->SetMeshResource(L"MODEL_MESH");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);
		}
	}

	//--------------------------------------------------------------------------------------
	///	Staticキャラ(マルチメッシュ版)
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticMultiMeshChara::StaticMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{}
	StaticMultiMeshChara::~StaticMultiMeshChara() {}

	//初期化
	void StaticMultiMeshChara::OnCreate() {
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Mat4x4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMultiMeshResource(L"ObjectOnly_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		if (m_TamgentUse) {
			auto PtrDraw = AddComponent<BcPNTnTStaticModelDraw>();
			PtrDraw->SetFogEnabled(true);
			PtrDraw->SetMultiMeshResource(L"ObjectOnly_MESH_WITH_TAN");
			PtrDraw->SetNormalMapTextureResource(L"OBJECT_NORMAL_TX");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);
		}
		else {
			auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
			PtrDraw->SetFogEnabled(true);
			PtrDraw->SetMultiMeshResource(L"ObjectOnly_MESH");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);
		}
	}




	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
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
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();

		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"Chara_R_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		if (m_TamgentUse) {
			//描画コンポーネントの設定
			auto PtrDraw = AddComponent<BcPNTnTBoneModelDraw>();
			PtrDraw->SetFogEnabled(true);
			//描画するメッシュを設定
			PtrDraw->SetMeshResource(L"Chara_R_MESH_WITH_TAN");
			PtrDraw->SetNormalMapTextureResource(L"Chara_R_NORMAL_TX");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);
			PtrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
			PtrDraw->ChangeCurrentAnimation(L"Default");
		}
		else {
			//描画コンポーネントの設定
			auto PtrDraw = AddComponent<BcPNTBoneModelDraw>();
			PtrDraw->SetFogEnabled(true);
			//描画するメッシュを設定
			PtrDraw->SetMeshResource(L"Chara_R_MESH");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);

			PtrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
			PtrDraw->ChangeCurrentAnimation(L"Default");

		}
		//透明処理
		SetAlphaActive(true);
	}

	//更新
	void BoneChara::OnUpdate() {
		//アニメーションを更新する
		if (m_TamgentUse) {
			auto PtrDraw = GetComponent<BcPNTnTBoneModelDraw>();
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			PtrDraw->UpdateAnimation(ElapsedTime);

		}
		else {
			auto PtrDraw = GetComponent<BcPNTBoneModelDraw>();
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			PtrDraw->UpdateAnimation(ElapsedTime);
		}
	}


	//--------------------------------------------------------------------------------------
	///	Boneキャラ(マルチメッシュ版)
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BoneMultiMeshChara::BoneMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{
	}
	BoneMultiMeshChara::~BoneMultiMeshChara() {}

	//初期化
	void BoneMultiMeshChara::OnCreate() {
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Mat4x4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();

		//影の形（メッシュ）を設定
		ShadowPtr->SetMultiMeshResource(L"Object_WalkAnimation_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		if (m_TamgentUse) {
			//描画コンポーネントの設定
			auto PtrDraw = AddComponent<BcPNTnTBoneModelDraw>();
			PtrDraw->SetFogEnabled(true);
			//描画するメッシュを設定
			PtrDraw->SetMultiMeshResource(L"Object_WalkAnimation_MESH_WITH_TAN");
			PtrDraw->SetNormalMapTextureResource(L"OBJECT_NORMAL_TX");
			PtrDraw->SetMeshToTransformMatrix(SpanMat);
			PtrDraw->AddAnimation(L"Default", 0, 30, true, 10.0f);
			PtrDraw->ChangeCurrentAnimation(L"Default");
		}
		else {
			//描画コンポーネントの設定
			auto PtrDraw = AddComponent<BcPNTBoneModelDraw>();
			PtrDraw->SetFogEnabled(true);
			//描画するメッシュを設定
			PtrDraw->SetMultiMeshResource(L"Object_WalkAnimation_MESH");
			PtrDraw->SetSamplerState(SamplerState::LinearWrap);
			PtrDraw->SetMeshToTransformMatrix(SpanMat);

			PtrDraw->AddAnimation(L"Default", 0, 30, true, 10.0f);
			PtrDraw->ChangeCurrentAnimation(L"Default");

		}
	}

	//更新
	void BoneMultiMeshChara::OnUpdate() {
		//アニメーションを更新する
		if (m_TamgentUse) {
			auto PtrDraw = GetComponent<BcPNTnTBoneModelDraw>();
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			PtrDraw->UpdateAnimation(ElapsedTime);

		}
		else {
			auto PtrDraw = GetComponent<BcPNTBoneModelDraw>();
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			PtrDraw->UpdateAnimation(ElapsedTime);
		}
	}


}
//end basecross
