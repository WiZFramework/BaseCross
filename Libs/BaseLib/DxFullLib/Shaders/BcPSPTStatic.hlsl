#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"


// Pixel shader: texture.
// PSBasicTx
float4 main(PSInputTx pin) : SV_Target0
{
	float4 color = pin.Diffuse;
	if (Activeflags.y > 0) {
		color = Texture.Sample(Sampler, pin.TexCoord) * pin.Diffuse;
	}

	ApplyFog(color, pin.Specular.w);

	return color;
}

