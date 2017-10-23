//--------------------------------------------------------------------------------------
// File: VSShadowmap.hlsl
//
//--------------------------------------------------------------------------------------


#include "INCShadowmap.hlsli"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main( VS_INPUT Input )
{
    VS_OUTPUT Output;
	Input.vPosition.w = 1.0f;
    Output.vPosition = mul( Input.vPosition, World );
    Output.vPosition = mul( Output.vPosition, View );
    Output.vPosition = mul( Output.vPosition, Projection );

    return Output;
}

