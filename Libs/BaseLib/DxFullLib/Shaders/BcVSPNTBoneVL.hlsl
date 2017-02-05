
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

	CommonVSOutput cout;
	if (Activeflags.x > 0) {
		cout = ComputeCommonVSOutputWithLighting(vin.Position, normal, Activeflags.x);
	}
	else {
		cout = ComputeCommonVSOutput(vin.Position);
	}

	SetCommonVSOutputParams;

	vout.TexCoord = vin.TexCoord;

	return vout;
}

