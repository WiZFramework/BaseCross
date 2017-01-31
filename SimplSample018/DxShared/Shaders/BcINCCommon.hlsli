
float ComputeFogFactor(float4 position)
{
    return saturate(dot(position, FogVector));
}


void ApplyFog(inout float4 color, float fogFactor)
{
    color.rgb = lerp(color.rgb, FogColor * color.a, fogFactor);
}


void AddSpecular(inout float4 color, float3 specular)
{
    color.rgb += specular * color.a;
}


float3 BiasX2(float3 x)
{
   return 2.0f * x - 1.0f;
}


struct CommonVSOutput
{
    float4 Pos_ps;
    float4 Diffuse;
    float3 Specular;
    float  FogFactor;
};


CommonVSOutput ComputeCommonVSOutput(float4 position)
{
    CommonVSOutput vout;
    
    vout.Pos_ps = mul(position, WorldViewProj);
    vout.Diffuse = DiffuseColor;
    vout.Specular = 0;
    vout.FogFactor = ComputeFogFactor(position);
    
    return vout;
}


#define SetCommonVSOutputParams \
    vout.PositionPS = cout.Pos_ps; \
    vout.Diffuse = cout.Diffuse; \
    vout.Specular = float4(cout.Specular, cout.FogFactor);


#define SetCommonVSOutputParamsNoFog \
    vout.PositionPS = cout.Pos_ps; \
    vout.Diffuse = cout.Diffuse;



float3 Skin(inout VSInputNmTxWeights vin, float3 normal, uniform int boneCount)
{
	float4x3 skinning = 0;

	[unroll]
	for (int i = 0; i < boneCount; i++)
	{
		skinning += Bones[vin.Indices[i]] * vin.Weights[i];
	}

	vin.Position.xyz = mul(vin.Position, skinning);
	return mul(normal, (float3x3)skinning);
}

