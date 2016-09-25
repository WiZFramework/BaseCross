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
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBDiffuseSprite)
	//PTSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTSprite, App::GetApp()->m_wstrRelativeShadersPath  +L"VSPTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTSprite, App::GetApp()->m_wstrRelativeShadersPath  +L"PSPTSprite.cso")

}
//end basecross


