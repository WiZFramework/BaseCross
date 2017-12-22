//--------------------------------------------------------------------------------------
// File: PSPCTSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"


Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
};

float4 main(PSPCTInput input) : SV_TARGET
{
	//テクスチャとカラーから仮想ライティングを作成
	float4 Light = g_texture.Sample(g_sampler, input.tex) * input.color;
	//エミッシブを足す
	return saturate(Light + Emissive);
}

