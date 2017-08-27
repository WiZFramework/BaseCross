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
		Mat4x4 World;
		/// ビュー行列
		Mat4x4 View;
		/// 射影行列
		Mat4x4 Projection;
		/// エミッシブ色
		Col4 Emissive;
		/// デフューズ色
		Col4 Diffuse;
		/// ライイト方向
		Vec4 LightDir;
		TestConstants() {
			memset(this, 0, sizeof(TestConstants));
			Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
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

