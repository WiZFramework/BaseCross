
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"


// Vertex shader: pixel lighting, four bones.
// VSSkinnedPixelLightingFourBones
VSOutputPixelLightingTx main(VSInputNmTxWeights vin)
{
	VSOutputPixelLightingTx vout;

	float3 normal = Skin(vin, vin.Normal, 4);

	CommonVSOutputPixelLighting cout = ComputeCommonVSOutputPixelLighting(vin.Position, normal);
	SetCommonVSOutputParamsPixelLighting;

	vout.Diffuse = float4(1, 1, 1, DiffuseColor.a);
	vout.TexCoord = vin.TexCoord;

	return vout;
}
