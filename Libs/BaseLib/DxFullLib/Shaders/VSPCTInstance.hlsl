
#include "INCStructs.hlsli"
#include "INCParameters.hlsli"


struct VSPCTInstanceInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float4x4 mat : MATRIX;          // インスタンスごとに設定される行列
	uint InstanceId           : SV_InstanceID;   // インスタンスＩＤ
};


PSPCTInput main(VSPCTInstanceInput input)
{
	PSPCTInput result;
	//頂点の位置を変換
	float4 pos = float4(input.position.xyz, 1.0f);
	//ワールド変換
	pos = mul(pos, input.mat);
	//ビュー変換
	pos = mul(pos, View);
	//射影変換
	pos = mul(pos, Projection);
	//ピクセルシェーダに渡す変数に設定
	result.position = pos;
	result.color = input.color;
	result.tex = input.tex;
	return result;
}
