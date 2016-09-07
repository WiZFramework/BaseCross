/*!
@file DrawComponents.cpp
@brief 描画コンポーネント実体
*/
#include "stdafx.h"

namespace basecross {

	//Shadowmap
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmap, App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmap.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmapBone, App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmapBone.cso")


	//PNTStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticNoTexture, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticNoTexture.cso")

	//PNTStaticShadow
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTStaticShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticShadow, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow2, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticShadow2.cso")



	//--------------------------------------------------------------------------------------
	//	struct DrawContext::Impl;
	//	用途: DrawContextイディオム
	//--------------------------------------------------------------------------------------
	struct DrawContext::Impl {
		struct PipelineResource {
			UINT m_PipelineNum;
			SamplerState m_SamplerState;
			ID3D11ShaderResourceView* m_ShaderResourceView;
			PipelineResource() :
				m_PipelineNum(0), m_SamplerState(SamplerState::SamplerNone), m_ShaderResourceView(nullptr)
			{}
		};
		//パイプライリソースの定義
		vector<PipelineResource> m_PipelineResourceVec;
		//パイプラインの定義
		PipeLineDesc m_PipeLineDesc;
		//シェーダー
		ID3D11VertexShader* m_pVertexShader;
		bool m_VertexShaderConstant;
		ID3D11PixelShader* m_pPixelShader;
		bool m_PixelShaderConstant;
		ID3D11InputLayout* m_pInputLayout;
		Impl() :
			m_pVertexShader(nullptr),
			m_VertexShaderConstant(false),
			m_pPixelShader(nullptr),
			m_PixelShaderConstant(false),
			m_pInputLayout(nullptr)
		{}
	};
	//--------------------------------------------------------------------------------------
	//	class DrawContext : public ObjectInterface;
	//--------------------------------------------------------------------------------------
	DrawContext::DrawContext() :
		pImpl(new Impl())
	{}
	DrawContext::~DrawContext() {}

	//パイプラインステートの設定
	void DrawContext::SetBlendState(BlendState State) {
		pImpl->m_PipeLineDesc.m_BlendState = State;
	}
	void DrawContext::SetDepthStencilState(DepthStencilState State) {
		pImpl->m_PipeLineDesc.m_DepthStencilState = State;
	}
	void DrawContext::SetRasterizerState(RasterizerState State) {
		pImpl->m_PipeLineDesc.m_RasterizerState = State;
	}
	void DrawContext::SetPipeLineDesc(const PipeLineDesc& Desc) {
		pImpl->m_PipeLineDesc = Desc;
	}

	const PipeLineDesc& DrawContext::GetPipeLineDesc()const {
		return pImpl->m_PipeLineDesc;
	}

	void DrawContext::AddSamplerAndSrv(UINT PilelineNum, SamplerState state, ID3D11ShaderResourceView* Srv) {
		Impl::PipelineResource res;
		res.m_PipelineNum = PilelineNum;
		res.m_SamplerState = state;
		res.m_ShaderResourceView = Srv;
		pImpl->m_PipelineResourceVec.push_back(res);
	}

	void DrawContext::DrawIndexedBase(const shared_ptr<GameObject>& GameObjectPtr,
		ID3D11Buffer* pConstantBuffer, const void* CBData,
		const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset) {
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステージの取得
		auto PtrStage = GameObjectPtr->GetStage();
		//ステータスのポインタ
		auto RenderStatePtr = Dev->GetRenderState();
		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, CBData, 0, 0);
		//コンスタントバッファの各シェーダへ設置
		if (pImpl->m_VertexShaderConstant) {
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		}
		if (pImpl->m_PixelShaderConstant) {
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		}

		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(pImpl->m_pVertexShader, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(pImpl->m_pPixelShader, nullptr, 0);
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, Mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(Mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(pImpl->m_pInputLayout);
		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		for (auto res : pImpl->m_PipelineResourceVec) {
			pID3D11DeviceContext->PSSetShaderResources(res.m_PipelineNum, 1, &res.m_ShaderResourceView);
			//nullに初期化
			ID3D11SamplerState* pSampler = nullptr;
			switch (res.m_SamplerState) {
			case SamplerState::SamplerNone:
				break;
			case SamplerState::PointWrap:
				pSampler = RenderStatePtr->GetPointWrap();
				break;
			case SamplerState::PointClamp:
				pSampler = RenderStatePtr->GetPointClamp();
				break;
			case SamplerState::LinearWrap:
				pSampler = RenderStatePtr->GetLinearWrap();
				break;
			case SamplerState::LinearClamp:
				pSampler = RenderStatePtr->GetLinearClamp();
				break;
			case SamplerState::AnisotropicWrap:
				pSampler = RenderStatePtr->GetAnisotropicWrap();
				break;
			case SamplerState::AnisotropicClamp:
				pSampler = RenderStatePtr->GetAnisotropicClamp();
				break;
			case SamplerState::ComparisonLinear:
				pSampler = RenderStatePtr->GetComparisonLinear();
				break;
			}
			//サンプラーを設定
			pID3D11DeviceContext->PSSetSamplers(res.m_PipelineNum, 1, &pSampler);
		}
		//デプスステンシル
		switch (GetPipeLineDesc().m_DepthStencilState) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		if (GetPipeLineDesc().m_RasterizerState == RasterizerState::Wireframe) {
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
			//描画
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
		}
		else {
			//ブレンドステートとラスタライザを設定して描画
			//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
			if (GameObjectPtr->GetAlphaActive()) {
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//ラスタライザステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				//描画
				pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
				//ラスタライザステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//描画
				pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
			}
			else {
				switch (GetPipeLineDesc().m_BlendState) {
				case BlendState::Opaque:
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
					break;
				case BlendState::AlphaBlend:
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					break;
				case BlendState::Additive:
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
					break;
				case BlendState::NonPremultiplied:
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
					break;
				}
				switch (GetPipeLineDesc().m_RasterizerState) {
				case RasterizerState::CullBack:
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					break;
				case RasterizerState::CullFront:
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					break;
				case RasterizerState::CullNone:
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
					break;
				}
				//描画
				pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
			}
		}
		//後始末
		pImpl->m_PipelineResourceVec.clear();
		Dev->InitializeStates();
	}


	void DrawContext::SetVertexShader(ID3D11VertexShader* pVShader, bool SetConstantBuffer) {
		pImpl->m_pVertexShader = pVShader;
		pImpl->m_VertexShaderConstant = SetConstantBuffer;
	}

	bool DrawContext::IsVertexShaderSetConstantBuffer() const {
		return pImpl->m_VertexShaderConstant;
	}


	void DrawContext::SetPixelShader(ID3D11PixelShader* pPShader, bool SetConstantBuffer) {
		pImpl->m_pPixelShader = pPShader;
		pImpl->m_PixelShaderConstant = SetConstantBuffer;
	}

	bool DrawContext::IsPixelShaderSetConstantBuffer() const {
		return pImpl->m_PixelShaderConstant;
	}


	void DrawContext::SetInputLayout(ID3D11InputLayout* pInputLayout) {
		pImpl->m_pInputLayout = pInputLayout;
	}




	//--------------------------------------------------------------------------------------
	//	struct DrawComponent::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct DrawComponent::Impl {
		BlendState m_BlendState;
		DepthStencilState m_DepthStencilState;
		RasterizerState m_RasterizerState;
		SamplerState m_SamplerState;
		Matrix4X4 m_MeshToTransformMatrix;
		Impl() :
			m_MeshToTransformMatrix() {}
	};

	//--------------------------------------------------------------------------------------
	///	描画コンポーネントの親クラス実体
	//--------------------------------------------------------------------------------------
	DrawComponent::DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	DrawComponent::~DrawComponent() {}


	BlendState DrawComponent::GetBlendState() const {
		return pImpl->m_BlendState;
	}
	DepthStencilState DrawComponent::GetDepthStencilState() const {
		return pImpl->m_DepthStencilState;
	}
	RasterizerState DrawComponent::GetRasterizerState() const {
		return pImpl->m_RasterizerState;
	}
	SamplerState DrawComponent::GetSamplerState() const {
		return pImpl->m_SamplerState;
	}


	void DrawComponent::SetBlendState(const BlendState state) {
		pImpl->m_BlendState = state;
	}
	void DrawComponent::SetDepthStencilState(const DepthStencilState state) {
		pImpl->m_DepthStencilState = state;

	}
	void DrawComponent::SetRasterizerState(const RasterizerState state) {
		pImpl->m_RasterizerState = state;
	}
	void DrawComponent::SetSamplerState(const SamplerState state) {
		pImpl->m_SamplerState = state;
	}

	const Matrix4X4& DrawComponent::GetMeshToTransformMatrix() const {
		return pImpl->m_MeshToTransformMatrix;
	}
	void DrawComponent::SetMeshToTransformMatrix(const Matrix4X4& Mat) {
		pImpl->m_MeshToTransformMatrix = Mat;
	}



	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl {
		static float m_LightHeight;	//ライトの高さ（向きをこの値で掛ける）
		static float m_LightNear;	//ライトのNear
		static float m_LightFar;		//ライトのFar
		static float m_ViewWidth;
		static float m_ViewHeight;

		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース


		Impl()
		{}
		~Impl() {}
	};

	float Shadowmap::Impl::m_LightHeight(20.0f);
	float Shadowmap::Impl::m_LightNear(1.0f);
	float Shadowmap::Impl::m_LightFar(200.0f);
	float Shadowmap::Impl::m_ViewWidth(32.0f);
	float Shadowmap::Impl::m_ViewHeight(32.0f);


	//--------------------------------------------------------------------------------------
	//	class Shadowmap : public Component;
	//	用途: シャドウマップコンポーネント（前処理用）
	//--------------------------------------------------------------------------------------
	Shadowmap::Shadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	Shadowmap::~Shadowmap() {}

	float Shadowmap::GetLightHeight() { return Impl::m_LightHeight; }
	float Shadowmap::GetLightNear() { return  Impl::m_LightNear; }
	float Shadowmap::GetLightFar() { return  Impl::m_LightFar; }
	float Shadowmap::GetViewWidth() { return  Impl::m_ViewWidth; }
	float Shadowmap::GetViewHeight() { return  Impl::m_ViewHeight; }

	void Shadowmap::SetLightHeight(float f) { Impl::m_LightHeight = f; }
	void Shadowmap::SetLightNear(float f) { Impl::m_LightNear = f; }
	void Shadowmap::SetLightFar(float f) { Impl::m_LightFar = f; }
	void Shadowmap::SetViewWidth(float f) { Impl::m_ViewWidth = f; }
	void Shadowmap::SetViewHeight(float f) { Impl::m_ViewHeight = f; }
	void Shadowmap::SetViewSize(float f) { Impl::m_ViewWidth = Impl::m_ViewHeight = f; }



	shared_ptr<MeshResource> Shadowmap::GetMeshResource(bool ExceptionActive) const {
		if (!pImpl->m_MeshResource.expired()) {
			return pImpl->m_MeshResource.lock();
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"メッシュリソースが見つかりません",
					L"if (pImpl->m_MeshResource.expired())",
					L"ShadowmapComp::GetMeshResource()"
				);
			}
		}
		return nullptr;
	}


	void Shadowmap::SetMeshResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"メッシュキーが空白です",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMeshResource()"
				);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Shadowmap::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr) {
		pImpl->m_MeshResource = MeshResourcePtr;
	}


	void Shadowmap::OnDraw() {
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();

		if (PtrMeshResource) {


			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//ステータスのポインタ
			auto RenderStatePtr = Dev->GetRenderState();

			//行列の定義
			Matrix4X4 World, LightView, LightProj;
			//行列の定義
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			//ワールド行列の決定
			World.AffineTransformation(
				PtrTrans->GetScale(),			//スケーリング
				PtrTrans->GetPivot(),		//回転の中心（重心）
				PtrTrans->GetQuaternion(),				//回転角度
				PtrTrans->GetPosition()				//位置
			);
			Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * World;
			//ビュー行列の決定
			auto StageView = PtrStage->GetView();
			//ライトの取得
			auto StageLight = PtrStage->GetLight();
			Vector3 LightDir = -1.0 * StageLight->GetTargetLight().m_Directional;
			Vector3 LightAt = StageView->GetTargetCamera()->GetAt();
			Vector3 LightEye = LightAt + (LightDir * GetLightHeight());
			//ライトのビューと射影を計算
			LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
			LightProj.OrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

			ShadowConstantBuffer Cb;
			Cb.mWorld = Matrix4X4EX::Transpose(RealWorldMatrix);
			Cb.mView = Matrix4X4EX::Transpose(LightView);
			Cb.mProj = Matrix4X4EX::Transpose(LightProj);

			bool IsSkin = false;
			bool IsSkinStride = false;
			if (PtrMeshResource->IsSkining()) {
				auto DrawCompPtr = GetGameObject()->GetDynamicComponent<DrawComponent>(false);
				if (auto* pLocalBoneVec = DrawCompPtr->GetVecLocalBonesPtr()) {
					if (pLocalBoneVec) {
						//ボーンの設定
						size_t BoneSz = pLocalBoneVec->size();
						UINT cb_count = 0;
						for (size_t b = 0; b < BoneSz; b++) {
							Matrix4X4 mat = pLocalBoneVec->at(b);
							mat.Transpose();
							Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
							Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
							Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
							cb_count += 3;
						}
						IsSkin = true;
					}
				}
				IsSkinStride = true;
			}
			//これより描画処理
			//コンスタントバッファの更新
			pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);

			if (IsSkin) {
				//インプットレイアウトのセット
				pID3D11DeviceContext->IASetInputLayout(VSShadowmapBone::GetPtr()->GetInputLayout());
				//ストライドとオフセット
				UINT stride = sizeof(VertexPositionNormalTextureSkinning);
				UINT offset = 0;
				//頂点バッファをセット
				pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//頂点シェーダーのセット
				pID3D11DeviceContext->VSSetShader(VSShadowmapBone::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//インプットレイアウトのセット
				pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
				//ストライドとオフセット
				UINT stride = sizeof(VertexPositionNormalTexture);
				if (IsSkinStride) {
					stride = sizeof(VertexPositionNormalTextureSkinning);
				}
				UINT offset = 0;
				//頂点バッファをセット
				pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//頂点シェーダーのセット
				pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
			}



			//インデックスバッファのセット
			pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//描画方法（3角形）
			pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//ピクセルシェーダはセットしない！
			pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
			//ジオメトリシェーダの設定（使用しない）
			pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//コンスタントバッファをピクセルシェーダにセット
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
			//描画
			pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
			//後始末
			Dev->InitializeStates();

		}

	}



	//--------------------------------------------------------------------------------------
	//	struct PNTStaticDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTStaticDraw::Impl {
		//描画コンテキスト
		shared_ptr<DrawContext> m_DrawContext;
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		weak_ptr<TextureResource> m_TextureResource; //テクスチャ
		Color4 m_Emissive;	//色
		bool m_OwnShadowActive;
		Impl() :
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f),
			m_OwnShadowActive(false)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class PNTStaticDraw : public DrawComponent;
	//	用途: PNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::OnCreate() {
		pImpl->m_DrawContext = ObjectFactory::Create<DrawContext>();
	}

	shared_ptr<MeshResource> PNTStaticDraw::GetMeshResource() const {
		//メッシュがなければリターン
		auto shptr = pImpl->m_MeshResource.lock();
		if (!shptr) {
			throw BaseException(
				L"メッシュが設定されてません",
				L"if (!shptr)",
				L"PNTStaticDraw::GetMeshResource()"
			);
		}
		return shptr;
	}

	void PNTStaticDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void PNTStaticDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}
	void PNTStaticDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
	}
	void PNTStaticDraw::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
	}
	shared_ptr<TextureResource> PNTStaticDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = pImpl->m_TextureResource.lock();
		if (!shptr) {
			return nullptr;
		}
		return shptr;
	}

	Color4 PNTStaticDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void PNTStaticDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}

	bool PNTStaticDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTStaticDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTStaticDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}



	void PNTStaticDraw::DrawNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		auto PtrTextureResource = GetTextureResource();

		PipeLineDesc Desc;
		Desc.m_BlendState = GetBlendState();
		Desc.m_DepthStencilState = GetDepthStencilState();
		Desc.m_RasterizerState = GetRasterizerState();
		pImpl->m_DrawContext->SetPipeLineDesc(Desc);

		pImpl->m_DrawContext->SetVertexShader<VSPNTStatic>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPNTStatic>(true);

		//行列の定義
		Matrix4X4 World, View, Proj;
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//ワールド行列の決定
		World.AffineTransformation(
			PtrTrans->GetScale(),			//スケーリング
			PtrTrans->GetPivot(),		//回転の中心（重心）
			PtrTrans->GetQuaternion(),				//回転角度
			PtrTrans->GetPosition()				//位置
		);
		//転置する
		World.Transpose();
		//ビュー行列の決定
		auto StageView = PtrStage->GetView();
		View = StageView->GetTargetCamera()->GetViewMatrix();
		//転置する
		View.Transpose();
		//射影行列の決定
		Proj = StageView->GetTargetCamera()->GetProjMatrix();
		//転置する
		Proj.Transpose();
		//コンスタントバッファの設定
		PNTStaticConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = World;
		cb1.View = View;
		cb1.Projection = Proj;
		//ライティング
		auto StageLight = PtrStage->GetLight();
		cb1.LightDir = StageLight->GetTargetLight().m_Directional;
		cb1.LightDir.w = 1.0f;
		cb1.Emissive = pImpl->m_Emissive;
		cb1.Diffuse = StageLight->GetTargetLight().m_DiffuseColor;

		pImpl->m_DrawContext->AddSamplerAndSrv(0, GetSamplerState(), PtrTextureResource->GetShaderResourceView().Get());
		pImpl->m_DrawContext->DrawIndexed<CBPNTStatic>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionNormalTexture), 0);
	}

	void PNTStaticDraw::DrawWithShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		auto PtrTextureResource = GetTextureResource();
		PipeLineDesc Desc;
		Desc.m_BlendState = GetBlendState();
		Desc.m_DepthStencilState = GetDepthStencilState();
		Desc.m_RasterizerState = GetRasterizerState();
		pImpl->m_DrawContext->SetPipeLineDesc(Desc);

		bool IsShadowmap = false;
		if (GetGameObject()->GetComponent<Shadowmap>(false)) {
			//シャドウマップがあれば自己影防止用のピクセルシェーダ
			pImpl->m_DrawContext->SetVertexShader<VSPNTStaticShadow>(true);
			pImpl->m_DrawContext->SetPixelShader<PSPNTStaticShadow2>(true);
			IsShadowmap = true;
		}
		else {
			pImpl->m_DrawContext->SetVertexShader<VSPNTStaticShadow>(true);
			pImpl->m_DrawContext->SetPixelShader<PSPNTStaticShadow>(true);
		}

		//ビュー行列の決定
		auto StageView = PtrStage->GetView();
		//ステージからライトを取り出す
		auto StageLight = PtrStage->GetLight();


		//行列の取得
		Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();

		//コンスタントバッファの設定
		PNTStaticShadowConstantBuffer cb1;

		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(RealWorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(StageView->GetTargetCamera()->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(StageView->GetTargetCamera()->GetProjMatrix());
		cb1.LightDir = StageLight->GetTargetLight().m_Directional;
		cb1.LightDir.w = 1.0f;
		cb1.Emissive = pImpl->m_Emissive;
		cb1.Diffuse = StageLight->GetTargetLight().m_DiffuseColor;

		Vector3 CalcLightDir = -1.0 * StageLight->GetTargetLight().m_Directional;
		Vector3 LightAt = StageView->GetTargetCamera()->GetAt();
		Vector3 LightEye = CalcLightDir;
		LightEye *= Shadowmap::GetLightHeight();
		LightEye = LightAt + LightEye;

		cb1.LightPos = LightEye;
		cb1.LightPos.w = 1.0f;

		cb1.EyePos = StageView->GetTargetCamera()->GetEye();
		cb1.EyePos.w = 1.0f;
		if (PtrTextureResource) {
			cb1.ActiveFlg.x = 1;
		}
		else {
			cb1.ActiveFlg.x = 0;
		}


		Matrix4X4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		cb1.LightView = Matrix4X4EX::Transpose(LightView);
		cb1.LightProjection = Matrix4X4EX::Transpose(LightProj);

		//シャドウマップのリソースビューを取得
		//シャドウマップのレンダラーターゲット
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		//シャドウマップのレンダラーターゲット
		auto ShadoumapPtr = Dev->GetShadowMapRenderTarget();
		ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();

		if (PtrTextureResource) {

			pImpl->m_DrawContext->AddSamplerAndSrv(0, GetSamplerState(), PtrTextureResource->GetShaderResourceView().Get());
		}
		else {
			pImpl->m_DrawContext->AddSamplerAndSrv(0, SamplerState::SamplerNone, nullptr);
		}
		//自己影
		pImpl->m_DrawContext->AddSamplerAndSrv(1, SamplerState::ComparisonLinear, ShadoumapPtr->GetShaderResourceView());
		pImpl->m_DrawContext->DrawIndexed<CBPNTStaticShadow>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionNormalTexture), 0);

	}




	void PNTStaticDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}


}
//end basecross
