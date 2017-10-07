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
	struct PNTStaticConstantBuffer
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Projection;
		Vec4 LightDir;
		Col4 Emissive;
		Col4 Diffuse;
		Col4 Specular;
		Vec4 EyePos;
		PNTStaticConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticConstantBuffer));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStatic, PNTStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)


	//--------------------------------------------------------------------------------------
	///	PCTStaticConstantsコンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct PCTStaticConstants
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
		PCTStaticConstants() {
			memset(this, 0, sizeof(PCTStaticConstants));
			Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	//CBPCTStatic
	DECLARE_DX11_CONSTANT_BUFFER(CBPCTStatic, PCTStaticConstants)
	//PCTStaticInstance
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


}
//end basecross

