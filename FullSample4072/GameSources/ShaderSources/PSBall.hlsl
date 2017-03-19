

#include "BallHeader.hlsli"


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(GSPS_INPUT input) : SV_Target
{
	float fLighting = saturate(dot(input.Norm, vLightDir) + 0.7);

	float4 cDiffuse = g_txDiffuse.Sample(samLinear, input.Tex) *fLighting;

	return cDiffuse;
}
