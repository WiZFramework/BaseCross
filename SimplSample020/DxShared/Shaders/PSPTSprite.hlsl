//--------------------------------------------------------------------------------------
// File: PSPTSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
	float4 Diffuse : packoffset(c5);
};


Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSPTInput input) : SV_TARGET
{
	//テクスチャとデフィーズからライティングを作成
	float4 Light = g_texture.Sample(g_sampler, input.tex) * saturate(Diffuse);
	//エミッシブを足す
	return saturate(Light + Emissive);
}

