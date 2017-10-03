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
		const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
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
		//メッシュの作成（変更できない）
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void SquareObject::OnCreate() {
		CreateBuffers(m_Scale.x, m_Scale.y);
	}
	void SquareObject::OnUpdate() {
	}
	void SquareObject::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale, 
			Vec3(0, 0, 0),
			m_Qt, 
			m_Pos
		);
		auto shptr = PtrGameStage->FindTagGameObject<PNTDrawObject>(L"PNTDrawObject");
		shptr->AddDrawMesh(
			m_SquareMesh,
			TexPtr,
			World,
			false,
			true
		);
	}


	//--------------------------------------------------------------------------------------
	///	固定のボックス実体
	//--------------------------------------------------------------------------------------

	//static変数の初期化
	shared_ptr<MeshResource> BoxObject::m_BoxMesh(nullptr);

	BoxObject::BoxObject(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, bool Trace,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Pos) :
		BoxBase(StagePtr),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	BoxObject::~BoxObject() {}

	void BoxObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//メッシュの作成（変更できない）
		if (!m_BoxMesh) {
			m_BoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		}

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
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);
	}
	void BoxObject::OnUpdate() {
	}

	void BoxObject::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		//行列の定義
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			body.m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			body.m_Quat,				//回転角度
			body.m_Pos				//位置
		);
		auto shptr = PtrGameStage->FindTagGameObject<PNTDrawObject>(L"PNTDrawObject");
		shptr->AddDrawMesh(
			m_BoxMesh,
			TexPtr,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	固定のカプセル実体
	//--------------------------------------------------------------------------------------

	//static変数の初期化
	shared_ptr<MeshResource> CapsuleObject::m_CapsuleMesh(nullptr);

	CapsuleObject::CapsuleObject(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, bool Trace,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	CapsuleObject::~CapsuleObject() {}

	void CapsuleObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCapsule(1.0f, Vec3(0, -0.5f, 0), Vec3(0, 0.5f, 0), 18, vertices, indices);
		//メッシュの作成（変更できない）
		if (!m_CapsuleMesh) {
			m_CapsuleMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		}

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
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);
	}
	void CapsuleObject::OnUpdate() {
	}

	void CapsuleObject::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		//行列の定義
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			body.m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			body.m_Quat,				//回転角度
			body.m_Pos				//位置
		);
		auto shptr = PtrGameStage->FindTagGameObject<PNTDrawObject>(L"PNTDrawObject");
		shptr->AddDrawMesh(
			m_CapsuleMesh,
			TexPtr,
			World,
			true
		);
	}


	//--------------------------------------------------------------------------------------
	///	固定の球体実体
	//--------------------------------------------------------------------------------------

	//static変数の初期化
	shared_ptr<MeshResource> SphereObject::m_SphereMesh(nullptr);

	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, bool Trace,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SphereObject::~SphereObject() {}

	void SphereObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//メッシュの作成（変更できない）
		if (!m_SphereMesh) {
			m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		}
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
		body.SetToBefore();
		PtrGameStage->AddRigidbody(body);
	}
	void SphereObject::OnUpdate() {
	}

	void SphereObject::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		//行列の定義
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			body.m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			body.m_Quat,				//回転角度
			body.m_Pos				//位置
		);
		auto shptr = PtrGameStage->FindTagGameObject<PNTDrawObject>(L"PNTDrawObject");
		shptr->AddDrawMesh(
			m_SphereMesh,
			TexPtr,
			World,
			true
		);
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
