//--------------------------------------------------------------------------------------
// File: PSPVTStaticShadow.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

Texture2D<float4> g_texture : register(t0);
// 深度マップ
Texture2D g_DepthMap : register(t1);
SamplerState g_sampler : register(s0);
SamplerState g_SamplerDepthMap : register(s1);


cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 LightDir	: packoffset(c12);
	float4 Emissive : packoffset(c13);
	float4 Diffuse	: packoffset(c14);
	float4 LightPos	: packoffset(c15);
	float4 EyePos	: packoffset(c16);
	uint4 Activeflags	: packoffset(c17);			//フラグ
	float4x4 LightView	: packoffset(c18);
	float4x4 LightProjection	: packoffset(c22);
};



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PSPNTInputShadow input) : SV_TARGET
{

	float shadowColor = 1.0f;
	// テクセルを計算
	float2 texel = float2(
		input.lightSpacePos.x / input.lightSpacePos.w * 0.5f + 0.5f,
		input.lightSpacePos.y / input.lightSpacePos.w * -0.5f + 0.5f
		);
	float depth = 0;
	float sm = 0;
	depth = input.lightSpacePos.z / input.lightSpacePos.w - 0.00005f;
	sm = g_DepthMap.Sample(g_SamplerDepthMap, texel).x;
	if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1){
		// 現在の深度値と深度マップ上の深度値を比較
		if (sm > depth){
			shadowColor = 1.0f;
		}
		else{
			shadowColor = 0.7f;
		}
	}

	//法線ライティング
	float3 lightdir = normalize(LightDir.xyz);
	float3 N1 = normalize(input.norm);
	float4 RetColor = saturate(dot(N1, -lightdir) + Diffuse);
	RetColor.a = Diffuse.a;
	if (Activeflags.x){
		//テクスチャとデフィーズからライティングを作成
		RetColor = g_texture.Sample(g_sampler, input.tex) * RetColor;
	}
	//エミッシブを足す
	RetColor = saturate(RetColor + Emissive);
	//影を足す
	RetColor.rgb = RetColor.rgb * shadowColor;
	return RetColor;
}