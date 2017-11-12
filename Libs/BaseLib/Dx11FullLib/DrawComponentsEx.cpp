/*!
@file DrawComponentsEx.cpp
@brief 拡張描画コンポーネント実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	DirectXTKのEffect関連移植
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	///	Basicシェーダー
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	/// コンスタントバッファ
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBBasic)


	//--------------------------------------------------------------------------------------
	/// PC頂点シェーダ
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPCStatic, App::GetApp()->GetShadersPath() + L"BcVSPCStatic.cso")

	//--------------------------------------------------------------------------------------
	/// PT頂点シェーダ
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPTStatic, App::GetApp()->GetShadersPath() + L"BcVSPTStatic.cso")

	//--------------------------------------------------------------------------------------
	/// PCT頂点シェーダ
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPCTStatic, App::GetApp()->GetShadersPath() + L"BcVSPCTStatic.cso")

	//--------------------------------------------------------------------------------------
	/// PC用ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPCStatic, App::GetApp()->GetShadersPath() + L"BcPSPCStatic.cso")


	//--------------------------------------------------------------------------------------
	/// PT,PCT用ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPTStatic, App::GetApp()->GetShadersPath() + L"BcPSPTStatic.cso")



	//--------------------------------------------------------------------------------------
	/// PNT頂点シェーダ(頂点ライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticVL, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticVL.cso")
	//影無しBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBoneVL, App::GetApp()->GetShadersPath() + L"BcVSPNTBoneVL.cso")
	//影付き
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticVLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticVLShadow.cso")
	//影付きBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBoneVLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTBoneVLShadow.cso")


		

	//バイアス付き影無し
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticVLBn, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticVLBn.cso")
	//バイアス付き影無しBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBoneVLBn, App::GetApp()->GetShadersPath() + L"BcVSPNTBoneVLBn.cso")

	//バイアス付き影付き
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticVLBnShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticVLBnShadow.cso")

	//バイアス付き影付きBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBoneVLBnShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTBoneVLBnShadow.cso")


		
	//--------------------------------------------------------------------------------------
	/// PNTピクセルシェーダ(頂点ライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTVL, App::GetApp()->GetShadersPath() + L"BcPSPNTVL.cso")
	//影付き
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTVLShadow, App::GetApp()->GetShadersPath() + L"BcPSPNTVLShadow.cso")


	//--------------------------------------------------------------------------------------
	/// PNT頂点シェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPL, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticPL.cso")
	//影無しBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePL, App::GetApp()->GetShadersPath() + L"BcVSPNTBonePL.cso")
	//影付き
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticPLShadow.cso")
	//影付きBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTBonePLShadow.cso")
		
	//バイス付き影無し
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPLBn, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticPLBn.cso")
	//バイス付き影無しBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePLBn, App::GetApp()->GetShadersPath() + L"BcVSPNTBonePLBn.cso")

	//バイス付き影付き
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTStaticPLBnShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticPLBnShadow.cso")
	//バイス付き影付きBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTBonePLBnShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTBonePLBnShadow.cso")


		

	//--------------------------------------------------------------------------------------
	/// PNTピクセルシェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTPL, App::GetApp()->GetShadersPath() + L"BcPSPNTPL.cso")
	//影付き
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTPLShadow, App::GetApp()->GetShadersPath() + L"BcPSPNTPLShadow.cso")


	//--------------------------------------------------------------------------------------
	/// PNTnT頂点シェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTnTStaticPL, App::GetApp()->GetShadersPath() + L"BcVSPNTnTStaticPL.cso")
	//影無しBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTnTBonePL, App::GetApp()->GetShadersPath() + L"BcVSPNTnTBonePL.cso")

	//影つき
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTnTStaticPLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTnTStaticPLShadow.cso")
	//影つきBone
	IMPLEMENT_DX11_VERTEX_SHADER(BcVSPNTnTBonePLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTnTBonePLShadow.cso")


	//--------------------------------------------------------------------------------------
	/// PNTnTピクセルシェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTnTPL, App::GetApp()->GetShadersPath() + L"BcPSPNTnTPL.cso")
	//影つき
	IMPLEMENT_DX11_PIXEL_SHADER(BcPSPNTnTPLShadow, App::GetApp()->GetShadersPath() + L"BcPSPNTnTPLShadow.cso")

	//--------------------------------------------------------------------------------------
	///	Bc描画に使用する構造体(影対応)
	//--------------------------------------------------------------------------------------
	struct BcDrawObject :public DrawObjectBase {
		//頂点変更する場合のメッシュ（オリジナル）
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//オリジナルメッシュを使うかどうか
		bool m_UseOriginalMeshResource;
		//テクスチャリソース
		weak_ptr<TextureResource> m_TextureResource;
		//NormalMapテクスチャ
		weak_ptr<TextureResource> m_NormalMapTextureResource;
		vector<weak_ptr<TextureResource>> m_NormalMapTextureResourceVec;
		//エミッシブ色
		bsm::Col4 m_Emissive;
		//デフューズ色
		bsm::Col4 m_Diffuse;
		//透明度
		float m_Alpha;
		//スペキュラーとパワー色
		bsm::Col4 m_SpecularColorAndPower;
		//ライティングするかどうか
		bool m_LightingEnabled;
		//ピクセルライティングをするかどうか
		bool m_PreferPerPixelLighting;
		//法線にバイアスをかけるかどうか
		bool m_BiasedNormals;
		//アンビエント
		bsm::Col4 m_AmbientLightColor;
		//ライトのMAX
		static const int MaxDirectionalLights = 3;
		//ライトが有効かどうか
		bool m_LightEnabled[MaxDirectionalLights];
		//ライトの向き
		bsm::Vec3 m_LightDirection[MaxDirectionalLights];
		//ライトのデフューズ色
		bsm::Col4 m_LightDiffuseColor[MaxDirectionalLights];
		//ライトのスペキュラー色
		bsm::Col4 m_LightSpecularColor[MaxDirectionalLights];
		//フォグが有効かどうか
		bool m_FogEnabled;
		//フォグの開始位置
		float m_FogStart;
		//フォグの終了位置
		float m_FogEnd;
		//フォグ色
		bsm::Col4 m_FogColor;
		//フォグベクトル
		bsm::Vec3 m_FogVector;
		//影を投影するかどうか
		bool m_OwnShadowActive;
		//モデルに入っているDiffuseを使うかどうか
		bool m_ModelDiffusePriority;
		//モデルに入っているEmissiveを使うかどうか
		bool m_ModelEmissivePriority;
		//モデルに入っているテクスチャを使うかどうか
		bool m_ModelTextureEnabled;
		BcDrawObject() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Alpha(1.0f),
			m_SpecularColorAndPower(0.0f, 0.0f, 0.0f, 1.0f),
			m_LightingEnabled(false),
			m_PreferPerPixelLighting(false),
			m_BiasedNormals(false),
			m_AmbientLightColor(0, 0, 0, 0),
			m_FogEnabled(false),
			m_FogStart(-25.0f),
			m_FogEnd(-40.0f),
			m_FogColor(0.8f, 0.8f, 0.8f, 1.0f),
			m_FogVector(0.0, 0.0, 1.0f),
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(true),
			m_ModelEmissivePriority(true),
			m_ModelTextureEnabled(true)
		{
			static const XMVECTORF32 defaultSpecular = { 1, 1, 1, 16 };
			static const bsm::Vec3 defaultLightDirection = { 0, -1, 0 };
			m_SpecularColorAndPower = defaultSpecular;
			for (int i = 0; i < MaxDirectionalLights; i++)
			{
				m_LightDirection[i] = defaultLightDirection;
				m_LightDiffuseColor[i] = g_XMZero;
				m_LightSpecularColor[i] = g_XMZero;
			}
		}
	};


	//--------------------------------------------------------------------------------------
	//	BcBaseDraw::Impl
	//--------------------------------------------------------------------------------------
	struct BcBaseDraw::Impl {
		BcDrawObject m_BcDrawObject;
	};

	//--------------------------------------------------------------------------------------
	///	Bc描画オブジェクトの親
	//--------------------------------------------------------------------------------------
	BcBaseDraw::BcBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	BcBaseDraw::~BcBaseDraw() {}


	bool BcBaseDraw::IsOriginalMeshUse() const {
		return pImpl->m_BcDrawObject.m_UseOriginalMeshResource;
	}

	void BcBaseDraw::SetOriginalMeshUse(bool b) {
		pImpl->m_BcDrawObject.m_UseOriginalMeshResource = b;
	}

	shared_ptr<MeshResource> BcBaseDraw::GetOriginalMeshResource() const {
		return pImpl->m_BcDrawObject.m_OriginalMeshResource;
	}
	void BcBaseDraw::SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_BcDrawObject.m_OriginalMeshResource = MeshRes;
	}


	shared_ptr<MeshResource> BcBaseDraw::GetMeshResource() const {
		if (IsOriginalMeshUse()) {
			return pImpl->m_BcDrawObject.m_OriginalMeshResource;
		}
		else {
			auto shptr = pImpl->m_BcDrawObject.m_MeshResource.lock();
			if (shptr) {
				return shptr;
			}
		}
		return nullptr;
	}
	void BcBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_BcDrawObject.m_MeshResource = MeshRes;
	}

	void BcBaseDraw::SetMeshResource(const wstring& MeshKey) {
		this->SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}

	shared_ptr<TextureResource> BcBaseDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = pImpl->m_BcDrawObject.m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}


	void BcBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_BcDrawObject.m_TextureResource = TextureRes;
	}

	void BcBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<MultiMeshResource> BcBaseDraw::GetMultiMeshResource() const {
		//なければnullを返す
		auto shptr = pImpl->m_BcDrawObject.m_MultiMeshResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}

	void BcBaseDraw::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_BcDrawObject.m_MultiMeshResource = MeshResourcePtr;
	}
	void BcBaseDraw::SetMultiMeshResource(const wstring& ResKey) {
		this->SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(ResKey));
	}


	bsm::Col4 BcBaseDraw::GetEmissive() const {
		return pImpl->m_BcDrawObject.m_Emissive;
	}
	void BcBaseDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_Emissive = col;
	}
	bsm::Col4 BcBaseDraw::GetDiffuse() const {
		return pImpl->m_BcDrawObject.m_Diffuse;
	}
	void BcBaseDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_Diffuse = col;
	}

	bsm::Col4 BcBaseDraw::GetSpecularColor() const {
		bsm::Col4 ret = pImpl->m_BcDrawObject.m_SpecularColorAndPower;
		ret.w = 0.0f;
		return ret;
	}
	void BcBaseDraw::SetSpecularColor(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_SpecularColorAndPower = XMVectorSelect(pImpl->m_BcDrawObject.m_SpecularColorAndPower, col, g_XMSelect1110);
	}

	void BcBaseDraw::DisableSpecular() {
		pImpl->m_BcDrawObject.m_SpecularColorAndPower = g_XMIdentityR3;
	}

	float BcBaseDraw::GetSpecularPower() const {
		return pImpl->m_BcDrawObject.m_SpecularColorAndPower.w;
	}


	void BcBaseDraw::SetSpecularPower(float pow) {
		pImpl->m_BcDrawObject.m_SpecularColorAndPower.w = pow;
	}



	bsm::Col4 BcBaseDraw::GetSpecularColorAndPower() const {
		return pImpl->m_BcDrawObject.m_SpecularColorAndPower;
	}
	void BcBaseDraw::SetSpecularColorAndPower(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_SpecularColorAndPower = col;
	}

	float BcBaseDraw::GetAlpha() const {
		return pImpl->m_BcDrawObject.m_Alpha;
	}

	void BcBaseDraw::SetAlpha(float a) {
		pImpl->m_BcDrawObject.m_Alpha = a;
	}

	bsm::Col4 BcBaseDraw::GetColorAndAlpha() const {
		bsm::Col4 ret = pImpl->m_BcDrawObject.m_Diffuse;
		ret.w = pImpl->m_BcDrawObject.m_Alpha;
		return ret;
	}

	void BcBaseDraw::SetColorAndAlpha(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_Diffuse = col;
		pImpl->m_BcDrawObject.m_Alpha = col.w;
	}
	bool BcBaseDraw::IsLightingEnabled() const {
		return pImpl->m_BcDrawObject.m_LightingEnabled;
	}
	bool BcBaseDraw::GetLightingEnabled() const {
		return pImpl->m_BcDrawObject.m_LightingEnabled;

	}
	void BcBaseDraw::SetLightingEnabled(bool value) {
		pImpl->m_BcDrawObject.m_LightingEnabled = value;
	}
	bool BcBaseDraw::GetPerPixelLighting() const {
		return pImpl->m_BcDrawObject.m_PreferPerPixelLighting;
	}

	bool BcBaseDraw::IsPerPixelLighting() const {
		return pImpl->m_BcDrawObject.m_PreferPerPixelLighting;

	}
	void BcBaseDraw::SetPerPixelLighting(bool value) {
		pImpl->m_BcDrawObject.m_PreferPerPixelLighting = value;
	}

	void BcBaseDraw::SetLightingParamaters() {
		//ライトを得る
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//マルチライトだった
			for (int i = 0; i < GetMaxDirectionalLights(); i++) {
				if (IsLightEnabled(i)) {
					SetLightDirection(i, PtrMultiLight->GetLight(i).m_Directional);
					SetLightDiffuseColor(i, PtrMultiLight->GetLight(i).m_DiffuseColor);
					SetLightSpecularColor(i, PtrMultiLight->GetLight(i).m_SpecularColor);
				}
			}
		}
		else {
			//そうではない
			auto LightPtr = GetGameObject()->OnGetDrawLight();
			SetLightEnabled(0, true);
			SetLightDirection(0, LightPtr.m_Directional);
			SetLightDiffuseColor(0, LightPtr.m_DiffuseColor);
			SetLightSpecularColor(0, LightPtr.m_SpecularColor);
			for (int i = 1; i < GetMaxDirectionalLights(); i++) {
				SetLightEnabled(i, false);
			}
		}
		SetAmbientLightColor(PtrLightObj->GetAmbientLightColor());
	}


	bool BcBaseDraw::GetBiasedNormals() const {
		return pImpl->m_BcDrawObject.m_BiasedNormals;
	}

	bool BcBaseDraw::IsBiasedNormals() const {
		return pImpl->m_BcDrawObject.m_BiasedNormals;
	}

	void BcBaseDraw::SetBiasedNormals(bool value) {
		pImpl->m_BcDrawObject.m_BiasedNormals = value;
	}

	bsm::Col4 BcBaseDraw::GetAmbientLightColor()const {
		return pImpl->m_BcDrawObject.m_AmbientLightColor;
	}

	void BcBaseDraw::SetAmbientLightColor(const bsm::Col4& value) {
		pImpl->m_BcDrawObject.m_AmbientLightColor = value;
	}
	void BcBaseDraw::ValidateLightIndex(int whichLight)
	{
		if (whichLight < 0 || whichLight >= BcDrawObject::MaxDirectionalLights)
		{
			throw BaseException(
				L"ライトのインデックスが範囲外です",
				L"if (whichLight < 0 || whichLight >= Impl::MaxDirectionalLights)",
				L"Bc3DDraw::ValidateLightIndex()"
			);
		}
	}

	int BcBaseDraw::GetMaxDirectionalLights() {
		return BcDrawObject::MaxDirectionalLights;
	}

	bool BcBaseDraw::GetLightEnabled(int whichLight)const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightEnabled[whichLight];
	}

	bool BcBaseDraw::IsLightEnabled(int whichLight)const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightEnabled[whichLight];
	}
	void BcBaseDraw::SetLightEnabled(int whichLight, bool value) {
		ValidateLightIndex(whichLight);
		pImpl->m_BcDrawObject.m_LightEnabled[whichLight] = value;
	}

	bsm::Vec3 BcBaseDraw::GetLightDirection(int whichLight) const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightDirection[whichLight];
	}

	void BcBaseDraw::SetLightDirection(int whichLight, const bsm::Vec3& value) {
		ValidateLightIndex(whichLight);
		pImpl->m_BcDrawObject.m_LightDirection[whichLight] = value;
	}

	bsm::Col4 BcBaseDraw::GetLightDiffuseColor(int whichLight) const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightDiffuseColor[whichLight];
	}


	void BcBaseDraw::SetLightDiffuseColor(int whichLight, const bsm::Col4& value) {
		ValidateLightIndex(whichLight);
		pImpl->m_BcDrawObject.m_LightDiffuseColor[whichLight] = value;
	}

	bsm::Col4 BcBaseDraw::GetLightSpecularColor(int whichLight) const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightSpecularColor[whichLight];
	}

	void BcBaseDraw::SetLightSpecularColor(int whichLight, const bsm::Col4& value) {
		ValidateLightIndex(whichLight);
		pImpl->m_BcDrawObject.m_LightSpecularColor[whichLight] = value;
	}


	bool BcBaseDraw::GetFogEnabled() const {
		return pImpl->m_BcDrawObject.m_FogEnabled;

	}
	bool BcBaseDraw::IsFogEnabled() const {
		return pImpl->m_BcDrawObject.m_FogEnabled;
	}
	void BcBaseDraw::SetFogEnabled(bool b) {
		pImpl->m_BcDrawObject.m_FogEnabled = b;
	}

	float BcBaseDraw::GetFogStart() const {
		return pImpl->m_BcDrawObject.m_FogStart;
	}

	void BcBaseDraw::SetFogStart(float value) {
		//常にマイナス値
		pImpl->m_BcDrawObject.m_FogStart = -(abs(value));
	}

	float BcBaseDraw::GetFogEnd() const {
		return pImpl->m_BcDrawObject.m_FogEnd;
	}
	void BcBaseDraw::SetFogEnd(float value) {
		//常にマイナス値
		pImpl->m_BcDrawObject.m_FogEnd = -(abs(value));
	}



	bsm::Col4 BcBaseDraw::GetFogColor() const {
		return pImpl->m_BcDrawObject.m_FogColor;
	}
	void BcBaseDraw::SetFogColor(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_FogColor = col;
	}
	bool BcBaseDraw::GetOwnShadowActive() const {
		return pImpl->m_BcDrawObject.m_OwnShadowActive;
	}
	bool BcBaseDraw::IsOwnShadowActive() const {
		return pImpl->m_BcDrawObject.m_OwnShadowActive;
	}
	void BcBaseDraw::SetOwnShadowActive(bool b) {
		pImpl->m_BcDrawObject.m_OwnShadowActive = b;
	}

	bool BcBaseDraw::GetModelDiffusePriority() const {
		return pImpl->m_BcDrawObject.m_ModelDiffusePriority;

	}
	bool BcBaseDraw::IsModelDiffusePriority() const {
		return pImpl->m_BcDrawObject.m_ModelDiffusePriority;

	}
	void BcBaseDraw::SetModelDiffusePriority(bool b) {
		pImpl->m_BcDrawObject.m_ModelDiffusePriority = b;
	}
	bool BcBaseDraw::GetModelEmissivePriority() const {
		return pImpl->m_BcDrawObject.m_ModelEmissivePriority;
	}
	bool BcBaseDraw::IsModelEmissivePriority() const {
		return pImpl->m_BcDrawObject.m_ModelEmissivePriority;
	}
	void BcBaseDraw::SetModelEmissivePriority(bool b) {
		pImpl->m_BcDrawObject.m_ModelEmissivePriority = b;
	}


	bool BcBaseDraw::GetModelTextureEnabled() const {
		return pImpl->m_BcDrawObject.m_ModelTextureEnabled;

	}
	bool BcBaseDraw::IsModelTextureEnabled() const {
		return pImpl->m_BcDrawObject.m_ModelTextureEnabled;
	}
	void BcBaseDraw::SetModelTextureEnabled(bool b) {
		pImpl->m_BcDrawObject.m_ModelTextureEnabled = b;
	}

	shared_ptr<TextureResource> BcBaseDraw::GetNormalMapTextureResource() const {
		auto shptr = pImpl->m_BcDrawObject.m_NormalMapTextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}
	void BcBaseDraw::SetNormalMapTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_BcDrawObject.m_NormalMapTextureResource = TextureRes;

	}
	void BcBaseDraw::SetNormalMapTextureResource(const wstring& TextureKey) {
		pImpl->m_BcDrawObject.m_NormalMapTextureResource = App::GetApp()->GetResource<TextureResource>(TextureKey);
	}


	const vector<weak_ptr<TextureResource>>& BcBaseDraw::GetNormalMapTextureResourceVec() const {
		return pImpl->m_BcDrawObject.m_NormalMapTextureResourceVec;

	}
	vector<weak_ptr<TextureResource>>& BcBaseDraw::GetNormalMapTextureResourceVec() {
		return pImpl->m_BcDrawObject.m_NormalMapTextureResourceVec;
	}

	void BcBaseDraw::BoneInit() {
		pImpl->m_BcDrawObject.BoneInit();
	}

	void BcBaseDraw::InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out) {
		pImpl->m_BcDrawObject.InterpolationMatrix(m1, m2, t, out);
	}

	void BcBaseDraw::AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
		float SamplesParSecond) {
		pImpl->m_BcDrawObject.AddAnimation(Name, StartSample, SampleLength, Loop, SamplesParSecond);
	}
	void  BcBaseDraw::ChangeCurrentAnimation(const wstring& AnemationName, float StartTime) {
		pImpl->m_BcDrawObject.ChangeCurrentAnimation(AnemationName, StartTime);
	}
	const wstring& BcBaseDraw::GetCurrentAnimation() const {
		return pImpl->m_BcDrawObject.m_CurrentAnimeName;

	}

	float BcBaseDraw::GetCurrentAnimationTime() const {
		return pImpl->m_BcDrawObject.m_CurrentAnimeTime;
	}

	bool BcBaseDraw::IsTargetAnimeEnd() const {
		auto& AnimData = pImpl->m_BcDrawObject.GetAnimationData();
		return AnimData.m_IsAnimeEnd;
	}


	bool BcBaseDraw::UpdateAnimation(float ElapsedTime) {
		return pImpl->m_BcDrawObject.UpdateAnimation(ElapsedTime);
	}
	const vector< bsm::Mat4x4 >* BcBaseDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_BcDrawObject.m_LocalBonesMatrix;
	}

	void BcBaseDraw::SetConstants(BasicConstants& BcCb, const MeshPrimData& data) {
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//カメラを得る
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//ライトを得る
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//マルチライトだった
			BcCb.activeFlg.x = 3;
		}
		else {
			//そうではない
			BcCb.activeFlg.x = 1;
		}
		auto StageLight = GetGameObject()->OnGetDrawLight();
		//ワールド行列
		bsm::Mat4x4 world;
		if (data.m_UseMeshToTransformMatrix) {
			world = data.m_MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
		}
		else {
			world = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		}
		//ビュー行列
		bsm::Mat4x4 view = CameraPtr->GetViewMatrix();
		//射影行列
		bsm::Mat4x4 projection = CameraPtr->GetProjMatrix();

		//行列の設定
		auto worldView = world * view;
		BcCb.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, projection));

		//フォグの設定
		if (IsFogEnabled())
		{
			auto start = GetFogStart();
			auto end = GetFogEnd();
			if (start == end)
			{
				// Degenerate case: force everything to 100% fogged if start and end are the same.
				static const XMVECTORF32 fullyFogged = { 0, 0, 0, 1 };
				BcCb.fogVector = fullyFogged;
			}
			else
			{
				XMMATRIX worldViewTrans = worldView;
				// _13, _23, _33, _43
				XMVECTOR worldViewZ = XMVectorMergeXY(XMVectorMergeZW(worldViewTrans.r[0], worldViewTrans.r[2]),
					XMVectorMergeZW(worldViewTrans.r[1], worldViewTrans.r[3]));
				XMVECTOR wOffset = XMVectorSwizzle<1, 2, 3, 0>(XMLoadFloat(&start));
				BcCb.fogVector = (worldViewZ + wOffset) / (start - end);
			}
			BcCb.fogColor = GetFogColor();
		}
		else
		{
			BcCb.fogVector = g_XMZero;
			BcCb.fogColor = g_XMZero;

		}
		//ライトの設定
		if (IsLightingEnabled())
		{
			if (BcCb.activeFlg.x == 1) {
				BcCb.lightDirection[0] = GetLightDirection(0);
				BcCb.lightDiffuseColor[0] = GetLightDiffuseColor(0);
				BcCb.lightSpecularColor[0] = GetLightSpecularColor(0);
			}
			else {
				for (int i = 0; i < GetMaxDirectionalLights(); i++) {
					if (IsLightEnabled(i)) {
						BcCb.lightDirection[i] = GetLightDirection(i);
						BcCb.lightDiffuseColor[i] = GetLightDiffuseColor(i);
						BcCb.lightSpecularColor[i] = GetLightSpecularColor(i);
					}
				}
			}
			BcCb.world = XMMatrixTranspose(world);

			XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);

			BcCb.worldInverseTranspose[0] = worldInverse.r[0];
			BcCb.worldInverseTranspose[1] = worldInverse.r[1];
			BcCb.worldInverseTranspose[2] = worldInverse.r[2];


			XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);

			BcCb.eyePosition = viewInverse.r[3];
		}
		XMVECTOR diffuse = GetDiffuse();
		XMVECTOR alphaVector = XMVectorReplicate(GetAlpha());
		XMVECTOR emissiveColor = GetEmissive();
		XMVECTOR ambientLightColor = GetAmbientLightColor();
		if (IsLightingEnabled())
		{
			// Merge emissive and ambient light contributions.
			BcCb.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
			BcCb.specularColorAndPower = GetSpecularColorAndPower();
		}
		else
		{
			BcCb.activeFlg.x = 0;
			// Merge diffuse and emissive light contributions.
			diffuse += emissiveColor;
			BcCb.specularColorAndPower = GetSpecularColorAndPower();
		}
		// xyz = diffuse * alpha, w = alpha.
		BcCb.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);

		//影用
		if (GetOwnShadowActive()) {
			bsm::Vec3 CalcLightDir(StageLight.m_Directional * -1.0);
			bsm::Vec3 LightAt(CameraPtr->GetAt());
			bsm::Vec3 LightEye(CalcLightDir);
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			bsm::Vec4 LightEye4(LightEye, 1.0f);
			LightEye4.w = 1.0f;
			BcCb.lightPos = LightEye4;
			bsm::Vec4 eyePos4(CameraPtr->GetEye(), 1.0f);
			eyePos4.w = 1.0f;
			BcCb.eyePos = eyePos4;
			bsm::Mat4x4 LightView, LightProj;
			//ライトのビューと射影を計算
			LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			BcCb.lightView = bsm::transpose(LightView);
			BcCb.lightProjection = bsm::transpose(LightProj);
		}
		//ボーンの設定
		size_t BoneSz = pImpl->m_BcDrawObject.m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				bsm::Mat4x4 mat = pImpl->m_BcDrawObject.m_LocalBonesMatrix[b];
				mat.transpose();
				BcCb.bones[cb_count] = ((XMMATRIX)mat).r[0];
				BcCb.bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				BcCb.bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
	}

	//--------------------------------------------------------------------------------------
	///BcPCStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	BcPCStaticDraw::BcPCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr):
		BcBaseDraw(GameObjectPtr)
	{}
	BcPCStaticDraw::~BcPCStaticDraw() {}
	void BcPCStaticDraw::OnCreate() {
		SetLightingEnabled(false);
	}
	void BcPCStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<BcVSPCStatic, BcPSPCStatic>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawStatic<BcVSPCStatic, BcPSPCStatic>(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	BasicPTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	BcPTStaticDraw::BcPTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr):
		BcBaseDraw(GameObjectPtr)
	{}
	BcPTStaticDraw::~BcPTStaticDraw() {}
	void BcPTStaticDraw::OnCreate() {
		SetLightingEnabled(false);
	}
	void BcPTStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<BcVSPTStatic, BcPSPTStatic>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawStatic<BcVSPTStatic, BcPSPTStatic>(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	BasicPCTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	BcPCTStaticDraw::BcPCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr):
		BcBaseDraw(GameObjectPtr)
	{}
	BcPCTStaticDraw::~BcPCTStaticDraw(){}

	void BcPCTStaticDraw::OnCreate() {
		SetLightingEnabled(false);
	}
	void BcPCTStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<BcVSPCTStatic, BcPSPTStatic>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				DrawStatic<BcVSPCTStatic, BcPSPTStatic>(vec[i]);
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}




	//--------------------------------------------------------------------------------------
	///	BcPNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	BcPNTStaticDraw::BcPNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr):
		BcBaseDraw(GameObjectPtr)
	{}
	BcPNTStaticDraw::~BcPNTStaticDraw() {}

	void BcPNTStaticDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}
	void BcPNTStaticDraw::OnDraw() {
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
			if (IsPerPixelLighting()) {
				//ピクセルライティング
				if (IsOwnShadowActive()) {
					//影付き
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawStatic<BcVSPNTStaticPLBnShadow, BcPSPNTPLShadow>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawStatic<BcVSPNTStaticPLShadow, BcPSPNTPLShadow>(PtrMeshResource->GetMashData());
					}
				}
				else {
					//影無し
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawStatic<BcVSPNTStaticPLBn, BcPSPNTPL>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawStatic<BcVSPNTStaticPL, BcPSPNTPL>(PtrMeshResource->GetMashData());
					}
				}
			}
			else {
				//頂点ライティング
				if (IsOwnShadowActive()) {
					//影付き
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawStatic<BcVSPNTStaticVLBnShadow, BcPSPNTVLShadow>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawStatic<BcVSPNTStaticVLShadow, BcPSPNTVLShadow>(PtrMeshResource->GetMashData());
					}
				}
				else {
					//影無し
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawStatic<BcVSPNTStaticVLBn, BcPSPNTVL>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawStatic<BcVSPNTStaticVL, BcPSPNTVL>(PtrMeshResource->GetMashData());
					}
				}
			}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				//シェーダの設定
				if (IsPerPixelLighting()) {
					//ピクセルライティング
					if (IsOwnShadowActive()) {
						//影付き
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawStatic<BcVSPNTStaticPLBnShadow, BcPSPNTPLShadow>(vec[i]);
						}
						else {
							//バイアス無し
							DrawStatic<BcVSPNTStaticPLShadow, BcPSPNTPLShadow>(vec[i]);
						}
					}
					else {
						//影無し
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawStatic<BcVSPNTStaticPLBn, BcPSPNTPL>(vec[i]);
						}
						else {
							//バイアス無し
							DrawStatic<BcVSPNTStaticPL, BcPSPNTPL>(vec[i]);
						}
					}
				}
				else {
					//頂点ライティング
					if (IsOwnShadowActive()) {
						//影付き
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawStatic<BcVSPNTStaticVLBnShadow, BcPSPNTVLShadow>(vec[i]);
						}
						else {
							//バイアス無し
							DrawStatic<BcVSPNTStaticVLShadow, BcPSPNTVLShadow>(vec[i]);
						}
					}
					else {
						//影無し
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawStatic<BcVSPNTStaticVLBn, BcPSPNTVL>(vec[i]);
						}
						else {
							//バイアス無し
							DrawStatic<BcVSPNTStaticVL, BcPSPNTVL>(vec[i]);
						}
					}
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}


	//--------------------------------------------------------------------------------------
	///	BasicPNTStaticモデル描画コンポーネント
	//--------------------------------------------------------------------------------------
	BcPNTStaticModelDraw::BcPNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr):
		BcBaseDraw(GameObjectPtr)
	{}
	BcPNTStaticModelDraw::~BcPNTStaticModelDraw() {}

	void BcPNTStaticModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}
	void BcPNTStaticModelDraw::OnDraw() {
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
			if (IsPerPixelLighting()) {
				//ピクセルライティング
				if (IsOwnShadowActive()) {
					//影付き
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawModel<BcVSPNTStaticPLBnShadow, BcPSPNTPLShadow>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawModel<BcVSPNTStaticPLShadow, BcPSPNTPLShadow>(PtrMeshResource->GetMashData());
					}
				}
				else {
					//影無し
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawModel<BcVSPNTStaticPLBn, BcPSPNTPL>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawModel<BcVSPNTStaticPL, BcPSPNTPL>(PtrMeshResource->GetMashData());
					}
				}
			}
			else {
				//頂点ライティング
				if (IsOwnShadowActive()) {
					//影付き
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawModel<BcVSPNTStaticVLBnShadow, BcPSPNTVLShadow>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawModel<BcVSPNTStaticVLShadow, BcPSPNTVLShadow>(PtrMeshResource->GetMashData());
					}
				}
				else {
					//影無し
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawModel<BcVSPNTStaticVLBn, BcPSPNTVL>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawModel<BcVSPNTStaticVL, BcPSPNTVL>(PtrMeshResource->GetMashData());
					}
				}
			}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				//シェーダの設定
				if (IsPerPixelLighting()) {
					//ピクセルライティング
					if (IsOwnShadowActive()) {
						//影付き
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawModel<BcVSPNTStaticPLBnShadow, BcPSPNTPLShadow>(vec[i]);
						}
						else {
							//バイアス無し
							DrawModel<BcVSPNTStaticPLShadow, BcPSPNTPLShadow>(vec[i]);
						}
					}
					else {
						//影無し
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawModel<BcVSPNTStaticPLBn, BcPSPNTPL>(vec[i]);
						}
						else {
							//バイアス無し
							DrawModel<BcVSPNTStaticPL, BcPSPNTPL>(vec[i]);
						}
					}
				}
				else {
					//頂点ライティング
					if (IsOwnShadowActive()) {
						//影付き
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawModel<BcVSPNTStaticVLBnShadow, BcPSPNTVLShadow>(vec[i]);
						}
						else {
							//バイアス無し
							DrawModel<BcVSPNTStaticVLShadow, BcPSPNTVLShadow>(vec[i]);
						}
					}
					else {
						//影無し
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawModel<BcVSPNTStaticVLBn, BcPSPNTVL>(vec[i]);
						}
						else {
							//バイアス無し
							DrawModel<BcVSPNTStaticVL, BcPSPNTVL>(vec[i]);
						}
					}
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}
	//--------------------------------------------------------------------------------------
	///	BcPNTBoneModel描画コンポーネント（ボーンモデル描画用）
	//--------------------------------------------------------------------------------------
	BcPNTBoneModelDraw::BcPNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr):
		BcBaseDraw(GameObjectPtr)
	{}
	BcPNTBoneModelDraw::~BcPNTBoneModelDraw() {}

	void BcPNTBoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		BcBaseDraw::SetMeshResource(MeshRes);
		BoneInit();
	}

	void BcPNTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		BcPNTBoneModelDraw::SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}


	void BcPNTBoneModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}

	void BcPNTBoneModelDraw::OnDraw() {
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
			if (IsPerPixelLighting()) {
				//ピクセルライティング
				if (IsOwnShadowActive()) {
					//影付き
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawModel<BcVSPNTBonePLBnShadow, BcPSPNTPLShadow>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawModel<BcVSPNTBonePLShadow, BcPSPNTPLShadow>(PtrMeshResource->GetMashData());
					}
				}
				else {
					//影無し
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawModel<BcVSPNTBonePLBn, BcPSPNTPL>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawModel<BcVSPNTBonePL, BcPSPNTPL>(PtrMeshResource->GetMashData());
					}
				}
			}
			else {
				//頂点ライティング
				if (IsOwnShadowActive()) {
					//影付き
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawModel<BcVSPNTBoneVLBnShadow, BcPSPNTVLShadow>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawModel<BcVSPNTBoneVLShadow, BcPSPNTVLShadow>(PtrMeshResource->GetMashData());
					}
				}
				else {
					//影無し
					if (IsBiasedNormals()) {
						//バイアス付き
						DrawModel<BcVSPNTBoneVLBn, BcPSPNTVL>(PtrMeshResource->GetMashData());
					}
					else {
						//バイアス無し
						DrawModel<BcVSPNTBoneVL, BcPSPNTVL>(PtrMeshResource->GetMashData());
					}
				}
			}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				//シェーダの設定
				if (IsPerPixelLighting()) {
					//ピクセルライティング
					if (IsOwnShadowActive()) {
						//影付き
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawModel<BcVSPNTBonePLBnShadow, BcPSPNTPLShadow>(vec[i]);
						}
						else {
							//バイアス無し
							DrawModel<BcVSPNTBonePLShadow, BcPSPNTPLShadow>(vec[i]);
						}
					}
					else {
						//影無し
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawModel<BcVSPNTBonePLBn, BcPSPNTPL>(vec[i]);
						}
						else {
							//バイアス無し
							DrawModel<BcVSPNTBonePL, BcPSPNTPL>(vec[i]);
						}
					}
				}
				else {
					//頂点ライティング
					if (IsOwnShadowActive()) {
						//影付き
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawModel<BcVSPNTBoneVLBnShadow, BcPSPNTVLShadow>(vec[i]);
						}
						else {
							//バイアス無し
							DrawModel<BcVSPNTBoneVLShadow, BcPSPNTVLShadow>(vec[i]);
						}
					}
					else {
						//影無し
						if (IsBiasedNormals()) {
							//バイアス付き
							DrawModel<BcVSPNTBoneVLBn, BcPSPNTVL>(vec[i]);
						}
						else {
							//バイアス無し
							DrawModel<BcVSPNTBoneVL, BcPSPNTVL>(vec[i]);
						}
					}
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	BasicPNTnTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	BcPNTnTStaticDraw::BcPNTnTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		BcBaseDraw(GameObjectPtr)
	{}
	BcPNTnTStaticDraw::~BcPNTnTStaticDraw() {}
	void BcPNTnTStaticDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}
	void BcPNTnTStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//ピクセルライティングのみ
			if (IsOwnShadowActive()) {
				//影付き
				//バイアス無し
				DrawStatic<BcVSPNTnTStaticPLShadow, BcPSPNTnTPLShadow>(PtrMeshResource->GetMashData());
			}
			else {
				//影無し
				//バイアス無し
				DrawStatic<BcVSPNTnTStaticPL, BcPSPNTnTPL>(PtrMeshResource->GetMashData());
			}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				//ピクセルライティングのみ
				if (IsOwnShadowActive()) {
					//影付き
					//バイアス無し
					DrawStatic<BcVSPNTnTStaticPLShadow, BcPSPNTnTPLShadow>(vec[i]);
				}
				else {
					//影無し
					//バイアス無し
					DrawStatic<BcVSPNTnTStaticPL, BcPSPNTnTPL>(vec[i]);
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	BasicPNTnTモデル描画コンポーネント
	//--------------------------------------------------------------------------------------
	BcPNTnTStaticModelDraw::BcPNTnTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr):
		BcBaseDraw(GameObjectPtr)
	{}
	BcPNTnTStaticModelDraw::~BcPNTnTStaticModelDraw() {

	}
	void BcPNTnTStaticModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}
	void BcPNTnTStaticModelDraw::OnDraw() {
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
			//ピクセルライティングのみ
			if (IsOwnShadowActive()) {
				//影付き
				//バイアス無しのみ
				DrawModel<BcVSPNTnTStaticPLShadow, BcPSPNTnTPLShadow>(PtrMeshResource->GetMashData());
			}
			else {
				//影無し
				//バイアス無しのみ
				DrawModel<BcVSPNTnTStaticPL, BcPSPNTnTPL>(PtrMeshResource->GetMashData());
			}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				//シェーダの設定
				//ピクセルライティングのみ
				if (IsOwnShadowActive()) {
					//影付き
					//バイアス無しのみ
					DrawModel<BcVSPNTnTStaticPLShadow, BcPSPNTnTPLShadow>(vec[i]);
				}
				else {
					//影無し
					//バイアス無しのみ
					DrawModel<BcVSPNTnTStaticPL, BcPSPNTnTPL>(vec[i]);
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	BcPNTnTBoneModelDraw描画コンポーネント（ボーンモデル描画用）
	//--------------------------------------------------------------------------------------
	BcPNTnTBoneModelDraw::BcPNTnTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr):
		BcBaseDraw(GameObjectPtr)
	{}

	BcPNTnTBoneModelDraw::~BcPNTnTBoneModelDraw() {}
	void BcPNTnTBoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		BcBaseDraw::SetMeshResource(MeshRes);
		BoneInit();
	}
	void BcPNTnTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		BcPNTnTBoneModelDraw::SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}
	void BcPNTnTBoneModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}
	void BcPNTnTBoneModelDraw::OnDraw() {
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
			//ピクセルライティングのみ
			if (IsOwnShadowActive()) {
				//影付き
				//バイアス無しのみ
				DrawModel<BcVSPNTnTBonePLShadow, BcPSPNTnTPLShadow>(PtrMeshResource->GetMashData());
			}
			else {
				//影無し
				//バイアス無しのみ
				DrawModel<BcVSPNTnTBonePL, BcPSPNTnTPL>(PtrMeshResource->GetMashData());
			}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				//シェーダの設定
				//ピクセルライティングのみ
				if (IsOwnShadowActive()) {
					//影付き
					//バイアス無しのみ
					DrawModel<BcVSPNTnTBonePLShadow, BcPSPNTnTPLShadow>(vec[i]);
				}
				else {
					//影無し
					//バイアス無しのみ
					DrawModel<BcVSPNTnTBonePL, BcPSPNTnTPL>(vec[i]);
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}




}

//end basecross
