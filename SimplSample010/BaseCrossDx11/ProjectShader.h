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
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Color4 Emissive;
		Color4 Diffuse;
		PNStaticConstantBuffer() {
			memset(this, 0, sizeof(PNStaticConstantBuffer));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPNStatic, PNStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNStatic, VertexPositionNormal)
	DECLARE_DX11_PIXEL_SHADER(PSPNStatic)


}
//end basecross

