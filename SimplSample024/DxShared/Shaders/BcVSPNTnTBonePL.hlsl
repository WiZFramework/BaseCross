#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: pixel lighting + texture. + tangent
// VSInputNmTxTangentWeights
VSOutputPixelLightingTxTangent main(VSInputNmTxTangentWeights vin)
{
	VSOutputPixelLightingTxTangent vout;

	float3 normal = SkinWithTangent(vin, vin.Normal, 4);

	CommonVSOutputPixelLighting cout = ComputeCommonVSOutputPixelLighting(vin.Position, normal);
	SetCommonVSOutputParamsPixelLighting;

	vout.Diffuse = float4(1, 1, 1, DiffuseColor.a);
	vout.TexCoord = vin.TexCoord;

	// For normal mapping, we need tangent to form tangent space transform
	vout.TangentWS = normalize(mul(vin.Tangent.xyz, WorldInverseTranspose));

	return vout;
}
