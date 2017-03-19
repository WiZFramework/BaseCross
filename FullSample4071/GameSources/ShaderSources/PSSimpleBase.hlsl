
#include "SimpleInc.hlsli"


float4 main(PixelShaderInput input) : SV_TARGET
{
	//ライトの向きを得る
	float3 LightDirection = -normalize(float3(LightDir.xyz));
	float3 InputNormal = normalize(input.norm);
	//ライトによるピクセルの色を決定
	float4 LightColor = saturate(dot(InputNormal, LightDirection) + float4(0.7, 0.7, 0.7, 1.0));
	//ライトの透明処理はなし
	LightColor.a = 1;
	//ピクセルの色を決定
	float4 Col = input.color;
	//テクスチャ色とライト色を合成
	return Col * LightColor;
}


