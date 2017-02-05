
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: pixel lighting + texture.
// VSNormalPixelLightingTx
VSOutputPixelLightingTxTangent main(VSInputNmTxTangent vin)
{
	VSOutputPixelLightingTxTangent vout;

	CommonVSOutputPixelLighting cout = ComputeCommonVSOutputPixelLighting(vin.Position, vin.Normal);
	SetCommonVSOutputParamsPixelLighting;

	vout.Diffuse = float4(1, 1, 1, DiffuseColor.a);
	vout.TexCoord = vin.TexCoord;

	// For normal mapping, we need tangent to form tangent space transform
	vout.TangentWS = normalize(mul(vin.Tangent.xyz, WorldInverseTranspose));

	return vout;
}