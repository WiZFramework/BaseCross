/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	IMPLEMENT_DX12SHADER(VSPNStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNStatic.cso")
	IMPLEMENT_DX12SHADER(PSPNStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNStatic.cso")

}
//end basecross


