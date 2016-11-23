//--------------------------------------------------------------------------------------
// File: VSPCSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
	float4 Diffuse : packoffset(c5);
};



PSPCInput main(VSPCInput input)
{
	PSPCInput result;

	result.position = mul(input.position, MatrixTransform);
	result.color = input.color;

	return result;
}
