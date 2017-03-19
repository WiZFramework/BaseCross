
cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World[3]	: packoffset(c0);
	float4x4 View	: packoffset(c12);
	float4x4 Projection	: packoffset(c16);
	float4 LightDir	: packoffset(c20);
	float4 Param : packoffset(c21);
};


struct VertexShaderInput
{
	float4 pos : SV_Position;
	float3 norm : NORMAL;
	float4 color : COLOR0;
};

typedef VertexShaderInput GeometryShaderInput;

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float4 color : COLOR0;
};
