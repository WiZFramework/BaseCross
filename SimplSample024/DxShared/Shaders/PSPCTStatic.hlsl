//--------------------------------------------------------------------------------------
// File: PSPTSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 Emissive : packoffset(c12);
	float4 Diffuse	: packoffset(c13);
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSPCTInput input) : SV_TARGET
{
	float4 Light = (saturate(input.color) * Diffuse) + Emissive;
	Light.a = Diffuse.a;
	Light = g_texture.Sample(g_sampler, input.tex) * Light;
	return Light;
}

