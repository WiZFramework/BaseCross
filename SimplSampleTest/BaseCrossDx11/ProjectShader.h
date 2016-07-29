/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//PNTStatic
	struct PNTStaticConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Color4 Emissive;
		Color4 Diffuse;
		PNTStaticConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticConstantBuffer));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStatic, PNTStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)


}
//end basecross

