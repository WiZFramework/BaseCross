/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	IMPLEMENT_DX12SHADER(VSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTSprite.cso")

}
//end basecross


