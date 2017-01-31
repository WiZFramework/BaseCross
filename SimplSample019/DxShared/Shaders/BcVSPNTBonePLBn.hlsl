
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: pixel lighting + texture.(biased vertex normals).
// VSSkinnedPixelLightingFourBonesBn
VSOutputPixelLightingTx main(VSInputNmTxWeights vin)
{
	VSOutputPixelLightingTx vout;

	float3 normal = BiasX2(vin.Normal);

	normal = Skin(vin, normal, 4);

	CommonVSOutputPixelLighting cout = ComputeCommonVSOutputPixelLighting(vin.Position, normal);
	SetCommonVSOutputParamsPixelLighting;

	vout.Diffuse = float4(1, 1, 1, DiffuseColor.a);
	vout.TexCoord = vin.TexCoord;

	return vout;
}
