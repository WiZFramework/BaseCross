
Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);



cbuffer Parameters : register(b0)
{
	float4 DiffuseColor             : packoffset(c0);
	float3 EmissiveColor            : packoffset(c1);
	float3 SpecularColor            : packoffset(c2);
	float  SpecularPower            : packoffset(c2.w);

	float3 LightDirection[3]        : packoffset(c3);
	float3 LightDiffuseColor[3]     : packoffset(c6);
	float3 LightSpecularColor[3]    : packoffset(c9);

	float3 EyePosition              : packoffset(c12);

	float3 FogStartEnd            : packoffset(c13);
	float  FogStart : packoffset(c13.x);
	float  FogEnd : packoffset(c13.y);
	float3 FogColor                 : packoffset(c14);

	float4x4 World                  : packoffset(c15);
	float4x4 View                   : packoffset(c19);
	float4x4 Projection             : packoffset(c23);
	float4x4 WorldView              : packoffset(c27);	//Transpose‚³‚ê‚È‚¢WorldView

	uint4 Activeflags              : packoffset(c31);			//ƒtƒ‰ƒO

	float4 LightPos                 : packoffset(c32);
	float4 EyePos                   : packoffset(c33);
	float4x4 LightView              : packoffset(c34);
	float4x4 LightProjection        : packoffset(c38);


};
