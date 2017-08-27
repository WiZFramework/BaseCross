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
			{ VertexPositionColor(Vec3(-HelfSize, HelfSize, -HelfSize), Col4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, HelfSize, -HelfSize), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, -HelfSize, -HelfSize), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, -HelfSize, -HelfSize), Col4(1.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, HelfSize, HelfSize), Col4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, HelfSize, HelfSize), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, -HelfSize, HelfSize), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, -HelfSize, HelfSize), Col4(1.0f, 0.0f, 1.0f, 1.0f)) }
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


	void CubeObject::OnCreate() {
		CreateBuffers();
		m_Scale = Vec3(1.0f, 1.0f, 1.0f);
		m_Qt.identity();
		m_Pos = Vec3(0, 0, 0.0);
	}
	void CubeObject::OnUpdate() {
		Quat QtSpan;
		QtSpan.rotation(0.02f, Vec3(0, 1.0f, 0));
		m_Qt *= QtSpan;
		m_Qt.normalize();
	}
	void CubeObject::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//行列の定義
		Mat4x4 World, View, Proj;
		//ワールド行列の決定
		World.affineTransformation(
			m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		//転置する
		World.transpose();
		//ビュー行列の決定
		View = XMMatrixLookAtLH(Vec3(0, 2.0, -5.0f), Vec3(0, 0, 0), Vec3(0, 1.0f, 0));
		//転置する
		View.transpose();
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//転置する
		Proj.transpose();
		//コンスタントバッファの準備
		PCStaticConstantBuffer sb;
		sb.World = World;
		sb.View = View;
		sb.Projection = Proj;
		//エミッシブ加算は行わない。
		sb.Emissive = Col4(0, 0, 0, 0);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBPCStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_CubeMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_CubeMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBPCStatic::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCStatic::GetPtr()->GetInputLayout());
		//ブレンドステート
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//描画
		pD3D11DeviceContext->DrawIndexed(m_CubeMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}

}
//end basecross
