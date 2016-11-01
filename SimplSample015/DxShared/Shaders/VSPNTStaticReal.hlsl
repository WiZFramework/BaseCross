
//--------------------------------------------------------------------------------------
// File: VSPNTStaticReal.hlsl
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
	float4 EyePosition              : packoffset(c15);
};

struct VSOutputPixelLightingTx
{
	float2 TexCoord   : TEXCOORD0;
	float4 PositionWS : TEXCOORD1;
	float3 NormalWS   : TEXCOORD2;
	float4 Diffuse    : COLOR0;
	float4 PositionPS : SV_Position;
};

struct CommonVSOutputPixelLighting
{
	float4 Pos_ps;
	float3 Pos_ws;
	float3 Normal_ws;
	float  FogFactor;
};


float ComputeFogFactor(float4 position, float4 FogVector)
{
	return saturate(dot(position, FogVector));
}


CommonVSOutputPixelLighting ComputeCommonVSOutputPixelLighting(float4 position, float3 normal)
{
	CommonVSOutputPixelLighting vout;

	//頂点の位置を変換
	float4 pos = float4(position.xyz, 1.0f);
	//ワールド変換
	pos = mul(pos, World);
	vout.Pos_ws = pos.xyz;
	//ビュー変換
	pos = mul(pos, View);
	//射影変換
	pos = mul(pos, Projection);
	vout.Pos_ps = pos;
	vout.Normal_ws = normalize(mul(normal, (float3x3)World));

	vout.FogFactor = ComputeFogFactor(position, float4(0, 0, 0, 1));

	return vout;
}



VSOutputPixelLightingTx main(VSPNTInput vin)
{
	VSOutputPixelLightingTx vout;

	CommonVSOutputPixelLighting cout = ComputeCommonVSOutputPixelLighting(vin.position, vin.norm);

	vout.PositionPS = cout.Pos_ps;
	vout.PositionWS = float4(cout.Pos_ws, cout.FogFactor);
	vout.NormalWS = cout.Normal_ws;

	vout.Diffuse = float4(1, 1, 1, Diffuse.a);
	vout.TexCoord = vin.tex;

	return vout;
}

