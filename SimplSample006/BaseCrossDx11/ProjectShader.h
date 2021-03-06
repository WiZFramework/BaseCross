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
	// Diffuse入りコンスタントバッファ
	struct DiffuseSpriteConstantBuffer
	{
		Mat4x4 World;
		Col4 Emissive;
		Col4 Diffuse;
		DiffuseSpriteConstantBuffer() {
			memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBDiffuseSprite, DiffuseSpriteConstantBuffer)
	//PTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPTSprite, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTSprite)

}
//end basecross

