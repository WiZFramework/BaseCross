//--------------------------------------------------------------------------------------
// File: PSPCStatic.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 Emissive : packoffset(c12);
};

float4 main(PSPCInput input) : SV_TARGET
{
	return saturate(Emissive + input.color);
}