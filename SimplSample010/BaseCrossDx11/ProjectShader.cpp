/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	描画用のシェーダクラスの実体（マクロ使用）
	//--------------------------------------------------------------------------------------
	//PNStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNStatic.cso")

}
//end basecross


