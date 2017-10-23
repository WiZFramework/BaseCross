
#include "INCStructs.hlsli"
#include "INCSimpleConstant.hlsli"
/*
cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 Emissive : packoffset(c12);
	float4 Diffuse	: packoffset(c13);
};
*/


PSPCTInput main(VSPCTInstanceInput input)
{
	PSPCTInput result;
	//頂点の位置を変換
	float4 pos = float4(input.position.xyz, 1.0f);
	//ワールド変換
	pos = mul(pos, input.mat);
	//ビュー変換
	pos = mul(pos, View);
	//射影変換
	pos = mul(pos, Projection);
	//ピクセルシェーダに渡す変数に設定
	result.position = pos;
	result.color = input.color;
	result.tex = input.tex;
	return result;

}
