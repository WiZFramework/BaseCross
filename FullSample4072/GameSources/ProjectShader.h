/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	struct BallConstantBuffer
	{
		Mat4x4 m_World;
		Mat4x4 m_View;
		Mat4x4 m_Proj;
		Vec3  m_LightDir;
		float m_Explode;
		BallConstantBuffer() {
			memset(this, 0, sizeof(BallConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBBall, BallConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSBall, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSBall)
	DECLARE_DX11_GEOMETRY_SHADER(GSBall)




}
//end basecross

