/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	//コンスタントバッファ実体マクロ
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBTest)
	//頂点シェーダ実体マクロ
	IMPLEMENT_DX11_VERTEX_SHADER(VSTest, App::GetApp()->GetShadersPath() + L"VSTest.cso")
	//ピクセルシェーダ実体マクロ
	IMPLEMENT_DX11_PIXEL_SHADER(PSTest, App::GetApp()->GetShadersPath() + L"PSTest.cso")


}
//end basecross


