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
	//PNTStatic
	struct PNTStaticConstantBuffer
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Projection;
		Vec4 LightDir;
		Col4 Emissive;
		Col4 Diffuse;
		PNTStaticConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticConstantBuffer));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStatic, PNTStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTInstance, VertexPositionNormalTextureMatrix)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)


}
//end basecross

