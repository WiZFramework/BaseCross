
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"


// Vertex shader: vertex lighting, four bones. (biased vertex normals).
// VSSkinnedVertexLightingFourBonesBn
VSOutputTx main(VSInputNmTxWeights vin)
{
	VSOutputTx vout;

	float3 normal = BiasX2(vin.Normal);

	normal = Skin(vin, normal, 4);

	CommonVSOutput cout = ComputeCommonVSOutputWithLighting(vin.Position, normal, Activeflags.x);
	SetCommonVSOutputParams;

	vout.TexCoord = vin.TexCoord;

	return vout;
}

