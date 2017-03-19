/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	struct BallConstantBuffer
	{
		XMMATRIX m_World;
		XMMATRIX m_View;
		XMMATRIX m_Proj;
		Vector3  m_LightDir;
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

