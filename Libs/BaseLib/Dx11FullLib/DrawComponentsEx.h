
/*!
@file DrawComponentsEx.h
@brief 拡張描画コンポーネント
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Basicシェーダー用コンスタントバッファ
	//--------------------------------------------------------------------------------------
	struct BasicConstants
	{
		bsm::Col4 diffuseColor;
		bsm::Col4 emissiveColor;
		bsm::Col4 specularColorAndPower;

		bsm::Col4 lightDirection[3];
		bsm::Col4 lightDiffuseColor[3];
		bsm::Col4 lightSpecularColor[3];

		bsm::Vec4 eyePosition;

		bsm::Col4 fogColor;
		bsm::Vec4 fogVector;

		bsm::Mat4x4 world;
		bsm::Vec4 worldInverseTranspose[3];
		bsm::Mat4x4 worldViewProj;
		//汎用フラグ
		XMUINT4 activeFlg;
		//以下影
		bsm::Vec4 lightPos;
		bsm::Vec4 eyePos;
		bsm::Mat4x4 lightView;
		bsm::Mat4x4 lightProjection;

		bsm::Vec4 bones[3 * 72];

	};

	//--------------------------------------------------------------------------------------
	/// コンスタントバッファ
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_CONSTANT_BUFFER(CBBasic, BasicConstants)


	//--------------------------------------------------------------------------------------
	/// PC頂点シェーダ
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_VERTEX_SHADER(BcVSPCStatic, VertexPositionColor)
	//--------------------------------------------------------------------------------------
	/// PT頂点シェーダ
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_VERTEX_SHADER(BcVSPTStatic, VertexPositionTexture)
	//--------------------------------------------------------------------------------------
	/// PCT頂点シェーダ
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_VERTEX_SHADER(BcVSPCTStatic, VertexPositionColorTexture)

	//--------------------------------------------------------------------------------------
	/// PC用ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_PIXEL_SHADER(BcPSPCStatic);

	//--------------------------------------------------------------------------------------
	/// PT,PCT用ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_PIXEL_SHADER(BcPSPTStatic);


	//--------------------------------------------------------------------------------------
	/// PNT頂点シェーダ(頂点ライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticVL, VertexPositionNormalTexture)
	//影無しBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBoneVL, VertexPositionNormalTextureSkinning)

	//影付き
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticVLShadow, VertexPositionNormalTexture)
	//影付きBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBoneVLShadow, VertexPositionNormalTextureSkinning)


	
	//バイアス付き影無し
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticVLBn, VertexPositionNormalTexture)
	//バイアス付き影無しBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBoneVLBn, VertexPositionNormalTextureSkinning)

	//バイアス付き影付き
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticVLBnShadow, VertexPositionNormalTexture)
	//バイアス付き影付きBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBoneVLBnShadow, VertexPositionNormalTextureSkinning)


	//--------------------------------------------------------------------------------------
	/// PNTピクセルシェーダ(頂点ライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTVL);
	//影付き
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTVLShadow);


	//--------------------------------------------------------------------------------------
	/// PNT頂点シェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPL, VertexPositionNormalTexture)
	//影無しBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePL, VertexPositionNormalTextureSkinning)
	//影付き
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPLShadow, VertexPositionNormalTexture)
	//影付きBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePLShadow, VertexPositionNormalTextureSkinning)

	//バイアス付き影無し
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPLBn, VertexPositionNormalTexture)
	//バイアス付き影無しBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePLBn, VertexPositionNormalTextureSkinning)

	//バイアス付き影付き
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPLBnShadow, VertexPositionNormalTexture)
	//バイアス付き影付きBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePLBnShadow, VertexPositionNormalTextureSkinning)

	//--------------------------------------------------------------------------------------
	/// PNTピクセルシェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTPL);
	//影付き
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTPLShadow);


	//--------------------------------------------------------------------------------------
	/// PNTnT頂点シェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTStaticPL, VertexPositionNormalTangentTexture)
	//影無しBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTBonePL, VertexPositionNormalTangentTextureSkinning)

	//影つき
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTStaticPLShadow, VertexPositionNormalTangentTexture)
	//影つきBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTBonePLShadow, VertexPositionNormalTangentTextureSkinning)


	//--------------------------------------------------------------------------------------
	/// PNTnTピクセルシェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTnTPL)
	//影つき
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTnTPLShadow);

	//--------------------------------------------------------------------------------------
	///	Bc描画オブジェクトの親
	//--------------------------------------------------------------------------------------
	class BcBaseDraw : public DrawComponent{
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		explicit BcBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcBaseDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスタントバッファの設定
		@param[out]	BcCb	設定するコンスタントバッファ構造体
		@param[in]	data	メッシュのデータ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(BasicConstants& BcCb, const MeshPrimData& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタティックメッシュの描画
		@tparam[in]	T_VShader	使用する頂点シェーダ
		@tparam[in]	T_PShader	使用するピクセルシェーダ
		@param[in]	data	メッシュのデータ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawStatic(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULLのシェーダリソースの準備
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//サンプラーの準備
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//各オブジェクト共通処理
			//シェーダの設定
			//頂点シェーダ
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//ピクセルシェーダ
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//ライトを設定
			SetLightingParamaters();
			//個別処理
			BasicConstants BcCb;
			//コンスタントバッファの作成
			SetConstants(BcCb, data);
			//テクスチャ
			auto shTex = GetTextureResource();
			if (shTex) {
				//テクスチャがある
				BcCb.activeFlg.y = 1;
			}
			else {
				//描画コンポーネントにはテクスチャがない
				if (shTex = data.m_TextureResource.lock()) {
					//テクスチャがある
					BcCb.activeFlg.y = 1;
				}
				else {
					BcCb.activeFlg.y = 0;
				}
			}
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ストライドとオフセット
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			//描画方法のセット
			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//各レンダリングステートの設定
			//ブレンドステート
			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
			//デプスステンシルステート
			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
			//テクスチャとサンプラー
			if (shTex) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
				//サンプラーを設定
				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
				//法線マップ
				if (GetNormalMapTextureResource()) {
					if (IsOwnShadowActive()) {
						//２番目に入れる
						pD3D11DeviceContext->PSSetShaderResources(2, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
						//サンプラーを設定
						RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
					}
					else {
						//1番目に入れる
						pD3D11DeviceContext->PSSetShaderResources(1, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
						//サンプラーを設定
						RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 1);
					}
				}
			}
			else {
				//シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//サンプラーもクリア
				RenderState->SetSamplerAllClear(pD3D11DeviceContext);
			}
			//影とサンプラー
			if (IsOwnShadowActive()) {
				//シャドウマップのレンダラーターゲット
				auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
				ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
				pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
				//シャドウマップサンプラー
				ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
				pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			}
			//ラスタライザステートと描画
			if (GetRasterizerState() == RasterizerState::DoubleDraw) {
				//透明処理用
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}
			else {
				RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
				//描画
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}
			//後始末
//			Dev->InitializeStates();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	モデルメッシュの描画
		@tparam[in]	T_VShader	使用する頂点シェーダ
		@tparam[in]	T_PShader	使用するピクセルシェーダ
		@param[in]	data	メッシュのデータ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawModel(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULLのシェーダリソースの準備
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//サンプラーの準備
			ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
			ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//各オブジェクト共通処理
			//シェーダの設定
			//頂点シェーダ
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//ピクセルシェーダ
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//ストライドとオフセット
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			//描画方法のセット
			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//各レンダリングステートの設定
			//ブレンドステート
			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
			//デプスステンシルステート
			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
			//影とサンプラー
			if (GetOwnShadowActive()) {
				//シャドウマップのレンダラーターゲット
				auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
				ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
				pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
				//シャドウマップサンプラー
				ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
				pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			}
			//ライトを設定
			SetLightingParamaters();
			BasicConstants BcCb;
			//コンスタントバッファの作成
			SetConstants(BcCb, data);
			//メッシュ内のマテリアルの配列
			auto& MatVec = data.m_MaterialExVec;
			size_t MatIndex = 0;
			for (auto& m : MatVec) {
				if (m.m_TextureResource) {
					//テクスチャがある
					BcCb.activeFlg.y = 1;
				}
				else {
					//テクスチャがない
					BcCb.activeFlg.y = 0;
				}
				if (IsModelDiffusePriority()) {
					BcCb.diffuseColor = m.m_Diffuse;
				}
				if (IsModelEmissivePriority()) {
					bsm::Col4 Em4 = m.m_Emissive;
					Em4.w = 0.0f;
					BcCb.emissiveColor = Em4;
				}
				//コンスタントバッファの更新
				pD3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//頂点シェーダに渡す
				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//ピクセルシェーダに渡す
				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				if (BcCb.activeFlg.y) {
					//シェーダにテクスチャの設定がされている
					//サンプラーの設定
					RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
					//テクスチャの選択
					if (IsModelTextureEnabled()) {
						//モデルのテクスチャが有効
						pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
					}
					else {
						//モデルのテクスチャが無効
						auto shTex = GetTextureResource();
						if (shTex) {
							//コンポーネントにテクスチャがある
							pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
						}
						else {
							//コンポーネントにテクスチャがない
							BcCb.activeFlg.y = 0;
						}
					}
					if (BcCb.activeFlg.y) {
						//法線マップ
						if (GetNormalMapTextureResourceVec().size() > MatIndex) {
							//複数の法線マップがある
							auto& normalVec = GetNormalMapTextureResourceVec();
							auto normalShptr = normalVec[MatIndex].lock();
							if (GetOwnShadowActive()) {
								//２番目に入れる
								pD3D11DeviceContext->PSSetShaderResources(2, 1, normalShptr->GetShaderResourceView().GetAddressOf());
								//サンプラーを設定
								RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
							}
							else {
								//1番目に入れる
								pD3D11DeviceContext->PSSetShaderResources(1, 1, normalShptr->GetShaderResourceView().GetAddressOf());
								//サンプラーを設定
								RenderState->SetSamplerState(pD3D11DeviceContext,GetSamplerState(), 1);
							}
						}
						else if (GetNormalMapTextureResource()) {
							//そうではなくて一つだけ法線マップがある
							if (GetOwnShadowActive()) {
								//２番目に入れる
								pD3D11DeviceContext->PSSetShaderResources(2, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
								//サンプラーを設定
								RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
							}
							else {
								//1番目に入れる
								pD3D11DeviceContext->PSSetShaderResources(1, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
								//サンプラーを設定
								RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 1);
							}
						}
					}
					else {
						//シェーダーリソースもクリア
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//サンプラーもクリア
						RenderState->SetSamplerAllClear(pD3D11DeviceContext);
					}
				}
				else {
					//シェーダーリソースもクリア
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//サンプラーもクリア
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
				//ラスタライザステートと描画
				if (GetRasterizerState() == RasterizerState::DoubleDraw) {
					//透明処理用
					//ラスタライザステート(裏描画)
					pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
					//描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート（表描画）
					pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
					//描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
					//描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				MatIndex++;
			}
		}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュを使うかどうか
		@return	使う場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsOriginalMeshUse() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュを使うかどうかを設定
		@param[in]	b	オリジナルメッシュを使うかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetOriginalMeshUse(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルなメッシュリソースの取得
		@return	オリジナルなメッシュリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MeshResource> GetOriginalMeshResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルなメッシュリソースの設定
		@param[in]	MeshRes	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュを作成する
		@param[in]	Vertices	頂点の配列
		@param[in]	indices		インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		void CreateOriginalMesh(vector<T>& Vertices, vector<uint16_t>& indices) {
			try {
				auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
				SetOriginalMeshResource(MeshRes);
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュの頂点バッファを変更する
		@param[in]	Vertices	頂点配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		void UpdateVertices(const vector<T>& Vertices) {
			auto MeshRes = GetOriginalMeshResource();
			if (!MeshRes) {
				throw BaseException(
					L"オリジナルメッシュが作成されていません",
					L"if (!GetOriginalMeshResource())",
					L"BcStaticBaseDraw::UpdateVertices()"
				);

			}
			MeshRes->UpdateVirtexBuffer(Vertices);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの取得(オリジナルかどうかは内部で処理される)
		@return	メッシュリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MeshResource> GetMeshResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルではないメッシュリソースの設定(仮想関数)
		@param[in]	MeshRes	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルではないメッシュリソースの設定
		@param[in]	MeshKey	メッシュキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const wstring& MeshKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースの設定
		@param[in]	TextureRes	テクスチャリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースの設定
		@param[in]	TextureKey	登録されているテクスチャキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const wstring& TextureKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースの取得
		@return	テクスチャリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチメッシュリソースを得る
		@param[in]	ExceptionActive	エラー時に例外が発生するかどうか
		@return	メッシュリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MultiMeshResource> GetMultiMeshResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチメッシュリソースを設定する
		@param[in]	MeshResourcePtr	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチメッシュリソースを設定する
		@param[in]	ResKey	メッシュリソースのキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMultiMeshResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッシブ色の取得
		@return	エミッシブ色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetEmissive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッシブ色の設定
		@param[in]	col	エミッシブ色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEmissive(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デフューズ色の取得
		@return	デフューズ色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetDiffuse() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デフューズ色の設定
		@param[in]	col	デフューズ色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDiffuse(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スペキュラー色の取得
		@return	スペキュラー色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetSpecularColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スペキュラー色の設定
		@param[in]	col	スペキュラー色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSpecularColor(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スペキュラー色をなくす
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DisableSpecular();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スペキュラーパワーの取得
		@return	スペキュラーパワー色
		*/
		//--------------------------------------------------------------------------------------
		float GetSpecularPower() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スペキュラーパワーの設定
		@param[in]	pow	スペキュラーパワー色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSpecularPower(float pow);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スペキュラーとパワー色の取得
		@return	スペキュラーとパワー色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetSpecularColorAndPower() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スペキュラーとパワー色の設定
		@param[in]	col	スペキュラーとパワー色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSpecularColorAndPower(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アルファ値の取得
		@return	アルファ値
		*/
		//--------------------------------------------------------------------------------------
		float GetAlpha() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アルファ値の設定
		@param[in]	pow	アルファ値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAlpha(float a);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーとアルファ色の取得
		@return	カラーとアルファ色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetColorAndAlpha() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーとアルファ色の設定
		@param[in]	col カラーとアルファ色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetColorAndAlpha(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングするかどうか
		@return	する場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsLightingEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングするかどうか
		@return	する場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetLightingEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングするかどうか設定
		@param[in] value ライティングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLightingEnabled(bool value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ピクセルライティングするかどうか
		@return	する場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetPerPixelLighting() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ピクセルライティングするかどうか
		@return	する場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsPerPixelLighting() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ピクセルライティングするかどうか設定
		@param[in] value ピクセルライティングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPerPixelLighting(bool value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングパラメータを設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLightingParamaters();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	法線にバイアスをかけるかどうか
		@return	する場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetBiasedNormals() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	法線にバイアスをかけるかどうか
		@return	する場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsBiasedNormals() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	法線にバイアスをかけるかどうか設定
		@param[in] value 法線にバイアスをかけるかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBiasedNormals(bool value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アンビエントライト色の取得
		@return	アンビエントライト色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetAmbientLightColor()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アンビエントライト色の設定
		@param[in] value アンビエントライト色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAmbientLightColor(const bsm::Col4& value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスが有効かどうかチェックする（無効なら例外）
		@param[in] whichLight ライトのインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ValidateLightIndex(int whichLight);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトの最大値を得る
		@return	ライトの最大値
		*/
		//--------------------------------------------------------------------------------------
		int GetMaxDirectionalLights();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のライトを有効にするかどうかを得る
		@param[in] whichLight ライトのインデックス
		@return	有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetLightEnabled(int whichLight)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のライトを有効にするかどうかを得る
		@param[in] whichLight ライトのインデックス
		@return	有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsLightEnabled(int whichLight)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のライトを有効にするかどうか
		@param[in] whichLight ライトのインデックス
		@param[in] value 有効にするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLightEnabled(int whichLight, bool value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のライトの向きを得る
		@param[in] whichLight ライトのインデックス
		@return	指定のライトの向き
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetLightDirection(int whichLight) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のライトの向きを設定する
		@param[in] whichLight ライトのインデックス
		@param[in] value 値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLightDirection(int whichLight, const bsm::Vec3& value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のライトのデフューズ色を得る
		@param[in] whichLight ライトのインデックス
		@return	指定のライトのデフューズ色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetLightDiffuseColor(int whichLight) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のライトのデフューズ色を設定する
		@param[in] whichLight ライトのインデックス
		@param[in] value 値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLightDiffuseColor(int whichLight, const bsm::Col4& value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のライトのスペキュラー色を得る
		@param[in] whichLight ライトのインデックス
		@return	指定のライトのスペキュラー色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetLightSpecularColor(int whichLight) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のライトのスペキュラー色を設定する
		@param[in] whichLight ライトのインデックス
		@param[in] value 値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLightSpecularColor(int whichLight, const bsm::Col4& value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォグを使うかどうか
		@return	フォグを使うならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetFogEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォグを使うかどうか
		@return	フォグを使うならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsFogEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォグを使うかどうか指定
		@param[in]	b	フォグを使うならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFogEnabled(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォグの開始位置を得る
		@return	フォグの開始位置
		*/
		//--------------------------------------------------------------------------------------
		float GetFogStart() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォグの開始位置指定
		@param[in]	value	フォグの開始位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFogStart(float value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォグの終了位置を得る
		@return	フォグの終了位置
		*/
		//--------------------------------------------------------------------------------------
		float GetFogEnd() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォグの終了位置指定
		@param[in]	value	フォグの終了位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFogEnd(float value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォグ色の取得
		@return	フォグ色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetFogColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォグ色の設定
		@param[in]	col	フォグ色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFogColor(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	影を描画するかどうか得る
		@return	影を描画すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetOwnShadowActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	影を描画するかどうか得る
		@return	影を描画すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsOwnShadowActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	影を描画するかどうか設定する
		@param[in]	b		影を描画するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetOwnShadowActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デフィーズ色をモデル設定優先かどうか得る
		@return	デフィーズ色をモデル設定優先ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetModelDiffusePriority() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デフィーズ色をモデル設定優先かどうか得る
		@return	デフィーズ色をモデル設定優先ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsModelDiffusePriority() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デフィーズ色をモデル設定優先かどうか設定する
		@param[in]	b	デフィーズ色をモデル設定優先かどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetModelDiffusePriority(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッシブ色をモデル設定優先かどうか得る
		@return	エミッシブ色をモデル設定優先ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetModelEmissivePriority() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッシブ色をモデル設定優先かどうか得る
		@return	エミッシブ色をモデル設定優先ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsModelEmissivePriority() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッシブ色をモデル設定優先かどうか設定する
		@param[in]	b	エミッシブ色をモデル設定優先かどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetModelEmissivePriority(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	モデルのテクスチャを有効にするかどうか得る
		@return	モデルのテクスチャを有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetModelTextureEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	モデルのテクスチャを有効にするかどうか得る
		@return	モデルのテクスチャを有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsModelTextureEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	モデルのテクスチャを有効にするかどうか設定する
		@param[in]	b	モデルのテクスチャを有効にするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetModelTextureEnabled(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	法線マップテクスチャリソースの取得
		@return	法線マップテクスチャリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetNormalMapTextureResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	法線マップテクスチャリソースの設定
		@param[in]	TextureRes	テクスチャリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNormalMapTextureResource(const shared_ptr<TextureResource>& TextureRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	法線マップテクスチャリソースの設定
		@param[in]	TextureKey	テクスチャキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNormalMapTextureResource(const wstring& TextureKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	法線マップテクスチャリソース配列の取得
		@return	法線マップテクスチャリソース配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<weak_ptr<TextureResource>>& GetNormalMapTextureResourceVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	法線マップテクスチャリソース配列の取得
		@return	法線マップテクスチャリソース配列
		*/
		//--------------------------------------------------------------------------------------
		vector<weak_ptr<TextureResource>>& GetNormalMapTextureResourceVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ボーンの初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void BoneInit();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	開始行列から終了行列の間のt時間時の行列を返す
		@param[in]	m1	開始の行列
		@param[in]	m2	終了の行列
		@param[in]	t	時間（0から1.0f）
		@param[out]	out	結果を受け取る行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アニメーションを追加する
		@param[in]	Name	アニメーション名
		@param[in]	StartSample	開始サンプル
		@param[in]	SampleLength	サンプルの長さ
		@param[in]	Loop	ループするかどうか
		@param[in]	SamplesParSecond = 30.0f	1秒あたりのサンプル数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
			float SamplesParSecond = 30.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のアニメーションを変更する
		@param[in]	AnemationName	アニメーション名（すでに追加されているもの）
		@param[in]	StartTime = 0.0f	開始からの秒数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ChangeCurrentAnimation(const wstring& AnemationName, float StartTime = 0.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のアニメーション名を得る
		@return	アニメーション名（文字列）
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetCurrentAnimation() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のアニメーションタイムを得る
		@return	アニメーションタイム
		*/
		//--------------------------------------------------------------------------------------
		float GetCurrentAnimationTime() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のアニメーションが終了しているかどうか
		@return	修了していればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsTargetAnimeEnd() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のアニメーションを進める
		@param[in]	ElapsedTime	経過時間
		@return	アニメーションが終了すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool UpdateAnimation(float ElapsedTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ローカルボーン行列配列を得る
		@return	ローカルボーン行列配列の先頭ポインタ
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< bsm::Mat4x4 >* GetVecLocalBonesPtr() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ローカルボーン行列配列を得る（マルチメッシュ版）
		@return	ローカルボーン行列配列の先頭ポインタ
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< bsm::Mat4x4 >* GetVecMultiLocalBonesPtr(size_t index) const override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///BcPCStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BcPCStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPCStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BasicPTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BcPTStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BasicPCTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BcPCTStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPCTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



	//--------------------------------------------------------------------------------------
	///	BcPNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BcPNTStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



	//--------------------------------------------------------------------------------------
	///	BasicPNTStaticモデル描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BcPNTStaticModelDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTStaticModelDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNTBoneModel描画コンポーネント（ボーンモデル描画用）
	//--------------------------------------------------------------------------------------
	class BcPNTBoneModelDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTBoneModelDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの設定
		@param[in]	MeshRes	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの登録
		@param[in]	MeshKey	登録されているメッシュキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const wstring& MeshKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチメッシュリソースを設定する
		@param[in]	MeshResourcePtr	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチメッシュリソースを設定する
		@param[in]	ResKey	メッシュリソースのキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMultiMeshResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BasicPNTnTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BcPNTnTStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	BasicPNTnTモデル描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BcPNTnTStaticModelDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTStaticModelDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNTnTBoneModelDraw描画コンポーネント（ボーンモデル描画用）
	//--------------------------------------------------------------------------------------
	class BcPNTnTBoneModelDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTBoneModelDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの設定
		@param[in]	MeshRes	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの登録
		@param[in]	MeshKey	登録されているメッシュキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const wstring& MeshKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチメッシュリソースを設定する
		@param[in]	MeshResourcePtr	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチメッシュリソースを設定する
		@param[in]	ResKey	メッシュリソースのキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMultiMeshResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};




}
// end basecross
