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
		XMMATRIX mWorld[3];
		XMMATRIX mView;
		XMMATRIX mProj;
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
		Matrix4X4 World[3];
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Vector4 Param;	//汎用パラメータ
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

