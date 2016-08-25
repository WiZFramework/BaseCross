/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	IMPLEMENT_DX12SHADER(VSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCStatic.cso")
	IMPLEMENT_DX12SHADER(PSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCStatic.cso")

}
//end basecross


