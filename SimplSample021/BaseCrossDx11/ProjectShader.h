/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	描画用のシェーダクラスの宣言（マクロ使用）
	//--------------------------------------------------------------------------------------
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
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)

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


}
//end basecross

