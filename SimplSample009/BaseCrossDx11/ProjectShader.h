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
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Projection;
		Col4 Emissive;
		PCStaticConstantBuffer() {
			memset(this, 0, sizeof(PCStaticConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPCStatic, PCStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPCStatic, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCStatic)


}
//end basecross

