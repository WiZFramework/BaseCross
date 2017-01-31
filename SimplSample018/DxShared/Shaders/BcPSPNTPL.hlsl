#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"


// Pixel shader: pixel lighting + texture.
// PSBasicPixelLightingTx
float4 main(PSInputPixelLightingTx pin) : SV_Target0
{

	float4 color = float4(1,1,1,1);
	if (Activeflags.y > 0) {
		color = Texture.Sample(Sampler, pin.TexCoord) * pin.Diffuse;
	}

	float3 eyeVector = normalize(EyePosition - pin.PositionWS.xyz);
	float3 worldNormal = normalize(pin.NormalWS);

	ColorPair lightResult = ComputeLights(eyeVector, worldNormal, Activeflags.x);

	color.rgb *= lightResult.Diffuse;

	AddSpecular(color, lightResult.Specular);
	ApplyFog(color, pin.PositionWS.w);

	return color;
}
