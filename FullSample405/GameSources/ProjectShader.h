/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct TestConstants
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
		/// ライイト方向
		Vector4 LightDir;
		TestConstants() {
			memset(this, 0, sizeof(TestConstants));
			Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	//コンスタントバッファ用マクロ
	DECLARE_DX11_CONSTANT_BUFFER(CBTest, TestConstants)
	//頂点シェーダ用マクロ
	DECLARE_DX11_VERTEX_SHADER(VSTest, VertexPositionNormalTexture)
	//ピクセルシェーダ用マクロ
	DECLARE_DX11_PIXEL_SHADER(PSTest)


}
//end basecross

