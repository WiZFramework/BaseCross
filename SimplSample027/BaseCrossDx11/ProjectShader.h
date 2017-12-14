/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Shadowコンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct ShadowConstants
	{
		/// ワールド行列
		bsm::Mat4x4 mWorld;
		/// ビュー行列
		bsm::Mat4x4 mView;
		/// 射影行列
		bsm::Mat4x4 mProj;
		/// Bone用
		bsm::Vec4 Bones[3 * 72];
		ShadowConstants() {
			memset(this, 0, sizeof(ShadowConstants));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBShadow, ShadowConstants)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmap, VertexPositionNormalTexture)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBone, VertexPositionNormalTextureSkinning)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBoneWithTan, VertexPositionNormalTangentTextureSkinning)

	//--------------------------------------------------------------------------------------
	///	SimpleConstantsコンスタントバッファ構造体(シンプル３Ｄ共有)
	//--------------------------------------------------------------------------------------
	struct SimpleConstants
	{
		/// ワールド行列
		Mat4x4 World;
		/// ビュー行列
		Mat4x4 View;
		/// 射影行列
		Mat4x4 Projection;
		/// エミッシブ色
		Col4 Emissive;
		/// デフューズ色
		Col4 Diffuse;
		/// スペキュラー
		Col4 Specular;
		/// テクスチャ=xがアクティブかどうか
		XMUINT4 ActiveFlg;
		/// ライイト方向
		Vec4 LightDir;
		/// ライト位置
		Vec4 LightPos;
		/// Eyeの位置
		Vec4 EyePos;
		/// ライトビュー行列
		Mat4x4 LightView;
		/// ライト射影行列
		Mat4x4 LightProjection;
		/// Bone配列
		Vec4 Bones[3 * 72];
		SimpleConstants() {
			memset(this, 0, sizeof(SimpleConstants));
			Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	//CBSimple
	DECLARE_DX11_CONSTANT_BUFFER(CBSimple, SimpleConstants)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	///PNTStaticShadow
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticShadow, VertexPositionNormalTexture)
	//影無しBone
	DECLARE_DX11_VERTEX_SHADER(VSPNTBone, VertexPositionNormalTextureSkinning)
	//影付きBone
	DECLARE_DX11_VERTEX_SHADER(VSPNTBoneShadow, VertexPositionNormalTextureSkinning)

	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow2)

	//PCStatic
	DECLARE_DX11_VERTEX_SHADER(VSPCStatic, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCStatic)

	//PCTStatic
	DECLARE_DX11_VERTEX_SHADER(VSPCTStatic, VertexPositionColorTexture)
	DECLARE_DX11_VERTEX_SHADER(VSPCTStaticInstance, VertexPositionColorTextureMatrix)
	DECLARE_DX11_PIXEL_SHADER(PSPCTStatic)


	//スプライト用コンスタントバッファ構造体
	struct SpriteConstantBuffer
	{
		Mat4x4 World;
		Col4 Emissive;
		SpriteConstantBuffer() {
			memset(this, 0, sizeof(SpriteConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstantBuffer)

	//PCTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCTSprite, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTSprite)


	//--------------------------------------------------------------------------------------
	///	Basicシェーダー
	//--------------------------------------------------------------------------------------

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
	/// PNT頂点シェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPL, VertexPositionNormalTexture)
	//影付き
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPLShadow, VertexPositionNormalTexture)
	//影無しBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePL, VertexPositionNormalTextureSkinning)
	//影付きBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePLShadow, VertexPositionNormalTextureSkinning)

	//--------------------------------------------------------------------------------------
	/// PNTnT頂点シェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTStaticPL, VertexPositionNormalTangentTexture)
	//影つき
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTStaticPLShadow, VertexPositionNormalTangentTexture)
	//影無しBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTBonePL, VertexPositionNormalTangentTextureSkinning)
	//影つきBone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTBonePLShadow, VertexPositionNormalTangentTextureSkinning)

	//--------------------------------------------------------------------------------------
	/// PNTピクセルシェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTPL);
	//影付き
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTPLShadow);

	//--------------------------------------------------------------------------------------
	/// PNTnTピクセルシェーダ(ピクセルライティング)
	//--------------------------------------------------------------------------------------
	//影無し
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTnTPL);
	//影つき
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTnTPLShadow);


}
//end basecross

