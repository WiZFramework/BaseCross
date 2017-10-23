#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"
#include "BcINCShadow.hlsli"

Texture2D<float3> NormalTexture : register(t2);


// Pixel shader: pixel lighting + texture + no specular
//PSNormalPixelLightingTxNoSpec
float4 main(PSInputPixelLightingTxTangentShadow pin) : SV_Target0
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

	//‰e‚Ì”Z‚³
	const float3 ambient = float3(0.7f, 0.7f, 0.7f);
	float3 N = normalize(pin.norm);
	float3 L = normalize(pin.lightRay);
	float NdotL = dot(N, L);

	color = AddPixelShadow(color, ambient, pin.norm, pin.lightRay, pin.lightView, pin.lightSpacePos);

	return float4(color.xyz * (ambient + DplusS(N, L, NdotL, pin.lightView)), color.w);
}

