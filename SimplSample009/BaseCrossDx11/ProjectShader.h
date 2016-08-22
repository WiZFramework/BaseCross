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
	//PCStatic
	struct PCStaticConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Color4 Emissive;
		PCStaticConstantBuffer() {
			memset(this, 0, sizeof(PCStaticConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPCStatic, PCStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPCStatic, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCStatic)


}
//end basecross

