/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//カスタムシャドウマップ用コンスタントバッファ構造体
	struct CustomShadowmapConstantBuffer
	{
		Mat4x4 mWorld[3];
		Mat4x4 mView;
		Mat4x4 mProj;
		CustomShadowmapConstantBuffer() {
			memset(this, 0, sizeof(CustomShadowmapConstantBuffer));
		};
	};
	//シェーダ宣言（マクロ使用）
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomShadowmap, CustomShadowmapConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomShadowmap, VertexPositionNormalColor)
	DECLARE_DX11_GEOMETRY_SHADER(GSCustomShadowmap)
	//カスタム描画コンスタントバッファ構造体
	struct CustomDrawConstantBuffer
	{
		Mat4x4 World[3];
		Mat4x4 View;
		Mat4x4 Projection;
		Vec4 LightDir;
		Vec4 Param;	//汎用パラメータ
		CustomDrawConstantBuffer() {
			memset(this, 0, sizeof(CustomDrawConstantBuffer));
		};
	};
	//シェーダ宣言（マクロ使用）
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomDraw, CustomDrawConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomDraw, VertexPositionNormalColor)
	DECLARE_DX11_GEOMETRY_SHADER(GSCustomDraw)
	DECLARE_DX11_PIXEL_SHADER(PSCustomDraw)


}
//end basecross

