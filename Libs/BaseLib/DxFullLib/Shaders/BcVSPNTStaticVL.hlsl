
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: vertex lighting + texture.
// VSBasicVertexLightingTx
VSOutputTx main(VSInputNmTx vin)
{

    VSOutputTx vout;

	CommonVSOutput cout;

	if (Activeflags.x > 0) {
		cout = ComputeCommonVSOutputWithLighting(vin.Position, vin.Normal, Activeflags.x);
	}
	else {
		cout = ComputeCommonVSOutput(vin.Position);
	}

    SetCommonVSOutputParams;

    vout.TexCoord = vin.TexCoord;

    return vout;
}

