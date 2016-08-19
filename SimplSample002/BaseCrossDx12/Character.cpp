/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	三角形スプライト
	//--------------------------------------------------------------------------------------
	TriangleSprite::TriangleSprite() :
		ObjectInterface(),
		ShapeInterface(),
		m_Pos(0, 0, 0),
		m_MoveSpan(0.01f, 0, 0)
	{
	}
	TriangleSprite::~TriangleSprite() {}

	void TriangleSprite::OnCreate() {
		//頂点を作成するための配列
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vector3(0.0f, 0.5f, 0.0f), Color4(1.0f,0.0f,0.0f,1.0f)) },
			{ VertexPositionColor(Vector3(0.5f, -0.5f, 0.0f), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-0.5f, -0.5f, 0.0f), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
		};
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices, false);

		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateCbv);
		m_DrawContext->CreateConstantBuffer(sizeof(m_SpriteConstantBuffer));
		m_DrawContext->CreateDefault2DPipelineCmdList<VertexPositionColor, VSPCSprite, PSPCSprite>();



	}
	void TriangleSprite::OnUpdate() {
		m_Pos += m_MoveSpan;
		if (abs(m_Pos.x) >= 0.5f) {
			m_MoveSpan *= -1.0f;
		}
	}
	void TriangleSprite::OnDraw() {
		//コンスタントバッファの準備
		m_SpriteConstantBuffer.Emissive = Color4(0.0f, 0.0f, 0, 1.0f);
		Matrix4X4 mat;
		mat.TranslationFromVector(m_Pos);
		m_SpriteConstantBuffer.World = mat;
		//更新
		m_DrawContext->UpdateConstantBuffer(&m_SpriteConstantBuffer, sizeof(m_SpriteConstantBuffer));
		//描画
		m_DrawContext->DrawVertex<VertexPositionColor>(m_TriangleMesh);
	}



}
//end basecross
