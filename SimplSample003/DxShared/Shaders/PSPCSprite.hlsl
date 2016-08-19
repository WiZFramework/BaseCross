//--------------------------------------------------------------------------------------
// File: PSPCSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
};


float4 main(PSPCInput input) : SV_TARGET
{
	return saturate(Emissive + input.color);
}
