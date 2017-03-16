//--------------------------------------------------------------------------------------
// File: PSPTSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"
#include "INCParameters.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSPTInput input) : SV_TARGET
{
	float4 Light = Diffuse + Emissive;
	Light.a = Diffuse.a;
	if (Activeflags.x) {
		Light = g_texture.Sample(g_sampler, input.tex) * Light;
	}
	return Light;
}

