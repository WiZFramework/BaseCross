/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	立方体実体
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject() :
		ObjectInterface(),
		ShapeInterface()
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vector3(-HelfSize, HelfSize, -HelfSize), Color4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, HelfSize, -HelfSize), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, -HelfSize), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, -HelfSize, -HelfSize), Color4(1.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, HelfSize, HelfSize), Color4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, HelfSize, HelfSize), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, -HelfSize, HelfSize), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, HelfSize), Color4(1.0f, 0.0f, 1.0f, 1.0f)) }
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
			1, 4, 3,
			4, 6, 3,
			4, 5, 6,
			5, 7, 6,
			5, 0, 7,
			0, 2, 7,
			5, 4, 0,
			4, 1, 0,
			6, 7, 3,
			7, 2, 3
		};
		//メッシュの作成（変更できない）
		m_CubeMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}

	void CubeObject::UpdateConstantBuffer() {
		//行列の定義
		Matrix4X4 World, View, Proj;
		//ワールド行列の決定
		World.AffineTransformation(
			m_Scale,			//スケーリング
			Vector3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		//転置する
		World.Transpose();
		//ビュー行列の決定
		View.LookAtLH(Vector3(0, 2.0, -5.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		//転置する
		View.Transpose();
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//転置する
		Proj.Transpose();

		m_StaticConstantBuffer.World = World;
		m_StaticConstantBuffer.View = View;
		m_StaticConstantBuffer.Projection = Proj;
		m_StaticConstantBuffer.Emissive = Color4(0, 0, 0, 0);
		//初期値更新
		m_DrawContext->UpdateConstantBuffer(reinterpret_cast<void**>(&m_StaticConstantBuffer), sizeof(m_StaticConstantBuffer));
	}



	void CubeObject::OnCreate() {
		CreateBuffers();
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Qt.Identity();
		m_Pos = Vector3(0, 0, 0.0);
		//描画コンテキスト
		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContext->CreateConstantBuffer(sizeof(m_StaticConstantBuffer));
		m_DrawContext->CreateDefault3DPipelineCmdList<VertexPositionColorTexture, VSPCStatic, PSPCStatic>();
		//コンスタントバッファの更新
		UpdateConstantBuffer();
	}
	void CubeObject::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}
	void CubeObject::OnDraw() {
		//コンスタントバッファの更新
		UpdateConstantBuffer();
		//描画
		m_DrawContext->DrawIndexed<VertexPositionColor>(m_CubeMesh);
	}



}
//end basecross
