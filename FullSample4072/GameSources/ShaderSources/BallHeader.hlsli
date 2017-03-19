

Texture2D g_txDiffuse;
sampler samLinear : register(s0);

cbuffer cbChangesEveryFrame: register(b0)
{
	float4x4 World : packoffset(c0);
	float4x4 View : packoffset(c4);
	float4x4 Projection : packoffset(c8);
	float3 vLightDir : packoffset(c12);
	float Explode : packoffset(c12.w);
};


struct VS_INPUT
{
	float4 Pos          : SV_POSITION;
	float3 Norm         : NORMAL;
	float2 Tex          : TEXCOORD0;
};

struct GSPS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};
