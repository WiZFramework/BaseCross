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
		auto PtrDraw = AddComponent<PCStaticDraw>();
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
		Beh->ExpandAndContract<VertexPositionColor, PCStaticDraw>();
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
		auto PtrDraw = AddComponent<PTStaticDraw>();
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
		Beh->ExpandAndContract<VertexPositionTexture, PTStaticDraw>();
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
		auto PtrDraw = AddComponent<PCTStaticDraw>();
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
		Beh->ExpandAndContract<VertexPositionColorTexture, PCTStaticDraw>();
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
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetOwnShadowActive(true);
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
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetSpecular(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		if (m_TextureUse) {
			PtrDraw->SetTextureResource(L"SKY_TX");
		}
	}


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
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"MODEL_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		auto PtrDraw = AddComponent<PNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"MODEL_MESH");
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
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();

		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"Chara_R_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<PNTBoneModelDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"Chara_R_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

		PtrDraw->AddAnimation(L"Default", 0, 50, true, 20.0f);
		PtrDraw->ChangeCurrentAnimation(L"Default");

		//透明処理
		SetAlphaActive(true);

	}

	//更新
	void BoneChara::OnUpdate() {
		//アニメーションを更新する
		auto PtrDraw = GetComponent<PNTBoneModelDraw>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		PtrDraw->UpdateAnimation(ElapsedTime);
	}


}
//end basecross
