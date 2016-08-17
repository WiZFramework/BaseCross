/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	三角形オブジェクト
	//--------------------------------------------------------------------------------------
	TriangleObject::TriangleObject() :
		ObjectInterface(),
		ShapeInterface()
	{
	}
	TriangleObject::~TriangleObject() {}

	void TriangleObject::OnCreate() {
		//頂点を作成するための配列
		vector<VertexPositionColor> vertices = {
			{ Vector3(0.0f, 0.5f , 0.0f),Color4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vector3(0.5f, -0.5f, 0.5f),Color4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vector3(-0.5f, -0.5f, 0.5f),Color4(0.0f, 0.0f, 1.0f, 1.0f) }
		};
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices,false);

		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSimple);
		m_DrawContext->CreateDefault2DPipelineCmdList<VertexPositionColor, VSPCDirect, PSPCDirect>();
	}
	void TriangleObject::OnUpdate() {
		//何もしない
	}
	void TriangleObject::OnDraw() {
		//描画
		m_DrawContext->DrawVertex<VertexPositionColor>(m_TriangleMesh);
	}


}
//end basecross
