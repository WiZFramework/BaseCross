
//--------------------------------------------------------------------------------------
// File: VSPNTBoneShadow.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"
#include "INCSimpleConstant.hlsli"

void Skin(inout VSPNTBoneInput vin, uniform int boneCount)
{
	float4x3 skinning = 0;

	[unroll]
	for (int i = 0; i < boneCount; i++)
	{
		skinning += Bones[vin.indices[i]] * vin.weights[i];
	}

	vin.position.xyz = mul(vin.position, skinning);
	vin.norm = mul(vin.norm, (float3x3)skinning);
}


PSPNTInputShadow main(VSPNTBoneInput input)
{
	PSPNTInputShadow result;

	Skin(input, 4);

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
	//スペキュラー
	float3 H = normalize(normalize(-LightDir.xyz) + normalize(EyePos.xyz - pos.xyz));
	result.specular = Specular * dot(result.norm, H);
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

