//--------------------------------------------------------------------------------------
// File: VSPTSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"


cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
	float4 Diffuse : packoffset(c5);
};



PSPTInput main(VSPTInput input)
{
	PSPTInput result;

	result.position = mul(input.position, MatrixTransform);
	result.tex = input.tex;

	return result;
}
