/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	IMPLEMENT_DX11_CONSTANT_BUFFER(CBBall)
	IMPLEMENT_DX11_VERTEX_SHADER(VSBall, App::GetApp()->GetShadersPath() + L"VSBall.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSBall, App::GetApp()->GetShadersPath() + L"PSBall.cso")
	IMPLEMENT_DX11_GEOMETRY_SHADER(GSBall, App::GetApp()->GetShadersPath() + L"GSBall.cso")

}
//end basecross


