//--------------------------------------------------------------------------------------
// File: VSPTSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"
#include "INCParameters.hlsli"

PSPTInput main(VSPTInput input)
{
	PSPTInput result;
	//頂点の位置を変換
	float4 pos = float4(input.position.xyz, 1.0f);
	//ワールド変換
	pos = mul(pos, World);
	//ビュー変換
	pos = mul(pos, View);
	//射影変換
	pos = mul(pos, Projection);
	//ピクセルシェーダに渡す変数に設定
	result.position = pos;
	result.tex = input.tex;

	return result;
}
