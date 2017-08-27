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
	//PCSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCSprite, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCSprite)

}
//end basecross

