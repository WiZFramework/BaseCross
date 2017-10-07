/*!
@file DrawObjects.cpp
@brief 描画オブジェクト実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	シャドウマップの描画クラス
	//--------------------------------------------------------------------------------------
	float ShadowmapDrawObject::m_LightHeight(20.0f);
	float ShadowmapDrawObject::m_LightNear(1.0f);
	float ShadowmapDrawObject::m_LightFar(200.0f);
	float ShadowmapDrawObject::m_ViewWidth(32.0f);
	float ShadowmapDrawObject::m_ViewHeight(32.0f);

	ShadowmapDrawObject::ShadowmapDrawObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	ShadowmapDrawObject::~ShadowmapDrawObject() {}

	void ShadowmapDrawObject::OnCreate() {
		//タグの追加
		AddTag(L"ShadowmapDrawObject");
	}

	void ShadowmapDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes, const Mat4x4& WorldMat) {
		ShadowmapObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_WorldMatrix = WorldMat;
		m_ShadowmapObjectVec.push_back(Obj);
	}


	void ShadowmapDrawObject::OnDrawShadowmap() {

		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//各オブジェクト共通処理
		//シェーダの設定
		//頂点シェーダーのセット
		pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
		//ピクセルシェーダはセットしない！
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//ジオメトリシェーダの設定（使用しない）
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//インプットレイアウトのセット
		pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;

		//個別処理
		for (auto& v : m_ShadowmapObjectVec) {
			//位置の取得
			auto Pos = v.m_WorldMatrix.transInMatrix();
			Vec4 LightDir4;
			PtrGameStage->GetLightDir(LightDir4);
			Vec3 LightDir(LightDir4.x, LightDir4.y, LightDir4.z);
			Vec3 PosSpan = LightDir;
			PosSpan *= 0.1f;
			Pos += PosSpan;
			//行列の定義
			Mat4x4 World, LightView, LightProj;
			//ワールド行列の決定
			World.affineTransformation(
				v.m_WorldMatrix.scaleInMatrix(),			//スケーリング
				Vec3(0, 0, 0),		//回転の中心（重心）
				v.m_WorldMatrix.quatInMatrix(),				//回転角度
				Pos				//位置
			);
			LightDir = LightDir  * -1.0;
			Vec3 LightAt = PtrGameStage->GetCamera().m_CamerAt;
			Vec3 LightEye = LightAt + (LightDir * m_LightHeight);
			//ライトのビューと射影を計算
			LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(m_ViewWidth, m_ViewHeight, m_LightNear, m_LightFar);
			ShadowConstants Cb;
			Cb.mWorld = bsm::transpose(World);
			Cb.mView = bsm::transpose(LightView);
			Cb.mProj = bsm::transpose(LightProj);
			//コンスタントバッファの更新
			pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
			//頂点バッファをセット
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pID3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//コンスタントバッファをピクセルシェーダにセット
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
			//描画
			pID3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
		}
		//後始末
		Dev->InitializeStates();
		m_ShadowmapObjectVec.clear();
	}


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


	void PNTDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
		const shared_ptr<TextureResource>& TextureRes,
		const Mat4x4& WorldMat,
		bool Trace, bool Wrap) {
		DrawObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_TextureRes = TextureRes;
		Obj.m_WorldMatrix = WorldMat;
		Obj.m_Trace = Trace;
		Obj.m_Wrap = Wrap;
		if (Trace) {
			m_TraceDrawObjectVec.push_back(Obj);
		}
		else {
			m_DrawObjectVec.push_back(Obj);
		}
	}

	void PNTDrawObject::OnDrawSub(vector<DrawObject>& ObjectVec, PNTStaticConstantBuffer& sb) {
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
		for (auto& v : ObjectVec) {
			//転置する
			v.m_WorldMatrix.transpose();
			//ワールド行列の決定
			sb.World = v.m_WorldMatrix;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//テクスチャの設定
			ID3D11ShaderResourceView* pNull[1] = { 0 };
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.m_TextureRes->GetShaderResourceView().GetAddressOf());
			//サンプラー
			if (v.m_Wrap) {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerWrap);
			}
			else {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerClamp);
			}
			//ブレンドステート
			if (v.m_Trace) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//透明処理の場合は、ラスタライザステートを変更して2回描画
				//ラスタライザステート（裏面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
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
		PNTStaticConstantBuffer sb;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//ディフューズ
		sb.Diffuse = Col4(0.8f, 0.8f, 0.8f, 1.0f);
		//エミッシブ加算。
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//スペキュラー。
		sb.Specular = Col4(0.2f, 0.2f, 0.2f, 0);
		//カメラの位置
		Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
		sb.EyePos = CameraEye;
		//サブ関数呼び出し(不透明)
		OnDrawSub(m_DrawObjectVec, sb);
		//--------------------------------------------------------
		//透明の3Dオブジェクトをカメラからの距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto func = [&](DrawObject& Left, DrawObject& Right)->bool {
			auto LeftPos = Left.m_WorldMatrix.transInMatrix();
			auto RightPos = Right.m_WorldMatrix.transInMatrix();
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


	//--------------------------------------------------------------------------------------
	///	影付きPNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	PNTShadowDrawObject::PNTShadowDrawObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	PNTShadowDrawObject::~PNTShadowDrawObject() {}

	void PNTShadowDrawObject::OnCreate() {
		//タグの追加
		AddTag(L"PNTShadowDrawObject");
	}

	void PNTShadowDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
		const shared_ptr<TextureResource>& TextureRes,
		const Mat4x4& WorldMat,
		bool Trace, bool Wrap, bool OwnShadowmapActive) {
		DrawObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_TextureRes = TextureRes;
		Obj.m_WorldMatrix = WorldMat;
		Obj.m_Trace = Trace;
		Obj.m_Wrap = Wrap;
		Obj.m_OwnShadowmapActive = OwnShadowmapActive;
		m_DrawObjectVec.push_back(Obj);
	}


	void PNTShadowDrawObject::SetConstants(DrawObject& DrawObj, PNTStaticShadowConstantBuffer& Cb) {
		auto PtrGameStage = GetStage<GameStage>();
		//行列の定義
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = DrawObj.m_WorldMatrix;
		//転置する
		World.transpose();
		//カメラを得る
		PtrGameStage->GetCamera().GetViewProjMatrix(ViewMat, ProjMat);
		Vec4 LightDir;
		PtrGameStage->GetLightDir(LightDir);
		//転置する
		ViewMat.transpose();
		//転置する
		ProjMat.transpose();
		Cb.World = World;
		Cb.View = ViewMat;
		Cb.Projection = ProjMat;
		//ディフューズ
		Cb.Diffuse = Col4(0.8f, 0.8f, 0.8f, 1.0f);
		//エミッシブ加算。
		Cb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//スペキュラー。
		Cb.Specular = Col4(0.2f, 0.2f, 0.2f, 0);
		//ライティング
		Cb.LightDir = LightDir;
		Cb.LightDir.w = 1.0f;
		Cb.EyePos = PtrGameStage->GetCamera().m_CamerEye;
		Cb.EyePos.w = 1.0f;
		Vec3 CalcLightDir(LightDir.x, LightDir.y, LightDir.z);
		CalcLightDir = -1.0 * CalcLightDir;
		Vec3 LightAt = PtrGameStage->GetCamera().m_CamerAt;
		Vec3 LightEye = CalcLightDir;
		auto ShadowObj = PtrGameStage->FindTagGameObject<ShadowmapDrawObject>(L"ShadowmapDrawObject");
		LightEye *= ShadowmapDrawObject::m_LightHeight;
		LightEye = LightAt + LightEye;
		Cb.LightPos = LightEye;
		Cb.LightPos.w = 1.0f;
		bsm::Mat4x4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
		LightProj = XMMatrixOrthographicLH(ShadowmapDrawObject::m_ViewWidth, ShadowmapDrawObject::m_ViewHeight,
			ShadowmapDrawObject::m_LightNear, ShadowmapDrawObject::m_LightFar);
		Cb.LightView = bsm::transpose(LightView);
		Cb.LightProjection = bsm::transpose(LightProj);

	}


	void PNTShadowDrawObject::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//サンプラーの準備
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();

		//個別処理
		for (auto& v : m_DrawObjectVec) {
			//コンスタントバッファの準備
			PNTStaticShadowConstantBuffer Cb;
			SetConstants(v, Cb);
			//テクスチャ
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.m_TextureRes->GetShaderResourceView().GetAddressOf());
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBPNTStaticShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
			//ストライドとオフセット
			UINT stride = v.m_MeshRes->GetNumStride();
			UINT offset = 0;
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//描画方法（3角形）
			pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBPNTStaticShadow::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//シェーダの設定
			pD3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			if (v.m_OwnShadowmapActive) {
				//自己影がある場合
				pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//自己影がない場合
				pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			//シャドウマップのレンダラーターゲット
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//シャドウマップサンプラー
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			//インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(VSPNTStaticShadow::GetPtr()->GetInputLayout());
			//オブジェクトのサンプラー
			if (v.m_Wrap) {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerWrap);
			}
			else {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerClamp);
			}
			//デプスステンシルステート
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
			//ブレンドステート
			if (v.m_Trace) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//透明処理の場合は、ラスタライザステートを変更して2回描画
				//ラスタライザステート（裏面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}

		}
		//後始末
		Dev->InitializeStates();
		m_DrawObjectVec.clear();
	}



}
//end basecross
