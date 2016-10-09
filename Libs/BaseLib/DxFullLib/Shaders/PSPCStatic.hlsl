//--------------------------------------------------------------------------------------
// File: PSPCStatic.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 Emissive : packoffset(c12);
	float4 Diffuse : packoffset(c13);
};

float4 main(PSPCInput input) : SV_TARGET
{
	float4 Light = input.color * saturate(Diffuse);
	return saturate(Emissive + Light);
}