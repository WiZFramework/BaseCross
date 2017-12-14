#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

Texture2D<float3> NormalTexture : register(t1);


// Pixel shader: pixel lighting + texture + no specular
//PSNormalPixelLightingTxNoSpec
float4 main(PSInputPixelLightingTxTangent pin) : SV_Target0
{
	float3 eyeVector = normalize(EyePosition - pin.PositionWS.xyz);

	// Before lighting, peturb the surface's normal by the one given in normal map.
	float3 localNormal = BiasX2(NormalTexture.Sample(Sampler, pin.TexCoord).xyz);
	float3 normal = PeturbNormal(localNormal, pin.NormalWS, pin.TangentWS);

	// Get color from albedo texture
	float4 color = float4(1, 1, 1, 1);
	if (Activeflags.y > 0) {
		color = Texture.Sample(Sampler, pin.TexCoord) * pin.Diffuse;
	}

	if (Activeflags.x > 0) {
		// Do lighting
		ColorPair lightResult = ComputeLights(eyeVector, normal, Activeflags.x);
		color.rgb *= lightResult.Diffuse;

		AddSpecular(color, lightResult.Specular);
		ApplyFog(color, pin.PositionWS.w);
	}
	else {
		ApplyFog(color, pin.PositionWS.w);
	}
	return color;
}

