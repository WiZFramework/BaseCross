
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: vertex lighting + texture. (biased vertex normals).
// VSBasicVertexLightingTxBn

VSOutputTxShadow main(VSInputNmTx vin)
{
	VSOutputTxShadow vout;

	float3 normal = BiasX2(vin.Normal);

	CommonVSOutput cout;
	if (Activeflags.x > 0) {
		cout = ComputeCommonVSOutputWithLighting(vin.Position, normal, Activeflags.x);
	}
	else {
		cout = ComputeCommonVSOutput(vin.Position);
	}

	SetCommonVSOutputParams;

	vout.TexCoord = vin.TexCoord;

	//âeóp
	vout.norm = mul(vin.Normal, (float3x3)World);
	vout.norm = normalize(vout.norm);
	float4 LightModelPos = float4(vin.Position.xyz, 1.0f);
	//ÉèÅ[ÉãÉhïœä∑
	LightModelPos = mul(LightModelPos, World);
	float4 LightSpacePos = mul(LightModelPos, LightView);
	LightSpacePos = mul(LightSpacePos, LightProjection);
	vout.lightSpacePos = LightSpacePos;
	// Light ray
	vout.lightRay = LightPos.xyz - LightModelPos.xyz;
	//View
	vout.lightView = EyePos.xyz - LightModelPos.xyz;


	return vout;
}
