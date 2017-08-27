/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//PNStatic
	struct PNStaticConstantBuffer
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Projection;
		Vec4 LightDir;
		Col4 Emissive;
		Col4 Diffuse;
		PNStaticConstantBuffer() {
			memset(this, 0, sizeof(PNStaticConstantBuffer));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPNStatic, PNStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNStatic, VertexPositionNormal)
	DECLARE_DX11_PIXEL_SHADER(PSPNStatic)


}
//end basecross

