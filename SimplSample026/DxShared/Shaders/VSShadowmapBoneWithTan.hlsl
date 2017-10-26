//--------------------------------------------------------------------------------------
// File: VSShadowmapBoneWithTan.hlsl
//
//--------------------------------------------------------------------------------------


#include "INCShadowmap.hlsli"


void SkinWithTangent(inout VS_INPUT_TANGENT_WEIGHTS vin, uniform int boneCount)
{
	float4x3 skinning = 0;

	[unroll]
	for (int i = 0; i < boneCount; i++)
	{
		skinning += Bones[vin.Indices[i]] * vin.Weights[i];
	}

	vin.vPosition.xyz = mul(vin.vPosition, skinning);
	vin.Normal = mul(vin.Normal, (float3x3)skinning);
	vin.Tangent.xyz = mul((float3)vin.Tangent, (float3x3)skinning);
}


//--------------------------------------------------------------------------------------
// Vertex Shader With Bone Tangent
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT_TANGENT_WEIGHTS InputWeights)
{
	VS_OUTPUT Output;
	InputWeights.vPosition.w = 1.0f;

	SkinWithTangent(InputWeights, 4);


	Output.vPosition = mul(InputWeights.vPosition, World);
	Output.vPosition = mul(Output.vPosition, View);
	Output.vPosition = mul(Output.vPosition, Projection);

	return Output;
}

