
#include "BallHeader.hlsli"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
GSPS_INPUT main(VS_INPUT input)
{
	GSPS_INPUT output = (GSPS_INPUT)0;

	output.Pos = mul(float4(input.Pos.xyz, 1), World);
	output.Norm = mul(input.Norm, (float3x3)World);
	output.Tex = input.Tex;

	return output;
}
