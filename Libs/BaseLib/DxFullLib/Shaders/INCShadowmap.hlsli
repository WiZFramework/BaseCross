//--------------------------------------------------------------------------------------
// File: ShadowmapParameters.hlsli
//
//--------------------------------------------------------------------------------------

cbuffer ShadowmapConstantBuffer : register( b0 )
{
	float4x4 World				: packoffset(c0);
	float4x4 View				: packoffset(c4);
	float4x4 Projection				: packoffset(c8);
    float4x3 Bones[72]              : packoffset(c12);
};

struct VS_INPUT
{
    float4 vPosition    : SV_POSITION;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct VS_INPUT_WEIGHTS
{
    float4 vPosition    : SV_POSITION;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD0;
    uint4  Indices  : BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};


struct VS_OUTPUT
{
    float4 vPosition    : SV_POSITION;
};
