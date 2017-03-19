/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	//シェーダ定義（マクロ使用）
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBCustomShadowmap)
	IMPLEMENT_DX11_VERTEX_SHADER(VSCustomShadowmap, App::GetApp()->GetShadersPath() + L"VSCustomShadowmap.cso")
	IMPLEMENT_DX11_GEOMETRY_SHADER(GSCustomShadowmap, App::GetApp()->GetShadersPath() + L"GSCustomShadowmap.cso")

	IMPLEMENT_DX11_CONSTANT_BUFFER(CBCustomDraw)
	IMPLEMENT_DX11_VERTEX_SHADER(VSCustomDraw, App::GetApp()->GetShadersPath() + L"VSSimpleBase.cso")
	IMPLEMENT_DX11_GEOMETRY_SHADER(GSCustomDraw, App::GetApp()->GetShadersPath() + L"GSSimpleBase.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSCustomDraw, App::App::GetApp()->GetShadersPath() + L"PSSimpleBase.cso")


}
//end basecross


