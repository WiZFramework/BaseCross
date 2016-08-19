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
	}
	void TriangleSprite::OnUpdate() {
		m_Pos += m_MoveSpan;
		if (abs(m_Pos.x) >= 0.5f) {
			m_MoveSpan *= -1.0f;
		}
	}
	void TriangleSprite::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//コンスタントバッファの準備
		SpriteConstantBuffer sb;
		sb.Emissive = Color4(0.0f, 0.0f, 0, 1.0f);
		Matrix4X4 mat;
		mat.TranslationFromVector(m_Pos);
		sb.World = mat;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_TriangleMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダには渡さない（nullptrを渡す）
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCSprite::GetPtr()->GetInputLayout());

		//ブレンドステート
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//描画
		pD3D11DeviceContext->Draw(m_TriangleMesh->GetNumVertices(), 0);
		//後始末
		Dev->InitializeStates();

	}



}
//end basecross
