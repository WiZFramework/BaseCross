/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	Pnt球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PntSphere::PntSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PntSphere::~PntSphere() {}

	//初期化
	void PntSphere::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

	}

	void PntSphere::OnDraw() {
		//メッシュの取得
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		//デバイスコンテキストの取得
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//レンダーステートの取得
		auto RenderState = Dev->GetRenderState();
		//コンスタントバッファの準備
		TestConstants Cb;
		//行列の定義
		auto PtrTrans = GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		//カメラを得る
		auto CameraPtr = OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.Transpose();
		ProjMat = CameraPtr->GetProjMatrix();
		//転置する
		ProjMat.Transpose();
		Cb.World = World;
		Cb.View = ViewMat;
		Cb.Projection = ProjMat;
		//エミッシブ
		Cb.Emissive = Color4(0.5f, 0.5f, 0.5f,0);
		//デフィーズ
		Cb.Diffuse = Color4(1, 1, 1, 1);
		//ライティング
		auto StageLight = OnGetDrawLight();
		Cb.LightDir = StageLight.m_Directional;
		Cb.LightDir.w = 1.0f;
		//テクスチャ（シェーダリソース）の取得
		auto Res = App::GetApp()->GetResource<TextureResource>(L"WALL_TX");
		pD3D11DeviceContext->PSSetShaderResources(0, 1, Res->GetShaderResourceView().GetAddressOf());
		auto pSampler = RenderState->GetLinearClamp();
		//サンプラーを設定
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBTest::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
		//ストライドとオフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBTest::GetPtr()->GetBuffer();
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSTest::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSTest::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSTest::GetPtr()->GetInputLayout());
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//ブレンドステート
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//描画
		pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}


}
//end basecross
