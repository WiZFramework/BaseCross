/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	四角形スプライト実体
	//--------------------------------------------------------------------------------------
	SquareSprite::SquareSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(StartPos)
	{}
	SquareSprite::~SquareSprite() {}
	void SquareSprite::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列
		vector<VertexPositionTexture> vertices = {
			{ VertexPositionTexture(Vector3(-HelfSize, HelfSize, 0), Vector2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, HelfSize, 0), Vector2(1.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(-HelfSize, -HelfSize, 0), Vector2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, -HelfSize, 0), Vector2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };

		//メッシュの作成
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");

		//矩形の大きさ
		m_Scale = Vector2(128.0f, 128.0f);
		//回転角
		m_Rot = 0;
		m_PosSpan = Vector2(3.0f, 0);


		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContext->CreateConstantBuffer(sizeof(m_DiffuseSpriteConstantBuffer));
		m_DrawContext->CreateDefault2DPipelineCmdList<VertexPositionTexture, VSPTSprite, PSPTSprite>();
		if (m_Trace) {
			m_DrawContext->SetBlendState(BlendState::AlphaBlend);
		}

		m_DiffuseSpriteConstantBuffer.World = Matrix4X4EX::Identity();
		//初期値更新
		m_DrawContext->UpdateConstantBuffer(reinterpret_cast<void**>(&m_DiffuseSpriteConstantBuffer), sizeof(m_DiffuseSpriteConstantBuffer));
		//テクスチャ設定
		m_DrawContext->SetTextureResource(m_TextureResource);


	}

	void SquareSprite::OnUpdate() {
		if (m_PosSpan.x > 0) {
			m_Rot -= 0.05f;
		}
		else {
			m_Rot += 0.05f;
		}
		m_Pos += m_PosSpan;
		if (abs(m_Pos.x) > 400.0f) {
			m_PosSpan *= -1.0f;
		}
	}

	void SquareSprite::OnDraw() {

		//行列の定義
		Matrix4X4 World, Proj;
		//ワールド行列の決定
		World.AffineTransformation2D(
			m_Scale,			//スケーリング
			Vector2(0, 0),		//回転の中心（重心）
			m_Rot,				//回転角度
			m_Pos				//位置
		);
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.OrthographicLH(w, h, -1.0, 1.0f);
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		m_DiffuseSpriteConstantBuffer.Emissive = Color4(0, 0, 0, 0.0f);
		m_DiffuseSpriteConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_DiffuseSpriteConstantBuffer.World = World;
		//更新
		m_DrawContext->UpdateConstantBuffer(&m_DiffuseSpriteConstantBuffer, sizeof(m_DiffuseSpriteConstantBuffer));
		//描画
		m_DrawContext->DrawIndexed<VertexPositionTexture>(m_SquareMesh);


	}



}
//end basecross
