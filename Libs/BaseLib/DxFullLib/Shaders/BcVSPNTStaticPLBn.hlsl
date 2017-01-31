
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: pixel lighting + texture.(biased vertex normals).
// VSBasicPixelLightingTxBn
VSOutputPixelLightingTx main(VSInputNmTx vin)
{
	VSOutputPixelLightingTx vout;

	float3 normal = BiasX2(vin.Normal);

	CommonVSOutputPixelLighting cout = ComputeCommonVSOutputPixelLighting(vin.Position, normal);
	SetCommonVSOutputParamsPixelLighting;

	vout.Diffuse = float4(1, 1, 1, DiffuseColor.a);
	vout.TexCoord = vin.TexCoord;

	return vout;
}
