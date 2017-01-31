
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"



// Vertex shader: vertex lighting, four bones.
// VSSkinnedVertexLightingFourBones
VSOutputTx main(VSInputNmTxWeights vin)
{
	VSOutputTx vout;

	float3 normal = Skin(vin, vin.Normal, 4);

	CommonVSOutput cout = ComputeCommonVSOutputWithLighting(vin.Position, normal, Activeflags.x);
	SetCommonVSOutputParams;

	vout.TexCoord = vin.TexCoord;

	return vout;
}

