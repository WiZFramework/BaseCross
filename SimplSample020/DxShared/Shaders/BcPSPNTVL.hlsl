#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Pixel shader: vertex lighting + texture.
// PSBasicVertexLightingTx
float4 main(PSInputTx pin) : SV_Target0
{
	float4 color = float4(1,1,1,1);
	if (Activeflags.y > 0) {
		color = Texture.Sample(Sampler, pin.TexCoord) * pin.Diffuse;
	}
	else {
		color *= pin.Diffuse;
	}

    AddSpecular(color, pin.Specular.rgb);
    ApplyFog(color, pin.Specular.w);

    return color;
}
