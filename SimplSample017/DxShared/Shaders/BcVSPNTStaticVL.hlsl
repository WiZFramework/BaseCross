
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: vertex lighting + texture.
// VSBasicVertexLightingTx
VSOutputTx main(VSInputNmTx vin)
{

    VSOutputTx vout;

	CommonVSOutput cout = ComputeCommonVSOutputWithLighting(vin.Position, vin.Normal, Activeflags.x);

    SetCommonVSOutputParams;

    vout.TexCoord = vin.TexCoord;

    return vout;
}
