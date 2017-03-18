/*!
@file DrawComponents.h
@brief Drawコンポーネント
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
	struct ShadowConstants
	{
		/// ワールド行列
		XMMATRIX mWorld;
		/// ビュー行列
		XMMATRIX mView;
		/// 射影行列
		XMMATRIX mProj;
		/// Bone用
		XMVECTOR Bones[3 * 72];
		ShadowConstants() {
			memset(this, 0, sizeof(ShadowConstants));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBShadow, ShadowConstants)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmap, VertexPositionNormalTexture)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBone, VertexPositionNormalTextureSkinning)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBoneWithTan, VertexPositionNormalTangentTextureSkinning)


	//--------------------------------------------------------------------------------------
	///	スプライト用コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct SpriteConstants
	{
		/// ワールド行列
		Matrix4X4 World;
		/// エミッシブ色
		Color4 Emissive;
		/// デフューズ色
		Color4 Diffuse;
		SpriteConstants() {
			memset(this, 0, sizeof(SpriteConstants));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	/// スプライトコンスタントバッファ
	DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstants)
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
	///	SimpleConstantsコンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct SimpleConstants
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
		/// スペキュラー
		Color4 Specular;
		/// テクスチャ=xがアクティブかどうか
		XMUINT4 ActiveFlg;
		/// ライイト方向
		Vector4 LightDir;
		/// ライト位置
		Vector4 LightPos;
		/// Eyeの位置
		Vector4 EyePos;
		/// ライトビュー行列
		Matrix4X4 LightView;
		/// ライト射影行列
		Matrix4X4 LightProjection;
		/// Bone配列
		XMVECTOR Bones[3 * 72];
		SimpleConstants() {
			memset(this, 0, sizeof(SimpleConstants));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	//CBSimple
	DECLARE_DX11_CONSTANT_BUFFER(CBSimple, SimpleConstants)
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
	///PNTStatic
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)
	///PNTStaticShadow
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticShadow, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow2)
	///PNTBone
	DECLARE_DX11_VERTEX_SHADER(VSPNTBone, VertexPositionNormalTextureSkinning)
	DECLARE_DX11_VERTEX_SHADER(VSPNTBoneShadow, VertexPositionNormalTextureSkinning)
	//PCStaticInstance
	DECLARE_DX11_VERTEX_SHADER(VSPCStaticInstance, VertexPositionColorMatrix)
	//PTStaticInstance
	DECLARE_DX11_VERTEX_SHADER(VSPTStaticInstance, VertexPositionTextureMatrix)
	//PCTStaticInstance
	DECLARE_DX11_VERTEX_SHADER(VSPCTStaticInstance, VertexPositionColorTextureMatrix)
	///PNTStaticInstance
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticInstance, VertexPositionNormalTextureMatrix)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticInstanceShadow, VertexPositionNormalTextureMatrix)


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
	///	シャドウマップコンポーネント（前処理用）
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
		@param[in]	AddType	加算処理するかどうか
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTParticleDraw(const shared_ptr<GameObject>& GameObjectPtr, size_t MaxInstance,bool AddType = false);
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
		@param[in]	TextureRes	テクスチャ
		@param[in]	Diffuse   Diffuseカラー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddParticle(float ToCaneraLength, const Matrix4X4& WorldMatrix, 
			const shared_ptr<TextureResource>& TextureRes,const Color4& Diffuse = Color4(1,1,1,1));
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
	///	static描画コンポーネントの親(頂点を変更する場合も可)
	//--------------------------------------------------------------------------------------
	class StaticBaseDraw : public DrawComponent, public TextureDrawInterface {
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスタントバッファの設定
		@param[out]	Cb	設定する構造体
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(SimpleConstants& Cb, bool shadowUse = false);
	public:
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スペキュラー色の取得
		@return	スペキュラー色
		*/
		//--------------------------------------------------------------------------------------
		Color4 GetSpecular() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スペキュラー色の設定
		@param[in]	col	スペキュラー色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSpecular(const Color4& col);
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
					L"StaticBaseDraw::UpdateVertices()"
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
		@brief	オリジナルではないメッシュリソースの設定（仮想関数）
		@param[in]	MeshRes	メッシュリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルではないメッシュリソースの登録
		@param[in]	MeshKey	登録されているメッシュキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const wstring& MeshKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタティックメッシュの描画
		@tparam[in]	T_VShader	使用する頂点シェーダ
		@tparam[in]	T_PShader	使用するピクセルシェーダ
		@param[in]	TextureUse	テクスチャを使用するかどうか
		@param[in]	ShadoUuse	影を描画するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawStatic(bool TextureUse, bool ShadoUuse) {
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
					L"StaticBaseDraw::DrawStatic()"
				);
			}
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//コンスタントバッファの準備
			SimpleConstants Cb;
			SetConstants(Cb, ShadoUuse);
			if (TextureUse) {
				//テクスチャ
				auto shTex = GetTextureResource();
				if (shTex) {
					Cb.ActiveFlg.x = 1;
					pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
					//サンプラーは設定に任せる
					SetDeviceSamplerState();
				}
				else {
					Cb.ActiveFlg.x = 0;
				}
			}

			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
			//ストライドとオフセット
			UINT stride = MeshRes->GetNumStride();
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
			if (ShadoUuse) {
				//シェーダの設定
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					//シャドウマップがあれば自己影防止用のピクセルシェーダ
					pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
					pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);
				}
				else {
					pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
					pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
				}
				//シャドウマップのレンダラーターゲット
				auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
				ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
				pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
				//シャドウマップサンプラー
				ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
				pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			}
			else {
				//シェーダの設定
				pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
				pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			}
			//インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//デプスステンシルステートは設定に任せる
			SetDeviceDepthStencilState();
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
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//透明処理しない
				//ブレンドステートは設定に任せる
				SetDeviceBlendState();
				//ラスタライザステートは設定に任せる
				SetDeviceRasterizerState();
				//描画
				pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
			}
			//後始末
			Dev->InitializeStates();
		}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	インスタンス描画コンポーネントの親
	//--------------------------------------------------------------------------------------
	class StaticInstanceDraw : public StaticBaseDraw {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列バッファの作成
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateMatrixBuffer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列バッファのマップ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void MapMatrixBuffer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit StaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~StaticInstanceDraw();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列の上限を得る
		@return	行列の上限を
		*/
		//--------------------------------------------------------------------------------------
		size_t GetMaxInstance() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列の上限を変更する
		@param[in]	NewSize		新しい上限
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ResizeMaxInstance(size_t NewSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	同時描画の行列を追加する
		@param[in]	NewMatVec		新しい行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddMatrix(const Matrix4X4& NewMat);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	同時描画の行列を完全に更新する
		@param[in]	NewMatVec		新しい行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateMultiMatrix(const vector<Matrix4X4>& NewMatVec);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	同時描画の行列の配列を取得する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		vector<Matrix4X4>& GetMatrixVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	同時描画の行列をクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ClearMatrixVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画後行列配列を自動クリアするかどうか
		@return	クリアするはtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsAutoClearMatrixVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画後行列配列を自動クリアするかどうかを設定
		@param[in]	b	描画後行列配列を自動クリアするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAutoClearMatrixVec(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief　行列のバッファを取得する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer>& GetMatrixBuffer() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタティックメッシュのインスタンス描画
		@tparam[in]	T_VShader	使用する頂点シェーダ
		@tparam[in]	T_PShader	使用するピクセルシェーダ
		@param[in]	TextureUse	テクスチャを使用するかどうか
		@param[in]	ShadoUuse	影を描画するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawStaticInstance(bool TextureUse, bool ShadoUuse){
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
					L"StaticInstanceDraw::DrawStaticInstance()"
				);
			}
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//コンスタントバッファの準備
			SimpleConstants Cb;
			SetConstants(Cb, ShadoUuse);
			if (TextureUse) {
				//テクスチャ
				auto shTex = GetTextureResource();
				if (shTex) {
					Cb.ActiveFlg.x = 1;
					pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
					//サンプラーは設定に任せる
					SetDeviceSamplerState();
				}
				else {
					Cb.ActiveFlg.x = 0;
				}
			}

			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);

			//ストライドとオフセット
			//形状の頂点バッファと行列バッファを設定
			UINT stride[2] = { MeshRes->GetNumStride(), sizeof(Matrix4X4) };
			UINT offset[2] = { 0, 0 };
			ID3D11Buffer* pBuf[2] = { MeshRes->GetVertexBuffer().Get(), GetMatrixBuffer().Get() };
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
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
			if (ShadoUuse) {
				//シェーダの設定
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					//シャドウマップがあれば自己影防止用のピクセルシェーダ
					pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
					pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);
				}
				else {
					pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
					pD3D11DeviceContext->PSSetShader(PSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
				}
				//シャドウマップのレンダラーターゲット
				auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
				ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
				pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
				//シャドウマップサンプラー
				ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
				pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			}
			else {
				//シェーダの設定
				pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
				pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			}
			//インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//デプスステンシルステートは設定に任せる
			SetDeviceDepthStencilState();
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
				pD3D11DeviceContext->DrawIndexedInstanced(MeshRes->GetNumIndicis(), GetMatrixVec().size(), 0, 0, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexedInstanced(MeshRes->GetNumIndicis(), GetMatrixVec().size(), 0, 0, 0);
			}
			else {
				//透明処理しない
				//ブレンドステートは設定に任せる
				SetDeviceBlendState();
				//ラスタライザステートは設定に任せる
				SetDeviceRasterizerState();
				//描画
				pD3D11DeviceContext->DrawIndexedInstanced(MeshRes->GetNumIndicis(), GetMatrixVec().size(), 0, 0, 0);
			}
			//後始末
			Dev->InitializeStates();
			//自動行列クリアなら
			if (IsAutoClearMatrixVec()) {
				//行列配列のクリア
				ClearMatrixVec();
			}
		}


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
	///	PCStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PCStaticInstanceDraw : public StaticInstanceDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PCStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCStaticInstanceDraw();
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
	class PTStaticDraw : public StaticBaseDraw {
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
	///	PTStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PTStaticInstanceDraw : public StaticInstanceDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PTStaticInstanceDraw();
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
	class PCTStaticDraw : public StaticBaseDraw {
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
	///	PCTStaticInstance描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PCTStaticInstanceDraw : public StaticInstanceDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCTStaticInstanceDraw();
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
	///	PNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PNTStaticDraw : public StaticBaseDraw {
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
	///	PNTStaticインスタンス描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PNTStaticInstanceDraw : public StaticInstanceDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTStaticInstanceDraw();
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
	//	class PNTStaticModelDraw : public PNTStaticDraw;
	//	用途: PNTStaticModelDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PNTStaticModelDraw : public PNTStaticDraw {
		void DrawWithShadow();
		void DrawNotShadow();
	public:
		explicit PNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PNTStaticModelDraw();
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
		void SeModelTextureEnabled(bool b);
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
	class PNTBoneModelDraw : public PNTStaticModelDraw {
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
