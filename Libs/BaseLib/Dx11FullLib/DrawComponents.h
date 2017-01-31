/*!
@file DrawComponents.h
@brief 描画コンポーネント
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/*!
	@brief	ビューポートとシサー矩形設定
	@param[in]	vp	ビューポート
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	inline void RsSetViewport(const Viewport& vp) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		D3D11_VIEWPORT vp11;
		vp11.Height = vp.Height;
		vp11.Width = vp.Width;
		vp11.MinDepth = vp.MinDepth;
		vp11.MaxDepth = vp.MaxDepth;
		vp11.TopLeftX = vp.TopLeftX;
		vp11.TopLeftY = vp.TopLeftY;
		pID3D11DeviceContext->RSSetViewports(1, &vp11);
		CD3D11_RECT rect(
			(LONG)vp11.TopLeftX,
			(LONG)vp11.TopLeftY,
			(LONG)vp11.TopLeftX + (LONG)vp11.Width,
			(LONG)vp11.TopLeftY + (LONG)vp11.Height
		);
		pID3D11DeviceContext->RSSetScissorRects(1, &rect);
	}



	//--------------------------------------------------------------------------------------
	///	Shadowコンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct ShadowConstantBuffer
	{
		/// ワールド行列
		XMMATRIX mWorld;
		/// ビュー行列
		XMMATRIX mView;
		/// 射影行列
		XMMATRIX mProj;
		/// Bone用
		XMVECTOR Bones[3 * 72];
		ShadowConstantBuffer() {
			memset(this, 0, sizeof(ShadowConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBShadow, ShadowConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmap, VertexPositionNormalTexture)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBone, VertexPositionNormalTextureSkinning)


	//--------------------------------------------------------------------------------------
	///	スプライト用コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct SpriteConstantBuffer
	{
		/// ワールド行列
		Matrix4X4 World;
		/// エミッシブ色
		Color4 Emissive;
		/// デフューズ色
		Color4 Diffuse;
		SpriteConstantBuffer() {
			memset(this, 0, sizeof(SpriteConstantBuffer));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstantBuffer)
	//PCSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCSprite, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCSprite)
	//PTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPTSprite, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTSprite)
	//PCTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCTSprite, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTSprite)


	//--------------------------------------------------------------------------------------
	///	static系コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct StaticConstantBuffer
	{
		/// ワールド行列
		Matrix4X4 World;
		/// ビュー行列
		Matrix4X4 View;
		/// 射影行列
		Matrix4X4 Projection;
		/// エミッシブ色
		Color4 Emissive;
		/// デフューズ色
		Color4 Diffuse;
		StaticConstantBuffer() {
			memset(this, 0, sizeof(StaticConstantBuffer));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBStatic, StaticConstantBuffer)
	//PCStatic
	DECLARE_DX11_VERTEX_SHADER(VSPCStatic, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCStatic)
	//PTStatic
	DECLARE_DX11_VERTEX_SHADER(VSPTStatic, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTStatic)
	///PCTStatic
	DECLARE_DX11_VERTEX_SHADER(VSPCTInstance, VertexPositionColorTextureMatrix)
	DECLARE_DX11_VERTEX_SHADER(VSPCTStatic, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTStatic)

	//--------------------------------------------------------------------------------------
	///	ライティングを使用するstatic系コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct StaticLightingConstantBuffer
	{
		/// ワールド行列
		Matrix4X4 World;
		/// ビュー行列
		Matrix4X4 View;
		/// 射影行列
		Matrix4X4 Projection;
		/// ライイト方向
		Vector4 LightDir;
		/// エミッシブ色
		Color4 Emissive;
		/// デフューズ色
		Color4 Diffuse;
		StaticLightingConstantBuffer() {
			memset(this, 0, sizeof(StaticLightingConstantBuffer));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBStaticLighting, StaticLightingConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNStatic, VertexPositionNormal)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticMidium, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNStatic)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticNoTex)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticMidium)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticMidiumNoTex)

	//--------------------------------------------------------------------------------------
	///	リアルライティングを使用するstatic系コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct StaticRealLightingConstantBuffer
	{
		/// ワールド行列
		Matrix4X4 World;
		/// ビュー行列
		Matrix4X4 View;
		/// 射影行列
		Matrix4X4 Projection;
		/// ライイト方向
		Vector4 LightDir;
		/// エミッシブ色
		Color4 Emissive;
		/// デフューズ色
		Color4 Diffuse;
		/// Eyeの位置
		Vector4 EyePosition;
		StaticRealLightingConstantBuffer() {
			memset(this, 0, sizeof(StaticRealLightingConstantBuffer));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBStaticRealLighting, StaticRealLightingConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticReal, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticReal)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticRealNoTex)



	//--------------------------------------------------------------------------------------
	///	影付きstatic系コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct PNTStaticShadowConstantBuffer
	{
		/// ワールド行列
		Matrix4X4 World;
		/// ビュー行列
		Matrix4X4 View;
		/// 射影行列
		Matrix4X4 Projection;
		/// ライイト方向
		Vector4 LightDir;
		/// エミッシブ色
		Color4 Emissive;
		/// デフューズ色
		Color4 Diffuse;
		/// ライト位置
		Vector4 LightPos;
		/// Eyeの位置
		Vector4 EyePos;
		/// 汎用フラグ
		XMUINT4 ActiveFlg;
		/// ライトビュー行列
		Matrix4X4 LightView;
		/// ライト射影行列
		Matrix4X4 LightProjection;
		PNTStaticShadowConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticShadowConstantBuffer));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStaticShadow, PNTStaticShadowConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticShadow, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow2)


	//--------------------------------------------------------------------------------------
	///	Bone付コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct PNTBoneConstantBuffer
	{
		/// ワールド行列
		Matrix4X4 World;
		/// ビュー行列
		Matrix4X4 View;
		/// 射影行列
		Matrix4X4 Projection;
		/// ライイト方向
		Vector4 LightDir;
		/// エミッシブ色
		Color4 Emissive;
		/// デフューズ色
		Color4 Diffuse;
		/// Bone配列
		XMVECTOR Bones[3 * 72];
		PNTBoneConstantBuffer() {
			memset(this, 0, sizeof(PNTBoneConstantBuffer));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPNTBone, PNTBoneConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTBone, VertexPositionNormalTextureSkinning)
	DECLARE_DX11_VERTEX_SHADER(VSPNTBoneMidium, VertexPositionNormalTextureSkinning)

	//--------------------------------------------------------------------------------------
	///	リアルライティングを使用するBone付コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct PNTBoneRealLightingConstantBuffer
	{
		/// ワールド行列
		Matrix4X4 World;
		/// ビュー行列
		Matrix4X4 View;
		/// 射影行列
		Matrix4X4 Projection;
		/// ライイト方向
		Vector4 LightDir;
		/// エミッシブ色
		Color4 Emissive;
		/// デフューズ色
		Color4 Diffuse;
		/// Eyeの位置
		Vector4 EyePosition;
		/// Bone配列
		XMVECTOR Bones[3 * 72];
		PNTBoneRealLightingConstantBuffer() {
			memset(this, 0, sizeof(PNTBoneRealLightingConstantBuffer));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPNTBoneRealLighting, PNTBoneRealLightingConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTBoneReal, VertexPositionNormalTextureSkinning)



	//--------------------------------------------------------------------------------------
	///	影とBone付コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct PNTBoneShadowConstantBuffer
	{
		/// ワールド行列
		Matrix4X4 World;
		/// ビュー行列
		Matrix4X4 View;
		/// 射影行列
		Matrix4X4 Projection;
		/// ライイト方向
		Vector4 LightDir;
		/// エミッシブ色
		Color4 Emissive;
		/// デフューズ色
		Color4 Diffuse;
		/// ライト位置
		Vector4 LightPos;
		/// Eyeの位置
		Vector4 EyePos;
		/// テクスチャ=xがアクティブかどうか
		XMUINT4 ActiveFlg;
		/// ライトビュー行列
		Matrix4X4 LightView;
		/// ライト射影行列
		Matrix4X4 LightProjection;
		/// Bone配列
		XMVECTOR Bones[3 * 72];
		PNTBoneShadowConstantBuffer() {
			memset(this, 0, sizeof(PNTBoneShadowConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPNTBoneShadow, PNTBoneShadowConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTBoneShadow, VertexPositionNormalTextureSkinning)



	class GameObject;

	//--------------------------------------------------------------------------------------
	///	描画コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	class DrawComponent : public Component {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		DrawComponent(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DrawComponent();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ブレンドステートの取得
		@return	ブレンドステート
		*/
		//--------------------------------------------------------------------------------------
		BlendState GetBlendState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デプスステンシルステートの取得
		@return	デプスステンシルステート
		*/
		//--------------------------------------------------------------------------------------
		DepthStencilState GetDepthStencilState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラスタライザステートの取得
		@return	ラスタライザステート
		*/
		//--------------------------------------------------------------------------------------
		RasterizerState GetRasterizerState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サンプラーステートの取得
		@return	サンプラーステート
		*/
		//--------------------------------------------------------------------------------------
		SamplerState GetSamplerState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ブレンドステートの設定
		@param[in]	state	ブレンドステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBlendState(const BlendState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ブレンドステートのデバイスへの設定。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDeviceBlendState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デプスステンシルステートの設定
		@param[in]	state	デプスステンシルステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDepthStencilState(const DepthStencilState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デプスステンシルステートのデバイスへの設定
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDeviceDepthStencilState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラスタライザステートの設定
		@param[in]	state	ラスタライザステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetRasterizerState(const RasterizerState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラスタライザステートのデバイスへの設定
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDeviceRasterizerState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サンプラーステートの設定
		@param[in]	state	サンプラーステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSamplerState(const SamplerState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サンプラーステートのデバイスへの設定
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDeviceSamplerState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュとワールド行列の間を補完する行列の取得<br />
		メッシュのサイズや向き、中心の位置がゲームオブジェクトのワールド行列と差異がある場合、<br />
		その間を補完する行列を設定できる。
		@return	メッシュとワールド行列の間を補完する行列
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetMeshToTransformMatrix() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュとワールド行列の間を補完する行列の取得<br />
		メッシュのサイズや向き、中心の位置がゲームオブジェクトのワールド行列と差異がある場合、<br />
		その間を補完する行列を設定できる。
		@param[in]	Mat	設定する行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshToTransformMatrix(const Matrix4X4& Mat);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	各オブジェクトごとにボーンを所持しておくポインタ<br />
		シャドウマップなどから参照できるように仮想関数にする<br />
		派生クラスでボーンを所持する場合は多重定義する
		@return	ボーン行列の配列のポインタ
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< Matrix4X4 >* GetVecLocalBonesPtr() const { return nullptr; }
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	シャドウマップコンポーネント（前処理用）
	//--------------------------------------------------------------------------------------
	class Shadowmap : public DrawComponent {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Shadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Shadowmap();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトの高さを得る
		@return	ライトの高さ
		*/
		//--------------------------------------------------------------------------------------
		static float GetLightHeight();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトのNear値を得る
		@return	ライトのNear値
		*/
		//--------------------------------------------------------------------------------------
		static float GetLightNear();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトのFar値を得る
		@return	ライトのFar値
		*/
		//--------------------------------------------------------------------------------------
		static float GetLightFar();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューの幅を得る
		@return	ビューの幅
		*/
		//--------------------------------------------------------------------------------------
		static float GetViewWidth();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューの高さを得る
		@return	ビューの高さ
		*/
		//--------------------------------------------------------------------------------------
		static float GetViewHeight();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトの高さを設定する
		@param[in]	f	高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void SetLightHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトのNear値を設定する
		@param[in]	f	Near値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void SetLightNear(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトのFar値を設定する
		@param[in]	f	Far値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void SetLightFar(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューの幅を設定する
		@param[in]	f	ビューの幅
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void SetViewWidth(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューの高さを設定する
		@param[in]	f	ビューの高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void SetViewHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューの幅及び高さを同じ値に設定する
		@param[in]	f	ビューの幅及び高さ（同値）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void SetViewSize(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースを得る
		@param[in]	ExceptionActive	エラー時に例外が発生するかどうか
		@return	メッシュリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MeshResource> GetMeshResource(bool ExceptionActive = true) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースを設定する
		@param[in]	ResKey	メッシュリソースのキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースを設定する
		@param[in]	MeshResourcePtr	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr);
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理。（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	PCTParticle描画コンポーネント(パーティクル描画)
	//--------------------------------------------------------------------------------------
	class PCTParticleDraw : public DrawComponent {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		@param[in]	MaxInstance	パーティクル数上限
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTParticleDraw(const shared_ptr<GameObject>& GameObjectPtr, size_t MaxInstance);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCTParticleDraw();
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画用パーティクルを登録する
		@param[in]	ToCaneraLength	カメラまでの距離
		@param[in]	WorldMatrix	ワールド行列
		@param[in]	TextureRes	テクスチャ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddParticle(float ToCaneraLength, const Matrix4X4& WorldMatrix, const shared_ptr<TextureResource>& TextureRes);
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	テクスチャ描画インターフェイス
	//--------------------------------------------------------------------------------------
	class TextureDrawInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		TextureDrawInterface();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TextureDrawInterface();
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
		@brief	ラッピングサンプラーかどうかを得る
		@return	ラッピングサンプラーかどうか
		*/
		//--------------------------------------------------------------------------------------
		bool GetWrapSampler() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラッピングサンプラーかどうか設定する
		@param[in]	b	ラッピングサンプラーかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetWrapSampler(bool b);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	Sprite描画コンポーネントの親
	//--------------------------------------------------------------------------------------
	class SpriteBaseDraw : public DrawComponent {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SpriteBaseDraw();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの取得
		@return	メッシュリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MeshResource> GetMeshResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの設定
		@param[in]	MeshRes	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッシブ色の取得
		@return	エミッシブ色
		*/
		//--------------------------------------------------------------------------------------
		Color4 GetEmissive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッシブ色の設定
		@param[in]	col	エミッシブ色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEmissive(const Color4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デフューズ色の取得
		@return	デフューズ色
		*/
		//--------------------------------------------------------------------------------------
		Color4 GetDiffuse() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デフューズ色の設定
		@param[in]	col	デフューズ色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDiffuse(const Color4& col);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	PCSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PCSpriteDraw : public SpriteBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		@param[in]	Vertices	初期頂点の配列
		@param[in]	indices		初期インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		explicit PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCSpriteDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点変更できるスプライトメッシュを作成する（すでにある場合は差し替える）
		@param[in]	Vertices	頂点の配列
		@param[in]	indices		インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		void CreateMesh(vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの更新
		@param[in]	Vertices	頂点配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVertices(const vector<VertexPositionColor>& Vertices);
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	PTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PTSpriteDraw : public SpriteBaseDraw,public TextureDrawInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		@param[in]	Vertices	初期頂点の配列
		@param[in]	indices		初期インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		explicit PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PTSpriteDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点変更できるスプライトメッシュを作成する（すでにある場合は差し替える）
		@param[in]	Vertices	頂点の配列
		@param[in]	indices		インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		void CreateMesh(vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの更新
		@param[in]	Vertices	頂点配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVertices(const vector<VertexPositionTexture>& Vertices);
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	PCTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PCTSpriteDraw : public SpriteBaseDraw, public TextureDrawInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		@param[in]	Vertices	初期頂点の配列
		@param[in]	indices		初期インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCTSpriteDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点変更できるスプライトメッシュを作成する（すでにある場合は差し替える）
		@param[in]	Vertices	頂点の配列
		@param[in]	indices		インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		void CreateMesh(vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの更新
		@param[in]	Vertices	頂点配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVertices(const vector<VertexPositionColorTexture>& Vertices);
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	/// ライティングシェーダ設定用のenum
	//--------------------------------------------------------------------------------------
	enum class ShaderLighting {
		Simple,
		Midium,
		Real
	};

	//--------------------------------------------------------------------------------------
	///	3D描画コンポーネントの親(3D描画の親)
	//--------------------------------------------------------------------------------------
	class Base3DDraw : public DrawComponent {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Base3DDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Base3DDraw();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの取得(仮想関数)
		@return	メッシュリソース
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<MeshResource> GetMeshResource() const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの設定（仮想関数）
		@param[in]	MeshRes	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッシブ色の取得
		@return	エミッシブ色
		*/
		//--------------------------------------------------------------------------------------
		Color4 GetEmissive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッシブ色の設定
		@param[in]	col	エミッシブ色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEmissive(const Color4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デフューズ色の取得
		@return	デフューズ色
		*/
		//--------------------------------------------------------------------------------------
		Color4 GetDiffuse() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デフューズ色の設定
		@param[in]	col	デフューズ色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDiffuse(const Color4& col);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;

	};



	//--------------------------------------------------------------------------------------
	///	Dynamic描画コンポーネントの親(頂点を変更できる3D描画)
	//--------------------------------------------------------------------------------------
	class DynamicBaseDraw : public Base3DDraw {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit DynamicBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DynamicBaseDraw();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの取得
		@return	メッシュリソース
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<MeshResource> GetMeshResource() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの設定
		@param[in]	MeshRes	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes)override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	PCDynamic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PCDynamicDraw : public DynamicBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PCDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCDynamicDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点変更できるメッシュを作成する（すでにある場合は差し替える）
		@param[in]	Vertices	頂点の配列
		@param[in]	indices		インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		void CreateMesh(vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの設定
		@param[in]	Vertices	頂点配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVertices(const vector<VertexPositionColor>& Vertices);
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	PTDynamic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PTDynamicDraw : public DynamicBaseDraw, public TextureDrawInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PTDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PTDynamicDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点変更できるメッシュを作成する（すでにある場合は差し替える）
		@param[in]	Vertices	頂点の配列
		@param[in]	indices		インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		void CreateMesh(vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの設定
		@param[in]	Vertices	頂点配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVertices(const vector<VertexPositionTexture>& Vertices);
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	PCTDynamic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PCTDynamicDraw : public DynamicBaseDraw, public TextureDrawInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCTDynamicDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点変更できるメッシュを作成する（すでにある場合は差し替える）
		@param[in]	Vertices	頂点の配列
		@param[in]	indices		インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		void CreateMesh(vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの設定
		@param[in]	Vertices	頂点配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVertices(const vector<VertexPositionColorTexture>& Vertices);
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	PNTDynamic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PNTDynamicDraw : public DynamicBaseDraw, public TextureDrawInterface {
		void DrawNotShadow();
		void DrawWithShadow();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTDynamicDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点変更できるメッシュを作成する（すでにある場合は差し替える）
		@param[in]	Vertices	頂点の配列
		@param[in]	indices		インデックスの配列
		*/
		//--------------------------------------------------------------------------------------
		void CreateMesh(vector<VertexPositionNormalTexture>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの設定
		@param[in]	Vertices	頂点配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVertices(const vector<VertexPositionNormalTexture>& Vertices);
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};






	//--------------------------------------------------------------------------------------
	///	static描画コンポーネントの親(頂点を変更できない3D描画)
	//--------------------------------------------------------------------------------------
	class StaticBaseDraw : public Base3DDraw {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit StaticBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~StaticBaseDraw();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの取得
		@return	メッシュリソース
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<MeshResource> GetMeshResource() const override;
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
		virtual void SetMeshResource(const wstring& MeshKey);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	PCStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PCStaticDraw : public StaticBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	PTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PTStaticDraw : public StaticBaseDraw, public TextureDrawInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	PCTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PCTStaticDraw : public StaticBaseDraw, public TextureDrawInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	PNStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PNStaticDraw : public StaticBaseDraw {
		void DrawNotShadow();
		void DrawWithShadow();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PNStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNStaticDraw();
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
		//操作
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	PNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PNTStaticDraw : public StaticBaseDraw, public TextureDrawInterface {
		void DrawNotShadow();
		void DrawWithShadow();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングを得る
		@return	シェーダーライティング
		*/
		//--------------------------------------------------------------------------------------
		ShaderLighting GetLighting() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングを設定する
		@param[in]	Lighting	シェーダーライティング
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLighting(ShaderLighting Lighting);
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
		//操作
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PNTStaticModelDraw : public DrawComponent;
	//	用途: PNTStaticModelDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PNTStaticModelDraw : public StaticBaseDraw {
		void DrawWithShadow();
		void DrawNotShadow();
	public:
		explicit PNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PNTStaticModelDraw();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングを得る
		@return	シェーダーライティング
		*/
		//--------------------------------------------------------------------------------------
		ShaderLighting GetLighting() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングを設定する
		@param[in]	Lighting	シェーダーライティング
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLighting(ShaderLighting Lighting);
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
		//操作
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
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
	///	PNTBoneModelDraw描画コンポーネント（ボーンモデル描画用）
	//--------------------------------------------------------------------------------------
	class PNTBoneModelDraw : public Base3DDraw {
		void DrawWithShadow();
		void DrawNotShadow();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTBoneModelDraw();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングを得る
		@return	シェーダーライティング
		*/
		//--------------------------------------------------------------------------------------
		ShaderLighting GetLighting() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライティングを設定する
		@param[in]	Lighting	シェーダーライティング
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLighting(ShaderLighting Lighting);

		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの取得
		@return	メッシュリソース
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<MeshResource> GetMeshResource() const override;
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
		@brief	自己影描画を行うかどうかの取得
		@return	自己影描画を行うかどうか
		*/
		//--------------------------------------------------------------------------------------
		bool GetOwnShadowActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	自己影描画を行うかどうかの取得
		@return	自己影描画を行うかどうか
		*/
		//--------------------------------------------------------------------------------------
		bool IsOwnShadowActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	自己影描画を行うかどうかの設定
		@param[in]	b	自己影描画を行うかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetOwnShadowActive(bool b);
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
		void InterpolationMatrix(const Matrix4X4& m1, const Matrix4X4& m2, float t, Matrix4X4& out);
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
		virtual const vector< Matrix4X4 >* GetVecLocalBonesPtr() const;
		//操作
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	汎用描画用（PNTStatic固定）
	//--------------------------------------------------------------------------------------
	class GenericDraw :public ObjectInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		GenericDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GenericDraw();
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワイアフレーム描画処理
		@param[in]	GameObj	ゲームオブジェクト
		@param[in]	Mesh	PCメッシュ

		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DrawWireFrame(const shared_ptr<GameObject>& GameObj,
			const shared_ptr<MeshResource>& Mesh,const Matrix4X4& MeshToTransformMatrix = Matrix4X4());
	};




}
//end basecross
