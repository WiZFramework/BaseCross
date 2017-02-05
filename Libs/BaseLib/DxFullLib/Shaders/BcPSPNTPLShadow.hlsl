#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"
#include "BcINCShadow.hlsli"

// Pixel shader: pixel lighting + texture.
// PSBasicPixelLightingTx
float4 main(PSInputPixelLightingTxShadow pin) : SV_Target0
{

	float4 color = float4(1,1,1,1);
	if (Activeflags.y > 0) {
		color = Texture.Sample(Sampler, pin.TexCoord) * pin.Diffuse;
	}
	if (Activeflags.x > 0) {
		float3 eyeVector = normalize(EyePosition - pin.PositionWS.xyz);
		float3 worldNormal = normalize(pin.NormalWS);

		ColorPair lightResult = ComputeLights(eyeVector, worldNormal, Activeflags.x);

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

	color = AddPixelShadow(color, ambient,pin.norm, pin.lightRay, pin.lightView, pin.lightSpacePos);

	return float4(color.xyz * (ambient + DplusS(N, L, NdotL, pin.lightView)), color.w);
}

