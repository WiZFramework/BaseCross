//--------------------------------------------------------------------------------------
// File: VSShadowmapBone.hlsl
//
//--------------------------------------------------------------------------------------


#include "INCShadowmap.hlsli"

void Skin(inout VS_INPUT_WEIGHTS vin, uniform int boneCount)
{
    float4x3 skinning = 0;

    [unroll]
    for (int i = 0; i < boneCount; i++)
    {
        skinning += Bones[vin.Indices[i]] * vin.Weights[i];
    }

    vin.vPosition.xyz = mul(vin.vPosition, skinning);
    vin.Normal = mul(vin.Normal, (float3x3)skinning);
}

//--------------------------------------------------------------------------------------
// Vertex Shader With Bone
//--------------------------------------------------------------------------------------
VS_OUTPUT main( VS_INPUT_WEIGHTS InputWeights )
{
    VS_OUTPUT Output;
	InputWeights.vPosition.w = 1.0f;

    Skin(InputWeights, 4);


    Output.vPosition = mul( InputWeights.vPosition, World );
    Output.vPosition = mul( Output.vPosition, View );
    Output.vPosition = mul( Output.vPosition, Projection );

    return Output;
}

