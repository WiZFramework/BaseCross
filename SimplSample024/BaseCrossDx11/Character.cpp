/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	平面実体
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const wstring& NormalTextureResName,
		const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_NormalTextureResName(NormalTextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::CreateBuffers(float WrapX, float WrapY) {
		float HelfSize = 0.5f;
		vector<VertexPositionNormalTexture> vertices = {
			{ VertexPositionNormalTexture(Vec3(-HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(-HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, WrapY)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, WrapY)) },

		};

		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
		};
		vector<VertexPositionNormalTangentTexture> new_vertices;
		BcRenderer::ConvertToNormalVertex(vertices, new_vertices);
		//メッシュの作成（変更できない）
		m_SquareMesh = MeshResource::CreateMeshResource(new_vertices, indices, false);
	}


	void SquareObject::OnCreate() {

		CreateBuffers(m_Scale.x, m_Scale.y);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);

		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto NormTexPtr = App::GetApp()->GetResource<TextureResource>(m_NormalTextureResName);

		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_SquareMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_NormalTextureRes = NormTexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_SamplerState = SamplerState::LinearWrap;
		m_PtrObj->m_ShadowmapUse = false;
		m_PtrObj->m_FogEnabled = true;
		//フォグはきつめに
		m_PtrObj->m_FogColor = Col4(0.3f, 0.3f, 0.3f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;


	}
	void SquareObject::OnUpdate() {
	}

	void SquareObject::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTnTStaticRenderer>(L"BcPNTnTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//用途: 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//初期化
	void MultiSpark::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
		//タグの追加
		AddTag(L"MultiSpark");
	}


	void MultiSpark::InsertSpark(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(64);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->m_MaxTime = 0.5f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalScale = Vec2(0.2, 0.2);
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 30.0f,
				rParticleSprite.m_LocalPos.y * 30.0f,
				rParticleSprite.m_LocalPos.z * 30.0f
			);
			//色の指定
			int r = (int)((Util::RandZeroToOne() * 0.5f + 0.3f) * 256.0f);
			int g = (int)((Util::RandZeroToOne() * 0.5f + 0.3f) * 256.0f);
			int b = (int)((Util::RandZeroToOne() * 0.5f + 0.3f) * 256.0f);
			r = r >> 7;
			g = g >> 7;
			b = b >> 7;
			rParticleSprite.m_Color = Col4((float)r, (float)g, (float)b, 1.0f);
		}
	}

	void MultiSpark::OnUpdate() {
		MultiParticle::OnUpdate();
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		for (auto ParticlePtr : GetParticleVec()) {
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_Color += 0.01f;
					rParticleSprite.m_Color.w = 1.0f;
					if (rParticleSprite.m_Color.x >= 1.0f) {
						rParticleSprite.m_Color.x = 1.0f;
					}
					if (rParticleSprite.m_Color.y >= 1.0f) {
						rParticleSprite.m_Color.y = 1.0f;
					}
					if (rParticleSprite.m_Color.z >= 1.0f) {
						rParticleSprite.m_Color.z = 1.0f;
					}
				}
			}
		}

	}


	//--------------------------------------------------------------------------------------
	//class MultiFire : public MultiParticle;
	//用途: 複数の炎クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiFire::MultiFire(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiFire::~MultiFire() {}

	//初期化
	void MultiFire::OnCreate() {
		//タグの追加
		AddTag(L"MultiFire");
	}

	void MultiFire::InsertFire(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(16);
		ParticlePtr->m_EmitterPos = Pos;
		ParticlePtr->SetTextureResource(L"FIRE_TX");
		ParticlePtr->m_MaxTime = 0.5f;
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	///	シンプルな3D上の平面
	//--------------------------------------------------------------------------------------
	void SimpleSquare::CreateBuffers(float WrapX, float WrapY) {
		float HelfSize = 0.5f;
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(WrapX, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, WrapY)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(WrapX, WrapY)) },

		};
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
		};
		//メッシュの作成（変更できる）
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);

	}

	//構築と破棄
	SimpleSquare::SimpleSquare(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName,
		const Vec3& Scale, const Vec3& Pos, SquareDrawOption Option):
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(),
		m_Pos(Pos),
		m_DrawOption(Option),
		m_TotalTime(0)
	{}
	SimpleSquare::~SimpleSquare() {}

	//初期化
	void SimpleSquare::OnCreate() {
		CreateBuffers(4.0f, 4.0f);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);

		//描画データの構築
		m_PtrObj = make_shared<SimpleDrawObject>();
		m_PtrObj->m_MeshRes = m_SquareMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_SamplerState = SamplerState::LinearWrap;

	}

	void SimpleSquare::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 1.0f) {
			m_TotalTime = 0;
		}
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vec2 UV(m_BackupVertices[i].textureCoordinate);
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				m_BackupVertices[i].color,
				UV
			);
		}
	}


	void SimpleSquare::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//頂点の変更
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pD3D11DeviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"SimpleSquare::OnUpdate()"
			);
		}
		//頂点の変更
		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		//関数呼び出し
		UpdateVertex(ElapsedTime, vertices);
		//アンマップ
		pD3D11DeviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);


		auto PtrGameStage = GetStage<GameStage>();
		//カメラの位置
		Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
		Vec3 CameraAt = PtrGameStage->GetCamera().m_CamerAt;
		switch (m_DrawOption) {
		case SquareDrawOption::Billboard:
		{
			m_Qt.facing(CameraAt - CameraEye);
		}
		break;
		case SquareDrawOption::Faceing:
		{
			m_Qt.facing(m_Pos - CameraEye);
		}
		break;
		case SquareDrawOption::FaceingY:
			m_Qt.facingY(m_Pos - CameraEye);
			break;
		default:
			m_Qt.normalize();
			break;
		}
	}

	void SimpleSquare::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		m_PtrObj->m_WorldMatrix = World;
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<SimplePCTStaticRenderer>(L"SimplePCTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}



	//--------------------------------------------------------------------------------------
	///	Staticキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticChara::StaticChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool OwnShadowActive) :
		GameObject(StagePtr),
		m_Scale(0.5f, 0.5f, 0.5f),
		m_Qt(),
		m_Pos(StartPos),
		m_OwnShadowActive(OwnShadowActive)
	{
	}
	StaticChara::~StaticChara() {}
	//初期化
	void StaticChara::OnCreate() {

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeCAPSULE;
		body.m_IsFixed = true;
//		body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		//メッシュ
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"MODEL_MESH");

		//描画データの構築
		m_PtrObj = make_shared<SimpleDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		//アフィン変換
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_UsedModelColor = false;
		m_PtrObj->m_UsedModelTextre = true;
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
	}

	void StaticChara::OnUpdate() {

	}

	void StaticChara::OnDrawShadowmap() {
		auto shptr = GetStage<GameStage>()->GetShadowmapRenderer();
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void StaticChara::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<SimplePNTStaticModelRenderer2>(L"SimplePNTStaticModelRenderer2");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool OwnShadowActive) :
		GameObject(StagePtr),
		m_Scale(0.5f, 0.5f, 0.5f),
		m_Qt(),
		m_Pos(StartPos),
		m_OwnShadowActive(OwnShadowActive)
	{
	}
	BoneChara::~BoneChara() {}
	//初期化
	void BoneChara::OnCreate() {

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeCAPSULE;
		body.m_IsFixed = true;
//		body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"Chara_R_MESH");
		//描画データの構築
		m_PtrObj = make_shared<SimpleDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_UsedModelColor = false;
		m_PtrObj->m_UsedModelTextre = true;
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->BoneInit();
		m_PtrObj->AddAnimation(L"Default", 0, 100, true, 20.0f);
		m_PtrObj->ChangeCurrentAnimation(L"Default");

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;

	}

	void BoneChara::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_PtrObj->UpdateAnimation(ElapsedTime);

	}

	void BoneChara::OnDrawShadowmap() {
		m_PtrShadowmapObj->m_pLocalBoneVec = &m_PtrObj->m_LocalBonesMatrix;
		auto shptr = GetStage<GameStage>()->GetShadowmapRenderer();
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void BoneChara::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<SimplePNTBoneModelRenderer2>(L"SimplePNTBoneModelRenderer2");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	//--------------------------------------------------------------------------------------
	///	Simple描画をする球体
	//--------------------------------------------------------------------------------------
	void SimpleSphereObject::CreateBuffers() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);

	}

	SimpleSphereObject::SimpleSphereObject(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos,
		bool OwnShadowActive) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_OwnShadowActive(OwnShadowActive)
		{}
	SimpleSphereObject::~SimpleSphereObject() {}

	void SimpleSphereObject::OnCreate() {
		CreateBuffers();

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsFixed = true;
//		body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);


		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<SimpleDrawObject>();
		m_PtrObj->m_MeshRes = m_SphereMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_ShadowmapUse = true;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_SphereMesh;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
	}

	void SimpleSphereObject::OnDrawShadowmap() {
		auto shptr = GetStage<GameStage>()->GetShadowmapRenderer();
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void SimpleSphereObject::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<SimplePNTStaticRenderer2>(L"SimplePNTStaticRenderer2");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}



	//--------------------------------------------------------------------------------------
	///	Basic描画をする球体
	//--------------------------------------------------------------------------------------
	void BcSphereObject::CreateBuffers() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);

	}

	BcSphereObject::BcSphereObject(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos):
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos){}
	BcSphereObject::~BcSphereObject() {}

	void BcSphereObject::OnCreate() {
		CreateBuffers();

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeSPHERE;
		body.m_IsFixed = true;
//		body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);


		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_SphereMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_OwnShadowmapActive = false;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは青色っぽく
		m_PtrObj->m_FogColor = Col4(0.4f, 0.4f, 0.8f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_SphereMesh;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
	}

	void BcSphereObject::OnDrawShadowmap() {
		auto shptr = GetStage<GameStage>()->GetShadowmapRenderer();
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void BcSphereObject::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	//--------------------------------------------------------------------------------------
	///	Basic法線マップ描画をするボックス
	//--------------------------------------------------------------------------------------

	BcBoxObject::BcBoxObject(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, const wstring& NormalTextureResName,
		const Vec3& Scale, const Quat& Qt, const Vec3& Pos):
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_NormalTextureResName(NormalTextureResName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos) {}
	BcBoxObject::~BcBoxObject() {}
	//初期化
	void BcBoxObject::OnCreate() {

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeOBB;
		body.m_IsFixed = true;
//		body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);


		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			body.m_Scale,
			Vec3(0, 0, 0),
			body.m_Quat,
			body.m_Pos
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto NormTexPtr = App::GetApp()->GetResource<TextureResource>(m_NormalTextureResName);
		//メッシュの取得
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"NORMAL_BOX");

		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_NormalTextureRes = NormTexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは黄色っぽく
		m_PtrObj->m_FogColor = Col4(0.5f, 0.5f, 0.1f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;


	}

	void BcBoxObject::OnDrawShadowmap() {
		auto shptr = GetStage<GameStage>()->GetShadowmapRenderer();
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void BcBoxObject::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTnTStaticRenderer>(L"BcPNTnTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}



	//--------------------------------------------------------------------------------------
	///	BcStaticキャラ（Basic描画をするstaticキャラ）
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BcStaticChara::BcStaticChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool OwnShadowActive) :
		GameObject(StagePtr),
		m_Scale(0.5f, 0.5f, 0.5f),
		m_Qt(),
		m_Pos(StartPos),
		m_OwnShadowActive(OwnShadowActive)
	{
	}
	BcStaticChara::~BcStaticChara() {}
	//初期化
	void BcStaticChara::OnCreate() {

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeCAPSULE;
		body.m_IsFixed = true;
//		body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"MODEL_MESH");
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_UsedModelColor = false;
		m_PtrObj->m_UsedModelTextre = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは黄色っぽく
		m_PtrObj->m_FogColor = Col4(0.5f, 0.5f, 0.1f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_WorldMatrix = World;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;


	}

	void BcStaticChara::OnUpdate() {

	}

	void BcStaticChara::OnDrawShadowmap() {
		auto shptr = GetStage<GameStage>()->GetShadowmapRenderer();
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void BcStaticChara::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTStaticModelRenderer>(L"BcPNTStaticModelRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}


	//--------------------------------------------------------------------------------------
	///	BcStaticNormalキャラ（Basic描画で法線マップを使うstaticキャラ）
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BcStaticNormalChara::BcStaticNormalChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool OwnShadowActive) :
		GameObject(StagePtr),
		m_Scale(0.5f, 0.5f, 0.5f),
		m_Qt(),
		m_Pos(StartPos),
		m_OwnShadowActive(OwnShadowActive)
	{
	}
	BcStaticNormalChara::~BcStaticNormalChara() {}

	//初期化
	void BcStaticNormalChara::OnCreate() {

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeCAPSULE;
		body.m_IsFixed = true;
//		body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"MODEL_MESH_WITH_TAN");
		auto NormTexPtr = App::GetApp()->GetResource<TextureResource>(L"MODEL_NORMAL_TX");


		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_UsedModelColor = false;
		m_PtrObj->m_UsedModelTextre = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは黄色っぽく
		m_PtrObj->m_FogColor = Col4(0.5f, 0.5f, 0.1f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_NormalTextureRes = NormTexPtr;
		m_PtrObj->m_WorldMatrix = World;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//法線マップを入れる
		m_PtrShadowmapObj->m_IsNormalmap = true;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;


	}
	void BcStaticNormalChara::OnUpdate() {
	}

	void BcStaticNormalChara::OnDrawShadowmap() {
		auto shptr = GetStage<GameStage>()->GetShadowmapRenderer();
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}

	void BcStaticNormalChara::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTnTStaticModelRenderer>(L"BcPNTnTStaticModelRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	//--------------------------------------------------------------------------------------
	///	BcBoneキャラ（Basic描画をするボーンキャラ）
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BcBoneChara::BcBoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool OwnShadowActive) :
		GameObject(StagePtr),
		m_Scale(0.5f, 0.5f, 0.5f),
		m_Qt(),
		m_Pos(StartPos),
		m_OwnShadowActive(OwnShadowActive)
	{
	}
	BcBoneChara::~BcBoneChara() {}
	//初期化
	void BcBoneChara::OnCreate() {

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeCAPSULE;
		body.m_IsFixed = true;
//		body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"Chara_R_MESH");

		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();

		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_UsedModelColor = false;
		m_PtrObj->m_UsedModelTextre = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは黄色っぽく
		m_PtrObj->m_FogColor = Col4(0.5f, 0.5f, 0.1f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->BoneInit();
		m_PtrObj->AddAnimation(L"Default", 0, 100, true, 20.0f);
		m_PtrObj->ChangeCurrentAnimation(L"Default");

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;

	}

	void BcBoneChara::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_PtrObj->UpdateAnimation(ElapsedTime);

	}

	void BcBoneChara::OnDrawShadowmap() {
		m_PtrShadowmapObj->m_pLocalBoneVec = &m_PtrObj->m_LocalBonesMatrix;
		auto shptr = GetStage<GameStage>()->GetShadowmapRenderer();
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void BcBoneChara::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTBoneModelRenderer>(L"BcPNTBoneModelRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}



	//--------------------------------------------------------------------------------------
	///	BcBoneNormalキャラ（Basic描画をするボーンキャラ）
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BcBoneNormalChara::BcBoneNormalChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool OwnShadowActive) :
		GameObject(StagePtr),
		m_Scale(0.5f, 0.5f, 0.5f),
		m_Qt(),
		m_Pos(StartPos),
		m_OwnShadowActive(OwnShadowActive)
	{
	}
	BcBoneNormalChara::~BcBoneNormalChara() {}
	//初期化
	void BcBoneNormalChara::OnCreate() {

		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = m_Scale;
		body.m_Quat = m_Qt;
		body.m_Pos = m_Pos;
		body.m_CollType = CollType::typeCAPSULE;
		body.m_IsFixed = true;
//		body.m_IsDrawActive = true;
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);

		//メッシュとトランスフォームの差分の設定
		m_MeshToTransformMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		//差分を計算
		World = m_MeshToTransformMatrix * World;
		auto MeshPtr = App::GetApp()->GetResource<MeshResource>(L"Chara_R_MESH_WITH_TAN");
		auto NormTexPtr = App::GetApp()->GetResource<TextureResource>(L"Chara_R_NORMAL_TX");

		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();

		m_PtrObj->m_OwnShadowmapActive = m_OwnShadowActive;
		m_PtrObj->m_UsedModelColor = false;
		m_PtrObj->m_UsedModelTextre = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは黄色っぽく
		m_PtrObj->m_FogColor = Col4(0.5f, 0.5f, 0.1f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;
		m_PtrObj->m_MeshRes = MeshPtr;
		m_PtrObj->m_NormalTextureRes = NormTexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->BoneInit();
		m_PtrObj->AddAnimation(L"Default", 0, 100, true, 20.0f);
		m_PtrObj->ChangeCurrentAnimation(L"Default");

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = MeshPtr;
		//法線マップを入れる
		m_PtrShadowmapObj->m_IsNormalmap = true;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;

	}

	void BcBoneNormalChara::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_PtrObj->UpdateAnimation(ElapsedTime);

	}

	void BcBoneNormalChara::OnDrawShadowmap() {
		m_PtrShadowmapObj->m_pLocalBoneVec = &m_PtrObj->m_LocalBonesMatrix;
		auto shptr = GetStage<GameStage>()->GetShadowmapRenderer();
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void BcBoneNormalChara::OnDraw() {
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTnTBoneModelRenderer>(L"BcPNTnTBoneModelRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}






	//--------------------------------------------------------------------------------------
	///	ラッピング処理されたスプライト
	//--------------------------------------------------------------------------------------
	SpriteBase::SpriteBase(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, 
		const Vec2& StartScale, 
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Scale(StartScale),
		m_Rot(StartRot),
		m_Pos(StartPos),
		m_XWrap(XWrap),
		m_YWrap(YWrap),
		m_Emissive(0,0,0,0),
		m_BlendState(BlendState::Opaque)
	{}
	void SpriteBase::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列(縦横指定数ずつ表示)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
		//頂点の初期修正（仮想関数呼びだし）
		AdjustVertex();
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成（変更できる）
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
	}

	void SpriteBase::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//頂点の変更
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pD3D11DeviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"WrappedSprite::UpdateVertex()"
			);
		}
		//頂点の変更
		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		//仮想関数呼び出し
		UpdateVertex(ElapsedTime, vertices);
		//アンマップ
		pD3D11DeviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);
	}



	void SpriteBase::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//ワールド行列の決定
		Mat4x4 World;
		World.affineTransformation2D(
			m_Scale,			//スケーリング
			Vec2(0, 0),		//回転の中心（重心）
			m_Rot,				//回転角度
			m_Pos				//位置
		);
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Mat4x4 Proj(XMMatrixOrthographicLH(w, h, -1.0, 1.0f));
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		SpriteConstantBuffer sb;
		//エミッシブ加算。
		sb.Emissive = m_Emissive;
		//行列の設定
		sb.World = World;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());
		//ブレンドステート
		switch (m_BlendState) {
		case BlendState::Opaque:
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			break;
		case BlendState::Trace:
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			break;
		case BlendState::Additive:
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
			break;
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, TexPtr->GetShaderResourceView().GetAddressOf());
		//ラッピングサンプラー
		ID3D11SamplerState* pSampler = RenderState->GetLinearWrap();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//描画
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}
	//--------------------------------------------------------------------------------------
	///	回転するスプライト
	//--------------------------------------------------------------------------------------
	RotateSprite::RotateSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, 
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap):
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Additive);
	}

	void RotateSprite::AdjustVertex() {
		//頂点色を変更する
		for (size_t i = 0; i < m_BackupVertices.size();i++) {
			switch (i) {
			case 0:
				m_BackupVertices[i].color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 1:
				m_BackupVertices[i].color = Col4(0.0f, 1.0f, 0.0f, 1.0f);
				break;
			case 2:
				m_BackupVertices[i].color = Col4(0.0f, 0.0f, 1.0f, 1.0f);
				break;
			case 3:
				m_BackupVertices[i].color = Col4(1.0f, 1.0f, 0, 1.0);
				break;
			}
		}
	}

	void RotateSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		m_Rot += ElapsedTime;
		if (m_Rot >= XM_2PI) {
			m_Rot = 0;
		}
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 1.0f) {
			m_TotalTime = 0;
		}
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vec2 UV(m_BackupVertices[i].textureCoordinate);
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				m_BackupVertices[i].color,
				UV
			);
		}


	}


	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	MessageSprite::MessageSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName,
		const Vec2& StartScale,
		float StartRot,
		const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		SpriteBase(StagePtr, TextureResName, StartScale, StartRot, StartPos, XWrap, YWrap),
		m_TotalTime(0)
	{
		SetBlendState(BlendState::Trace);
	}

	void MessageSprite::AdjustVertex() {
		//ここでは何もしない
	}

	void  MessageSprite::UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) {
		m_TotalTime += (ElapsedTime * 5.0f);
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		float sin_val = sin(m_TotalTime) * 0.5f + 0.5f;
		Col4 UpdateCol(1.0f, 1.0f, 1.0f, sin_val);
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				UpdateCol,
				m_BackupVertices[i].textureCoordinate
			);

		}
	}



}
//end basecross
