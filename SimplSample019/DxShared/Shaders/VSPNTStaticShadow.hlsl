//--------------------------------------------------------------------------------------
// File: VSPVTStaticShadow.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"


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


PSPNTInputShadow main(VSPNTInput input)
{
	PSPNTInputShadow result;
	//頂点の位置を変換
	float4 pos = float4(input.position.xyz, 1.0f);
	//ワールド変換
	pos = mul(pos, World);
	//ビュー変換
	pos = mul(pos, View);
	//射影変換
	pos = mul(pos, Projection);
	//ピクセルシェーダに渡す変数に設定
	result.position = pos;
	//ライティング
	result.norm = mul(input.norm, (float3x3)World);
	result.norm = normalize(result.norm);
	//テクスチャUV
	result.tex = input.tex;
	//影のための変数
	float4 LightModelPos = float4(input.position.xyz, 1.0f);
	//ワールド変換
	LightModelPos = mul(LightModelPos, World);
	
	float4 LightSpacePos = mul(LightModelPos, LightView);
	LightSpacePos = mul(LightSpacePos, LightProjection);
	result.lightSpacePos = LightSpacePos;

	// Light ray
	result.lightRay = LightPos.xyz - LightModelPos.xyz;
	//View
	result.lightView = EyePos.xyz - LightModelPos.xyz;




	return result;
}


