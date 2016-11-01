//--------------------------------------------------------------------------------------
// File: PSPNTStaticRealNoTex.hlsl
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

	float3 diffuse = zeroL * dotL;
//	float3 specular = pow(max(dotH, 0) * zeroL, SpecularPower);
	float3 specular = pow(max(dotH, 0) * zeroL, 0);

	ColorPair result;

	result.Diffuse = mul(diffuse, lightDiffuse)  * Diffuse.rgb + Emissive.rgb;
	result.Specular = mul(specular, lightSpecular) * float3(0,0,0);

	return result;
}


float4 main(PSInputPixelLightingTx pin) : SV_TARGET
{

	float4 color = pin.Diffuse;

	float3 eyeVector = normalize(EyePosition.xyz - pin.PositionWS.xyz);
	float3 worldNormal = normalize(pin.NormalWS);

	ColorPair lightResult = ComputeLights(eyeVector, worldNormal);

	color.rgb *= lightResult.Diffuse;

//	AddSpecular(color, lightResult.Specular);
//	ApplyFog(color, pin.PositionWS.w);

	return color;
}
