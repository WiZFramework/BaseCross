
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: vertex lighting + texture. (biased vertex normals).
// VSBasicVertexLightingTxBn

VSOutputTx main(VSInputNmTx vin)
{
	VSOutputTx vout;

	float3 normal = BiasX2(vin.Normal);

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
