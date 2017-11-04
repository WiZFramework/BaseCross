/*!
@file DrawComponents.cpp
@brief 描画コンポーネント実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"
#include "DrawComponents.h"

namespace basecross {

	//Shadowmap
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmap, App::GetApp()->GetShadersPath() + L"VSShadowmap.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmapBone, App::GetApp()->GetShadersPath() + L"VSShadowmapBone.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmapBoneWithTan, App::GetApp()->GetShadersPath() + L"VSShadowmapBoneWithTan.cso")

	//スプライト用
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSprite)

	//PCSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCSprite, App::GetApp()->GetShadersPath() + L"VSPCSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCSprite, App::GetApp()->GetShadersPath() + L"PSPCSprite.cso")

	//PTSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTSprite, App::GetApp()->GetShadersPath() + L"VSPTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTSprite, App::GetApp()->GetShadersPath() + L"PSPTSprite.cso")

	//PCTSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTSprite, App::GetApp()->GetShadersPath() + L"VSPCTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTSprite, App::GetApp()->GetShadersPath() + L"PSPCTSprite.cso")


	//CBSimple
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSimple)
	//PCStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCStatic, App::GetApp()->GetShadersPath() + L"VSPCStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCStatic, App::GetApp()->GetShadersPath() + L"PSPCStatic.cso")
	//PTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTStatic, App::GetApp()->GetShadersPath() + L"VSPTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTStatic, App::GetApp()->GetShadersPath() + L"PSPTStatic.cso")
	//PCTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTStatic, App::GetApp()->GetShadersPath() + L"VSPCTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTStatic, App::GetApp()->GetShadersPath() + L"PSPCTStatic.cso")
	//PNTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStatic, App::GetApp()->GetShadersPath() + L"VSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStatic, App::GetApp()->GetShadersPath() + L"PSPNTStatic.cso")
	//PNTStaticShadow
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"VSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow2, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow2.cso")
	//PNTBone
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBone, App::GetApp()->GetShadersPath() + L"VSPNTBone.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBoneShadow, App::GetApp()->GetShadersPath() + L"VSPNTBoneShadow.cso")

	//PCStaticInstance
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCStaticInstance, App::GetApp()->GetShadersPath() + L"VSPCStaticInstance.cso")
	//PTStaticInstance
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTStaticInstance, App::GetApp()->GetShadersPath() + L"VSPTStaticInstance.cso")
	//PCTStaticInstance
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTStaticInstance, App::GetApp()->GetShadersPath() + L"VSPCTStaticInstance.cso")
	//PNTStaticInstance
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticInstance, App::GetApp()->GetShadersPath() + L"VSPNTStaticInstance.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticInstanceShadow, App::GetApp()->GetShadersPath() + L"VSPNTStaticInstanceShadow.cso")



	//--------------------------------------------------------------------------------------
	//	struct DrawComponent::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct DrawComponent::Impl {
		BlendState m_BlendState;
		DepthStencilState m_DepthStencilState;
		RasterizerState m_RasterizerState;
		SamplerState m_SamplerState;
		bsm::Mat4x4 m_MeshToTransformMatrix;

		Impl() :
			m_MeshToTransformMatrix()
		{}
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



	const bsm::Mat4x4& DrawComponent::GetMeshToTransformMatrix() const {
		return pImpl->m_MeshToTransformMatrix;
	}
	void DrawComponent::SetMeshToTransformMatrix(const bsm::Mat4x4& Mat) {
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

		//メッシュリソース
		weak_ptr<MeshResource> m_MeshResource;
		//マルチメッシュリソース
		weak_ptr<MultiMeshResource> m_MultiMeshResource;


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
		auto shptr = pImpl->m_MeshResource.lock();
		if (shptr) {
			return shptr;
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

	shared_ptr<MultiMeshResource> Shadowmap::GetMultiMeshResource(bool ExceptionActive) const {
		auto shptr = pImpl->m_MultiMeshResource.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"メッシュリソースが見つかりません",
					L"if (pImpl->m_MultiMeshResource.expired())",
					L"ShadowmapComp::GetMultiMeshResource()"
				);
			}
		}
		return nullptr;
	}
	void Shadowmap::SetMultiMeshResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"メッシュキーが空白です",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMultiMeshResource()"
				);
			}
			pImpl->m_MultiMeshResource = App::GetApp()->GetResource<MultiMeshResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Shadowmap::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_MultiMeshResource = MeshResourcePtr;
	}

	void Shadowmap::DrawSigle(const MeshPrimData& data) {
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = Dev->GetRenderState();

		//行列の定義
		bsm::Mat4x4 World, LightView, LightProj;
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();

		//ライトの取得
		auto StageLight = PtrGameObject->OnGetDrawLight();
		//位置の取得
		auto Pos = PtrTrans->GetWorldMatrix().transInMatrix();
		bsm::Vec3 PosSpan = StageLight.m_Directional;
		PosSpan *= 0.1f;
		Pos += PosSpan;
		//ワールド行列の決定
		World.affineTransformation(
			PtrTrans->GetScale(),			//スケーリング
			PtrTrans->GetPivot(),		//回転の中心（重心）
			PtrTrans->GetQuaternion(),				//回転角度
			Pos				//位置
		);
		bsm::Mat4x4 RealWorldMatrix;
		//ワールド行列の決定
		if (data.m_UseMeshToTransformMatrix) {
			RealWorldMatrix = data.m_MeshToTransformMatrix * World;
		}
		else {
			RealWorldMatrix = GetMeshToTransformMatrix() * World;
		}
		//ビュー行列の決定
		auto StageView = PtrStage->GetView();
		bsm::Vec3 LightDir = -1.0 * StageLight.m_Directional;
		bsm::Vec3 LightAt = StageView->GetTargetCamera()->GetAt();
		bsm::Vec3 LightEye = LightAt + (LightDir * GetLightHeight());
		//ライトのビューと射影を計算
		LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
		LightProj = XMMatrixOrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

		ShadowConstants Cb;
		Cb.mWorld = bsm::transpose(RealWorldMatrix);
		Cb.mView = bsm::transpose(LightView);
		Cb.mProj = bsm::transpose(LightProj);

		bool IsSkin = false;
		bool IsSkinStride = false;
		if (data.m_IsSkining) {
			auto DrawCompPtr = GetGameObject()->GetDynamicComponent<DrawComponent>(false);
			if (auto* pLocalBoneVec = DrawCompPtr->GetVecLocalBonesPtr()) {
				if (pLocalBoneVec) {
					//ボーンの設定
					size_t BoneSz = pLocalBoneVec->size();
					UINT cb_count = 0;
					for (size_t b = 0; b < BoneSz; b++) {
						bsm::Mat4x4 mat = pLocalBoneVec->at(b);
						mat.transpose();
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
			//ストライドとオフセット
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			if (stride == sizeof(VertexPositionNormalTangentTextureSkinning)) {
				//頂点シェーダーのセット
				pID3D11DeviceContext->VSSetShader(VSShadowmapBoneWithTan::GetPtr()->GetShader(), nullptr, 0);
				//インプットレイアウトのセット
				pID3D11DeviceContext->IASetInputLayout(VSShadowmapBoneWithTan::GetPtr()->GetInputLayout());
			}
			else {
				//頂点シェーダーのセット
				pID3D11DeviceContext->VSSetShader(VSShadowmapBone::GetPtr()->GetShader(), nullptr, 0);
				//インプットレイアウトのセット
				pID3D11DeviceContext->IASetInputLayout(VSShadowmapBone::GetPtr()->GetInputLayout());
			}
			//頂点バッファをセット
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
		}
		else {
			//インプットレイアウトのセット
			pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
			//ストライドとオフセット
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			//頂点バッファをセット
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//頂点シェーダーのセット
			pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
		}
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
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
		pID3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
		//後始末は呼び出し側で行う
//		Dev->InitializeStates();
	}


	void Shadowmap::OnDraw() {
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource(false);
		if (PtrMeshResource) {
			DrawSigle(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource(false);
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawSigle(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}



	//--------------------------------------------------------------------------------------
	//ParticleDraw準備のための構造体
	//--------------------------------------------------------------------------------------
	struct DrawParticleSprite {
		//パーティクルのカメラまでの距離
		float m_ToCaneraLength;
		//ワールド行列
		bsm::Mat4x4 m_WorldMatrix;
		//テクスチャ
		shared_ptr<TextureResource> m_TextureRes;
		//Diffuseカラー
		bsm::Col4 m_Diffuse;
		DrawParticleSprite() :
			m_ToCaneraLength(0),
			m_Diffuse(1,1,1,1)

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
		//加算処理するかどうか
		bool m_Addtype;
		Impl(size_t MaxInstance,bool AddType) :
			m_MaxInstance(MaxInstance),
			m_Addtype(AddType)
		{}
		~Impl() {}
		//頂点バッファの作成
		void CreateParticleBuffers();
	};


	void PCTParticleDraw::Impl::CreateParticleBuffers() {
		try {
			float HelfSize = 0.5f;
			bsm::Vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
			//頂点配列
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),  col,bsm::Vec2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), col, bsm::Vec2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0),  col,bsm::Vec2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0),  col, bsm::Vec2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成
			m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);

			//インスタンス行列バッファの作成
			//Max値で作成する
			vector<bsm::Mat4x4> matrices(m_MaxInstance, bsm::Mat4x4());
			MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);

		}
		catch (...) {
			throw;
		}
	}




	//--------------------------------------------------------------------------------------
	///	PCTParticle描画コンポーネント(パーティクル描画)
	//--------------------------------------------------------------------------------------
	PCTParticleDraw::PCTParticleDraw(const shared_ptr<GameObject>& GameObjectPtr, size_t MaxInstance, bool AddType) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl(MaxInstance, AddType))
	{}

	PCTParticleDraw::~PCTParticleDraw() {}

	void PCTParticleDraw::AddParticle(float ToCaneraLength, const bsm::Mat4x4& WorldMatrix,
		const shared_ptr<TextureResource>& TextureRes,const bsm::Col4& Diffuse) {
		DrawParticleSprite Item;
		Item.m_ToCaneraLength = ToCaneraLength;
		Item.m_WorldMatrix = WorldMatrix;
		Item.m_TextureRes = TextureRes;
		Item.m_Diffuse = Diffuse;
		pImpl->m_DrawParticleSpriteVec.push_back(Item);
	}

	void PCTParticleDraw::OnCreate() {
		pImpl->CreateParticleBuffers();
	}

	struct InstanceDrawStr {
		size_t Start;
		size_t Count;
		shared_ptr<TextureResource> Tex;
		bsm::Col4 m_Diffuse;
		InstanceDrawStr(size_t s, size_t c, shared_ptr<TextureResource> t,
			const bsm::Col4& Diffuse) :
			Start(s), Count(c), Tex(t), m_Diffuse(Diffuse){}
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
		bsm::Col4 NowDiffuse  = bsm::Col4(1,1,1,1);
		for (size_t i = 0; i < pImpl->m_DrawParticleSpriteVec.size(); i++) {
			if (pImpl->m_DrawParticleSpriteVec[i].m_TextureRes != NowTexRes || 
				pImpl->m_DrawParticleSpriteVec[i].m_Diffuse != NowDiffuse) {
				m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));
				NowStartIndex = i;
				NowDrawCount = 0;
				NowTexRes = pImpl->m_DrawParticleSpriteVec[i].m_TextureRes;
				NowDiffuse = pImpl->m_DrawParticleSpriteVec[i].m_Diffuse;
			}
			else {
				NowDrawCount++;
			}
		}
		m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));


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
		auto* matrices = (bsm::Mat4x4*)mappedBuffer.pData;
		bsm::Mat4x4 World;
		for (size_t i = 0; i < pImpl->m_DrawParticleSpriteVec.size(); i++) {
			World = pImpl->m_DrawParticleSpriteVec[i].m_WorldMatrix;
			//転置する
			World.transpose();
			matrices[i] = World;
		}
		//アンマップ
		pD3D11DeviceContext->Unmap(pImpl->m_MatrixBuffer.Get(), 0);
		//ストライドとオフセット
		//形状の頂点バッファと行列バッファを設定
		UINT stride[2] = { sizeof(VertexPositionColorTexture), sizeof(bsm::Mat4x4) };
		UINT offset[2] = { 0, 0 };

		ID3D11Buffer* pBuf[2] = { pImpl->m_MeshResource->GetVertexBuffer().Get(), pImpl->m_MatrixBuffer.Get() };
		pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(pImpl->m_MeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCTStaticInstance::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTStaticInstance::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (pImpl->m_Addtype) {
			//加算処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
		}
		else {
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthRead(), 0);
		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//ラスタライザステート（表面描画）
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//ビュー行列の決定
		bsm::Mat4x4 View, Proj;
		//ビュー行列の決定
		auto StageView = PtrStage->GetView();
		View = StageView->GetTargetCamera()->GetViewMatrix();
		//転置する
		View.transpose();
		//射影行列の決定
		Proj = StageView->GetTargetCamera()->GetProjMatrix();
		//転置する
		Proj.transpose();


		//コンスタントバッファの準備
		SimpleConstants sb;
		sb.World = bsm::Mat4x4();	//ワールド行列はダミー
		sb.View = View;
		sb.Projection = Proj;
		//エミッシブ加算は行わない。
		sb.Emissive = bsm::Col4(0, 0, 0, 0);
		sb.ActiveFlg.x = 1;

		for (auto& v : m_InstancVec) {
			sb.Diffuse = v.m_Diffuse;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.Tex->GetShaderResourceView().GetAddressOf());
			//描画
			pD3D11DeviceContext->DrawIndexedInstanced(pImpl->m_MeshResource->GetNumIndicis(), v.Count, 0, 0, v.Start);
		}
		//後始末
		Dev->InitializeStates();
		pImpl->m_DrawParticleSpriteVec.clear();
	}

	//--------------------------------------------------------------------------------------
	//	SpriteBaseDraw::Impl
	//--------------------------------------------------------------------------------------
	struct SpriteBaseDraw::Impl {
		//メッシュ
		shared_ptr<MeshResource> m_SpriteMesh;
		///テクスチャリソース
		weak_ptr<TextureResource> m_TextureResource;
		//エミッシブ色
		bsm::Col4 m_Emissive;
		//デフューズ色
		bsm::Col4 m_Diffuse;
		Impl() :
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		{}
	};

	SpriteBaseDraw::SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr):
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}

	SpriteBaseDraw::~SpriteBaseDraw() {}

	void SpriteBaseDraw::SetConstants(SpriteConstants& SpCb) {
		//行列の取得
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		bsm::Mat4x4 World, Proj;
		World = PtrTrans->Get2DWorldMatrix();
		//射影行列の決定
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//行列の合成
		World *= Proj;
		//エミッシブ
		SpCb.Emissive = GetEmissive();
		//デフィーズはすべて通す
		SpCb.Diffuse = GetDiffuse();
		//行列の設定
		SpCb.World = World;
	}

	shared_ptr<MeshResource> SpriteBaseDraw::GetMeshResource() const {
		return pImpl->m_SpriteMesh;
	}
	void SpriteBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SpriteMesh = MeshRes;
	}

	void SpriteBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;

	}

	void SpriteBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> SpriteBaseDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = pImpl->m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}



	bsm::Col4 SpriteBaseDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void SpriteBaseDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_Emissive = col;
	}

	bsm::Col4 SpriteBaseDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void SpriteBaseDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_Diffuse = col;
	}

	//--------------------------------------------------------------------------------------
	///	PCSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr):
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices) : 
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(Vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	PCSpriteDraw::~PCSpriteDraw(){}

	void PCSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionColor> Vertices = {
				{ VertexPositionColor(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
				{ VertexPositionColor(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
				{ VertexPositionColor(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
				{ VertexPositionColor(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			CreateMesh(Vertices, indices);
		}
	}

	void PCSpriteDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::CullNone);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//シェーダの設定
			DrawSprite<VSPCSprite, PSPCSprite>(PtrMeshResource->GetMashData());
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	PTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(Vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	PTSpriteDraw::~PTSpriteDraw() {}

	void PTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionTexture> Vertices = {
				{ VertexPositionTexture(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Vec2(0.0f, 0.0f)) },
				{ VertexPositionTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Vec2(1.0f, 0.0f)) },
				{ VertexPositionTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Vec2(0.0f, 1.0f)) },
				{ VertexPositionTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Vec2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			CreateMesh(Vertices, indices);
		}
	}

	void PTSpriteDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::CullNone);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//シェーダの設定
			DrawSprite<VSPTSprite, PSPTSprite>(PtrMeshResource->GetMashData());
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PCTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(Vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	PCTSpriteDraw::~PCTSpriteDraw() {}

	void PCTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionColorTexture> Vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成（変更できる）
			CreateMesh(Vertices, indices);
		}
	}

	void PCTSpriteDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::CullNone);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//シェーダの設定
			DrawSprite<VSPCTSprite, PSPCTSprite>(PtrMeshResource->GetMashData());
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}


	//--------------------------------------------------------------------------------------
	///	汎用描画用
	//--------------------------------------------------------------------------------------
	GenericDraw::GenericDraw() :
		ObjectInterface() {}
	GenericDraw::~GenericDraw() {}
	void GenericDraw::OnCreate() {}

	void GenericDraw::PCWireFrameDraw(const shared_ptr<GameObject>& GameObjectPtr, const shared_ptr<MeshResource>& MeshRes,
		const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//行列の定義
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		//行列の定義
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
		//転置する
		World.transpose();
		//カメラを得る
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		//コンスタントバッファの準備
		SimpleConstants sb;
		sb.World = World;
		sb.View = ViewMat;
		sb.Projection = ProjMat;
		//エミッシブ
		sb.Emissive = Emissive;
		//デフィーズはすべて通す
		sb.Diffuse = Diffuse;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

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
		ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
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


	void GenericDraw::DrawWireFrame(const shared_ptr<GameObject>& GameObj,
		const shared_ptr<MeshResource>& Mesh, const bsm::Mat4x4& MeshToTransformMatrix) {

		PCWireFrameDraw(GameObj, Mesh,
			bsm::Col4(0,0,0,0), bsm::Col4(1, 1, 1, 1), MeshToTransformMatrix);
	}

	//--------------------------------------------------------------------------------------
	///	Simple描画に使用する構造体(影対応)
	//--------------------------------------------------------------------------------------
	struct SmDrawObject :public DrawObjectBase {
		//頂点変更する場合のメッシュ（オリジナル）
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//オリジナルメッシュを使うかどうか
		bool m_UseOriginalMeshResource;
		///テクスチャリソース
		weak_ptr<TextureResource> m_TextureResource;
		///エミッシブ色
		bsm::Col4 m_Emissive;
		/// デフューズ色
		bsm::Col4 m_Diffuse;
		/// スペキュラー色
		bsm::Col4 m_Specular;
		///影を投影するかどうか
		bool m_OwnShadowActive;
		//モデルに入っているDiffuseを使うかどうか
		bool m_ModelDiffusePriority;
		//モデルに入っているEmissiveを使うかどうか
		bool m_ModelEmissivePriority;
		//モデルに入っているテクスチャを使うかどうか
		bool m_ModelTextureEnabled;
		///Instance描画用
		///Instance最大値
		size_t m_MaxInstance;
		/// 行列用の頂点バッファ
		ComPtr<ID3D11Buffer> m_MatrixBuffer;
		///行列の配列
		vector<bsm::Mat4x4> m_MatrixVec;
		//行列を自動クリアするかどうか
		bool m_AutoClearMatrixVec;
		SmDrawObject() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Specular(0, 0, 0, 0),
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(false),
			m_ModelEmissivePriority(false),
			m_ModelTextureEnabled(true),
			m_MaxInstance(2000),
			m_AutoClearMatrixVec(false)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	SmBaseDraw::Impl
	//--------------------------------------------------------------------------------------
	struct SmBaseDraw::Impl {
		SmDrawObject m_SmDrawObject;
	};

	//--------------------------------------------------------------------------------------
	///	Simple描画オブジェクトの親
	//--------------------------------------------------------------------------------------
	SmBaseDraw::SmBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	SmBaseDraw::~SmBaseDraw() {}

	void SmBaseDraw::SetConstants(SimpleConstants& SmCb, const MeshPrimData& data) {
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		if (data.m_UseMeshToTransformMatrix) {
			World = data.m_MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
		}
		else {
			World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		}
		//転置する
		World.transpose();
		//カメラを得る
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		SmCb.World = World;
		SmCb.View = ViewMat;
		SmCb.Projection = ProjMat;
		//エミッシブ
		SmCb.Emissive = GetEmissive();
		//デフィーズ
		SmCb.Diffuse = GetDiffuse();
		//スペキュラー
		SmCb.Specular = GetSpecular();
		//ライティング
		auto StageLight = GetGameObject()->OnGetDrawLight();
		SmCb.LightDir = StageLight.m_Directional;
		SmCb.LightDir.w = 1.0f;
		SmCb.EyePos = CameraPtr->GetEye();
		SmCb.EyePos.w = 1.0f;
		//影用
		if (GetOwnShadowActive()) {
			bsm::Vec3 CalcLightDir = -1.0 * StageLight.m_Directional;
			bsm::Vec3 LightAt = CameraPtr->GetAt();
			bsm::Vec3 LightEye = CalcLightDir;
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			SmCb.LightPos = LightEye;
			SmCb.LightPos.w = 1.0f;
			bsm::Mat4x4 LightView, LightProj;
			//ライトのビューと射影を計算
			LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			SmCb.LightView = bsm::transpose(LightView);
			SmCb.LightProjection = bsm::transpose(LightProj);
		}
		//ボーンの設定
		size_t BoneSz = pImpl->m_SmDrawObject.m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				bsm::Mat4x4 mat = pImpl->m_SmDrawObject.m_LocalBonesMatrix[b];
				mat.transpose();
				SmCb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
				SmCb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				SmCb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
	}

	//行列バッファの作成
	void SmBaseDraw::CreateMatrixBuffer() {
		//インスタンス行列バッファの作成
		//Max値で作成する
		vector<bsm::Mat4x4> matrices(pImpl->m_SmDrawObject.m_MaxInstance);
		for (auto& m : matrices) {
			m = bsm::Mat4x4();
		}
		MeshResource::CreateDynamicVertexBuffer(pImpl->m_SmDrawObject.m_MatrixBuffer, matrices);
	}

	//行列バッファのマップ
	void SmBaseDraw::MapMatrixBuffer() {
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//インスタンスバッファにマップ
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//行列のマップ
		if (FAILED(pID3D11DeviceContext->Map(pImpl->m_SmDrawObject.m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"行列のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"SmBaseDraw::MapMatrixBuffer()"
			);
		}
		//行列の変更
		auto* matrices = (bsm::Mat4x4*)mappedBuffer.pData;
		bsm::Mat4x4 World;
		for (size_t i = 0; i < pImpl->m_SmDrawObject.m_MatrixVec.size(); i++) {
			//ワールド行列
			World = pImpl->m_SmDrawObject.m_MatrixVec[i];
			//転置する
			World.transpose();
			matrices[i] = World;
		}
		//アンマップ
		pID3D11DeviceContext->Unmap(pImpl->m_SmDrawObject.m_MatrixBuffer.Get(), 0);

	}



	bool SmBaseDraw::IsOriginalMeshUse() const {
		return pImpl->m_SmDrawObject.m_UseOriginalMeshResource;

	}

	void SmBaseDraw::SetOriginalMeshUse(bool b) {
		pImpl->m_SmDrawObject.m_UseOriginalMeshResource = b;
	}

	shared_ptr<MeshResource> SmBaseDraw::GetOriginalMeshResource() const {
		return pImpl->m_SmDrawObject.m_OriginalMeshResource;

	}

	void SmBaseDraw::SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SmDrawObject.m_OriginalMeshResource = MeshRes;

	}

	shared_ptr<MeshResource> SmBaseDraw::GetMeshResource() const {
		if (IsOriginalMeshUse()) {
			return pImpl->m_SmDrawObject.m_OriginalMeshResource;
		}
		else {
			auto shptr = pImpl->m_SmDrawObject.m_MeshResource.lock();
			if (shptr) {
				return shptr;
			}
		}
		return nullptr;
	}

	void SmBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SmDrawObject.m_MeshResource = MeshRes;
	}

	void SmBaseDraw::SetMeshResource(const wstring& MeshKey) {
		this->SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}

	void SmBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_SmDrawObject.m_TextureResource = TextureRes;

	}

	void SmBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> SmBaseDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = pImpl->m_SmDrawObject.m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;

	}

	shared_ptr<MultiMeshResource> SmBaseDraw::GetMultiMeshResource() const {
		//なければnullを返す
		auto shptr = pImpl->m_SmDrawObject.m_MultiMeshResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;

	}

	void SmBaseDraw::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_SmDrawObject.m_MultiMeshResource = MeshResourcePtr;

	}

	void SmBaseDraw::SetMultiMeshResource(const wstring& ResKey) {
		this->SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(ResKey));

	}

	bsm::Col4 SmBaseDraw::GetEmissive() const {
		return pImpl->m_SmDrawObject.m_Emissive;

	}

	void SmBaseDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_SmDrawObject.m_Emissive = col;
	}

	bsm::Col4 SmBaseDraw::GetDiffuse() const {
		return pImpl->m_SmDrawObject.m_Diffuse;

	}

	void SmBaseDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_SmDrawObject.m_Diffuse = col;

	}

	bsm::Col4 SmBaseDraw::GetSpecular() const {
		return pImpl->m_SmDrawObject.m_Specular;

	}

	void SmBaseDraw::SetSpecular(const bsm::Col4& col) {
		pImpl->m_SmDrawObject.m_Specular = col;
	}

	bool SmBaseDraw::GetOwnShadowActive() const {
		return pImpl->m_SmDrawObject.m_OwnShadowActive;

	}

	bool SmBaseDraw::IsOwnShadowActive() const {
		return pImpl->m_SmDrawObject.m_OwnShadowActive;

	}

	void SmBaseDraw::SetOwnShadowActive(bool b) {
		pImpl->m_SmDrawObject.m_OwnShadowActive = b;
	}

	bool SmBaseDraw::GetModelDiffusePriority() const {
		return pImpl->m_SmDrawObject.m_ModelDiffusePriority;

	}

	bool SmBaseDraw::IsModelDiffusePriority() const {
		return pImpl->m_SmDrawObject.m_ModelDiffusePriority;

	}

	void SmBaseDraw::SetModelDiffusePriority(bool b) {
		pImpl->m_SmDrawObject.m_ModelDiffusePriority = b;

	}

	bool SmBaseDraw::GetModelEmissivePriority() const {
		return pImpl->m_SmDrawObject.m_ModelEmissivePriority;

	}

	bool SmBaseDraw::IsModelEmissivePriority() const {
		return pImpl->m_SmDrawObject.m_ModelEmissivePriority;

	}

	void SmBaseDraw::SetModelEmissivePriority(bool b) {
		pImpl->m_SmDrawObject.m_ModelEmissivePriority = b;
	}

	bool SmBaseDraw::GetModelTextureEnabled() const {
		return pImpl->m_SmDrawObject.m_ModelTextureEnabled;

	}

	bool SmBaseDraw::IsModelTextureEnabled() const {
		return pImpl->m_SmDrawObject.m_ModelTextureEnabled;

	}

	void SmBaseDraw::SetModelTextureEnabled(bool b) {
		pImpl->m_SmDrawObject.m_ModelTextureEnabled = b;

	}

	void SmBaseDraw::BoneInit() {
		pImpl->m_SmDrawObject.BoneInit();

	}

	void SmBaseDraw::InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out) {
		pImpl->m_SmDrawObject.InterpolationMatrix(m1, m2, t, out);

	}

	void SmBaseDraw::AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
		float SamplesParSecond) {
		pImpl->m_SmDrawObject.AddAnimation(Name, StartSample, SampleLength, Loop, SamplesParSecond);

	}

	void SmBaseDraw::ChangeCurrentAnimation(const wstring& AnemationName, float StartTime) {
		pImpl->m_SmDrawObject.ChangeCurrentAnimation(AnemationName, StartTime);

	}

	const wstring& SmBaseDraw::GetCurrentAnimation() const {
		return pImpl->m_SmDrawObject.m_CurrentAnimeName;

	}

	bool SmBaseDraw::UpdateAnimation(float ElapsedTime) {
		return pImpl->m_SmDrawObject.UpdateAnimation(ElapsedTime);

	}

	const vector< bsm::Mat4x4 >* SmBaseDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_SmDrawObject.m_LocalBonesMatrix;
	}

	size_t SmBaseDraw::GetMaxInstance() const {
		return pImpl->m_SmDrawObject.m_MaxInstance;
	}

	void SmBaseDraw::ResizeMaxInstance(size_t NewSize) {
		pImpl->m_SmDrawObject.m_MaxInstance = NewSize;
		CreateMatrixBuffer();
	}

	void SmBaseDraw::AddMatrix(const bsm::Mat4x4& NewMat) {
		if (pImpl->m_SmDrawObject.m_MatrixVec.size() >= GetMaxInstance()) {
			throw BaseException(
				L"インスタンス上限を超えてます",
				L"if (pImpl->m_MatrixVec.size() >= GetMaxInstance())",
				L"SmBaseDraw::AddMatrix()"
			);
		}
		pImpl->m_SmDrawObject.m_MatrixVec.push_back(NewMat);
	}

	void SmBaseDraw::UpdateMultiMatrix(const vector<bsm::Mat4x4>& NewMatVec) {
		pImpl->m_SmDrawObject.m_MatrixVec.resize(NewMatVec.size());
		pImpl->m_SmDrawObject.m_MatrixVec = NewMatVec;
	}

	vector<bsm::Mat4x4>& SmBaseDraw::GetMatrixVec() const {
		return pImpl->m_SmDrawObject.m_MatrixVec;
	}

	void SmBaseDraw::ClearMatrixVec() {
		pImpl->m_SmDrawObject.m_MatrixVec.clear();
	}

	bool SmBaseDraw::IsAutoClearMatrixVec() const {
		return pImpl->m_SmDrawObject.m_AutoClearMatrixVec;
	}
	void SmBaseDraw::SetAutoClearMatrixVec(bool b) {
		pImpl->m_SmDrawObject.m_AutoClearMatrixVec = b;
	}

	ComPtr<ID3D11Buffer>& SmBaseDraw::GetMatrixBuffer() const {
		return pImpl->m_SmDrawObject.m_MatrixBuffer;
	}

	//--------------------------------------------------------------------------------------
	///	PCStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCStaticDraw::PCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr):
		SmBaseDraw(GameObjectPtr)
	{}

	PCStaticDraw::~PCStaticDraw(){}

	void PCStaticDraw::OnCreate(){}

	void PCStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<VSPCStatic, PSPCStatic>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawStatic<VSPCStatic, PSPCStatic>(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTStaticDraw::PTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PTStaticDraw::~PTStaticDraw() {}

	void PTStaticDraw::OnCreate() {}

	void PTStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<VSPTStatic, PSPTStatic>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawStatic<VSPTStatic, PSPTStatic>(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PCTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTStaticDraw::PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PCTStaticDraw::~PCTStaticDraw() {}

	void PCTStaticDraw::OnCreate() {}

	void PCTStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<VSPCTStatic, PSPCTStatic>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawStatic<VSPCTStatic, PSPCTStatic>(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::OnCreate() {
		//ライティングのみだと極端になるので調整
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void PNTStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			if (GetOwnShadowActive()) {
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow2>(PtrMeshResource->GetMashData());
				}
				else {
					DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
				}
			}
			else {
				DrawStatic<VSPNTStatic, PSPNTStatic>(PtrMeshResource->GetMashData());
			}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetOwnShadowActive()) {
					if (GetGameObject()->GetComponent<Shadowmap>(false)) {
						DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow2>(vec[i]);
					}
					else {
						DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow>(vec[i]);
					}
				}
				else {
					DrawStatic<VSPNTStatic, PSPNTStatic>(vec[i]);
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PNTStaticModel描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticModelDraw::PNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PNTStaticModelDraw::~PNTStaticModelDraw() {}

	void PNTStaticModelDraw::OnCreate() {
		//ライティングのみだと極端になるので調整
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void PNTStaticModelDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			if (GetOwnShadowActive()) {
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					DrawModel<VSPNTStaticShadow, PSPNTStaticShadow2>(PtrMeshResource->GetMashData());
				}
				else {
					DrawModel<VSPNTStaticShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
				}
			}
			else {
				DrawModel<VSPNTStatic, PSPNTStatic>(PtrMeshResource->GetMashData());
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PNTBoneModel描画コンポーネント（ボーンモデル描画用）
	//--------------------------------------------------------------------------------------
	PNTBoneModelDraw::PNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}
	PNTBoneModelDraw::~PNTBoneModelDraw() {}

	void PNTBoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		SmBaseDraw::SetMeshResource(MeshRes);
		BoneInit();
	}

	void PNTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		PNTBoneModelDraw::SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}


	void PNTBoneModelDraw::OnCreate() {
		//ライティングのみだと極端になるので調整
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void PNTBoneModelDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//シェーダの設定
			if (IsOwnShadowActive()) {
				//影付き
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					//シャドウマップがあれば自己影防止用のピクセルシェーダ
					DrawModel<VSPNTBoneShadow, PSPNTStaticShadow2>(PtrMeshResource->GetMashData());
				}
				else {
					DrawModel<VSPNTBoneShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
				}
			}
			else {
				DrawModel<VSPNTBone, PSPNTStatic>(PtrMeshResource->GetMashData());
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	PCStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCStaticInstanceDraw::PCStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PCStaticInstanceDraw::~PCStaticInstanceDraw() {}

	void PCStaticInstanceDraw::OnCreate() {
		//行列バッファの作成
		CreateMatrixBuffer();
	}

	void PCStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStaticInstance<VSPCStaticInstance, PSPCStatic>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawStaticInstance<VSPCStaticInstance, PSPCStatic>(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
		//自動行列クリアなら
		if (IsAutoClearMatrixVec()) {
			//行列配列のクリア
			ClearMatrixVec();
		}
	}

	//--------------------------------------------------------------------------------------
	///	PTStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTStaticInstanceDraw::PTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PTStaticInstanceDraw::~PTStaticInstanceDraw() {}

	void PTStaticInstanceDraw::OnCreate() {
		//行列バッファの作成
		CreateMatrixBuffer();
	}

	void PTStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStaticInstance<VSPTStaticInstance, PSPTStatic>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawStaticInstance<VSPTStaticInstance, PSPTStatic>(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
		//自動行列クリアなら
		if (IsAutoClearMatrixVec()) {
			//行列配列のクリア
			ClearMatrixVec();
		}
	}

	//--------------------------------------------------------------------------------------
	///	PCTStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTStaticInstanceDraw::PCTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PCTStaticInstanceDraw::~PCTStaticInstanceDraw() {}

	void PCTStaticInstanceDraw::OnCreate() {
		//行列バッファの作成
		CreateMatrixBuffer();
	}

	void PCTStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStaticInstance<VSPCTStaticInstance, PSPCTStatic>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawStaticInstance<VSPCTStaticInstance, PSPCTStatic>(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
		//自動行列クリアなら
		if (IsAutoClearMatrixVec()) {
			//行列配列のクリア
			ClearMatrixVec();
		}
	}

	//--------------------------------------------------------------------------------------
	///	PNTStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticInstanceDraw::PNTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PNTStaticInstanceDraw::~PNTStaticInstanceDraw() {}

	void PNTStaticInstanceDraw::OnCreate() {
		//ライティングのみだと極端になるので調整
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
		//行列バッファの作成
		CreateMatrixBuffer();
	}

	void PNTStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			if (GetOwnShadowActive()) {
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					DrawStaticInstance<VSPNTStaticInstanceShadow, PSPNTStaticShadow2>(PtrMeshResource->GetMashData());
				}
				else {
					DrawStaticInstance<VSPNTStaticInstanceShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
				}
			}
			else {
				DrawStaticInstance<VSPNTStaticInstance, PSPNTStatic>(PtrMeshResource->GetMashData());
			}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetOwnShadowActive()) {
					if (GetGameObject()->GetComponent<Shadowmap>(false)) {
						DrawStaticInstance<VSPNTStaticInstanceShadow, PSPNTStaticShadow2>(vec[i]);
					}
					else {
						DrawStaticInstance<VSPNTStaticInstanceShadow, PSPNTStaticShadow>(vec[i]);
					}
				}
				else {
					DrawStaticInstance<VSPNTStaticInstance, PSPNTStatic>(vec[i]);
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
		//自動行列クリアなら
		if (IsAutoClearMatrixVec()) {
			//行列配列のクリア
			ClearMatrixVec();
		}

	}


}
//end basecross

