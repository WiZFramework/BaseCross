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
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices, false);
	}
	void TriangleObject::OnUpdate() {
		//何もしない
	}
	void TriangleObject::OnDraw() {
		//描画
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = Dev->GetRenderState();

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_TriangleMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//nullテクスチャを設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
		//nullサンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//シェーダの設定
		//頂点シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSPCDirect::GetPtr()->GetShader(), nullptr, 0);
		//ピクセルシェーダの設定
		pID3D11DeviceContext->PSSetShader(PSPCDirect::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPCDirect::GetPtr()->GetInputLayout());

		//塗りつぶし
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//描画
		pID3D11DeviceContext->Draw(3, 0);
		//後始末
		Dev->InitializeStates();

	}



}
//end basecross
