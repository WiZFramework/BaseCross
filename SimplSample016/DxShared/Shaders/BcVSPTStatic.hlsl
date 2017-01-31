
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"


// Vertex shader: texture.
// VSBasicTx
VSOutputTx main(VSInputTx vin)
{
	VSOutputTx vout;

	CommonVSOutput cout = ComputeCommonVSOutput(vin.Position);
	SetCommonVSOutputParams;

	vout.TexCoord = vin.TexCoord;

	return vout;
}

