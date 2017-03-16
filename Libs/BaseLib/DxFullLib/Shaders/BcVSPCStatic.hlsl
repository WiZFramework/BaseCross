
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: vertex color.
// VSBasicVc
VSOutput main(VSInputVc vin)
{
	VSOutput vout;

	CommonVSOutput cout = ComputeCommonVSOutput(vin.Position);
	SetCommonVSOutputParams;

	vout.Diffuse *= vin.Color;

	return vout;
}

