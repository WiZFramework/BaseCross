
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"


// Vertex shader: vertex color + texture .
// VSBasicTxVc
VSOutputTx main(VSInputVcTx vin)
{
	VSOutputTx vout;

	CommonVSOutput cout = ComputeCommonVSOutput(vin.Position);
	SetCommonVSOutputParams;

	vout.TexCoord = vin.TexCoord;
	vout.Diffuse *= vin.Color;

	return vout;
}
