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
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSprite)
		//PCSprite
		IMPLEMENT_DX11_VERTEX_SHADER(VSPCSprite, App::GetApp()->m_wstrRelativeShadersPath  +L"VSPCSprite.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSPCSprite, App::GetApp()->m_wstrRelativeShadersPath  +L"PSPCSprite.cso")

}
//end basecross


