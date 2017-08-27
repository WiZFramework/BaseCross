/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class Ball : public GameObject;
	//	用途: 頂点が変更されるボール
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Ball::Ball(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_Explode(0)
	{}

	//初期化
	void Ball::OnCreate() {
		auto Ptr = GetComponent<Transform>();
		Ptr->SetPosition(m_StartPos);
		Ptr->SetScale(1.0f, 1.0f, 1.0f);	//直径メートルの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddRotateBy(1.0f, Vec3(0, XM_PI, 0));
		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();
		//透過処理する
		SetAlphaActive(true);
	}

	//変化
	void Ball::OnUpdate() {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Explode += ElapsedTime;
		if (m_Explode >= XM_PI) {
			m_Explode = 0;
		}
	}



	//描画
	void Ball::OnDraw() {
		auto PtrStage = GetStage();
		if (!PtrStage) {
			return;
		}
		auto PtrT = GetComponent<Transform>();
		//ステージからカメラを取り出す
		auto PtrCamera = OnGetDrawCamera();
		//ライトを取り出す(シャドウ用)
		auto DrawLight = OnGetDrawLight();
		//メッシュリソースの取得
		auto PtrMeshResource = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");
		//テクスチャの取得
		auto PtrTextureResource = App::GetApp()->GetResource<TextureResource>(L"TRACE_TX");
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = Dev->GetRenderState();
		//コンスタントバッファの設定
		BallConstantBuffer Cb;
		Cb.m_World = transpose(PtrT->GetWorldMatrix());
		Cb.m_View = transpose(PtrCamera->GetViewMatrix());
		Cb.m_Proj = transpose(PtrCamera->GetProjMatrix());
		Cb.m_LightDir = DrawLight.m_Directional;
		Cb.m_Explode = abs(sin(m_Explode) / 2.0f);
		//コンスタントバッファの更新
		ID3D11Buffer* pConstantBuffer = CBBall::GetPtr()->GetBuffer();
		pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &Cb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//頂点シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSBall::GetPtr()->GetShader(), nullptr, 0);

		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//テクスチャを設定
		pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
		//リニアサンプラーを設定
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//ピクセルシェーダの設定
		pID3D11DeviceContext->PSSetShader(PSBall::GetPtr()->GetShader(), nullptr, 0);
		//ジオメトリシェーダの設定
		pID3D11DeviceContext->GSSetShader(GSBall::GetPtr()->GetShader(), nullptr, 0);

		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSBall::GetPtr()->GetInputLayout());

		//コンスタントバッファの設定
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}


}
//end basecross
