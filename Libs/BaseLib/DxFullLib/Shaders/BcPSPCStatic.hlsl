#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Pixel shader: basic.
// PSBasic
float4 main(PSInput pin) : SV_Target0
{
	float4 color = pin.Diffuse;

	ApplyFog(color, pin.Specular.w);

	return color;
}
