/*!
@file DrawObjects.h
@brief 描画オブジェクト
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Scene;
	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	シャドウマップ描画に使用する構造体
	//--------------------------------------------------------------------------------------
	struct ShadowmapObject {
		shared_ptr<MeshResource> m_MeshRes;
		Mat4x4 m_WorldMatrix;
		vector<Mat4x4>* m_pLocalBoneVec;
		//法線マップを使用するか
		bool m_IsNormalmap;
		ShadowmapObject() :
			m_MeshRes(nullptr),
			m_WorldMatrix(),
			m_pLocalBoneVec(nullptr),
			m_IsNormalmap(false)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	シャドウマップの描画クラス
	//--------------------------------------------------------------------------------------
	class ShadowmapRenderer : public GameObject {
		vector<shared_ptr<ShadowmapObject>> m_ShadowmapObjectVec;
	public:
		static float m_LightHeight;	//ライトの高さ（向きをこの値で掛ける）
		static float m_LightNear;	//ライトのNear
		static float m_LightFar;		//ライトのFar
		static float m_ViewWidth;
		static float m_ViewHeight;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit ShadowmapRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShadowmapRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトを追加する
		@param[in]	Obj	ShadowmapObject構造体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawObject(const shared_ptr<ShadowmapObject>& Obj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	};

	//--------------------------------------------------------------------------------------
	///	アニメーションデータ構造体.
	//--------------------------------------------------------------------------------------
	struct	AnimationData
	{
		///	スタートサンプル
		UINT	m_StartSample;
		///	サンプルの長さ
		UINT	m_SampleLength;
		///	ループするかどうか
		bool	m_IsLoop;
		///	アニメが終了したかどうか
		bool	m_IsAnimeEnd;
		///	1秒当たりのフレーム
		float	m_SamplesParSecond;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ.
		*/
		//--------------------------------------------------------------------------------------
		AnimationData()
		{
			ZeroMemory(this, sizeof(AnimationData));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ.
		@param[in]	StartSample	スタートフレーム
		@param[in]	SampleLength	フレームの長さ
		@param[in]	bLoop	ループするかどうか
		@param[in]	SamplesParSecond = 30.0f	1秒あたりのフレーム数
		*/
		//--------------------------------------------------------------------------------------
		AnimationData(UINT StartSample, UINT SampleLength, bool bLoop,
			float SamplesParSecond = 30.0f) :
			m_StartSample{ StartSample },
			m_SampleLength{ SampleLength },
			m_IsLoop{ bLoop },
			m_IsAnimeEnd{ false },
			m_SamplesParSecond{ SamplesParSecond }
		{}
	};

	//--------------------------------------------------------------------------------------
	///	描画に使用する構造体(親)
	//--------------------------------------------------------------------------------------
	struct DrawObjectBase {
		//メッシュリソース
		shared_ptr<MeshResource> m_MeshRes;
		//以下、ボーンモデル用
		//ローカルボーン行列の配列
		vector<Mat4x4> m_LocalBonesMatrix;
		//アニメーション定義のマップ
		map<wstring, AnimationData> m_AnimationMap;
		//現在のアニメーション名
		wstring m_CurrentAnimeName;
		//カレントアニメーション
		float m_CurrentAnimeTime;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		DrawObjectBase():
			m_MeshRes(nullptr),
			m_CurrentAnimeName(L""),
			m_CurrentAnimeTime(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ボーンの初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void BoneInit() {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				//先頭のボーン数の行列で初期化
				m_LocalBonesMatrix.resize(m_MeshRes->GetBoneCount());
				auto& SampleMatrixVec = m_MeshRes->GetSampleMatrixVec();
				for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
					m_LocalBonesMatrix[i] = SampleMatrixVec[i];
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のアニメーションを変更する
		@param[in]	AnemationName	アニメーション名（すでに追加されているもの）
		@param[in]	StartTime = 0.0f	開始からの秒数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ChangeCurrentAnimation(const wstring& AnemationName, float StartTime = 0.0f) {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				if (AnemationName == L"") {
					throw BaseException(
						L"アニメーション名が空白です",
						L"if (AnemationName == L\"\")",
						L"BcDrawObject::SetCurrentAnimation()"
					);
				}
				auto it = m_AnimationMap.find(AnemationName);
				if (it != m_AnimationMap.end()) {
					//指定の名前が見つかった
					m_CurrentAnimeName = AnemationName;
					m_CurrentAnimeTime = StartTime;
					//アニメーションは終了していない
					it->second.m_IsAnimeEnd = false;
				}
				else {
					//見つからない
					throw BaseException(
						L"指定のアニメーションは登録されてません",
						AnemationName,
						L"BcDrawObject::SetCurrentAnimation()"
					);
				}
			}
		}
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
			float SamplesParSecond = 30.0f) {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				if (Name == L"") {
					throw BaseException(
						L"アニメーション名が空白です",
						L"if (Name == L\"\")",
						L"BcDrawObject::AddAnimation()"
					);
				}
				if (StartSample < 0 || SampleLength < 0) {
					throw BaseException(
						L"開始サンプルかサンプル数が0未満です",
						L"if (StartSample < 0 || SampleLength < 0)",
						L"BcDrawObject::AddAnimation()"
					);
				}
				if (SamplesParSecond <= 0.0f) {
					throw BaseException(
						L"サンプル毎秒が0以下です",
						L"if (SamplesParSecond <= 0.0f)",
						L"BcDrawObject::AddAnimation()"
					);
				}
				//重複キーがあれば差し替える
				AnimationData Data((UINT)StartSample, (UINT)SampleLength, Loop, SamplesParSecond);
				m_AnimationMap[Name] = Data;
				if (m_AnimationMap.size() == 1) {
					//1つしか登録がなかったら、カレントアニメは該当アニメとなる
					ChangeCurrentAnimation(Name, 0);
				}
			}
		}
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
		void InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out) {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				Vec3 Scale1, Pos1;
				Quat Qt1;
				m1.decompose(Scale1, Qt1, Pos1);
				Qt1.normalize();

				Vec3 Scale2, Pos2;
				Quat Qt2;

				m2.decompose(Scale2, Qt2, Pos2);
				Qt2.normalize();

				Vec3 ScaleOut, PosOut;
				Quat QtOut;

				ScaleOut = Lerp::CalculateLerp(Scale1, Scale2, 0.0f, 1.0f, t, Lerp::Linear);
				PosOut = Lerp::CalculateLerp(Pos1, Pos2, 0.0f, 1.0f, t, Lerp::Linear);
				QtOut = XMQuaternionSlerp(Qt1, Qt2, t);
				out.affineTransformation(ScaleOut, Vec3(0, 0, 0), QtOut, PosOut);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のアニメーションを進める
		@param[in]	ElapsedTime	経過時間
		@return	アニメーションが終了すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool UpdateAnimation(float ElapsedTime) {
			if (m_MeshRes && m_MeshRes->IsSkining() && m_MeshRes->GetBoneCount() > 0 && m_MeshRes->GetSampleCount() > 0) {
				if (ElapsedTime < 0.0f) {
					throw BaseException(
						L"アニメーション更新にマイナスは設定できません",
						L"if (ElapsedTime < 0.0f)",
						L"BcDrawObject::UpdateAnimation()"
					);
				}
				if (m_CurrentAnimeName == L"") {
					//見つからない
					throw BaseException(
						L"カレントアニメーションが設定されてません",
						L"if (m_CurrentAnimeName == L\"\")",
						L"BcDrawObject::UpdateAnimation()"
					);
				}
				auto PtrMesh = m_MeshRes;
				UINT SampleCount = PtrMesh->GetSampleCount();
				auto& SampleMatrixVec = PtrMesh->GetSampleMatrixVec();
				UINT BoneCount = PtrMesh->GetBoneCount();
				auto& TgtAnimeData = m_AnimationMap[m_CurrentAnimeName];
				if (TgtAnimeData.m_StartSample >= SampleCount) {
					//スタートのサンプルが最後のサンプル以降だった
					TgtAnimeData.m_StartSample = SampleCount - 1;
					TgtAnimeData.m_SampleLength = 0;
					UINT UITgtSample = TgtAnimeData.m_StartSample;
					//最後のサンプルを表示
					for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
						m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
					}
					m_CurrentAnimeTime = 0;
					if (TgtAnimeData.m_IsLoop) {
						TgtAnimeData.m_IsAnimeEnd = false;
						return false;
					}
					else {
						TgtAnimeData.m_IsAnimeEnd = true;
						return true;
					}
				}
				//すでにアニメが終了している
				if (TgtAnimeData.m_IsAnimeEnd) {
					//現在のローカル行列を使用
					return true;
				}
				//カレントタイムを更新
				m_CurrentAnimeTime += ElapsedTime;
				//スタート位置を計算
				auto FLOATTgtSample = (float)TgtAnimeData.m_StartSample + m_CurrentAnimeTime * TgtAnimeData.m_SamplesParSecond;
				UINT UITgtSample = (UINT)FLOATTgtSample;
				UINT UILastSample = TgtAnimeData.m_StartSample + TgtAnimeData.m_SampleLength;
				if (UILastSample >= SampleCount) {
					UILastSample = SampleCount - 1;
				}
				if (UITgtSample >= UILastSample) {
					UITgtSample = UILastSample - 1;
					//最後のサンプルを表示
					for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
						m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
					}
					if (TgtAnimeData.m_IsLoop) {
						TgtAnimeData.m_IsAnimeEnd = false;
						//ループするのでカレントタイムを0にする
						m_CurrentAnimeTime = 0;
						return false;
					}
					else {
						m_CurrentAnimeTime = TgtAnimeData.m_SampleLength / TgtAnimeData.m_SamplesParSecond;
						TgtAnimeData.m_IsAnimeEnd = true;
						return true;
					}
				}
				else {
					//サンプルとサンプルの間の割合を計算
					FLOATTgtSample -= (float)UITgtSample;
					UINT UINextSample = UITgtSample + 1;
					for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
						InterpolationMatrix(
							SampleMatrixVec[BoneCount * UITgtSample + i],
							SampleMatrixVec[BoneCount * UINextSample + i],
							FLOATTgtSample, m_LocalBonesMatrix[i]);
					}
					//アニメは終わってない
					return false;
				}
			}
			return true;
		}
	};


	//--------------------------------------------------------------------------------------
	///	シンプル描画に使用する構造体
	//--------------------------------------------------------------------------------------
	struct SimpleDrawObject :public DrawObjectBase {
		//テクスチャリソース
		shared_ptr<TextureResource> m_TextureRes;
		//ブレンドステート
		BlendState m_BlendState;
		//デプスステンシルステート
		DepthStencilState m_DepthStencilState;
		//ラスタライザステート
		RasterizerState m_RasterizerState;
		//サンプラーステート
		SamplerState m_SamplerState;
		//ワールド行列
		Mat4x4 m_WorldMatrix;
		//モデルに入っている色を使うかどうか
		bool m_UsedModelColor;
		//モデルに入っているテクスチャを使うかどうか
		bool m_UsedModelTextre;
		//自己影があるかどうか
		bool m_OwnShadowmapActive;
		//シャドウマップを発行しているか
		bool m_ShadowmapUse;
		//エミッシブ色
		Col4 m_Emissive;
		//デフューズ色
		Col4 m_Diffuse;
		//スペキュラー
		Col4 m_Specular;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		SimpleDrawObject() :
			DrawObjectBase(),
			m_TextureRes(nullptr),
			m_BlendState(BlendState::Opaque),
			m_DepthStencilState(DepthStencilState::Default),
			m_RasterizerState(RasterizerState::CullBack),
			m_SamplerState(SamplerState::LinearClamp),
			m_WorldMatrix(),
			m_UsedModelColor(true),
			m_UsedModelTextre(true),
			m_OwnShadowmapActive(false),
			m_ShadowmapUse(false),
			m_Emissive(0.4f, 0.4f, 0.4f, 0),
			m_Diffuse(0.8f, 0.8f, 0.8f, 1.0f),
			m_Specular(0.2f, 0.2f, 0.2f, 0)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	Simple描画オブジェクトの親
	//--------------------------------------------------------------------------------------
	class SimpleRenderer : public GameObject {
	protected:
		vector<shared_ptr<SimpleDrawObject>> m_DrawObjectVec;
		vector<shared_ptr<SimpleDrawObject>> m_TraceDrawObjectVec;
		vector<shared_ptr<SimpleDrawObject>> m_ShadowDrawObjectVec;
		vector<shared_ptr<SimpleDrawObject>> m_ShadowTraceDrawObjectVec;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		explicit SimpleRenderer(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimpleRenderer() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスタントバッファの設定
		@param[in]	Obj	設定するデータ
		@param[out]	BcCb	設定するコンスタントバッファ構造体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(const shared_ptr<SimpleDrawObject>& Obj, SimpleConstants& Cb);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタティックメッシュの描画(配列単位)
		@tparam[in]	T_VShader	使用する頂点シェーダ
		@tparam[in]	T_PShader	使用するピクセルシェーダ
		@param[in]	ObjectVec	設定するデータ配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawStaticVec(const vector<shared_ptr<SimpleDrawObject>>& ObjectVec) {
			auto PtrGameStage = GetStage<GameStage>();
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
			//個別処理
			for (auto& v : ObjectVec) {
				SimpleConstants Cb;
				//コンスタントバッファの作成
				SetConstants(v, Cb);
				if (v->m_TextureRes) {
					//テクスチャがある
					Cb.ActiveFlg.x = 1;
				}
				else {
					//テクスチャがない
					Cb.ActiveFlg.x = 0;
				}
				//コンスタントバッファの更新
				pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//頂点シェーダに渡す
				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//ピクセルシェーダに渡す
				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				//ストライドとオフセット
				UINT stride = v->m_MeshRes->GetNumStride();
				UINT offset = 0;
				//描画方法のセット
				pD3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
				//頂点バッファのセット
				pD3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//インデックスバッファのセット
				pD3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//各レンダリングステートの設定
				//ブレンドステート
				RenderState->SetBlendState(pD3D11DeviceContext, v->m_BlendState);
				//デプスステンシルステート
				RenderState->SetDepthStencilState(pD3D11DeviceContext, v->m_DepthStencilState);
				//テクスチャとサンプラー
				if (v->m_TextureRes) {
					pD3D11DeviceContext->PSSetShaderResources(0, 1, v->m_TextureRes->GetShaderResourceView().GetAddressOf());
					//サンプラーを設定
					RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 0);
				}
				else {
					//シェーダーリソースもクリア
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//サンプラーもクリア
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
				//影とサンプラー
				if (v->m_OwnShadowmapActive) {
					//シャドウマップのレンダラーターゲット
					auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
					ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
					pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//シャドウマップサンプラー
					ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
					pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
				}
				//ラスタライザステートと描画
				if (v->m_RasterizerState == RasterizerState::DoubleDraw) {
					//透明処理用
					//ラスタライザステート(裏描画)
					pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
					//描画
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
					//ラスタライザステート（表描画）
					pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
					//描画
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
				}
				else {
					RenderState->SetRasterizerState(pD3D11DeviceContext, v->m_RasterizerState);
					//描画
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	モデルメッシュの描画(配列単位)
		@tparam[in]	T_VShader	使用する頂点シェーダ
		@tparam[in]	T_PShader	使用するピクセルシェーダ
		@param[in]	ObjectVec	設定するデータ配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawModelVec(const vector<shared_ptr<SimpleDrawObject>>& ObjectVec) {
			auto PtrGameStage = GetStage<GameStage>();
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
			//個別処理
			for (auto& v : ObjectVec) {
				//ストライドとオフセット
				UINT stride = v->m_MeshRes->GetNumStride();
				UINT offset = 0;
				//描画方法のセット
				pD3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
				//頂点バッファのセット
				pD3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//インデックスバッファのセット
				pD3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//各レンダリングステートの設定
				//ブレンドステート
				RenderState->SetBlendState(pD3D11DeviceContext, v->m_BlendState);
				//デプスステンシルステート
				RenderState->SetDepthStencilState(pD3D11DeviceContext, v->m_DepthStencilState);
				//影とサンプラー
				if (v->m_OwnShadowmapActive) {
					//シャドウマップのレンダラーターゲット
					auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
					ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
					pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//シャドウマップサンプラー
					ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
					pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
				}
				SimpleConstants Cb;
				//コンスタントバッファの作成
				SetConstants(v, Cb);
				//メッシュ内のマテリアルの配列
				auto& MatVec = v->m_MeshRes->GetMaterialExVec();
				size_t MatIndex = 0;
				for (auto& m : MatVec) {
					if (v->m_UsedModelColor) {
						Cb.Emissive = m.m_Emissive;
						Cb.Emissive.w = 0;
						Cb.Diffuse = m.m_Diffuse;
					}
					//テクスチャが有効無効の設定
					if (v->m_UsedModelTextre) {
						//モデルのテクスチャが有効
						if (m.m_TextureResource) {
							//テクスチャがある
							Cb.ActiveFlg.x = 1;
						}
						else {
							//テクスチャがない
							Cb.ActiveFlg.x = 0;
						}
					}
					else {
						//モデルのテクスチャが有効ではない
						if (v->m_TextureRes) {
							//しかし、データにテクスチャがある
							Cb.ActiveFlg.x = 1;
						}
						else {
							//テクスチャがない
							Cb.ActiveFlg.x = 0;
						}
					}
					//コンスタントバッファの更新
					pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
					//コンスタントバッファの設定
					ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
					ID3D11Buffer* pNullConstantBuffer = nullptr;
					//頂点シェーダに渡す
					pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					//ピクセルシェーダに渡す
					pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
					if (Cb.ActiveFlg.x) {
						//シェーダにテクスチャの設定がされている
						//サンプラーの設定
						RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 0);
						//テクスチャの選択
						if (v->m_UsedModelTextre) {
							//モデルのテクスチャが有効
							pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
						}
						else {
							//モデルのテクスチャが無効
							pD3D11DeviceContext->PSSetShaderResources(0, 1, v->m_TextureRes->GetShaderResourceView().GetAddressOf());
						}
					}
					else {
						//シェーダーリソースもクリア
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//サンプラーもクリア
						RenderState->SetSamplerAllClear(pD3D11DeviceContext);
					}
					//ラスタライザステートと描画
					if (v->m_RasterizerState == RasterizerState::DoubleDraw) {
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
						RenderState->SetRasterizerState(pD3D11DeviceContext, v->m_RasterizerState);
						//描画
						pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					}
					MatIndex++;
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュの描画
		@tparam[in]	T_VShader	使用する頂点シェーダ（影無し）
		@tparam[in]	T_VShaderShadow	使用する頂点シェーダ（影付き）
		@tparam[in]	T_PShader	使用するピクセルシェーダ（影無し）
		@tparam[in]	T_PShaderShadow	使用するピクセルシェーダ（影付き）
		@param[in]	IsModel	モデルかどうか。モデルならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_VShaderShadow, typename T_PShader, typename T_PShaderShadow>
		void Draw(bool IsModel) {
			auto PtrGameStage = GetStage<GameStage>();
			//サブ関数呼び出し(不透明)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_DrawObjectVec);
				DrawModelVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_DrawObjectVec);
				DrawStaticVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowDrawObjectVec);
			}
			//カメラの位置
			Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
			//--------------------------------------------------------
			//透明の3Dオブジェクトをカメラからの距離でソート
			//以下は、オブジェクトを引数に取りboolを返すラムダ式
			//--------------------------------------------------------
			auto func = [&](shared_ptr<SimpleDrawObject>& Left, shared_ptr<SimpleDrawObject>& Right)->bool {
				auto LeftPos = Left->m_WorldMatrix.transInMatrix();
				auto RightPos = Right->m_WorldMatrix.transInMatrix();
				auto LeftLen = bsm::length(LeftPos - CameraEye);
				auto RightLen = bsm::length(RightPos - CameraEye);
				return (LeftLen > RightLen);
			};
			//ラムダ式を使ってソート
			std::sort(m_TraceDrawObjectVec.begin(), m_TraceDrawObjectVec.end(), func);
			std::sort(m_ShadowTraceDrawObjectVec.begin(), m_ShadowTraceDrawObjectVec.end(), func);
			//サブ関数呼び出し(透明)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
				DrawModelVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowTraceDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
				DrawStaticVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowTraceDrawObjectVec);
			}
			//後始末
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->InitializeStates();
			ClearDrawObject();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュの描画(影描画無し)
		@tparam[in]	T_VShader	使用する頂点シェーダ（影無し）
		@tparam[in]	T_PShader	使用するピクセルシェーダ（影無し）
		@param[in]	IsModel	モデルかどうか。モデルならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void Draw(bool IsModel) {
			auto PtrGameStage = GetStage<GameStage>();
			//サブ関数呼び出し(不透明)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_DrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_DrawObjectVec);
			}
			//カメラの位置
			Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
			//--------------------------------------------------------
			//透明の3Dオブジェクトをカメラからの距離でソート
			//以下は、オブジェクトを引数に取りboolを返すラムダ式
			//--------------------------------------------------------
			auto func = [&](shared_ptr<SimpleDrawObject>& Left, shared_ptr<SimpleDrawObject>& Right)->bool {
				auto LeftPos = Left->m_WorldMatrix.transInMatrix();
				auto RightPos = Right->m_WorldMatrix.transInMatrix();
				auto LeftLen = bsm::length(LeftPos - CameraEye);
				auto RightLen = bsm::length(RightPos - CameraEye);
				return (LeftLen > RightLen);
			};
			//ラムダ式を使ってソート
			std::sort(m_TraceDrawObjectVec.begin(), m_TraceDrawObjectVec.end(), func);
			std::sort(m_ShadowTraceDrawObjectVec.begin(), m_ShadowTraceDrawObjectVec.end(), func);
			//サブ関数呼び出し(透明)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
			}
			//後始末
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->InitializeStates();
			ClearDrawObject();
		}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトを追加する
		@param[in]	DrawObj	SimpleDrawObject構造体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawObject(const shared_ptr<SimpleDrawObject>& DrawObj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトをクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ClearDrawObject();
	};

	//--------------------------------------------------------------------------------------
	///	SimplePCT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class SimplePCTStaticRenderer : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePCTStaticRenderer(const shared_ptr<Stage>& StagePtr,const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePCTStaticRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	
	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class SimplePNTStaticRenderer : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTStaticRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点オブジェクトの描画クラス(シャドウマップを持つ場合)
	//--------------------------------------------------------------------------------------
	class SimplePNTStaticRenderer2 : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTStaticRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTStaticRenderer2();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点モデルオブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class SimplePNTStaticModelRenderer : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTStaticModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点モデルオブジェクトの描画クラス(シャドウマップを持つ場合)
	//--------------------------------------------------------------------------------------
	class SimplePNTStaticModelRenderer2 : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTStaticModelRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTStaticModelRenderer2();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点ボーンモデルオブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class SimplePNTBoneModelRenderer : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTBoneModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点ボーンモデルオブジェクトの描画クラス(シャドウマップを持つ場合)
	//--------------------------------------------------------------------------------------
	class SimplePNTBoneModelRenderer2 : public SimpleRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	Tagname		タグ名
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		explicit SimplePNTBoneModelRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimplePNTBoneModelRenderer2();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};




	//--------------------------------------------------------------------------------------
	///	Bc描画に使用する構造体(影対応)
	//--------------------------------------------------------------------------------------
	struct BcDrawObject:public DrawObjectBase {
		//テクスチャリソース
		shared_ptr<TextureResource> m_TextureRes;
		//法線マップテクスチャリソース
		shared_ptr<TextureResource> m_NormalTextureRes;
		//モデルに複数のテクスチャがあった場合の法線マップテクスチャリソース
		vector<shared_ptr<TextureResource>> m_NormalTextureResVec;
		//ブレンドステート
		BlendState m_BlendState;
		//デプスステンシルステート
		DepthStencilState m_DepthStencilState;
		//ラスタライザステート
		RasterizerState m_RasterizerState;
		//サンプラーステート
		SamplerState m_SamplerState;
		//ワールド行列
		Mat4x4 m_WorldMatrix;
		//透明度
		float m_Alpha;
		//モデルに入っている色を使うかどうか
		bool m_UsedModelColor;
		//モデルに入っているテクスチャを使うかどうか
		bool m_UsedModelTextre;
		//自己影があるかどうか
		bool m_OwnShadowmapActive;
		//シャドウマップを発行しているか
		bool m_ShadowmapUse;
		//エミッシブ色
		Col4 m_Emissive;
		//デフューズ色
		Col4 m_Diffuse;
		//スペキュラーとパワー色
		Col4 m_SpecularColorAndPower;
		//ライティングするかどうか
		bool m_LightingEnabled;
		//フォグが有効かどうか
		bool m_FogEnabled;
		//フォグの開始位置
		float m_FogStart;
		//フォグの終了位置
		float m_FogEnd;
		//フォグ色
		Col4 m_FogColor;
		//フォグベクトル
		Vec3 m_FogVector;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		BcDrawObject() :
			DrawObjectBase(),
			m_TextureRes(nullptr),
			m_NormalTextureRes(nullptr),
			m_BlendState(BlendState::Opaque),
			m_DepthStencilState(DepthStencilState::Default),
			m_RasterizerState(RasterizerState::CullBack),
			m_SamplerState(SamplerState::LinearClamp),
			m_WorldMatrix(),
			m_Alpha(1.0f),
			m_UsedModelColor(true),
			m_UsedModelTextre(true),
			m_OwnShadowmapActive(false),
			m_ShadowmapUse(false),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_SpecularColorAndPower(0.0f, 0.0f, 0.0f, 1.0f),
			m_LightingEnabled(true),
			m_FogEnabled(false),
			m_FogStart(-25.0f),
			m_FogEnd(-40.0f),
			m_FogColor(0.8f, 0.8f, 0.8f, 1.0f),
			m_FogVector(0.0, 0.0, 1.0f)
		{}
	};



	//--------------------------------------------------------------------------------------
	///	Bc描画オブジェクトの親
	//--------------------------------------------------------------------------------------
	class BcRenderer : public GameObject {
	protected:
		vector<shared_ptr<BcDrawObject>> m_DrawObjectVec;
		vector<shared_ptr<BcDrawObject>> m_TraceDrawObjectVec;
		vector<shared_ptr<BcDrawObject>> m_ShadowDrawObjectVec;
		vector<shared_ptr<BcDrawObject>> m_ShadowTraceDrawObjectVec;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		explicit BcRenderer(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcRenderer() {}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスタントバッファの設定
		@param[in]	Obj	設定するデータ
		@param[out]	BcCb	設定するコンスタントバッファ構造体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(const shared_ptr<BcDrawObject>& Obj, BasicConstants& BcCb);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタティックメッシュの描画(配列単位)
		@tparam[in]	T_VShader	使用する頂点シェーダ
		@tparam[in]	T_PShader	使用するピクセルシェーダ
		@param[in]	ObjectVec	設定するデータ配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawStaticVec(const vector<shared_ptr<BcDrawObject>>& ObjectVec) {
			auto PtrGameStage = GetStage<GameStage>();
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
			//個別処理
			for (auto& v : ObjectVec) {
				BasicConstants BcCb;
				//コンスタントバッファの作成
				SetConstants(v, BcCb);
				if (v->m_TextureRes) {
					//テクスチャがある
					BcCb.activeFlg.y = 1;
				}
				else {
					//テクスチャがない
					BcCb.activeFlg.y = 0;
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
				UINT stride = v->m_MeshRes->GetNumStride();
				UINT offset = 0;
				//描画方法のセット
				pD3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
				//頂点バッファのセット
				pD3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//インデックスバッファのセット
				pD3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//各レンダリングステートの設定
				//ブレンドステート
				RenderState->SetBlendState(pD3D11DeviceContext, v->m_BlendState);
				//デプスステンシルステート
				RenderState->SetDepthStencilState(pD3D11DeviceContext, v->m_DepthStencilState);
				//テクスチャとサンプラー
				if (v->m_TextureRes) {
					pD3D11DeviceContext->PSSetShaderResources(0, 1, v->m_TextureRes->GetShaderResourceView().GetAddressOf());
					//サンプラーを設定
					RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 0);
					//法線マップ
					if (v->m_NormalTextureRes) {
						if (v->m_OwnShadowmapActive) {
							//２番目に入れる
							pD3D11DeviceContext->PSSetShaderResources(2, 1, v->m_NormalTextureRes->GetShaderResourceView().GetAddressOf());
							//サンプラーを設定
							RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 2);
						}
						else {
							//1番目に入れる
							pD3D11DeviceContext->PSSetShaderResources(1, 1, v->m_NormalTextureRes->GetShaderResourceView().GetAddressOf());
							//サンプラーを設定
							RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 1);
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
				if (v->m_OwnShadowmapActive) {
					//シャドウマップのレンダラーターゲット
					auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
					ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
					pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//シャドウマップサンプラー
					ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
					pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
				}
				//ラスタライザステートと描画
				if (v->m_RasterizerState == RasterizerState::DoubleDraw) {
					//透明処理用
					//ラスタライザステート(裏描画)
					pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
					//描画
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
					//ラスタライザステート（表描画）
					pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
					//描画
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
				}
				else {
					RenderState->SetRasterizerState(pD3D11DeviceContext, v->m_RasterizerState);
					//描画
					pD3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	モデルメッシュの描画(配列単位)
		@tparam[in]	T_VShader	使用する頂点シェーダ
		@tparam[in]	T_PShader	使用するピクセルシェーダ
		@param[in]	ObjectVec	設定するデータ配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawModelVec(const vector<shared_ptr<BcDrawObject>>& ObjectVec) {
			auto PtrGameStage = GetStage<GameStage>();
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
			//個別処理
			for (auto& v : ObjectVec) {
				//ストライドとオフセット
				UINT stride = v->m_MeshRes->GetNumStride();
				UINT offset = 0;
				//描画方法のセット
				pD3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
				//頂点バッファのセット
				pD3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//インデックスバッファのセット
				pD3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//各レンダリングステートの設定
				//ブレンドステート
				RenderState->SetBlendState(pD3D11DeviceContext, v->m_BlendState);
				//デプスステンシルステート
				RenderState->SetDepthStencilState(pD3D11DeviceContext, v->m_DepthStencilState);
				//影とサンプラー
				if (v->m_OwnShadowmapActive) {
					//シャドウマップのレンダラーターゲット
					auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
					ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
					pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//シャドウマップサンプラー
					ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
					pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
				}
				BasicConstants BcCb;
				//コンスタントバッファの作成
				SetConstants(v, BcCb);
				//メッシュ内のマテリアルの配列
				auto& MatVec = v->m_MeshRes->GetMaterialExVec();
				size_t MatIndex = 0;
				for (auto& m : MatVec) {
					if (v->m_UsedModelColor) {
						BcCb.emissiveColor = m.m_Emissive;
						BcCb.emissiveColor.w = 0;
						BcCb.diffuseColor = m.m_Diffuse;
					}
					//テクスチャが有効無効の設定
					if (v->m_UsedModelTextre) {
						//モデルのテクスチャが有効
						if (m.m_TextureResource) {
							//テクスチャがある
							BcCb.activeFlg.y = 1;
						}
						else {
							//テクスチャがない
							BcCb.activeFlg.y = 0;
						}
					}
					else {
						//モデルのテクスチャが有効ではない
						if (v->m_TextureRes) {
							//しかし、データにテクスチャがある
							BcCb.activeFlg.y = 1;
						}
						else {
							//テクスチャがない
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
					if (BcCb.activeFlg.y) {
						//シェーダにテクスチャの設定がされている
						//サンプラーの設定
						RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 0);
						//テクスチャの選択
						if (v->m_UsedModelTextre) {
							//モデルのテクスチャが有効
							pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
						}
						else {
							//モデルのテクスチャが無効
							pD3D11DeviceContext->PSSetShaderResources(0, 1, v->m_TextureRes->GetShaderResourceView().GetAddressOf());
						}
						//法線マップ
						if (v->m_NormalTextureResVec.size() > MatIndex) {
							//複数の法線マップがある
							if (v->m_OwnShadowmapActive) {
								//２番目に入れる
								pD3D11DeviceContext->PSSetShaderResources(2, 1, v->m_NormalTextureResVec[MatIndex]->GetShaderResourceView().GetAddressOf());
								//サンプラーを設定
								RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 2);
							}
							else {
								//1番目に入れる
								pD3D11DeviceContext->PSSetShaderResources(1, 1, v->m_NormalTextureResVec[MatIndex]->GetShaderResourceView().GetAddressOf());
								//サンプラーを設定
								RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 1);
							}
						}
						else if (v->m_NormalTextureRes) {
							//そうではなくて一つだけ法線マップがある
							if (v->m_OwnShadowmapActive) {
								//２番目に入れる
								pD3D11DeviceContext->PSSetShaderResources(2, 1, v->m_NormalTextureRes->GetShaderResourceView().GetAddressOf());
								//サンプラーを設定
								RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 2);
							}
							else {
								//1番目に入れる
								pD3D11DeviceContext->PSSetShaderResources(1, 1, v->m_NormalTextureRes->GetShaderResourceView().GetAddressOf());
								//サンプラーを設定
								RenderState->SetSamplerState(pD3D11DeviceContext, v->m_SamplerState, 1);
							}
						}
					}
					else {
						//シェーダーリソースもクリア
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//サンプラーもクリア
						RenderState->SetSamplerAllClear(pD3D11DeviceContext);
					}
					//ラスタライザステートと描画
					if (v->m_RasterizerState == RasterizerState::DoubleDraw) {
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
						RenderState->SetRasterizerState(pD3D11DeviceContext, v->m_RasterizerState);
						//描画
						pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					}
					MatIndex++;
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュの描画
		@tparam[in]	T_VShader	使用する頂点シェーダ（影無し）
		@tparam[in]	T_VShaderShadow	使用する頂点シェーダ（影付き）
		@tparam[in]	T_PShader	使用するピクセルシェーダ（影無し）
		@tparam[in]	T_PShaderShadow	使用するピクセルシェーダ（影付き）
		@param[in]	IsModel	モデルかどうか。モデルならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_VShaderShadow, typename T_PShader, typename T_PShaderShadow>
		void Draw(bool IsModel) {
			auto PtrGameStage = GetStage<GameStage>();
			//サブ関数呼び出し(不透明)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_DrawObjectVec);
				DrawModelVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_DrawObjectVec);
				DrawStaticVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowDrawObjectVec);
			}
			//カメラの位置
			Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
			//--------------------------------------------------------
			//透明の3Dオブジェクトをカメラからの距離でソート
			//以下は、オブジェクトを引数に取りboolを返すラムダ式
			//--------------------------------------------------------
			auto func = [&](shared_ptr<BcDrawObject>& Left, shared_ptr<BcDrawObject>& Right)->bool {
				auto LeftPos = Left->m_WorldMatrix.transInMatrix();
				auto RightPos = Right->m_WorldMatrix.transInMatrix();
				auto LeftLen = bsm::length(LeftPos - CameraEye);
				auto RightLen = bsm::length(RightPos - CameraEye);
				return (LeftLen > RightLen);
			};
			//ラムダ式を使ってソート
			std::sort(m_TraceDrawObjectVec.begin(), m_TraceDrawObjectVec.end(), func);
			std::sort(m_ShadowTraceDrawObjectVec.begin(), m_ShadowTraceDrawObjectVec.end(), func);
			//サブ関数呼び出し(透明)
			if (IsModel) {
				DrawModelVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
				DrawModelVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowTraceDrawObjectVec);
			}
			else {
				DrawStaticVec<T_VShader, T_PShader>(m_TraceDrawObjectVec);
				DrawStaticVec<T_VShaderShadow, T_PShaderShadow>(m_ShadowTraceDrawObjectVec);
			}
			//後始末
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->InitializeStates();
			ClearDrawObject();
		}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトを追加する
		@param[in]	DrawObj	BcDrawObject構造体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawObject(const shared_ptr<BcDrawObject>& DrawObj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトをクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ClearDrawObject();
		//ユーティリティ関数（static）
		//--------------------------------------------------------------------------------------
		/*!
		@brief 頂点フォーマットをPNTからPNTnTにコンバートをする
		@param[in]	vertices	元になるPNT頂点の配列
		@param[out]	new_vertices	コンバートされるPNTnT頂点の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertToNormalVertex(
			const vector<VertexPositionNormalTexture>& vertices,
			vector<VertexPositionNormalTangentTexture>& new_vertices
		);
	};



	//--------------------------------------------------------------------------------------
	///	BcPNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class BcPNTStaticRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTStaticRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNTnT頂点オブジェクト(法線マップ使用)の描画クラス
	//--------------------------------------------------------------------------------------
	class BcPNTnTStaticRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTStaticRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	BcPNT頂点モデルオブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class BcPNTStaticModelRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTStaticModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNTnT頂点モデルオブジェクトの描画クラス(法線マップ付きモデル)
	//--------------------------------------------------------------------------------------
	class BcPNTnTStaticModelRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTStaticModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNT頂点ボーンモデルオブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class BcPNTBoneModelRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTBoneModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	BcPNTnT頂点ボーンモデルオブジェクトの描画クラス(法線マップ付きモデル)
	//--------------------------------------------------------------------------------------
	class BcPNTnTBoneModelRenderer : public BcRenderer {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	Tagname		タグ名
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTBoneModelRenderer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



}
//end basecross
