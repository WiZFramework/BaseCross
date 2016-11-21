/*!
@file DrawComponents.cpp
@brief 描画コンポーネント実体
@copyright Copyright (c) 2016 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//Shadowmap
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmap, App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmap.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmapBone, App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmapBone.cso")

	//スプライト用
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSprite)

	//PCSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCSprite.cso")

	//PTSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTSprite.cso")

	//PCTSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCTSprite.cso")


	//PCStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCStatic.cso")
	//PTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTStatic.cso")

	//PCTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTInstance, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCTInstance.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCTStatic.cso")



	//PNStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBStaticLighting)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNStatic.cso")
	//PNTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticNoTex, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticNoTex.cso")

	//PNTStaticMidium
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticMidium, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStaticMidium.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticMidium, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticMidium.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticMidiumNoTex, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticMidiumNoTex.cso")


	//PNTStaticReal
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBStaticRealLighting)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticReal, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStaticReal.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticReal, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticReal.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticRealNoTex, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticRealNoTex.cso")



	//PNTStaticShadow
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTStaticShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticShadow, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow2, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticShadow2.cso")

	//PNTBone
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTBone)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBone, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTBone.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBoneMidium, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTBoneMidium.cso")

	//PNTBoneReal
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTBoneRealLighting)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBoneReal, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTBoneReal.cso")


	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTBoneShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBoneShadow, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTBoneShadow.cso")


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
	void DrawComponent::SetDeviceBlendState() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		switch (GetBlendState()) {
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
	}


	void DrawComponent::SetDepthStencilState(const DepthStencilState state) {
		pImpl->m_DepthStencilState = state;

	}

	void DrawComponent::SetDeviceDepthStencilState() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		switch (GetDepthStencilState()) {
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

	}

	void DrawComponent::SetRasterizerState(const RasterizerState state) {
		pImpl->m_RasterizerState = state;
	}
	void DrawComponent::SetDeviceRasterizerState() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		switch (GetRasterizerState()) {
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
	}


	void DrawComponent::SetSamplerState(const SamplerState state) {
		pImpl->m_SamplerState = state;
	}
	void DrawComponent::SetDeviceSamplerState() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
		case SamplerState::SamplerNone:
			//サンプラーもクリア
			pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
			return;
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
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
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
			
			auto StageLight = PtrGameObject->OnGetDrawLight();
			Vector3 LightDir = -1.0 * StageLight.m_Directional;
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
				UINT stride = PtrMeshResource->GetNumStride();
//				UINT stride = sizeof(VertexPositionNormalTextureSkinning);
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
				UINT stride = PtrMeshResource->GetNumStride();
				if (IsSkinStride) {
					//ストライドがスキンだった場合の特殊処理
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
	//ParticleDraw準備のための構造体
	//--------------------------------------------------------------------------------------
	struct DrawParticleSprite {
		//パーティクルのカメラまでの距離
		float m_ToCaneraLength;
		//ワールド行列
		Matrix4X4 m_WorldMatrix;
		//テクスチャ
		shared_ptr<TextureResource> m_TextureRes;
		DrawParticleSprite() :
			m_ToCaneraLength(0)
		{}
	};



	//--------------------------------------------------------------------------------------
	//	struct PCTParticleDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PCTParticleDraw::Impl {
		//描画コンテキスト
		shared_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		vector<DrawParticleSprite> m_DrawParticleSpriteVec;
		const size_t m_MaxInstance;				///<インスタンス最大値
		ComPtr<ID3D11Buffer> m_MatrixBuffer;	///<行列用の頂点バッファ

		Impl(size_t MaxInstance) :
			m_MaxInstance(MaxInstance)
		{}
		~Impl() {}
		//頂点バッファの作成
		void CreateParticleBuffers();
	};


	void PCTParticleDraw::Impl::CreateParticleBuffers() {
		try {
			float HelfSize = 0.5f;
			Vector4 col(1.0f, 1.0f, 1.0f, 1.0f);
			//頂点配列
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(Vector3(-HelfSize, HelfSize, 0),  col,Vector2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vector3(HelfSize, HelfSize, 0), col, Vector2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vector3(-HelfSize, -HelfSize, 0),  col,Vector2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(Vector3(HelfSize, -HelfSize, 0),  col, Vector2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成
			m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);

			//インスタンス行列バッファの作成
			//Max値で作成する
			vector<Matrix4X4> matrices(m_MaxInstance, Matrix4X4());
			MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);

		}
		catch (...) {
			throw;
		}
	}




	//--------------------------------------------------------------------------------------
	///	PCTParticle描画コンポーネント(パーティクル描画)
	//--------------------------------------------------------------------------------------
	PCTParticleDraw::PCTParticleDraw(const shared_ptr<GameObject>& GameObjectPtr, size_t MaxInstance) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl(MaxInstance))
	{}

	PCTParticleDraw::~PCTParticleDraw() {}

	void PCTParticleDraw::AddParticle(float ToCaneraLength, const Matrix4X4& WorldMatrix, const shared_ptr<TextureResource>& TextureRes) {
		DrawParticleSprite Item;
		Item.m_ToCaneraLength = ToCaneraLength;
		Item.m_WorldMatrix = WorldMatrix;
		Item.m_TextureRes = TextureRes;
		pImpl->m_DrawParticleSpriteVec.push_back(Item);
	}

	void PCTParticleDraw::OnCreate() {
		pImpl->CreateParticleBuffers();
	}

	struct InstanceDrawStr {
		size_t Start;
		size_t Count;
		shared_ptr<TextureResource> Tex;
		InstanceDrawStr(size_t s, size_t c, shared_ptr<TextureResource> t) :
			Start(s), Count(c), Tex(t) {}
	};

	void PCTParticleDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		if (pImpl->m_DrawParticleSpriteVec.size() <= 0) {
			return;
		}

		//カメラ位置でソート
		auto func = [](DrawParticleSprite& Left, DrawParticleSprite& Right)->bool {
			return (Left.m_ToCaneraLength > Right.m_ToCaneraLength);
		};
		std::sort(pImpl->m_DrawParticleSpriteVec.begin(), pImpl->m_DrawParticleSpriteVec.end(), func);
		//デバイスとコンテキストインターフェイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//先頭のテクスチャを得る
		auto  NowTexPtr = pImpl->m_DrawParticleSpriteVec[0].m_TextureRes;

		vector<InstanceDrawStr> m_InstancVec;
		size_t NowStartIndex = 0;
		size_t NowDrawCount = 0;

		shared_ptr<TextureResource> NowTexRes = pImpl->m_DrawParticleSpriteVec[0].m_TextureRes;
		for (size_t i = 0; i < pImpl->m_DrawParticleSpriteVec.size(); i++) {
			if (pImpl->m_DrawParticleSpriteVec[i].m_TextureRes != NowTexRes) {
				m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes));
				NowStartIndex = i;
				NowDrawCount = 0;
				NowTexRes = pImpl->m_DrawParticleSpriteVec[i].m_TextureRes;
			}
			else {
				NowDrawCount++;
			}
		}
		m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes));


		//インスタンスバッファにマップ
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//行列のマップ
		if (FAILED(pD3D11DeviceContext->Map(pImpl->m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"行列のMapに失敗しました。",
				L"if(FAILED(pD3D11DeviceContext->Map()))",
				L"PCTParticleDraw::OnDraw()"
			);
		}
		//行列の変更
		auto* matrices = (Matrix4X4*)mappedBuffer.pData;
		Matrix4X4 World;
		for (size_t i = 0; i < pImpl->m_DrawParticleSpriteVec.size(); i++) {
			World = pImpl->m_DrawParticleSpriteVec[i].m_WorldMatrix;
			//転置する
			World.Transpose();
			matrices[i] = World;
		}
		//アンマップ
		pD3D11DeviceContext->Unmap(pImpl->m_MatrixBuffer.Get(), 0);
		//ストライドとオフセット
		//形状の頂点バッファと行列バッファを設定
		UINT stride[2] = { sizeof(VertexPositionColorTexture), sizeof(Matrix4X4) };
		UINT offset[2] = { 0, 0 };

		ID3D11Buffer* pBuf[2] = { pImpl->m_MeshResource->GetVertexBuffer().Get(), pImpl->m_MatrixBuffer.Get() };
		pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(pImpl->m_MeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCTInstance::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTInstance::GetPtr()->GetInputLayout());

		//ブレンドステート
		//透明処理
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//ラスタライザステート（表面描画）
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//ビュー行列の決定
		Matrix4X4 View, Proj;
		//ビュー行列の決定
		auto StageView = PtrStage->GetView();
		View = StageView->GetTargetCamera()->GetViewMatrix();
		//転置する
		View.Transpose();
		//射影行列の決定
		Proj = StageView->GetTargetCamera()->GetProjMatrix();
		//転置する
		Proj.Transpose();


		//コンスタントバッファの準備
		StaticConstantBuffer sb;
		sb.World = Matrix4X4();	//ワールド行列はダミー
		sb.View = View;
		sb.Projection = Proj;
		//エミッシブ加算は行わない。
		sb.Emissive = Color4(0, 0, 0, 0);
		sb.Diffuse = Color4(1, 1, 1, 1);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBStatic::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		for (auto& v : m_InstancVec) {
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.Tex->GetShaderResourceView().GetAddressOf());
			//描画
			pD3D11DeviceContext->DrawIndexedInstanced(pImpl->m_MeshResource->GetNumIndicis(), v.Count, 0, 0, v.Start);
		}
		//後始末
		Dev->InitializeStates();
		pImpl->m_DrawParticleSpriteVec.clear();
	}

	//--------------------------------------------------------------------------------------
	//	struct TextureDrawInterface::Impl;
	//--------------------------------------------------------------------------------------
	struct TextureDrawInterface::Impl {
		//テクスチャ
		weak_ptr<TextureResource> m_TextureResource;
		//ラップサンプラーかどうか
		bool m_WrapSampler;
		Impl() :
			m_WrapSampler(false)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	テクスチャ描画インターフェイス
	//--------------------------------------------------------------------------------------
	TextureDrawInterface::TextureDrawInterface():
		pImpl(new Impl())
	{}

	TextureDrawInterface::~TextureDrawInterface() {}

	void TextureDrawInterface::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
	}

	void TextureDrawInterface::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
	}

	shared_ptr<TextureResource> TextureDrawInterface::GetTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = pImpl->m_TextureResource.lock();
		if (!shptr) {
			return nullptr;
		}
		return shptr;
	}

	bool TextureDrawInterface::GetWrapSampler() const {
		return pImpl->m_WrapSampler;

	}
	void TextureDrawInterface::SetWrapSampler(bool b) {
		pImpl->m_WrapSampler = b;
	}



	//--------------------------------------------------------------------------------------
	//	struct SpriteBaseDraw::Impl;
	//	用途: SpriteBaseDrawイディオム
	//--------------------------------------------------------------------------------------
	struct SpriteBaseDraw::Impl {
		//メッシュ
		shared_ptr<MeshResource> m_SpriteMesh;
		//エミッシブ色
		Color4 m_Emissive;
		//デフューズ色
		Color4 m_Diffuse;
		Impl():
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	class SpriteBaseDraw : public DrawComponent;
	//--------------------------------------------------------------------------------------
	SpriteBaseDraw::SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	SpriteBaseDraw::~SpriteBaseDraw() {}


	shared_ptr<MeshResource> SpriteBaseDraw::GetMeshResource() const {
		return pImpl->m_SpriteMesh;
	}
	void SpriteBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SpriteMesh = MeshRes;
	}


	Color4 SpriteBaseDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void SpriteBaseDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}

	Color4 SpriteBaseDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void SpriteBaseDraw::SetDiffuse(const Color4& col) {
		pImpl->m_Diffuse = col;
	}


	//--------------------------------------------------------------------------------------
	//	struct PCSpriteDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCSpriteDraw::Impl {
		Impl()
		{}
	};



	//--------------------------------------------------------------------------------------
	///	PCSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr):
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices):
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		try {
			//パイプラインステートをデフォルトの2D
			SetBlendState(BlendState::Opaque);
			SetDepthStencilState(DepthStencilState::None);
			SetRasterizerState(RasterizerState::CullBack);
			SetSamplerState(SamplerState::LinearClamp);
			//メッシュの作成（変更できる）
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}

	PCSpriteDraw::~PCSpriteDraw() {}

	void PCSpriteDraw::CreateMesh(vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices) {
		try {
			//メッシュの作成（変更できる）
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}

	void PCSpriteDraw::UpdateVertices(const vector<VertexPositionColor>& Vertices) {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!GetMeshResource())",
				L"PCSpriteDraw::UpdateVertices()"
			);

		}
		if (Vertices.size() > SpriteMesh->GetNumVertices()) {
			throw BaseException(
				L"更新する頂点が大きすぎます",
				L"if (Vertices.size() > SpriteMesh->GetNumVertices())",
				L"PCSpriteDraw::UpdateVertices()"
			);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//頂点の変更
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pD3D11DeviceContext->Map(SpriteMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"PCSpriteDraw::UpdateVertices()"
			);
		}
		//頂点の変更
		VertexPositionColor* vertices
			= (VertexPositionColor*)mappedBuffer.pData;
		for (size_t i = 0; i < SpriteMesh->GetNumVertices(); i++) {
			vertices[i] = Vertices[i];
		}
		//アンマップ
		pD3D11DeviceContext->Unmap(SpriteMesh->GetVertexBuffer().Get(), 0);
	}
	void PCSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionColor> Vertices = {
				{ VertexPositionColor(Vector3(-HelfSize, HelfSize, 0), Color4(1.0f,1.0f,1.0f,1.0f)) },
				{ VertexPositionColor(Vector3(HelfSize, HelfSize, 0), Color4(1.0f,1.0f,1.0f,1.0f)) },
				{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, 0), Color4(1.0f,1.0f,1.0f,1.0f)) },
				{ VertexPositionColor(Vector3(HelfSize, -HelfSize, 0), Color4(1.0f,1.0f,1.0f,1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成（変更できる）
			SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}

	}
	void PCSpriteDraw::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//メッシュがなければ描画しない
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!SpriteMesh)",
				L"PCSpriteDraw::OnDraw()"
			);
		}

		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		Vector2 Scale, Pos, Pivot;
		Scale.x = PtrTrans->GetScale().x;
		Scale.y = PtrTrans->GetScale().y;
		Pos.x = PtrTrans->GetPosition().x;
		Pos.y = PtrTrans->GetPosition().y;
		Pivot.x = PtrTrans->GetPivot().x;
		Pivot.y = PtrTrans->GetPivot().y;
		Vector3 Rot = PtrTrans->GetRotation();

		//行列の定義
		Matrix4X4 World, Proj;
		//ワールド行列の決定
		World.AffineTransformation2D(
			Scale,			//スケーリング
			Pivot,		//回転の中心（重心）
			Rot.z,			//回転角度(Z軸回転)
			Pos				//位置
		);
		//射影行列の決定
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		SpriteConstantBuffer sb;
		//エミッシブ
		sb.Emissive = GetEmissive();
		//デフィーズはすべて通す
		sb.Diffuse = GetDiffuse();
		//行列の設定
		sb.World = World;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, SpriteMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(SpriteMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCSprite::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (GetGameObject()->GetAlphaActive()) {
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//ラスタライザステート
			pD3D11DeviceContext->RSSetState(RenderState->GetCullNone());
		}
		else {
			//透明処理しない
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			//ラスタライザステート
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//描画
		pD3D11DeviceContext->DrawIndexed(SpriteMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}



	//--------------------------------------------------------------------------------------
	//	struct PTSpriteDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PTSpriteDraw::Impl {
		Impl()
		{}
	};

	//--------------------------------------------------------------------------------------
	///	PTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr):
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices):
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		try {
			//パイプラインステートをデフォルトの2D
			SetBlendState(BlendState::Opaque);
			SetDepthStencilState(DepthStencilState::None);
			SetRasterizerState(RasterizerState::CullBack);
			SetSamplerState(SamplerState::LinearClamp);
			//メッシュの作成（変更できる）
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}

	PTSpriteDraw::~PTSpriteDraw() {}

	void PTSpriteDraw::CreateMesh(vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices) {
		try {
			//メッシュの作成（変更できる）
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}

	void PTSpriteDraw::UpdateVertices(const vector<VertexPositionTexture>& Vertices) {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!GetMeshResource())",
				L"PTSpriteDraw::UpdateVertices()"
			);

		}
		if (Vertices.size() > SpriteMesh->GetNumVertices()) {
			throw BaseException(
				L"更新する頂点が大きすぎます",
				L"if (Vertices.size() > SpriteMesh->GetNumVertices())",
				L"PTSpriteDraw::UpdateVertices()"
			);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//頂点の変更
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pD3D11DeviceContext->Map(SpriteMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"PTSpriteDraw::UpdateVertices()"
			);
		}
		//頂点の変更
		VertexPositionTexture* vertices
			= (VertexPositionTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < SpriteMesh->GetNumVertices(); i++) {
			vertices[i] = Vertices[i];
		}
		//アンマップ
		pD3D11DeviceContext->Unmap(SpriteMesh->GetVertexBuffer().Get(), 0);
	}

	void PTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionTexture> Vertices = {
				{ VertexPositionTexture(Vector3(-HelfSize, HelfSize, 0), Vector2(0.0f, 0.0f)) },
				{ VertexPositionTexture(Vector3(HelfSize, HelfSize, 0), Vector2(1.0f, 0.0f)) },
				{ VertexPositionTexture(Vector3(-HelfSize, -HelfSize, 0), Vector2(0.0f, 1.0f)) },
				{ VertexPositionTexture(Vector3(HelfSize, -HelfSize, 0), Vector2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成（変更できる）
			SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
	}

	void PTSpriteDraw::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//メッシュがなければ描画しない
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!SpriteMesh)",
				L"PTSpriteDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}

		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		Vector2 Scale,Pos,Pivot;
		Scale.x = PtrTrans->GetScale().x;
		Scale.y = PtrTrans->GetScale().y;
		Pos.x = PtrTrans->GetPosition().x;
		Pos.y = PtrTrans->GetPosition().y;
		Pivot.x = PtrTrans->GetPivot().x;
		Pivot.y = PtrTrans->GetPivot().y;
		Vector3 Rot = PtrTrans->GetRotation();

		//行列の定義
		Matrix4X4 World, Proj;
		//ワールド行列の決定
		World.AffineTransformation2D(
			Scale,			//スケーリング
			Pivot,		//回転の中心（重心）
			Rot.z,			//回転角度(Z軸回転)
			Pos				//位置
		);

		//射影行列の決定
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		SpriteConstantBuffer sb;
		//エミッシブ
		sb.Emissive = GetEmissive();
		//デフィーズはすべて通す
		sb.Diffuse = GetDiffuse();
		//行列の設定
		sb.World = World;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, SpriteMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(SpriteMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPTSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPTSprite::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (GetGameObject()->GetAlphaActive()) {
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//ラスタライザステート
			pD3D11DeviceContext->RSSetState(RenderState->GetCullNone());
		}
		else {
			//透明処理しない
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			//ラスタライザステート
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
		//ラッピングサンプラー
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		if (GetWrapSampler()) {
			pSampler = RenderState->GetLinearWrap();
		}
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//描画
		pD3D11DeviceContext->DrawIndexed(SpriteMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	//	struct PCTSpriteDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCTSpriteDraw::Impl {
		Impl()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PCTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr):
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices):
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		try {
			//パイプラインステートをデフォルトの2D
			SetBlendState(BlendState::Opaque);
			SetDepthStencilState(DepthStencilState::None);
			SetRasterizerState(RasterizerState::CullBack);
			SetSamplerState(SamplerState::LinearClamp);
			//メッシュの作成（変更できる）
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}
	PCTSpriteDraw::~PCTSpriteDraw() {}


	void PCTSpriteDraw::CreateMesh(vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices) {
		try {
			//メッシュの作成（変更できる）
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}


	void PCTSpriteDraw::UpdateVertices(const vector<VertexPositionColorTexture>& Vertices) {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!SpriteMesh)",
				L"PCTSpriteDraw::UpdateVertices()"
			);

		}
		if (Vertices.size() > SpriteMesh->GetNumVertices()) {
			throw BaseException(
				L"更新する頂点が大きすぎます",
				L"if (Vertices.size() > SpriteMesh->GetNumVertices())",
				L"PCTSpriteDraw::UpdateVertices()"
			);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//頂点の変更
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pD3D11DeviceContext->Map(SpriteMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"PCTSpriteDraw::UpdateVertices()"
			);
		}
		//頂点の変更
		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < SpriteMesh->GetNumVertices(); i++) {
			vertices[i] = Vertices[i];
		}
		//アンマップ
		pD3D11DeviceContext->Unmap(SpriteMesh->GetVertexBuffer().Get(), 0);


	}
	void PCTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(Vector3(-HelfSize, HelfSize, 0),Color4(1.0f,1.0f,1.0f,1.0f), Vector2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vector3(HelfSize, HelfSize, 0), Color4(1.0f,1.0f,1.0f,1.0f), Vector2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vector3(-HelfSize, -HelfSize, 0), Color4(1.0f,1.0f,1.0f,1.0f), Vector2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(Vector3(HelfSize, -HelfSize, 0), Color4(1.0f,1.0f,1.0f,1.0f), Vector2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成（変更できる）
			SpriteMesh = MeshResource::CreateMeshResource(vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
	}

	void PCTSpriteDraw::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//メッシュがなければ描画しない
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!SpriteMesh)",
				L"PCTSpriteDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}

		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		Vector2 Scale, Pos, Pivot;
		Scale.x = PtrTrans->GetScale().x;
		Scale.y = PtrTrans->GetScale().y;
		Pos.x = PtrTrans->GetPosition().x;
		Pos.y = PtrTrans->GetPosition().y;
		Pivot.x = PtrTrans->GetPivot().x;
		Pivot.y = PtrTrans->GetPivot().y;
		Vector3 Rot = PtrTrans->GetRotation();

		//行列の定義
		Matrix4X4 World, Proj;
		//ワールド行列の決定
		World.AffineTransformation2D(
			Scale,			//スケーリング
			Pivot,		//回転の中心（重心）
			Rot.z,			//回転角度(Z軸回転)
			Pos				//位置
		);

		//射影行列の決定
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		SpriteConstantBuffer sb;
		//エミッシブ
		sb.Emissive = GetEmissive();
		//デフィーズはすべて通す
		sb.Diffuse = GetDiffuse();
		//行列の設定
		sb.World = World;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, SpriteMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(SpriteMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (GetGameObject()->GetAlphaActive()) {
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//ラスタライザステート
			pD3D11DeviceContext->RSSetState(RenderState->GetCullNone());
		}
		else {
			//透明処理しない
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			//ラスタライザステート
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
		//ラッピングサンプラー
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		if (GetWrapSampler()) {
			pSampler = RenderState->GetLinearWrap();
		}
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//描画
		pD3D11DeviceContext->DrawIndexed(SpriteMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}


	//--------------------------------------------------------------------------------------
	//	3D用シェーダごと描画構造体
	//--------------------------------------------------------------------------------------
	struct Draw3DPrim {
		static void PCDraw(const shared_ptr<GameObject>& GameObjectPtr, const shared_ptr<MeshResource>& MeshRes,
			const Color4& Emissive, const Color4& Diffuse,const Matrix4X4& MeshToTransformMatrix) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//行列の定義
			auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
			//行列の定義
			Matrix4X4 World, ViewMat, ProjMat;
			//ワールド行列の決定
			World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
			//転置する
			World.Transpose();
			//カメラを得る
			auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
			//ビューと射影行列を得る
			ViewMat = CameraPtr->GetViewMatrix();
			//転置する
			ViewMat.Transpose();
			//転置する
			ProjMat = CameraPtr->GetProjMatrix();
			ProjMat.Transpose();
			//コンスタントバッファの準備
			StaticConstantBuffer sb;
			sb.World = World;
			sb.View = ViewMat;
			sb.Projection = ProjMat;
			//エミッシブ
			sb.Emissive = Emissive;
			//デフィーズはすべて通す
			sb.Diffuse = Diffuse;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionColor);
			UINT offset = 0;
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

			//描画方法（3角形）
			pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBStatic::GetPtr()->GetBuffer();
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
			if (GameObjectPtr->GetAlphaActive()) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			}
			//デプスステンシルステート
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);

			//透明処理なら
			if (GameObjectPtr->GetAlphaActive()) {
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//ラスタライザステート
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			//後始末
			Dev->InitializeStates();
		}


		static void PCWireFrameDraw(const shared_ptr<GameObject>& GameObjectPtr, const shared_ptr<MeshResource>& MeshRes,
			const Color4& Emissive, const Color4& Diffuse, const Matrix4X4& MeshToTransformMatrix) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//行列の定義
			auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
			//行列の定義
			Matrix4X4 World, ViewMat, ProjMat;
			//ワールド行列の決定
			World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
			//転置する
			World.Transpose();
			//カメラを得る
			auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
			//ビューと射影行列を得る
			ViewMat = CameraPtr->GetViewMatrix();
			//転置する
			ViewMat.Transpose();
			//転置する
			ProjMat = CameraPtr->GetProjMatrix();
			ProjMat.Transpose();
			//コンスタントバッファの準備
			StaticConstantBuffer sb;
			sb.World = World;
			sb.View = ViewMat;
			sb.Projection = ProjMat;
			//エミッシブ
			sb.Emissive = Emissive;
			//デフィーズはすべて通す
			sb.Diffuse = Diffuse;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionColor);
			UINT offset = 0;
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

			//描画方法（3角形）
			pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBStatic::GetPtr()->GetBuffer();
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
			//透明処理しない
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			//デプスステンシルステート
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
			//ラスタライザステート(ワイアフレーム)
			pD3D11DeviceContext->RSSetState(RenderState->GetWireframe());
			pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			//後始末
			Dev->InitializeStates();
		}


		static void PTDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes, bool WrapSampler,
			const Color4& Emissive, const Color4& Diffuse, const Matrix4X4& MeshToTransformMatrix) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//行列の定義
			auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
			//行列の定義
			Matrix4X4 World, ViewMat, ProjMat;
			//ワールド行列の決定
			World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
			//転置する
			World.Transpose();
			//カメラを得る
			auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
			//ビューと射影行列を得る
			ViewMat = CameraPtr->GetViewMatrix();
			//転置する
			ViewMat.Transpose();
			//転置する
			ProjMat = CameraPtr->GetProjMatrix();
			ProjMat.Transpose();
			//コンスタントバッファの準備
			StaticConstantBuffer sb;
			sb.World = World;
			sb.View = ViewMat;
			sb.Projection = ProjMat;
			//エミッシブ
			sb.Emissive = Emissive;
			//デフィーズはすべて通す
			sb.Diffuse = Diffuse;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionTexture);
			UINT offset = 0;
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

			//描画方法（3角形）
			pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//シェーダの設定
			pD3D11DeviceContext->VSSetShader(VSPTStatic::GetPtr()->GetShader(), nullptr, 0);
			pD3D11DeviceContext->PSSetShader(PSPTStatic::GetPtr()->GetShader(), nullptr, 0);
			//インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(VSPTStatic::GetPtr()->GetInputLayout());
			//ブレンドステート
			if (GameObjectPtr->GetAlphaActive()) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			}
			//デプスステンシルステート
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);

			//テクスチャとサンプラーの設定
			ID3D11ShaderResourceView* pNull[1] = { 0 };
			pD3D11DeviceContext->PSSetShaderResources(0, 1, TextureRes->GetShaderResourceView().GetAddressOf());
			//ラッピングサンプラー
			ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
			if (WrapSampler) {
				pSampler = RenderState->GetLinearWrap();
			}
			pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

			//透明処理なら
			if (GameObjectPtr->GetAlphaActive()) {
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//ラスタライザステート
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			//後始末
			Dev->InitializeStates();
		}



		static void PCTDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes, bool WrapSampler,
			const Color4& Emissive, const Color4& Diffuse, const Matrix4X4& MeshToTransformMatrix) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//行列の定義
			auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
			//行列の定義
			Matrix4X4 World, ViewMat, ProjMat;
			//ワールド行列の決定
			World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
			//転置する
			World.Transpose();
			//カメラを得る
			auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
			//ビューと射影行列を得る
			ViewMat = CameraPtr->GetViewMatrix();
			//転置する
			ViewMat.Transpose();
			//転置する
			ProjMat = CameraPtr->GetProjMatrix();
			ProjMat.Transpose();
			//コンスタントバッファの準備
			StaticConstantBuffer sb;
			sb.World = World;
			sb.View = ViewMat;
			sb.Projection = ProjMat;
			//エミッシブ
			sb.Emissive = Emissive;
			//デフィーズはすべて通す
			sb.Diffuse = Diffuse;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionColorTexture);
			UINT offset = 0;
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

			//描画方法（3角形）
			pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//シェーダの設定
			pD3D11DeviceContext->VSSetShader(VSPCTStatic::GetPtr()->GetShader(), nullptr, 0);
			pD3D11DeviceContext->PSSetShader(PSPCTStatic::GetPtr()->GetShader(), nullptr, 0);
			//インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(VSPCTStatic::GetPtr()->GetInputLayout());
			//ブレンドステート
			if (GameObjectPtr->GetAlphaActive()) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			}
			//デプスステンシルステート
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);

			//テクスチャとサンプラーの設定
			ID3D11ShaderResourceView* pNull[1] = { 0 };
			pD3D11DeviceContext->PSSetShaderResources(0, 1, TextureRes->GetShaderResourceView().GetAddressOf());
			//ラッピングサンプラー
			ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
			if (WrapSampler) {
				pSampler = RenderState->GetLinearWrap();
			}
			pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

			//透明処理なら
			if (GameObjectPtr->GetAlphaActive()) {
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//ラスタライザステート
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			//後始末
			Dev->InitializeStates();
		}

		static void PNNotShadowDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<MeshResource>& MeshRes,
			const Color4& Emissive, const Color4& Diffuse, const Matrix4X4& MeshToTransformMatrix) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//行列の定義
			auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
			//行列の定義
			Matrix4X4 World, ViewMat, ProjMat;
			//ワールド行列の決定
			World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
			//転置する
			World.Transpose();
			//カメラを得る
			auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
			//ビューと射影行列を得る
			ViewMat = CameraPtr->GetViewMatrix();
			//転置する
			ViewMat.Transpose();
			//転置する
			ProjMat = CameraPtr->GetProjMatrix();
			ProjMat.Transpose();

			//コンスタントバッファの設定
			StaticLightingConstantBuffer cb1;
			//行列の設定(転置する)
			//コンスタントバッファの設定
			cb1.World = World;
			cb1.View = ViewMat;
			cb1.Projection = ProjMat;
			//ライティング
			auto StageLight = GameObjectPtr->OnGetDrawLight();
			cb1.LightDir = StageLight.m_Directional;
			cb1.LightDir.w = 1.0f;
			cb1.Emissive = Emissive;
			cb1.Diffuse = Diffuse;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBStaticLighting::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);

			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionNormal);
			UINT offset = 0;
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

			//描画方法（3角形）
			pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBStaticLighting::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//シェーダの設定
			pD3D11DeviceContext->VSSetShader(VSPNStatic::GetPtr()->GetShader(), nullptr, 0);
			pD3D11DeviceContext->PSSetShader(PSPNStatic::GetPtr()->GetShader(), nullptr, 0);
			//インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(VSPNStatic::GetPtr()->GetInputLayout());
			//ブレンドステート
			if (GameObjectPtr->GetAlphaActive()) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			}
			//デプスステンシルステート
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);

			//透明処理なら
			if (GameObjectPtr->GetAlphaActive()) {
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//ラスタライザステート
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			//後始末
			Dev->InitializeStates();
		}


		static void PNTNotShadowDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes, bool WrapSampler,
			const Color4& Emissive, const Color4& Diffuse, const Matrix4X4& MeshToTransformMatrix,
			ShaderLighting Lighting) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//行列の定義
			auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
			//行列の定義
			Matrix4X4 World, ViewMat, ProjMat;
			//ワールド行列の決定
			World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
			//転置する
			World.Transpose();
			//カメラを得る
			auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
			//ビューと射影行列を得る
			ViewMat = CameraPtr->GetViewMatrix();
			//転置する
			ViewMat.Transpose();
			//転置する
			ProjMat = CameraPtr->GetProjMatrix();
			ProjMat.Transpose();

			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			switch (Lighting) {
				case ShaderLighting::Simple:
				{
					//コンスタントバッファの設定
					StaticLightingConstantBuffer cb1;
					//行列の設定(転置する)
					//コンスタントバッファの設定
					cb1.World = World;
					cb1.View = ViewMat;
					cb1.Projection = ProjMat;
					//ライティング
					auto StageLight = GameObjectPtr->OnGetDrawLight();
					cb1.LightDir = StageLight.m_Directional;
					cb1.LightDir.w = 1.0f;
					cb1.Emissive = Emissive;
					cb1.Diffuse = Diffuse;
					//コンスタントバッファの更新
					pD3D11DeviceContext->UpdateSubresource(CBStaticLighting::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
					//コンスタントバッファの設定
					ID3D11Buffer* pConstantBuffer = CBStaticLighting::GetPtr()->GetBuffer();
					ID3D11Buffer* pNullConstantBuffer = nullptr;
					//頂点シェーダに渡す
					pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					//ピクセルシェーダに渡す
					pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
					//シェーダの設定
					pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
					if (TextureRes) {
						pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
						pD3D11DeviceContext->PSSetShaderResources(0, 1, TextureRes->GetShaderResourceView().GetAddressOf());
						//ラッピングサンプラー
						ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
						if (WrapSampler) {
							pSampler = RenderState->GetLinearWrap();
						}
						pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
					}
					else {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticNoTex::GetPtr()->GetShader(), nullptr, 0);
						//シェーダーリソースもクリア
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//サンプラーもクリア
						pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
					}
				}
				break;
				case ShaderLighting::Midium:
				{
					//コンスタントバッファの設定
					StaticLightingConstantBuffer cb1;
					//行列の設定(転置する)
					//コンスタントバッファの設定
					cb1.World = World;
					cb1.View = ViewMat;
					cb1.Projection = ProjMat;
					//ライティング
					auto StageLight = GameObjectPtr->OnGetDrawLight();
					cb1.LightDir = StageLight.m_Directional;
					cb1.LightDir.w = 1.0f;
					cb1.Emissive = Emissive;
					cb1.Diffuse = Diffuse;
					//コンスタントバッファの更新
					pD3D11DeviceContext->UpdateSubresource(CBStaticLighting::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
					//コンスタントバッファの設定
					ID3D11Buffer* pConstantBuffer = CBStaticLighting::GetPtr()->GetBuffer();
					ID3D11Buffer* pNullConstantBuffer = nullptr;
					//頂点シェーダに渡す
					pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					//ピクセルシェーダに渡す
					pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
					//シェーダの設定
					pD3D11DeviceContext->VSSetShader(VSPNTStaticMidium::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(VSPNTStaticMidium::GetPtr()->GetInputLayout());
					if (TextureRes) {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticMidium::GetPtr()->GetShader(), nullptr, 0);
						pD3D11DeviceContext->PSSetShaderResources(0, 1, TextureRes->GetShaderResourceView().GetAddressOf());
						//ラッピングサンプラー
						ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
						if (WrapSampler) {
							pSampler = RenderState->GetLinearWrap();
						}
						pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
					}
					else {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticMidiumNoTex::GetPtr()->GetShader(), nullptr, 0);
						//シェーダーリソースもクリア
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//サンプラーもクリア
						pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
					}
				}
				break;
				case ShaderLighting::Real:
				{
					StaticRealLightingConstantBuffer cb1;
					//行列の設定(転置する)
					//コンスタントバッファの設定
					cb1.World = World;
					cb1.View = ViewMat;
					cb1.Projection = ProjMat;
					//ライティング
					auto StageLight = GameObjectPtr->OnGetDrawLight();
					cb1.LightDir = StageLight.m_Directional;
					cb1.LightDir.w = 1.0f;
					cb1.Emissive = Emissive;
					cb1.Diffuse = Diffuse;
					cb1.EyePosition = CameraPtr->GetEye();
					cb1.EyePosition.w = 1.0f;
					//コンスタントバッファの更新
					pD3D11DeviceContext->UpdateSubresource(CBStaticRealLighting::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
					//コンスタントバッファの設定
					ID3D11Buffer* pConstantBuffer = CBStaticRealLighting::GetPtr()->GetBuffer();
					ID3D11Buffer* pNullConstantBuffer = nullptr;
					//頂点シェーダに渡す
					pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					//ピクセルシェーダに渡す
					pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
					//シェーダの設定
					pD3D11DeviceContext->VSSetShader(VSPNTStaticReal::GetPtr()->GetShader(), nullptr, 0);
					pD3D11DeviceContext->PSSetShader(PSPNTStaticReal::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(VSPNTStaticReal::GetPtr()->GetInputLayout());
					if (TextureRes) {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticReal::GetPtr()->GetShader(), nullptr, 0);
						pD3D11DeviceContext->PSSetShaderResources(0, 1, TextureRes->GetShaderResourceView().GetAddressOf());
						//ラッピングサンプラー
						ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
						if (WrapSampler) {
							pSampler = RenderState->GetLinearWrap();
						}
						pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
					}
					else {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticRealNoTex::GetPtr()->GetShader(), nullptr, 0);
						//シェーダーリソースもクリア
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//サンプラーもクリア
						pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
					}

				}
				break;
			}

			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionNormalTexture);
			UINT offset = 0;
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//描画方法（3角形）
			pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//ブレンドステート
			if (GameObjectPtr->GetAlphaActive()) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			}
			//デプスステンシルステート
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);

			//透明処理なら
			if (GameObjectPtr->GetAlphaActive()) {
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//ラスタライザステート
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			//後始末
			Dev->InitializeStates();
		}


		static void PNTWithShadowDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes, bool WrapSampler,
			const Color4& Emissive, const Color4& Diffuse, const Matrix4X4& MeshToTransformMatrix) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//行列の定義
			auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
			//行列の定義
			Matrix4X4 World, ViewMat, ProjMat;
			//ワールド行列の決定
			World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
			//転置する
			World.Transpose();
			//カメラを得る
			auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
			//ビューと射影行列を得る
			ViewMat = CameraPtr->GetViewMatrix();
			//転置する
			ViewMat.Transpose();
			//転置する
			ProjMat = CameraPtr->GetProjMatrix();
			ProjMat.Transpose();


			//コンスタントバッファの設定
			PNTStaticShadowConstantBuffer cb1;
			ZeroMemory(&cb1, sizeof(cb1));
			//行列の設定(転置する)
			//コンスタントバッファの設定
			cb1.World = World;
			cb1.View = ViewMat;
			cb1.Projection = ProjMat;
			//ライティング
			auto StageLight = GameObjectPtr->OnGetDrawLight();
			cb1.LightDir = StageLight.m_Directional;
			cb1.LightDir.w = 1.0f;
			cb1.Emissive = Emissive;
			cb1.Diffuse = Diffuse;
			Vector3 CalcLightDir = -1.0 * StageLight.m_Directional;
			Vector3 LightAt = CameraPtr->GetAt();
			Vector3 LightEye = CalcLightDir;
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			cb1.LightPos = LightEye;
			cb1.LightPos.w = 1.0f;
			cb1.EyePos = CameraPtr->GetEye();
			cb1.EyePos.w = 1.0f;
			if (TextureRes) {
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

			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBPNTStaticShadow::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionNormalTexture);
			UINT offset = 0;
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
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
			bool IsShadowmap = false;
			if (GameObjectPtr->GetComponent<Shadowmap>(false)) {
				//シャドウマップがあれば自己影防止用のピクセルシェーダ
				pD3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
				pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);
				IsShadowmap = true;
			}
			else {
				pD3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
				pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			//インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(VSPNTStaticShadow::GetPtr()->GetInputLayout());
			//ブレンドステート
			if (GameObjectPtr->GetAlphaActive()) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			}
			//デプスステンシルステート
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
			//テクスチャとサンプラーの設定

			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			if (TextureRes) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, TextureRes->GetShaderResourceView().GetAddressOf());
				//ラッピングサンプラー
				ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
				if (WrapSampler) {
					pSampler = RenderState->GetLinearWrap();
				}
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
			}
			else {
				//シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//サンプラーもクリア
				pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
			}
			//シャドウマップのレンダラーターゲット
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//シャドウマップサンプラー
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			//透明処理なら
			if (GameObjectPtr->GetAlphaActive()) {
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//ラスタライザステート
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			//後始末
			Dev->InitializeStates();
		}
	};




	//--------------------------------------------------------------------------------------
	//	struct Base3DDraw::Impl;
	//	用途: Base3DDrawイディオム
	//--------------------------------------------------------------------------------------
	struct Base3DDraw::Impl {
		//エミッシブ色
		Color4 m_Emissive;
		//デフューズ色
		Color4 m_Diffuse;
		Impl() :
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	3D描画コンポーネントの親(3D描画の親)
	//--------------------------------------------------------------------------------------
	Base3DDraw::Base3DDraw(const shared_ptr<GameObject>& GameObjectPtr):
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	Base3DDraw::~Base3DDraw() {}
	Color4 Base3DDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void Base3DDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}
	Color4 Base3DDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void Base3DDraw::SetDiffuse(const Color4& col) {
		pImpl->m_Diffuse = col;
	}


	//--------------------------------------------------------------------------------------
	//	struct DynamicBaseDraw::Impl;
	//	用途: DynamicBaseDrawイディオム
	//--------------------------------------------------------------------------------------
	struct DynamicBaseDraw::Impl {
		//メッシュ
		shared_ptr<MeshResource> m_MeshResource;
		Impl()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	Dynamic描画コンポーネントの親(頂点を変更できる3D描画)
	//--------------------------------------------------------------------------------------
	DynamicBaseDraw::DynamicBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		Base3DDraw(GameObjectPtr),
		pImpl(new Impl())
	{}
	DynamicBaseDraw::~DynamicBaseDraw() {}
	shared_ptr<MeshResource> DynamicBaseDraw::GetMeshResource() const {
		return pImpl->m_MeshResource;
	}
	void DynamicBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}

	//--------------------------------------------------------------------------------------
	//	struct PCDynamicDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCDynamicDraw::Impl {
		Impl()
		{}
	};



	//--------------------------------------------------------------------------------------
	///	PCDynamic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCDynamicDraw::PCDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DynamicBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCDynamicDraw::~PCDynamicDraw() {}

	void PCDynamicDraw::CreateMesh(vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices) {
		try {
			//メッシュの作成（変更できる）
			auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(MeshRes);
		}
		catch (...) {
			throw;
		}
	}

	void PCDynamicDraw::UpdateVertices(const vector<VertexPositionColor>& Vertices) {
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!GetMeshResource())",
				L"PCDynamicDraw::UpdateVertices()"
			);

		}
		MeshRes->UpdateVirtexBuffer(Vertices);

	}
	void PCDynamicDraw::OnCreate() {
	}
	void PCDynamicDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PCDynamicDraw::OnDraw()"
			);
		}

		Draw3DPrim::PCDraw(GetGameObject(), MeshRes,GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}



	//--------------------------------------------------------------------------------------
	//	struct PTDynamicDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PTDynamicDraw::Impl {
		Impl()
		{}
	};

	//--------------------------------------------------------------------------------------
	///	PTDynamic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTDynamicDraw::PTDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DynamicBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PTDynamicDraw::~PTDynamicDraw() {}

	void PTDynamicDraw::CreateMesh(vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices) {
		try {
			//メッシュの作成（変更できる）
			auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(MeshRes);
		}
		catch (...) {
			throw;
		}
	}

	void PTDynamicDraw::UpdateVertices(const vector<VertexPositionTexture>& Vertices) {
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!GetMeshResource())",
				L"PTDynamicDraw::UpdateVertices()"
			);

		}
		MeshRes->UpdateVirtexBuffer(Vertices);
	}

	void PTDynamicDraw::OnCreate() {
	}
	void PTDynamicDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PTDynamicDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		Draw3DPrim::PTDraw(GetGameObject(), MeshRes,
			shTex, GetWrapSampler(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}



	//--------------------------------------------------------------------------------------
	//	struct PCTDynamicDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCTDynamicDraw::Impl {
		Impl() 
		{}
	};

	//--------------------------------------------------------------------------------------
	///	PCTDynamic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTDynamicDraw::PCTDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DynamicBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCTDynamicDraw::~PCTDynamicDraw() {}

	void PCTDynamicDraw::CreateMesh(vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices) {
		try {
			//メッシュの作成（変更できる）
			auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(MeshRes);
		}
		catch (...) {
			throw;
		}
	}

	void PCTDynamicDraw::UpdateVertices(const vector<VertexPositionColorTexture>& Vertices) {
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!GetMeshResource())",
				L"PCTDynamicDraw::UpdateVertices()"
			);

		}
		MeshRes->UpdateVirtexBuffer(Vertices);
	}

	void PCTDynamicDraw::OnCreate() {
	}
	void PCTDynamicDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PCTDynamicDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		Draw3DPrim::PCTDraw(GetGameObject(),MeshRes,
			shTex, GetWrapSampler(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}

	//--------------------------------------------------------------------------------------
	//	struct PNTDynamicDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PNTDynamicDraw::Impl {
		bool m_OwnShadowActive;
		Impl() :
			m_OwnShadowActive(false)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	PNTDynamic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTDynamicDraw::PNTDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DynamicBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTDynamicDraw::~PNTDynamicDraw() {}

	void PNTDynamicDraw::CreateMesh(vector<VertexPositionNormalTexture>& Vertices, vector<uint16_t>& indices) {
		try {
			//メッシュの作成（変更できる）
			auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(MeshRes);
		}
		catch (...) {
			throw;
		}
	}

	void PNTDynamicDraw::UpdateVertices(const vector<VertexPositionNormalTexture>& Vertices) {
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!GetMeshResource())",
				L"PNTDynamicDraw::UpdateVertices()"
			);

		}
		MeshRes->UpdateVirtexBuffer(Vertices);
	}


	bool PNTDynamicDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTDynamicDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTDynamicDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}



	void PNTDynamicDraw::OnCreate() {
	}

	void PNTDynamicDraw::DrawNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNTStaticDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		Draw3DPrim::PNTNotShadowDraw(GetGameObject(),
			MeshRes,
			shTex, GetWrapSampler(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix(), ShaderLighting::Simple);
	}

	void PNTDynamicDraw::DrawWithShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNTStaticDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		Draw3DPrim::PNTWithShadowDraw(GetGameObject(),
			MeshRes,
			shTex, GetWrapSampler(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}

	void PNTDynamicDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}



	//--------------------------------------------------------------------------------------
	//	struct StaticBaseDraw::Impl;
	//	用途: StaticBaseDrawイディオム
	//--------------------------------------------------------------------------------------
	struct StaticBaseDraw::Impl {
		//メッシュのweak_ptr
		weak_ptr<MeshResource> m_MeshResource;
		Impl()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	static描画コンポーネントの親(頂点を変更できない3D描画)
	//--------------------------------------------------------------------------------------
	StaticBaseDraw::StaticBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		Base3DDraw(GameObjectPtr),
		pImpl(new Impl())
	{}
	StaticBaseDraw::~StaticBaseDraw() {}
	shared_ptr<MeshResource> StaticBaseDraw::GetMeshResource() const {
		auto shptr = pImpl->m_MeshResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}
	void StaticBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void StaticBaseDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}



	//--------------------------------------------------------------------------------------
	//	struct PCStaticDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCStaticDraw::Impl {
		Impl()
		{}
	};



	//--------------------------------------------------------------------------------------
	///	PCStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCStaticDraw::PCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCStaticDraw::~PCStaticDraw() {}

	void PCStaticDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PCStaticDraw::OnDraw()"
			);
		}
		Draw3DPrim::PCDraw(GetGameObject(), MeshRes, GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}

	//--------------------------------------------------------------------------------------
	//	struct PTStaticDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PTStaticDraw::Impl {
		Impl()
		{}
	};
	//--------------------------------------------------------------------------------------
	///	PTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTStaticDraw::PTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	PTStaticDraw::~PTStaticDraw() {}

	void PTStaticDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PTStaticDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		Draw3DPrim::PTDraw(GetGameObject(), MeshRes,
			shTex, GetWrapSampler(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}


	//--------------------------------------------------------------------------------------
	//	struct PCTStaticDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCTStaticDraw::Impl {
		Impl()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PCTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTStaticDraw::PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCTStaticDraw::~PCTStaticDraw() {}


	void PCTStaticDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PCTStaticDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		Draw3DPrim::PCTDraw(GetGameObject(), MeshRes,
			shTex, GetWrapSampler(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}

	//--------------------------------------------------------------------------------------
	//	struct PNStaticDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNStaticDraw::Impl {
		bool m_OwnShadowActive;
		Impl() :
			m_OwnShadowActive(false)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	PNStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNStaticDraw::PNStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNStaticDraw::~PNStaticDraw() {}

	void PNStaticDraw::OnCreate() {
	}

	bool PNStaticDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNStaticDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNStaticDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}

	void PNStaticDraw::DrawNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNStaticDraw::OnDraw()"
			);
		}
		Draw3DPrim::PNNotShadowDraw(GetGameObject(),
			MeshRes,
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}

	void PNStaticDraw::DrawWithShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNStaticDraw::OnDraw()"
			);
		}
		//Draw3DPrim::PNWithShadowDraw(GetGameObject(),
		//	MeshRes,
		//	GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}

	void PNStaticDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}




	//--------------------------------------------------------------------------------------
	//	struct PNTStaticDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTStaticDraw::Impl {
		bool m_OwnShadowActive;
		ShaderLighting m_ShaderLighting;
		Impl() :
			m_OwnShadowActive(false),
			m_ShaderLighting(ShaderLighting::Simple)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class PNTStaticDraw : public DrawComponent;
	//	用途: PNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::OnCreate() {
	}

	ShaderLighting  PNTStaticDraw::GetLighting() const {
		return pImpl->m_ShaderLighting;

	}
	void PNTStaticDraw::SetLighting(ShaderLighting Lighting) {
		pImpl->m_ShaderLighting = Lighting;
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
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNTStaticDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		Draw3DPrim::PNTNotShadowDraw(GetGameObject(),
			MeshRes,
			shTex, GetWrapSampler(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix(),pImpl->m_ShaderLighting);
	}

	void PNTStaticDraw::DrawWithShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNTStaticDraw::OnDraw()"
			);
		}
		//テクスチャがなければ描画しない
		auto shTex = GetTextureResource();
		if (!shTex) {
	//		return;
		}
		Draw3DPrim::PNTWithShadowDraw(GetGameObject(),
			MeshRes,
			shTex, GetWrapSampler(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}

	void PNTStaticDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct PNTStaticModelDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTStaticModelDraw::Impl {
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		bool m_OwnShadowActive;
		ShaderLighting m_ShaderLighting;
		Impl() :
			m_OwnShadowActive(false),
			m_ShaderLighting(ShaderLighting::Simple)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class PNTStaticModelDraw : public DrawComponent;
	//	用途: PNTStaticModelDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticModelDraw::PNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTStaticModelDraw::~PNTStaticModelDraw() {}

	void PNTStaticModelDraw::OnCreate() {
	}

	ShaderLighting  PNTStaticModelDraw::GetLighting() const {
		return pImpl->m_ShaderLighting;

	}
	void PNTStaticModelDraw::SetLighting(ShaderLighting Lighting) {
		pImpl->m_ShaderLighting = Lighting;
	}

	bool PNTStaticModelDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTStaticModelDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTStaticModelDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}



	void PNTStaticModelDraw::DrawWithShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNTStaticModelDraw::OnDraw()"
			);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();

		auto GameObjectPtr = GetGameObject();
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		//カメラを得る
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.Transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();

		//コンスタントバッファの設定
		PNTStaticShadowConstantBuffer cb1;

		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = World;
		cb1.View = ViewMat;
		cb1.Projection = ProjMat;

		//ライティング
		auto StageLight = GameObjectPtr->OnGetDrawLight();
		cb1.LightDir = StageLight.m_Directional;
		cb1.LightDir.w = 1.0f;

		Vector3 CalcLightDir = -1.0 * StageLight.m_Directional;
		Vector3 LightAt = CameraPtr->GetAt();
		Vector3 LightEye = CalcLightDir;
		LightEye *= Shadowmap::GetLightHeight();
		LightEye = LightAt + LightEye;

		cb1.LightPos = LightEye;
		cb1.LightPos.w = 1.0f;

		cb1.EyePos = CameraPtr->GetEye();
		cb1.EyePos.w = 1.0f;
		//モデルはテクスチャ必須
		cb1.ActiveFlg.x = 1;

		Matrix4X4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		cb1.LightView = Matrix4X4EX::Transpose(LightView);
		cb1.LightProjection = Matrix4X4EX::Transpose(LightProj);

		//シャドウマップのリソースビューを取得
		//シャドウマップのレンダラーターゲット
		auto ShadoumapPtr = Dev->GetShadowMapRenderTarget();
		ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();


		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		//シェーダの設定
		bool IsShadowmap = false;
		if (GameObjectPtr->GetComponent<Shadowmap>(false)) {
			//シャドウマップがあれば自己影防止用のピクセルシェーダ
			pID3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			pID3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);

			IsShadowmap = true;
		}
		else {
			pID3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			pID3D11DeviceContext->PSSetShader(PSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
		}

		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPNTStaticShadow::GetPtr()->GetInputLayout());
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshPtr->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshPtr->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//デプスステンシル
		switch (GetDepthStencilState()) {
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
		//サンプラー

		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
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
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//影のシェーダリソース
		pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
		pSampler = RenderStatePtr->GetComparisonLinear();
		//影のサンプラー
		pID3D11DeviceContext->PSSetSamplers(1, 1, &pSampler);

		for (auto& m : MatVec) {
			cb1.Emissive = m.m_Emissive;
			cb1.Diffuse = m.m_Diffuse;
			//コンスタントバッファの更新
			ID3D11Buffer* pConstantBuffer = CBPNTStaticShadow::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
			if (GetRasterizerState() == RasterizerState::Wireframe) {
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				//描画
				pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (GameObjectPtr->GetAlphaActive()) {
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					switch (GetBlendState()) {
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
					switch (GetRasterizerState()) {
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
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}
		}
		Dev->InitializeStates();
	}

	void PNTStaticModelDraw::DrawNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNTStaticModelDraw::OnDraw()"
			);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		auto GameObjectPtr = GetGameObject();


		//行列の定義
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		//カメラを得る
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.Transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシル
		SetDeviceDepthStencilState();

		for (auto& m : MatVec) {
			switch (GetLighting()) {
				case ShaderLighting::Simple:
				{
					//コンスタントバッファの設定
					StaticLightingConstantBuffer cb1;
					//行列の設定(転置する)
					//コンスタントバッファの設定
					cb1.World = World;
					cb1.View = ViewMat;
					cb1.Projection = ProjMat;
					//ライティング
					auto StageLight = GameObjectPtr->OnGetDrawLight();
					cb1.LightDir = StageLight.m_Directional;
					cb1.LightDir.w = 1.0f;
					//Simpleはモデルの色
					cb1.Emissive = m.m_Emissive;
					cb1.Diffuse = m.m_Diffuse;
					//コンスタントバッファの更新
					pD3D11DeviceContext->UpdateSubresource(CBStaticLighting::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
					//コンスタントバッファの設定
					ID3D11Buffer* pConstantBuffer = CBStaticLighting::GetPtr()->GetBuffer();
					ID3D11Buffer* pNullConstantBuffer = nullptr;
					//頂点シェーダに渡す
					pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					//ピクセルシェーダに渡す
					pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
					//シェーダの設定
					pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
					if (m.m_TextureResource) {
						pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
						pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
						//サンプラー
						SetSamplerState(SamplerState::LinearClamp);
						SetDeviceSamplerState();
					}
					else {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticNoTex::GetPtr()->GetShader(), nullptr, 0);
						//シェーダーリソースもクリア
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//サンプラーもクリア
						SetSamplerState(SamplerState::SamplerNone);
						SetDeviceSamplerState();
					}
				}
				break;
				case ShaderLighting::Midium:
				{
					//コンスタントバッファの設定
					StaticLightingConstantBuffer cb1;
					//行列の設定(転置する)
					//コンスタントバッファの設定
					cb1.World = World;
					cb1.View = ViewMat;
					cb1.Projection = ProjMat;
					//ライティング
					auto StageLight = GameObjectPtr->OnGetDrawLight();
					cb1.LightDir = StageLight.m_Directional;
					cb1.LightDir.w = 1.0f;
					//Midiumはモデルの色とオブジェクトの色を合成
					cb1.Emissive = m.m_Emissive + GetEmissive();
					cb1.Diffuse = (m.m_Diffuse + GetDiffuse()) / 2.0f;
					//コンスタントバッファの更新
					pD3D11DeviceContext->UpdateSubresource(CBStaticLighting::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
					//コンスタントバッファの設定
					ID3D11Buffer* pConstantBuffer = CBStaticLighting::GetPtr()->GetBuffer();
					ID3D11Buffer* pNullConstantBuffer = nullptr;
					//頂点シェーダに渡す
					pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					//ピクセルシェーダに渡す
					pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
					//シェーダの設定
					pD3D11DeviceContext->VSSetShader(VSPNTStaticMidium::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(VSPNTStaticMidium::GetPtr()->GetInputLayout());
					if (m.m_TextureResource) {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticMidium::GetPtr()->GetShader(), nullptr, 0);
						pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
						//サンプラー
						SetSamplerState(SamplerState::LinearClamp);
						SetDeviceSamplerState();
					}
					else {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticMidiumNoTex::GetPtr()->GetShader(), nullptr, 0);
						//シェーダーリソースもクリア
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//サンプラーもクリア
						SetSamplerState(SamplerState::SamplerNone);
						SetDeviceSamplerState();
					}
				}
				break;
				case ShaderLighting::Real:
				{
					StaticRealLightingConstantBuffer cb1;
					//行列の設定(転置する)
					//コンスタントバッファの設定
					cb1.World = World;
					cb1.View = ViewMat;
					cb1.Projection = ProjMat;
					//ライティング
					auto StageLight = GameObjectPtr->OnGetDrawLight();
					cb1.LightDir = StageLight.m_Directional;
					cb1.LightDir.w = 1.0f;
					//Realはモデルの色とオブジェクトの色を合成
					cb1.Emissive = m.m_Emissive + GetEmissive();
					cb1.Diffuse = (m.m_Diffuse + GetDiffuse()) / 2.0f;
					cb1.EyePosition = CameraPtr->GetEye();
					cb1.EyePosition.w = 1.0f;
					//コンスタントバッファの更新
					pD3D11DeviceContext->UpdateSubresource(CBStaticRealLighting::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
					//コンスタントバッファの設定
					ID3D11Buffer* pConstantBuffer = CBStaticRealLighting::GetPtr()->GetBuffer();
					ID3D11Buffer* pNullConstantBuffer = nullptr;
					//頂点シェーダに渡す
					pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					//ピクセルシェーダに渡す
					pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
					//シェーダの設定
					pD3D11DeviceContext->VSSetShader(VSPNTStaticReal::GetPtr()->GetShader(), nullptr, 0);
					pD3D11DeviceContext->PSSetShader(PSPNTStaticReal::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトの設定
					pD3D11DeviceContext->IASetInputLayout(VSPNTStaticReal::GetPtr()->GetInputLayout());
					if (m.m_TextureResource) {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticReal::GetPtr()->GetShader(), nullptr, 0);
						pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
						//サンプラー
						SetSamplerState(SamplerState::LinearClamp);
						SetDeviceSamplerState();
					}
					else {
						pD3D11DeviceContext->PSSetShader(PSPNTStaticRealNoTex::GetPtr()->GetShader(), nullptr, 0);
						//シェーダーリソースもクリア
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//サンプラーもクリア
						SetSamplerState(SamplerState::SamplerNone);
						SetDeviceSamplerState();
					}
				}
				break;
			}

			if (GetRasterizerState() == RasterizerState::Wireframe) {
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				pD3D11DeviceContext->RSSetState(RenderState->GetWireframe());
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (GameObjectPtr->GetAlphaActive()) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
					//描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
					//描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					//設定されているブレンドステート
					SetDeviceBlendState();
					//設定されているラスタライザステート
					SetDeviceRasterizerState();
					//描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}

		}
		//後始末
		Dev->InitializeStates();
	}

	void PNTStaticModelDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct PNTBoneModelDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTBoneModelDraw::Impl {
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		bool m_OwnShadowActive;
		ShaderLighting m_ShaderLighting;
		//シェーダに渡すボーン行列
		vector<Matrix4X4> m_LocalBonesMatrix;
		map<wstring, AnimationData> m_AnimationMap;
		wstring m_CurrentAnimeName;
		float m_CurrentAnimeTime;
		Impl() :
			m_OwnShadowActive(false),
			m_ShaderLighting(ShaderLighting::Simple),
			m_CurrentAnimeName(L""),
			m_CurrentAnimeTime(0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class PNTBoneModelDraw : public StaticBaseDraw;
	//	用途: PNTBoneModelDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTBoneModelDraw::PNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		Base3DDraw(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTBoneModelDraw::~PNTBoneModelDraw() {}

	void PNTBoneModelDraw::OnCreate() {

	}

	shared_ptr<MeshResource> PNTBoneModelDraw::GetMeshResource() const {
		//メッシュがなければリターン
		if (pImpl->m_MeshResource.expired()) {
			throw BaseException(
				L"メッシュが設定されてません",
				L"if (pImpl->m_MeshResource.expired())",
				L"PNTStaticModelDraw::GetMeshResource()"
			);
		}
		return pImpl->m_MeshResource.lock();
	}

	void PNTBoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0) {
			throw BaseException(
				L"メッシュがボーンメッシュではありません",
				L"if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0)",
				L"PNTBoneModelDraw::SetMeshResource()"
			);

		}
		//先頭のボーン数の行列で初期化
		pImpl->m_LocalBonesMatrix.resize(MeshRes->GetBoneCount());
		auto& SampleMatrixVec = MeshRes->GetSampleMatrixVec();
		for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
			pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[i];
		}
		pImpl->m_MeshResource = MeshRes;
	}
	void PNTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(MeshKey);
		if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0) {
			throw BaseException(
				L"メッシュがボーンメッシュではありません",
				L"if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0)",
				L"PNTBoneModelDraw::SetMeshResource()"
			);

		}
		//先頭のボーン数の行列で初期化
		pImpl->m_LocalBonesMatrix.resize(MeshRes->GetBoneCount());
		auto& SampleMatrixVec = MeshRes->GetSampleMatrixVec();
		for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
			pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[i];
		}
		pImpl->m_MeshResource = MeshRes;
	}

	bool PNTBoneModelDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTBoneModelDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTBoneModelDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}

	ShaderLighting  PNTBoneModelDraw::GetLighting() const {
		return pImpl->m_ShaderLighting;

	}
	void PNTBoneModelDraw::SetLighting(ShaderLighting Lighting) {
		pImpl->m_ShaderLighting = Lighting;
	}


	const vector< Matrix4X4 >* PNTBoneModelDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_LocalBonesMatrix;
	}


	void PNTBoneModelDraw::AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
		float SamplesParSecond) {
		if (Name == L"") {
			throw BaseException(
				L"アニメーション名が空白です",
				L"if (Name == L\"\")",
				L"PNTBoneModelDraw::AddAnimation()"
			);
		}
		if (StartSample < 0 || SampleLength < 0) {
			throw BaseException(
				L"開始サンプルかサンプル数が0未満です",
				L"if (StartSample < 0 || SampleLength < 0)",
				L"PNTBoneModelDraw::AddAnimation()"
			);
		}
		if (SamplesParSecond <= 0.0f) {
			throw BaseException(
				L"サンプル毎秒が0以下です",
				L"if (SamplesParSecond <= 0.0f)",
				L"PNTBoneModelDraw::AddAnimation()"
			);
		}
		//重複キーがあれば差し替える
		AnimationData Data((UINT)StartSample, (UINT)SampleLength, Loop, SamplesParSecond);
		pImpl->m_AnimationMap[Name] = Data;
		if (pImpl->m_AnimationMap.size() == 1) {
			//1つしか登録がなかったら、カレントアニメは該当アニメとなる
			ChangeCurrentAnimation(Name, 0);
		}


	}

	void PNTBoneModelDraw::ChangeCurrentAnimation(const wstring& AnemationName, float StartTime) {
		if (AnemationName == L"") {
			throw BaseException(
				L"アニメーション名が空白です",
				L"if (AnemationName == L\"\")",
				L"PNTBoneModelDraw::SetCurrentAnimation()"
			);
		}
		auto it = pImpl->m_AnimationMap.find(AnemationName);
		if (it != pImpl->m_AnimationMap.end()) {
			//指定の名前が見つかった
			pImpl->m_CurrentAnimeName = AnemationName;
			pImpl->m_CurrentAnimeTime = StartTime;
			//アニメーションは終了していない
			it->second.m_IsAnimeEnd = false;
		}
		else {
			//見つからない
			throw BaseException(
				L"指定のアニメーションは登録されてません",
				AnemationName,
				L"PNTBoneModelDraw::SetCurrentAnimation()"
			);
		}
	}

	const wstring& PNTBoneModelDraw::GetCurrentAnimation() const {
		return pImpl->m_CurrentAnimeName;
	}

	bool PNTBoneModelDraw::UpdateAnimation(float ElapsedTime) {
		if (ElapsedTime < 0.0f) {
			throw BaseException(
				L"アニメーション更新にマイナスは設定できません",
				L"if (ElapsedTime < 0.0f)",
				L"PNTBoneModelDraw::UpdateAnimation()"
			);
		}
		if (pImpl->m_CurrentAnimeName == L"") {
			//見つからない
			throw BaseException(
				L"カレントアニメーションが設定されてません",
				L"if (pImpl->m_CurrentAnimeName == L\"\")",
				L"PNTBoneModelDraw::UpdateAnimation()"
			);
		}
		auto PtrMesh = GetMeshResource();
		UINT SampleCount = PtrMesh->GetSampleCount();
		auto& SampleMatrixVec = PtrMesh->GetSampleMatrixVec();
		UINT BoneCount = PtrMesh->GetBoneCount();
		auto& TgtAnimeData = pImpl->m_AnimationMap[pImpl->m_CurrentAnimeName];
		if (TgtAnimeData.m_StartSample >= SampleCount) {
			//スタートのサンプルが最後のサンプル以降だった
			TgtAnimeData.m_StartSample = SampleCount - 1;
			TgtAnimeData.m_SampleLength = 0;
			UINT UITgtSample = TgtAnimeData.m_StartSample;
			//最後のサンプルを表示
			for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
				pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
			}
			pImpl->m_CurrentAnimeTime = 0;
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
		pImpl->m_CurrentAnimeTime += ElapsedTime;
		//スタート位置を計算
		auto FLOATTgtSample = (float)TgtAnimeData.m_StartSample + pImpl->m_CurrentAnimeTime * TgtAnimeData.m_SamplesParSecond;
		UINT UITgtSample = (UINT)FLOATTgtSample;
		UINT UILastSample = TgtAnimeData.m_StartSample + TgtAnimeData.m_SampleLength;
		if (UILastSample >= SampleCount) {
			UILastSample = SampleCount - 1;
		}
		if (UITgtSample >= UILastSample) {
			UITgtSample = UILastSample - 1;
			//最後のサンプルを表示
			for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
				pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
			}
			if (TgtAnimeData.m_IsLoop) {
				TgtAnimeData.m_IsAnimeEnd = false;
				//ループするのでカレントタイムを0にする
				pImpl->m_CurrentAnimeTime = 0;
				return false;
			}
			else {
				pImpl->m_CurrentAnimeTime = TgtAnimeData.m_SampleLength / TgtAnimeData.m_SamplesParSecond;
				TgtAnimeData.m_IsAnimeEnd = true;
				return true;
			}
		}
		else {
			//サンプルとサンプルの間の割合を計算
			FLOATTgtSample -= (float)UITgtSample;
			UINT UINextSample = UITgtSample + 1;
			for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++) {
				InterpolationMatrix(
					SampleMatrixVec[BoneCount * UITgtSample + i],
					SampleMatrixVec[BoneCount * UINextSample + i],
					FLOATTgtSample, pImpl->m_LocalBonesMatrix[i]);
			}
			//アニメは終わってない
			return false;
		}
	}


	void PNTBoneModelDraw::InterpolationMatrix(const Matrix4X4& m1, const Matrix4X4& m2, float t, Matrix4X4& out) {
		Vector3 Scale1, Pos1;
		Quaternion Qt1;
		m1.Decompose(Scale1, Qt1, Pos1);
		Qt1.Normalize();

		Vector3 Scale2, Pos2;
		Quaternion Qt2;

		m2.Decompose(Scale2, Qt2, Pos2);
		Qt2.Normalize();

		Vector3 ScaleOut, PosOut;
		Quaternion QtOut;

		ScaleOut = Lerp::CalculateLerp(Scale1, Scale2, 0.0f, 1.0f, t, Lerp::Linear);
		PosOut = Lerp::CalculateLerp(Pos1, Pos2, 0.0f, 1.0f, t, Lerp::Linear);
		QtOut = QuaternionEX::Slerp(Qt1, Qt2, t);
		out.DefTransformation(ScaleOut, QtOut, PosOut);
	}


	void PNTBoneModelDraw::DrawWithShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNTBoneModelDraw::OnDraw()"
			);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();

		auto GameObjectPtr = GetGameObject();
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		//カメラを得る
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.Transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();


		//コンスタントバッファの設定
		PNTBoneShadowConstantBuffer cb1;

		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = World;
		cb1.View = ViewMat;
		cb1.Projection = ProjMat;

		//ライティング
		auto StageLight = GameObjectPtr->OnGetDrawLight();
		cb1.LightDir = StageLight.m_Directional;
		cb1.LightDir.w = 1.0f;

		Vector3 CalcLightDir = -1.0 * StageLight.m_Directional;
		Vector3 LightAt = CameraPtr->GetAt();
		Vector3 LightEye = CalcLightDir;
		LightEye *= Shadowmap::GetLightHeight();
		LightEye = LightAt + LightEye;

		cb1.LightPos = LightEye;
		cb1.LightPos.w = 1.0f;

		cb1.EyePos = CameraPtr->GetEye();
		cb1.EyePos.w = 1.0f;
		//モデルはテクスチャ必須
		cb1.ActiveFlg.x = 1;

		Matrix4X4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		cb1.LightView = Matrix4X4EX::Transpose(LightView);
		cb1.LightProjection = Matrix4X4EX::Transpose(LightProj);

		//ボーンの設定
		size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
		UINT cb_count = 0;
		for (size_t b = 0; b < BoneSz; b++) {
			Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
			mat.Transpose();
			cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
			cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
			cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
			cb_count += 3;
		}


		//シャドウマップのリソースビューを取得
		//シャドウマップのレンダラーターゲット
		auto ShadoumapPtr = Dev->GetShadowMapRenderTarget();
		ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		//シェーダの設定
		bool IsShadowmap = false;
		if (GameObjectPtr->GetComponent<Shadowmap>(false)) {
			//シャドウマップがあれば自己影防止用のピクセルシェーダ
			pID3D11DeviceContext->VSSetShader(VSPNTBoneShadow::GetPtr()->GetShader(), nullptr, 0);
			pID3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);

			IsShadowmap = true;
		}
		else {
			pID3D11DeviceContext->VSSetShader(VSPNTBoneShadow::GetPtr()->GetShader(), nullptr, 0);
			pID3D11DeviceContext->PSSetShader(PSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
		}

		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPNTBoneShadow::GetPtr()->GetInputLayout());

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshPtr->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshPtr->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//デプスステンシル
		switch (GetDepthStencilState()) {
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
		//サンプラー

		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
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
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//影のシェーダリソース
		pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
		pSampler = RenderStatePtr->GetComparisonLinear();
		//影のサンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(1, 1, &pSampler);

		for (auto& m : MatVec) {
			cb1.Emissive = m.m_Emissive;
			cb1.Diffuse = m.m_Diffuse;
			//コンスタントバッファの更新
			ID3D11Buffer* pConstantBuffer = CBPNTBoneShadow::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
			if (GetRasterizerState() == RasterizerState::Wireframe) {
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				//描画
				pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (GameObjectPtr->GetAlphaActive()) {
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					switch (GetBlendState()) {
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
					switch (GetRasterizerState()) {
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
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}
		}
		Dev->InitializeStates();

	}

	void PNTBoneModelDraw::DrawNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNTBoneModelDraw::OnDraw()"
			);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		auto GameObjectPtr = GetGameObject();


		//行列の定義
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		//カメラを得る
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.Transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシル
		SetDeviceDepthStencilState();

		for (auto& m : MatVec) {
			switch (GetLighting()) {
			case ShaderLighting::Simple:
			{
				//コンスタントバッファの設定
				PNTBoneConstantBuffer cb1;
				//行列の設定(転置する)
				//コンスタントバッファの設定
				cb1.World = World;
				cb1.View = ViewMat;
				cb1.Projection = ProjMat;
				//ライティング
				auto StageLight = GameObjectPtr->OnGetDrawLight();
				cb1.LightDir = StageLight.m_Directional;
				cb1.LightDir.w = 1.0f;
				//Simpleはモデルの色
				cb1.Emissive = m.m_Emissive;
				cb1.Diffuse = m.m_Diffuse;
				//ボーンの設定
				size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
				UINT cb_count = 0;
				for (size_t b = 0; b < BoneSz; b++) {
					Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
					mat.Transpose();
					cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
					cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
					cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
					cb_count += 3;
				}

				//コンスタントバッファの更新
				pD3D11DeviceContext->UpdateSubresource(CBPNTBone::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBPNTBone::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//頂点シェーダに渡す
				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//ピクセルシェーダに渡す
				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				//シェーダの設定
				pD3D11DeviceContext->VSSetShader(VSPNTBone::GetPtr()->GetShader(), nullptr, 0);
				//インプットレイアウトの設定
				pD3D11DeviceContext->IASetInputLayout(VSPNTBone::GetPtr()->GetInputLayout());
				if (m.m_TextureResource) {
					pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
					pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
					//サンプラー
					SetSamplerState(SamplerState::LinearClamp);
					SetDeviceSamplerState();
				}
				else {
					pD3D11DeviceContext->PSSetShader(PSPNTStaticNoTex::GetPtr()->GetShader(), nullptr, 0);
					//シェーダーリソースもクリア
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//サンプラーもクリア
					SetSamplerState(SamplerState::SamplerNone);
					SetDeviceSamplerState();
				}
			}
			break;
			case ShaderLighting::Midium:
			{
				//コンスタントバッファの設定
				PNTBoneConstantBuffer cb1;
				//行列の設定(転置する)
				//コンスタントバッファの設定
				cb1.World = World;
				cb1.View = ViewMat;
				cb1.Projection = ProjMat;
				//ライティング
				auto StageLight = GameObjectPtr->OnGetDrawLight();
				cb1.LightDir = StageLight.m_Directional;
				cb1.LightDir.w = 1.0f;
				//Midiumはモデルの色とオブジェクトの色を合成
				cb1.Emissive = m.m_Emissive + GetEmissive();
				cb1.Diffuse = (m.m_Diffuse + GetDiffuse()) / 2.0f;
				//ボーンの設定
				size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
				UINT cb_count = 0;
				for (size_t b = 0; b < BoneSz; b++) {
					Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
					mat.Transpose();
					cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
					cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
					cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
					cb_count += 3;
				}


				//コンスタントバッファの更新
				pD3D11DeviceContext->UpdateSubresource(CBPNTBone::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBPNTBone::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//頂点シェーダに渡す
				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//ピクセルシェーダに渡す
				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				//シェーダの設定
				pD3D11DeviceContext->VSSetShader(VSPNTBoneMidium::GetPtr()->GetShader(), nullptr, 0);
				//インプットレイアウトの設定
				pD3D11DeviceContext->IASetInputLayout(VSPNTBoneMidium::GetPtr()->GetInputLayout());
				if (m.m_TextureResource) {
					pD3D11DeviceContext->PSSetShader(PSPNTStaticMidium::GetPtr()->GetShader(), nullptr, 0);
					pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
					//サンプラー
					SetSamplerState(SamplerState::LinearClamp);
					SetDeviceSamplerState();
				}
				else {
					pD3D11DeviceContext->PSSetShader(PSPNTStaticMidiumNoTex::GetPtr()->GetShader(), nullptr, 0);
					//シェーダーリソースもクリア
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//サンプラーもクリア
					SetSamplerState(SamplerState::SamplerNone);
					SetDeviceSamplerState();
				}
			}
			break;
			case ShaderLighting::Real:
			{
				PNTBoneRealLightingConstantBuffer cb1;
				//行列の設定(転置する)
				//コンスタントバッファの設定
				cb1.World = World;
				cb1.View = ViewMat;
				cb1.Projection = ProjMat;
				//ライティング
				auto StageLight = GameObjectPtr->OnGetDrawLight();
				cb1.LightDir = StageLight.m_Directional;
				cb1.LightDir.w = 1.0f;
				//Realはモデルの色とオブジェクトの色を合成
				cb1.Emissive = m.m_Emissive + GetEmissive();
				cb1.Diffuse = (m.m_Diffuse + GetDiffuse()) / 2.0f;
				cb1.EyePosition = CameraPtr->GetEye();
				cb1.EyePosition.w = 1.0f;
				//ボーンの設定
				size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
				UINT cb_count = 0;
				for (size_t b = 0; b < BoneSz; b++) {
					Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
					mat.Transpose();
					cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
					cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
					cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
					cb_count += 3;
				}

				//コンスタントバッファの更新
				pD3D11DeviceContext->UpdateSubresource(CBPNTBoneRealLighting::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBPNTBoneRealLighting::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//頂点シェーダに渡す
				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//ピクセルシェーダに渡す
				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				//シェーダの設定
				pD3D11DeviceContext->VSSetShader(VSPNTBoneReal::GetPtr()->GetShader(), nullptr, 0);
				pD3D11DeviceContext->PSSetShader(PSPNTStaticReal::GetPtr()->GetShader(), nullptr, 0);
				//インプットレイアウトの設定
				pD3D11DeviceContext->IASetInputLayout(VSPNTBoneReal::GetPtr()->GetInputLayout());
				if (m.m_TextureResource) {
					pD3D11DeviceContext->PSSetShader(PSPNTStaticReal::GetPtr()->GetShader(), nullptr, 0);
					pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
					//サンプラー
					SetSamplerState(SamplerState::LinearClamp);
					SetDeviceSamplerState();
				}
				else {
					pD3D11DeviceContext->PSSetShader(PSPNTStaticRealNoTex::GetPtr()->GetShader(), nullptr, 0);
					//シェーダーリソースもクリア
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//サンプラーもクリア
					SetSamplerState(SamplerState::SamplerNone);
					SetDeviceSamplerState();
				}
			}
			break;
			}

			if (GetRasterizerState() == RasterizerState::Wireframe) {
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				pD3D11DeviceContext->RSSetState(RenderState->GetWireframe());
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (GameObjectPtr->GetAlphaActive()) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
					//描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
					//描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					//設定されているブレンドステート
					SetDeviceBlendState();
					//設定されているラスタライザステート
					SetDeviceRasterizerState();
					//描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}

		}
		//後始末
		Dev->InitializeStates();






/*
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"PNTBoneModelDraw::OnDraw()"
			);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();

		auto GameObjectPtr = GetGameObject();
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		//カメラを得る
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.Transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();

		//コンスタントバッファの設定
		PNTBoneConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = World;
		cb1.View = ViewMat;
		cb1.Projection = ProjMat;
		auto StageLight = GameObjectPtr->OnGetDrawLight();
		cb1.LightDir = StageLight.m_Directional;
		cb1.LightDir.w = 1.0f;

		//ボーンの設定
		size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
		UINT cb_count = 0;
		for (size_t b = 0; b < BoneSz; b++) {
			Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
			mat.Transpose();
			cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
			cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
			cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
			cb_count += 3;
		}

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSPNTBone::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPNTBone::GetPtr()->GetInputLayout());

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshPtr->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshPtr->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//デプスステンシル
		switch (GetDepthStencilState()) {
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
		//サンプラー
		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
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
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		for (auto& m : MatVec) {
			cb1.Emissive = m.m_Emissive;
			cb1.Diffuse = m.m_Diffuse;
			//コンスタントバッファの更新
			ID3D11Buffer* pConstantBuffer = CBPNTBone::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
			if (GetRasterizerState() == RasterizerState::Wireframe) {
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				//描画
				pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (GameObjectPtr->GetAlphaActive()) {
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					switch (GetBlendState()) {
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
					switch (GetRasterizerState()) {
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
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}
		}
		Dev->InitializeStates();
*/

	}
	void PNTBoneModelDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}


	//--------------------------------------------------------------------------------------
	///	汎用描画用（PNTStatic固定）
	//--------------------------------------------------------------------------------------
	GenericDraw::GenericDraw() :
		ObjectInterface() {}
	GenericDraw::~GenericDraw() {}
	void GenericDraw::OnCreate() {}

	void GenericDraw::DrawWireFrame(const shared_ptr<GameObject>& GameObj,
		const shared_ptr<MeshResource>& Mesh) {
		Draw3DPrim::PCWireFrameDraw(GameObj, Mesh,
			Color4(0,0,0,0), Color4(1, 1, 1, 1),Matrix4X4());
	}

}
//end basecross
