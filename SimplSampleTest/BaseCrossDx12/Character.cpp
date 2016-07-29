/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public ObjectInterface;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	NormalTextureBox::NormalTextureBox(const Vector3& Pos) :
		ObjectInterface(),
		ShapeInterface(),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_Qt(),
		m_Pos(Pos)
	{
		m_Qt.Identity();
	}
	NormalTextureBox::~NormalTextureBox() {}

	void NormalTextureBox::OnCreate() {
		m_CubeMesh = MeshResource::CreateCube(1.0f);
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"wall.jpg";
		m_WallTex = TextureResource::CreateTextureResource(strTexture);
		m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateSrvSmpCbv);
		m_DrawContext->CreateConstantBuffer(sizeof(m_ConstantBufferData));
		m_DrawContext->CreateDefault3DPipelineCmdList<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>();
		ZeroMemory(&m_ConstantBufferData, sizeof(m_ConstantBufferData));
		//各行列をIdentityに初期化
		m_ConstantBufferData.World = Matrix4X4EX::Identity();
		m_ConstantBufferData.View = Matrix4X4EX::Identity();
		m_ConstantBufferData.Projection = Matrix4X4EX::Identity();
		//初期値更新
		m_DrawContext->UpdateConstantBuffer(reinterpret_cast<void**>(&m_ConstantBufferData), sizeof(m_ConstantBufferData));
		//テクスチャ設定
		m_DrawContext->SetTextureResource(m_WallTex);
	}
	void NormalTextureBox::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}
	void NormalTextureBox::OnDraw() {
		//行列の定義
		//ワールド行列の決定
		m_ConstantBufferData.World.AffineTransformation(
			m_Scale,			//スケーリング
			Vector3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		//転置する
		m_ConstantBufferData.World.Transpose();
		//ビュー行列の決定
		m_ConstantBufferData.View.LookAtLH(Vector3(0, 2.0, -5.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		//転置する
		m_ConstantBufferData.View.Transpose();
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		m_ConstantBufferData.Projection.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//転置する
		m_ConstantBufferData.Projection.Transpose();
		//ライティング
		m_ConstantBufferData.LightDir = Vector4(0.5f, -1.0f, 0.5f, 1.0f);
		//エミッシブ
		m_ConstantBufferData.Emissive = Color4(0, 0, 0, 0);
		//ディフューズ
		m_ConstantBufferData.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//更新
		m_DrawContext->UpdateConstantBuffer(&m_ConstantBufferData, sizeof(m_ConstantBufferData));
		//描画
		m_DrawContext->DrawIndexed<VertexPositionNormalTexture>(m_CubeMesh);

	}


}
//end basecross
