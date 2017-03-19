
#include "BallHeader.hlsli"

//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(12)]
void main(triangle GSPS_INPUT input[3], inout TriangleStream<GSPS_INPUT> TriStream)
{
	GSPS_INPUT output;

	//
	// Calculate the face normal
	//
	float3 faceEdgeA = input[1].Pos.xyz - input[0].Pos.xyz;
	float3 faceEdgeB = input[2].Pos.xyz - input[0].Pos.xyz;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));
	float3 ExplodeAmt = faceNormal*Explode;

	//
	// Calculate the face center
	//
	float3 centerPos = (input[0].Pos.xyz + input[1].Pos.xyz + input[2].Pos.xyz) / 3.0;
	float2 centerTex = (input[0].Tex + input[1].Tex + input[2].Tex) / 3.0;
	centerPos += faceNormal*Explode;

	//
	// Output the pyramid
	//
	for (uint i = 0; i<3; i++)
	{
		output.Pos = input[i].Pos + float4(ExplodeAmt, 0);
		output.Pos = mul(output.Pos, View);
		output.Pos = mul(output.Pos, Projection);
		output.Norm = input[i].Norm;
		output.Tex = input[i].Tex;
		TriStream.Append(output);

		uint iNext = (i + 1) % 3;
		output.Pos = input[iNext].Pos + float4(ExplodeAmt, 0);
		output.Pos = mul(output.Pos, View);
		output.Pos = mul(output.Pos, Projection);
		output.Norm = input[iNext].Norm;
		output.Tex = input[iNext].Tex;
		TriStream.Append(output);

		output.Pos = float4(centerPos, 1) + float4(ExplodeAmt, 0);
		output.Pos = mul(output.Pos, View);
		output.Pos = mul(output.Pos, Projection);
		output.Norm = faceNormal;
		output.Tex = centerTex;
		TriStream.Append(output);

		TriStream.RestartStrip();
	}

	for (int j = 2; j >= 0; j--)
	{
		output.Pos = input[j].Pos + float4(ExplodeAmt, 0);
		output.Pos = mul(output.Pos, View);
		output.Pos = mul(output.Pos, Projection);
		output.Norm = -input[j].Norm;
		output.Tex = input[j].Tex;
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}
