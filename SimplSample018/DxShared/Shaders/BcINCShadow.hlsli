
// [“xƒ}ƒbƒv
Texture2D g_DepthMap : register(t1);
SamplerComparisonState g_SamplerDepthMap : register(s1);

float3 DplusS(float3 N, float3 L, float NdotL, float3 view)
{
	const float3 Kdiffuse = float3(.5f, .5f, .4f);
	const float3 Kspecular = float3(.2f, .2f, .3f);
	const float exponent = 3.f;

	// Compute the diffuse coefficient.
	float diffuseConst = saturate(NdotL);

	// Compute the diffuse lighting value.
	float3 diffuse = Kdiffuse * diffuseConst;

	// Compute the specular highlight.
	float3 R = reflect(-L, N);
	float3 V = normalize(view);
	float3 RdotV = dot(R, V);
	float3 specular = Kspecular * pow(saturate(RdotV), exponent);

	return (diffuse + specular);
}



float4 AddPixelShadow(float4 color, float3 ambient, float3 norm, float3 lightRay, float3 lightView, float4 lightSpacePos) {
	float3 N = normalize(norm);
	float3 L = normalize(lightRay);
	float NdotL = dot(N, L);

	float2 shadowTexCoords;
	shadowTexCoords.x = 0.5f + (lightSpacePos.x / lightSpacePos.w * 0.5f);
	shadowTexCoords.y = 0.5f - (lightSpacePos.y / lightSpacePos.w * 0.5f);
	float pixelDepth = lightSpacePos.z / lightSpacePos.w;

	float lighting = 1;

	if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
		(saturate(shadowTexCoords.y) == shadowTexCoords.y) &&
		(pixelDepth > 0))
	{

		float margin = acos(saturate(NdotL));
		float epsilon = 0.0001 / margin;

		epsilon = clamp(epsilon, 0, 0.1);

		lighting = float(g_DepthMap.SampleCmpLevelZero(
			g_SamplerDepthMap,
			shadowTexCoords,
			pixelDepth + epsilon
		)
			);
		if (lighting == 0.f)
		{
			//‰e‚Ì’†
			return float4(color.xyz * ambient, color.w);
		}
		else if (lighting < 1.0f)
		{
			//‰e‚Æ‰eˆÈŠO‚Ì‹«ŠEü
			float3 light = lighting * (ambient + DplusS(N, L, NdotL, lightView));
			float3 shadow = (1.0f - lighting) * ambient;
			return float4(color.xyz * (light + shadow), color.w);
		}
		else {
			return color;
		}
	}
	else {
		return color;
	}
}

