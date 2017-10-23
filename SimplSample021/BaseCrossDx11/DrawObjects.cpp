/*!
@file DrawObjects.cpp
@brief 描画オブジェクト実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	PNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	PNTDrawObject::PNTDrawObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	PNTDrawObject::~PNTDrawObject() {}

	void PNTDrawObject::OnCreate() {
		//タグの追加
		AddTag(L"PNTDrawObject");
	}

	void PNTDrawObject::AddDrawMesh(const shared_ptr<DrawObject>& Obj) {
		if (Obj->m_Trace) {
			m_TraceDrawObjectVec.push_back(Obj);
		}
		else {
			m_DrawObjectVec.push_back(Obj);
		}
	}


	void PNTDrawObject::OnDrawSub(const vector<shared_ptr<DrawObject>>& ObjectVec, SimpleConstants& sb) {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//サンプラーの準備
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//個別処理
		for (auto v : ObjectVec) {
			//ワールド行列の決定
			sb.World = v->m_WorldMatrix;
			//転置する
			sb.World.transpose();
			//テクスチャの設定
			if (v->m_TextureRes) {
				//テクスチャ
				sb.ActiveFlg.x = 1;
				pD3D11DeviceContext->PSSetShaderResources(0, 1, v->m_TextureRes->GetShaderResourceView().GetAddressOf());
				//サンプラー
				if (v->m_Wrap) {
					pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerWrap);
				}
				else {
					pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerClamp);
				}
			}
			else {
				sb.ActiveFlg.x = 0;
			}
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//ブレンドステート
			if (v->m_Trace) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//透明処理の場合は、ラスタライザステートを変更して2回描画
				//ラスタライザステート（裏面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
			}
		}
	}

	void PNTDrawObject::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//各オブジェクト共通処理
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//行列の定義
		Mat4x4 View, Proj;
		//ライティング
		Vec4 LightDir;
		PtrGameStage->GetCamera().GetViewProjMatrix(View, Proj);
		PtrGameStage->GetLightDir(LightDir);
		//ビュー行列の決定
		//転置する
		View.transpose();
		//射影行列の決定
		//転置する
		Proj.transpose();
		//コンスタントバッファの準備
		SimpleConstants sb;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//ディフューズ
		sb.Diffuse = Col4(0.8f, 0.8f, 0.8f, 1.0f);
		//エミッシブ加算。
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//スペキュラー。
		sb.Specular= Col4(0.2f, 0.2f, 0.2f, 0);
		//カメラの位置
		Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
		sb.EyePos = CameraEye;
		//サブ関数呼び出し(不透明)
		OnDrawSub(m_DrawObjectVec,sb);
		//--------------------------------------------------------
		//透明の3Dオブジェクトをカメラからの距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto func = [&](shared_ptr<DrawObject>& Left, shared_ptr<DrawObject>& Right)->bool {
			auto LeftPos = Left->m_WorldMatrix.transInMatrix();
			auto RightPos = Right->m_WorldMatrix.transInMatrix();
			auto LeftLen = bsm::length(LeftPos - CameraEye);
			auto RightLen = bsm::length(RightPos - CameraEye);
			return (LeftLen > RightLen);
		};
		//ラムダ式を使ってソート
		std::sort(m_TraceDrawObjectVec.begin(), m_TraceDrawObjectVec.end(), func);
		//サブ関数呼び出し(透明)
		OnDrawSub(m_TraceDrawObjectVec, sb);
		//後始末
		Dev->InitializeStates();
		//描画用の配列をクリア
		m_DrawObjectVec.clear();
		m_TraceDrawObjectVec.clear();
	}

}
//end basecross
