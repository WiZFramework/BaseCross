//--------------------------------------------------------------------------------------
// File: PSPNTStaticReal.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 LightDir	: packoffset(c12);
	float4 Emissive : packoffset(c13);
	float4 Diffuse	: packoffset(c14);
	float4x4 WorldInverseTranspose  : packoffset(c15);
	float4 EyePosition              : packoffset(c19);
};


struct PSInputPixelLightingTx
{
	float2 TexCoord   : TEXCOORD0;
	float4 PositionWS : TEXCOORD1;
	float3 NormalWS   : TEXCOORD2;
	float4 Diffuse    : COLOR0;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

struct ColorPair
{
	float3 Diffuse;
	float3 Specular;
};


ColorPair ComputeLights(float3 eyeVector, float3 worldNormal)
{
	float3 lightDirections = 0;
	float3 lightDiffuse = 0;
	float3 lightSpecular = 0;
	float3 halfVectors = 0;

	lightDirections = LightDir.xyz;
	lightDiffuse = float3(1,1,1);
	lightSpecular = float3(0, 0, 0);
	halfVectors = normalize(eyeVector - lightDirections);


	float3 dotL = mul(-lightDirections, worldNormal);
	float3 dotH = mul(halfVectors, worldNormal);

	float3 zeroL = step(0, dotL);

	float SpecularPower = 0.2;
	float3 SpecularColor = float3(0.2,0.2,0.2);

	float3 diffuse = zeroL * dotL;
//	float3 specular = pow(max(dotH, 0) * zeroL, SpecularPower);
	float3 specular = pow(max(dotH, 0) * zeroL, 0);

	ColorPair result;

	result.Diffuse = mul(diffuse, lightDiffuse)  * Diffuse.rgb + Emissive.rgb;
	result.Specular = mul(specular, lightSpecular) * SpecularColor;

	return result;
}

void AddSpecular(inout float4 color, float3 specular)
{
	color.rgb += specular * color.a;
}

float4 main(PSInputPixelLightingTx pin) : SV_TARGET
{

	float4 color = g_texture.Sample(g_sampler, pin.TexCoord) * pin.Diffuse;

	float3 eyeVector = normalize(EyePosition.xyz - pin.PositionWS.xyz);
	float3 worldNormal = normalize(pin.NormalWS);

	ColorPair lightResult = ComputeLights(eyeVector, worldNormal);

	color.rgb *= lightResult.Diffuse;

//	AddSpecular(color, lightResult.Specular);
//	ApplyFog(color, pin.PositionWS.w);

	return color;
}
