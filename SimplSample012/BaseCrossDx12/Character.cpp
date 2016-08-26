/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	球実体
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos) :
		ObjectInterface(),
		ShapeInterface(),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(Pos)
	{}
	SphereObject::~SphereObject() {}

	void SphereObject::CreateBuffers() {
		float Radius = 0.5f;
		//緯度方向
		UINT Vertical = m_Division;
		//経度方向
		UINT Horizontal = m_Division * 2;

		vector<VertexPositionNormalTexture> vertices;
		for (UINT i = 0; i <= Vertical; i++)
		{
			float v = 1 - (float)i / Vertical;

			float Latitude = (i * XM_PI / Vertical) - XM_PIDIV2;
			float Dy = sin(Latitude);
			float Dxz = cos(Latitude);

			for (UINT j = 0; j <= Horizontal; j++)
			{
				float u = 1 - (float)j / Horizontal;

				float Longitude = j * XM_2PI / Horizontal;
				float Dx = sin(Longitude) * Dxz;
				float Dz = cos(Longitude) * Dxz;
				Vector3 normal(Dx, Dy, Dz);
				VertexPositionNormalTexture Data;
				Data.position = normal * Radius;
				Data.normal = normal;
				Data.normal.Normalize();
				Data.textureCoordinate = Vector2(u, v);
				vertices.push_back(Data);
			}
		}
		UINT Stride = Horizontal + 1;
		vector<uint16_t> indices;
		for (UINT i = 0; i < Vertical; i++)
		{
			for (UINT j = 0; j <= Horizontal; j++)
			{
				UINT NextI = i + 1;
				UINT NextJ = (j + 1) % Stride;
				indices.push_back((uint16_t)(i * Stride + NextJ));
				indices.push_back((uint16_t)(NextI * Stride + j));
				indices.push_back((uint16_t)(i * Stride + j));

				indices.push_back((uint16_t)(NextI * Stride + NextJ));
				indices.push_back((uint16_t)(NextI * Stride + j));
				indices.push_back((uint16_t)(i * Stride + NextJ));
			}
		}
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);

	}

	void SphereObject::UpdateConstantBuffer() {
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
		//ライティング
		Vector4 LightDir(0.5f, -1.0f, 0.5f, 0.0f);
		LightDir.Normalize();
		m_StaticConstantBuffer.World = World;
		m_StaticConstantBuffer.View = View;
		m_StaticConstantBuffer.Projection = Proj;
		m_StaticConstantBuffer.LightDir = LightDir;
		m_StaticConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_StaticConstantBuffer.Emissive = Color4(0, 0, 0, 0);
		//更新
		m_DrawContextCullBack->UpdateConstantBuffer(reinterpret_cast<void**>(&m_StaticConstantBuffer), sizeof(m_StaticConstantBuffer));
		m_DrawContextCullFront->UpdateConstantBuffer(reinterpret_cast<void**>(&m_StaticConstantBuffer), sizeof(m_StaticConstantBuffer));
	}



	void SphereObject::OnCreate() {
		CreateBuffers();
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Qt.Identity();

		//描画コンテキスト1
		m_DrawContextCullBack = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContextCullBack->CreateConstantBuffer(sizeof(m_StaticConstantBuffer));
		m_DrawContextCullBack->CreateDefault3DPipelineCmdList<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>();
		//テクスチャ設定
		m_DrawContextCullBack->SetTextureResource(m_TextureResource);
		//描画コンテキスト2
		m_DrawContextCullFront = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContextCullFront->CreateConstantBuffer(sizeof(m_StaticConstantBuffer));
		m_DrawContextCullFront->CreateDefault3DPipelineCmdList<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>();
		//テクスチャ設定
		m_DrawContextCullFront->SetTextureResource(m_TextureResource);
		//CullFront設定
		m_DrawContextCullFront->SetRasterizerState(RasterizerState::CullFront);
		if (m_Trace) {
			m_DrawContextCullBack->SetBlendState(BlendState::AlphaBlend);
			m_DrawContextCullFront->SetBlendState(BlendState::AlphaBlend);
		}
		//コンスタントバッファの更新
		UpdateConstantBuffer();

	}
	void SphereObject::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}
	void SphereObject::OnDraw() {
		//コンスタントバッファの更新
		UpdateConstantBuffer();
		//描画
		if (m_Trace) {
			m_DrawContextCullFront->DrawIndexed<VertexPositionNormalTexture>(m_SphereMesh);
			m_DrawContextCullBack->DrawIndexed<VertexPositionNormalTexture>(m_SphereMesh);
		}
		else {
			m_DrawContextCullBack->DrawIndexed<VertexPositionNormalTexture>(m_SphereMesh);
		}
	}



}
//end basecross
