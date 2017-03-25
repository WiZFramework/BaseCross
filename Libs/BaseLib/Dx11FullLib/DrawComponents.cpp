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
		Matrix4X4 m_MeshToTransformMatrix;

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
		auto PtrMeshResource = GetMeshResource(false);

		if (PtrMeshResource) {


			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//ステータスのポインタ
			auto RenderStatePtr = Dev->GetRenderState();

			//行列の定義
			Matrix4X4 World, LightView, LightProj;
			//行列の定義
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();

			//ライトの取得
			auto StageLight = PtrGameObject->OnGetDrawLight();
			//位置の取得
			auto Pos = PtrTrans->GetWorldMatrix().PosInMatrixSt();
			Vector3 PosSpan = StageLight.m_Directional;
			PosSpan *= 0.1f;
			Pos += PosSpan;
			//ワールド行列の決定
			World.AffineTransformation(
				PtrTrans->GetScale(),			//スケーリング
				PtrTrans->GetPivot(),		//回転の中心（重心）
				PtrTrans->GetQuaternion(),				//回転角度
				Pos				//位置
			);
			Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * World;
			//ビュー行列の決定
			auto StageView = PtrStage->GetView();
			Vector3 LightDir = -1.0 * StageLight.m_Directional;
			Vector3 LightAt = StageView->GetTargetCamera()->GetAt();
			Vector3 LightEye = LightAt + (LightDir * GetLightHeight());
			//ライトのビューと射影を計算
			LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
			LightProj.OrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

			ShadowConstants Cb;
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
				//ストライドとオフセット
				UINT stride = PtrMeshResource->GetNumStride();
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
				pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			}
			else {
				//インプットレイアウトのセット
				pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
				//ストライドとオフセット
				UINT stride = PtrMeshResource->GetNumStride();
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
		//Diffuseカラー
		Color4 m_Diffuse;
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
	PCTParticleDraw::PCTParticleDraw(const shared_ptr<GameObject>& GameObjectPtr, size_t MaxInstance, bool AddType) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl(MaxInstance, AddType))
	{}

	PCTParticleDraw::~PCTParticleDraw() {}

	void PCTParticleDraw::AddParticle(float ToCaneraLength, const Matrix4X4& WorldMatrix,
		const shared_ptr<TextureResource>& TextureRes,const Color4& Diffuse) {
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
		Color4 m_Diffuse;
		InstanceDrawStr(size_t s, size_t c, shared_ptr<TextureResource> t,
			const Color4& Diffuse) :
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
		Color4 NowDiffuse  = Color4(1,1,1,1);
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
		SimpleConstants sb;
		sb.World = Matrix4X4();	//ワールド行列はダミー
		sb.View = View;
		sb.Projection = Proj;
		//エミッシブ加算は行わない。
		sb.Emissive = Color4(0, 0, 0, 0);
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
	//	struct TextureDrawInterface::Impl;
	//--------------------------------------------------------------------------------------
	struct TextureDrawInterface::Impl {
		//テクスチャ
		weak_ptr<TextureResource> m_TextureResource;
		Impl()
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

		//行列の取得
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, Proj;
		World = PtrTrans->Get2DWorldMatrix();
		//射影行列の決定
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		SpriteConstants sb;
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

		//デプスステンシルステートは設定に任せる
		SetDeviceDepthStencilState();
		if (GetGameObject()->GetAlphaActive()) {
			//ブレンドステート
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//ラスタライザステート
			pD3D11DeviceContext->RSSetState(RenderState->GetCullNone());
		}
		else {
			//ブレンドステート
			//透明処理しない
			//ブレンドステートは設定に任せる
			SetDeviceBlendState();
			//ラスタライザステートは設定に任せる
			SetDeviceRasterizerState();
		}
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
		//行列の取得
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, Proj;
		World = PtrTrans->Get2DWorldMatrix();
		//射影行列の決定
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		SpriteConstants sb;
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

		//デプスステンシルステートは設定に任せる
		SetDeviceDepthStencilState();
		if (GetGameObject()->GetAlphaActive()) {
			//ブレンドステート
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//ラスタライザステート
			pD3D11DeviceContext->RSSetState(RenderState->GetCullNone());
		}
		else {
			//ブレンドステート
			//透明処理しない
			//ブレンドステートは設定に任せる
			SetDeviceBlendState();
			//ラスタライザステートは設定に任せる
			SetDeviceRasterizerState();
		}
		//テクスチャとサンプラーの設定
		pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
		//サンプラーは設定に任せる
		SetDeviceSamplerState();
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
		//行列の取得
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, Proj;
		World = PtrTrans->Get2DWorldMatrix();
		//射影行列の決定
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		SpriteConstants sb;
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

		//デプスステンシルステートは設定に任せる
		SetDeviceDepthStencilState();
		if (GetGameObject()->GetAlphaActive()) {
			//ブレンドステート
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//ラスタライザステート
			pD3D11DeviceContext->RSSetState(RenderState->GetCullNone());
		}
		else {
			//ブレンドステート
			//透明処理しない
			//ブレンドステートは設定に任せる
			SetDeviceBlendState();
			//ラスタライザステートは設定に任せる
			SetDeviceRasterizerState();
		}

		//テクスチャとサンプラーの設定
		pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
		//サンプラーは設定に任せる
		SetDeviceSamplerState();
		//描画
		pD3D11DeviceContext->DrawIndexed(SpriteMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}


	//--------------------------------------------------------------------------------------
	//	3D用シェーダごと描画構造体
	//--------------------------------------------------------------------------------------
	struct Draw3DPrim {
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
	};


	//--------------------------------------------------------------------------------------
	//	struct StaticBaseDraw::Impl;
	//	用途: StaticBaseDrawイディオム
	//--------------------------------------------------------------------------------------
	struct StaticBaseDraw::Impl {
		//メッシュのweak_ptr
		weak_ptr<MeshResource> m_MeshResource;
		//頂点変更する場合のメッシュ（オリジナル）
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//オリジナルメッシュを使うかどうか
		bool m_UseOriginalMeshResource;
		///エミッシブ色
		Color4 m_Emissive;
		/// デフューズ色
		Color4 m_Diffuse;
		/// スペキュラー色
		Color4 m_Specular;
		Impl() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Specular(0, 0, 0, 0)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	static描画コンポーネントの親(頂点を変更する場合も可)
	//--------------------------------------------------------------------------------------
	StaticBaseDraw::StaticBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	StaticBaseDraw::~StaticBaseDraw() {}

	void StaticBaseDraw::SetConstants(SimpleConstants& Cb, bool shadowUse) {
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		Matrix4X4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		//カメラを得る
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.Transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();
		Cb.World = World;
		Cb.View = ViewMat;
		Cb.Projection = ProjMat;
		//エミッシブ
		Cb.Emissive = GetEmissive();
		//デフィーズ
		Cb.Diffuse = GetDiffuse();
		//スペキュラー
		Cb.Specular = GetSpecular();
		//ライティング
		auto StageLight = GetGameObject()->OnGetDrawLight();
		Cb.LightDir = StageLight.m_Directional;
		Cb.LightDir.w = 1.0f;
		Cb.EyePos = CameraPtr->GetEye();
		Cb.EyePos.w = 1.0f;
		if (shadowUse) {
			Vector3 CalcLightDir = -1.0 * StageLight.m_Directional;
			Vector3 LightAt = CameraPtr->GetAt();
			Vector3 LightEye = CalcLightDir;
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			Cb.LightPos = LightEye;
			Cb.LightPos.w = 1.0f;
			Matrix4X4 LightView, LightProj;
			//ライトのビューと射影を計算
			LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
			LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			Cb.LightView = Matrix4X4EX::Transpose(LightView);
			Cb.LightProjection = Matrix4X4EX::Transpose(LightProj);
		}
	}

	Color4 StaticBaseDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void StaticBaseDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}
	Color4 StaticBaseDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void StaticBaseDraw::SetDiffuse(const Color4& col) {
		pImpl->m_Diffuse = col;
	}

	Color4 StaticBaseDraw::GetSpecular() const {
		return pImpl->m_Specular;
	}
	void StaticBaseDraw::SetSpecular(const Color4& col) {
		pImpl->m_Specular = col;
	}



	bool StaticBaseDraw::IsOriginalMeshUse() const {
		return pImpl->m_UseOriginalMeshResource;
	}

	void StaticBaseDraw::SetOriginalMeshUse(bool b) {
		pImpl->m_UseOriginalMeshResource = b;
	}

	shared_ptr<MeshResource> StaticBaseDraw::GetOriginalMeshResource() const {
		return pImpl->m_OriginalMeshResource;
	}
	void StaticBaseDraw::SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_OriginalMeshResource = MeshRes;
	}
	shared_ptr<MeshResource> StaticBaseDraw::GetMeshResource() const {
		if (IsOriginalMeshUse()) {
			return pImpl->m_OriginalMeshResource;
		}
		else {
			auto shptr = pImpl->m_MeshResource.lock();
			if (shptr) {
				return shptr;
			}
		}
		return nullptr;
	}
	void StaticBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}

	void StaticBaseDraw::SetMeshResource(const wstring& MeshKey) {
		SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}


	//--------------------------------------------------------------------------------------
	//	struct StaticInstanceDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct StaticInstanceDraw::Impl {
		size_t m_MaxInstance;
		///<行列用の頂点バッファ
		ComPtr<ID3D11Buffer> m_MatrixBuffer;
		//行列の配列
		vector<Matrix4X4> m_MatrixVec;
		//行列を自動クリアするかどうか
		bool m_AutoClearMatrixVec;
		Impl() :
			m_MaxInstance(2000),
			m_AutoClearMatrixVec(false)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	インスタンス描画コンポーネントの親
	//--------------------------------------------------------------------------------------
	StaticInstanceDraw::StaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		//ライティングのみだと極端になるので調整
		SetEmissive(Color4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(Color4(0.6f, 0.6f, 0.6f, 1.0f));
		//行列バッファの作成
		CreateMatrixBuffer();
	}

	StaticInstanceDraw::~StaticInstanceDraw() {}

	//行列バッファの作成
	void StaticInstanceDraw::CreateMatrixBuffer() {
		//インスタンス行列バッファの作成
		//Max値で作成する
		vector<Matrix4X4> matrices(pImpl->m_MaxInstance);
		for (auto& m : matrices) {
			m = Matrix4X4();
		}
		MeshResource::CreateDynamicVertexBuffer(pImpl->m_MatrixBuffer, matrices);
	}

	//行列バッファのマップ
	void StaticInstanceDraw::MapMatrixBuffer() {
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//インスタンスバッファにマップ
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//行列のマップ
		if (FAILED(pID3D11DeviceContext->Map(pImpl->m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"行列のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"StaticInstanceDraw::MapMatrixBuffer()"
			);
		}
		//行列の変更
		auto* matrices = (Matrix4X4*)mappedBuffer.pData;
		Matrix4X4 World;
		for (size_t i = 0; i < pImpl->m_MatrixVec.size(); i++) {
			//ワールド行列
			World = pImpl->m_MatrixVec[i];
			//転置する
			World.Transpose();
			matrices[i] = World;
		}
		//アンマップ
		pID3D11DeviceContext->Unmap(pImpl->m_MatrixBuffer.Get(), 0);

	}




	size_t StaticInstanceDraw::GetMaxInstance() const {
		return pImpl->m_MaxInstance;
	}

	void StaticInstanceDraw::ResizeMaxInstance(size_t NewSize) {
		pImpl->m_MaxInstance = NewSize;
		CreateMatrixBuffer();
	}

	void StaticInstanceDraw::AddMatrix(const Matrix4X4& NewMat) {
		if (pImpl->m_MatrixVec.size() >= GetMaxInstance()) {
			throw BaseException(
				L"インスタンス上限を超えてます",
				L"if (pImpl->m_MatrixVec.size() >= GetMaxInstance())",
				L"StaticInstanceDraw::AddMatrix()"
			);
		}
		pImpl->m_MatrixVec.push_back(NewMat);
	}

	void StaticInstanceDraw::UpdateMultiMatrix(const vector<Matrix4X4>& NewMatVec) {
		pImpl->m_MatrixVec.resize(NewMatVec.size());
		pImpl->m_MatrixVec = NewMatVec;
	}

	vector<Matrix4X4>& StaticInstanceDraw::GetMatrixVec() const {
		return pImpl->m_MatrixVec;
	}

	void StaticInstanceDraw::ClearMatrixVec() {
		pImpl->m_MatrixVec.clear();
	}

	bool StaticInstanceDraw::IsAutoClearMatrixVec() const {
		return pImpl->m_AutoClearMatrixVec;
	}
	void StaticInstanceDraw::SetAutoClearMatrixVec(bool b) {
		pImpl->m_AutoClearMatrixVec = b;
	}



	ComPtr<ID3D11Buffer>& StaticInstanceDraw::GetMatrixBuffer() const {
		return pImpl->m_MatrixBuffer;
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
		DrawStatic<VSPCStatic, PSPCStatic>(false, false);
	}


	//--------------------------------------------------------------------------------------
	//	struct PCStaticInstanceDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCStaticInstanceDraw::Impl {
		Impl()
		{}
	};

	//--------------------------------------------------------------------------------------
	///	PCStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCStaticInstanceDraw::PCStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticInstanceDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCStaticInstanceDraw::~PCStaticInstanceDraw() {}

	void PCStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		DrawStaticInstance<VSPCStaticInstance, PSPCStatic>(false, false);
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
		DrawStatic<VSPTStatic, PSPTStatic>(true, false);
	}



	//--------------------------------------------------------------------------------------
	//	struct PTStaticInstanceDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PTStaticInstanceDraw::Impl {
		Impl()
		{}
	};
	//--------------------------------------------------------------------------------------
	///	PTStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTStaticInstanceDraw::PTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticInstanceDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	PTStaticInstanceDraw::~PTStaticInstanceDraw() {}

	void PTStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		DrawStaticInstance<VSPTStaticInstance, PSPTStatic>(true, false);
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
		DrawStatic<VSPCTStatic, PSPCTStatic>(true, false);
	}


	//--------------------------------------------------------------------------------------
	//	struct PCTStaticInstanceDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCTStaticInstanceDraw::Impl {
		Impl()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PCTStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTStaticInstanceDraw::PCTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticInstanceDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCTStaticInstanceDraw::~PCTStaticInstanceDraw() {}


	void PCTStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		DrawStaticInstance<VSPCTStaticInstance, PSPCTStatic>(true,false);
	}



	//--------------------------------------------------------------------------------------
	//	struct PNTStaticDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTStaticDraw::Impl {
		bool m_OwnShadowActive;
		Impl() :
			m_OwnShadowActive(false)
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
		//ライティングのみだと極端になるので調整
		SetEmissive(Color4(0.5f,0.5f,0.5f,0.0f));
		SetDiffuse(Color4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::OnCreate() {
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

	void PNTStaticDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow>(true, true);
		}
		else {
			DrawStatic<VSPNTStatic, PSPNTStatic>(true, false);
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct PNTStaticInstanceDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTStaticInstanceDraw::Impl {
		bool m_OwnShadowActive;
		Impl() :
			m_OwnShadowActive(false)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	PNTStaticインスタンス描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticInstanceDraw::PNTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticInstanceDraw(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		//ライティングのみだと極端になるので調整
		SetEmissive(Color4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(Color4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	PNTStaticInstanceDraw::~PNTStaticInstanceDraw() {}

	void PNTStaticInstanceDraw::OnCreate() {
		CreateMatrixBuffer();
	}


	bool PNTStaticInstanceDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTStaticInstanceDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTStaticInstanceDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}


	void PNTStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		if (GetOwnShadowActive()) {
			DrawStaticInstance<VSPNTStaticInstanceShadow, PSPNTStaticShadow>(true, true);
		}
		else {
			DrawStaticInstance<VSPNTStaticInstance, PSPNTStatic>(true, false);
		}
	}




	//--------------------------------------------------------------------------------------
	//	struct PNTStaticModelDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTStaticModelDraw::Impl {
		bool m_ModelDiffusePriority;
		bool m_ModelEmissivePriority;
		bool m_ModelTextureEnabled;
		Impl() :
			m_ModelDiffusePriority(false),
			m_ModelEmissivePriority(false),
			m_ModelTextureEnabled(true)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class PNTStaticModelDraw : public DrawComponent;
	//	用途: PNTStaticModelDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticModelDraw::PNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		PNTStaticDraw(GameObjectPtr),
		pImpl(new Impl()) {
	}

	PNTStaticModelDraw::~PNTStaticModelDraw() {}

	void PNTStaticModelDraw::OnCreate() {
	}


	bool PNTStaticModelDraw::GetModelDiffusePriority() const {
		return pImpl->m_ModelDiffusePriority;

	}
	bool PNTStaticModelDraw::IsModelDiffusePriority() const {
		return pImpl->m_ModelDiffusePriority;

	}
	void PNTStaticModelDraw::SetModelDiffusePriority(bool b) {
		pImpl->m_ModelDiffusePriority = b;
	}
	bool PNTStaticModelDraw::GetModelEmissivePriority() const {
		return pImpl->m_ModelEmissivePriority;
	}
	bool PNTStaticModelDraw::IsModelEmissivePriority() const {
		return pImpl->m_ModelEmissivePriority;
	}
	void PNTStaticModelDraw::SetModelEmissivePriority(bool b) {
		pImpl->m_ModelEmissivePriority = b;
	}

	bool PNTStaticModelDraw::GetModelTextureEnabled() const {
		return pImpl->m_ModelTextureEnabled;

	}
	bool PNTStaticModelDraw::IsModelTextureEnabled() const {
		return pImpl->m_ModelTextureEnabled;
	}
	void PNTStaticModelDraw::SeModelTextureEnabled(bool b) {
		pImpl->m_ModelTextureEnabled = b;
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
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//シェーダの設定
		if (GetGameObject()->GetComponent<Shadowmap>(false)) {
			//シャドウマップがあれば自己影防止用のピクセルシェーダ
			pD3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);
		}
		else {
			pD3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
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
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//デプスステンシル
		SetDeviceDepthStencilState();
		//コンスタントバッファの設定
		SimpleConstants cb1;
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

		Matrix4X4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		cb1.LightView = Matrix4X4EX::Transpose(LightView);
		cb1.LightProjection = Matrix4X4EX::Transpose(LightProj);

		cb1.Emissive = GetEmissive();
		cb1.Diffuse = GetDiffuse();

		for (auto& m : MatVec) {
			if (IsModelEmissivePriority()) {
				cb1.Emissive = m.m_Emissive;
				cb1.Emissive.w = 0;
			}
			if (IsModelDiffusePriority()) {
				cb1.Diffuse = m.m_Diffuse;
			}
			if (IsModelTextureEnabled() && m.m_TextureResource) {
				cb1.ActiveFlg.x = 1;
			}
			else {
				cb1.ActiveFlg.x = 0;
			}
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);


			if (IsModelTextureEnabled() && m.m_TextureResource) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
				//サンプラー
				SetDeviceSamplerState();
			}
			else {
				ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
				ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
				//シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
				//サンプラーもクリア
				pD3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
			}
			//透明処理なら
			if (GetGameObject()->GetAlphaActive()) {
				//ブレンドステート
				//透明処理
				if (GetBlendState() == BlendState::Additive) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
				}
				else {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
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
				//透明処理しない
				//ブレンドステートは設定に任せる
				SetDeviceBlendState();
				//ラスタライザステートは設定に任せる
				SetDeviceRasterizerState();
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
		}
		//後始末
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
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//デプスステンシル
		SetDeviceDepthStencilState();
		//コンスタントバッファの設定
		SimpleConstants cb1;
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = World;
		cb1.View = ViewMat;
		cb1.Projection = ProjMat;
		//ライティング
		auto StageLight = GameObjectPtr->OnGetDrawLight();
		cb1.LightDir = StageLight.m_Directional;
		cb1.LightDir.w = 1.0f;
		cb1.Emissive = GetEmissive();
		cb1.Diffuse = GetDiffuse();

		for (auto& m : MatVec) {
			if (IsModelEmissivePriority()) {
				cb1.Emissive = m.m_Emissive;
				cb1.Emissive.w = 0;
			}
			if (IsModelDiffusePriority()) {
				cb1.Diffuse = m.m_Diffuse;
			}
			if (IsModelTextureEnabled() && m.m_TextureResource) {
				cb1.ActiveFlg.x = 1;
			}
			else {
				cb1.ActiveFlg.x = 0;
			}
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);


			if (IsModelTextureEnabled() && m.m_TextureResource) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
				//サンプラー
				SetDeviceSamplerState();
			}
			else {
				ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
				ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
				//シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//サンプラーもクリア
				pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
			}
			//透明処理なら
			if (GetGameObject()->GetAlphaActive()) {
				//ブレンドステート
				//透明処理
				if (GetBlendState() == BlendState::Additive) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
				}
				else {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
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
				//透明処理しない
				//ブレンドステートは設定に任せる
				SetDeviceBlendState();
				//ラスタライザステートは設定に任せる
				SetDeviceRasterizerState();
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
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
		bool m_OwnShadowActive;
		bool m_ModelDiffusePriority;
		bool m_ModelEmissivePriority;
		bool m_ModelTextureEnabled;
		//シェーダに渡すボーン行列
		vector<Matrix4X4> m_LocalBonesMatrix;
		map<wstring, AnimationData> m_AnimationMap;
		wstring m_CurrentAnimeName;
		float m_CurrentAnimeTime;
		Impl() :
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(false),
			m_ModelEmissivePriority(false),
			m_ModelTextureEnabled(true),
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
		PNTStaticModelDraw(GameObjectPtr),
		pImpl(new Impl()) {
	}

	PNTBoneModelDraw::~PNTBoneModelDraw() {}

	void PNTBoneModelDraw::OnCreate() {

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
		StaticBaseDraw::SetMeshResource(MeshRes);
	}

	void PNTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		PNTBoneModelDraw::SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
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
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//シェーダの設定
		if (GetGameObject()->GetComponent<Shadowmap>(false)) {
			//シャドウマップがあれば自己影防止用のピクセルシェーダ
			pD3D11DeviceContext->VSSetShader(VSPNTBoneShadow::GetPtr()->GetShader(), nullptr, 0);
			pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);
		}
		else {
			pD3D11DeviceContext->VSSetShader(VSPNTBoneShadow::GetPtr()->GetShader(), nullptr, 0);
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
		pD3D11DeviceContext->IASetInputLayout(VSPNTBone::GetPtr()->GetInputLayout());
		//デプスステンシル
		SetDeviceDepthStencilState();
		//コンスタントバッファの設定
		SimpleConstants cb1;
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

		Matrix4X4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		cb1.LightView = Matrix4X4EX::Transpose(LightView);
		cb1.LightProjection = Matrix4X4EX::Transpose(LightProj);
		cb1.Emissive = GetEmissive();
		cb1.Diffuse = GetDiffuse();
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

		for (auto& m : MatVec) {
			if (IsModelEmissivePriority()) {
				cb1.Emissive = m.m_Emissive;
				cb1.Emissive.w = 0;
			}
			if (IsModelDiffusePriority()) {
				cb1.Diffuse = m.m_Diffuse;
			}
			if (IsModelTextureEnabled() && m.m_TextureResource) {
				cb1.ActiveFlg.x = 1;
			}
			else {
				cb1.ActiveFlg.x = 0;
			}
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			if (IsModelTextureEnabled() && m.m_TextureResource) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
				//サンプラー
				SetDeviceSamplerState();
			}
			else {
				ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
				ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
				//シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
				//サンプラーもクリア
				pD3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
			}
			//透明処理なら
			if (GetGameObject()->GetAlphaActive()) {
				//ブレンドステート
				//透明処理
				if (GetBlendState() == BlendState::Additive) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
				}
				else {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
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
				//透明処理しない
				//ブレンドステートは設定に任せる
				SetDeviceBlendState();
				//ラスタライザステートは設定に任せる
				SetDeviceRasterizerState();
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
		}
		//後始末
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
		auto& MatVec = MeshRes->GetMaterialExVec();
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPNTBone::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPNTBone::GetPtr()->GetInputLayout());
		//デプスステンシル
		SetDeviceDepthStencilState();
		//コンスタントバッファの設定
		SimpleConstants cb1;
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = World;
		cb1.View = ViewMat;
		cb1.Projection = ProjMat;
		//ライティング
		auto StageLight = GameObjectPtr->OnGetDrawLight();
		cb1.LightDir = StageLight.m_Directional;
		cb1.LightDir.w = 1.0f;
		cb1.Emissive = GetEmissive();
		cb1.Diffuse = GetDiffuse();
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
		for (auto& m : MatVec) {
			if (IsModelEmissivePriority()) {
				cb1.Emissive = m.m_Emissive;
				cb1.Emissive.w = 0;
			}
			if (IsModelDiffusePriority()) {
				cb1.Diffuse = m.m_Diffuse;
			}
			if (IsModelTextureEnabled() && m.m_TextureResource) {
				cb1.ActiveFlg.x = 1;
			}
			else {
				cb1.ActiveFlg.x = 0;
			}
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			if (IsModelTextureEnabled() && m.m_TextureResource) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
				//サンプラー
				SetDeviceSamplerState();
			}
			else {
				ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
				ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
				//シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//サンプラーもクリア
				pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
			}
			//透明処理なら
			if (GetGameObject()->GetAlphaActive()) {
				//ブレンドステート
				//透明処理
				if (GetBlendState() == BlendState::Additive) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
				}
				else {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
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
				//透明処理しない
				//ブレンドステートは設定に任せる
				SetDeviceBlendState();
				//ラスタライザステートは設定に任せる
				SetDeviceRasterizerState();
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
		}
		//後始末
		Dev->InitializeStates();

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
		const shared_ptr<MeshResource>& Mesh, const Matrix4X4& MeshToTransformMatrix) {
		Draw3DPrim::PCWireFrameDraw(GameObj, Mesh,
			Color4(0,0,0,0), Color4(1, 1, 1, 1), MeshToTransformMatrix);
	}

}
//end basecross

