//--------------------------------------------------------------------------------------
// File: INCStructs.hlsi
//
//--------------------------------------------------------------------------------------
struct VSPCInstanceInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	// インスタンスごとに設定される行列
	float4x4 mat : MATRIX;
	// インスタンスＩＤ
	uint InstanceId : SV_InstanceID;
};

struct VSPCInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


struct PSPCInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct VSPTInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

struct VSPTInstanceInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	// インスタンスごとに設定される行列
	float4x4 mat : MATRIX;
	// インスタンスＩＤ
	uint InstanceId : SV_InstanceID;
};




struct PSPTInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

struct VSPNInput
{
	float4 position : SV_POSITION;
	float3 norm : NORMAL;
};

struct VSPNBoneInput
{
	float4 position : SV_Position;
	float3 norm   : NORMAL;
	uint4  indices  : BLENDINDICES0;
	float4 weights  : BLENDWEIGHT0;
};


struct PSPNInput
{
	float4 position : SV_POSITION;
	float3 norm : NORMAL;
};

struct PSPNInputShadow
{
	float4 position : SV_Position;
	float3 norm   : NORMAL0;
	float3 lightRay		: NORMAL1;
	float3 lightView : NORMAL2;
	float4 lightSpacePos : POSITION1;
};


struct VSPCTInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};


struct VSPCTInstanceInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	// インスタンスごとに設定される行列
	float4x4 mat : MATRIX;
	// インスタンスＩＤ
	uint InstanceId : SV_InstanceID;
};



struct PSPCTInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};


struct VSPNTInput
{
	float4 position : SV_POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD;
};

struct VSPNTInstanceInput
{
	float4 position : SV_Position;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	// インスタンスごとに設定される行列
	float4x4 mat : MATRIX;
	// インスタンスＩＤ
	uint InstanceId  : SV_InstanceID;
};


struct VSPNTBoneInput
{
	float4 position : SV_Position;
	float3 norm   : NORMAL;
	float2 tex : TEXCOORD0;
	uint4  indices  : BLENDINDICES0;
	float4 weights  : BLENDWEIGHT0;
};

struct PSPNTInput
{
	float4 position : SV_POSITION;
	float3 norm : NORMAL;
	float4 specular : COLOR;
	float2 tex : TEXCOORD;
};

struct PSPNTInputShadow
{
	float4 position : SV_Position;
	float3 norm   : NORMAL0;
	float3 lightRay		: NORMAL1;
	float3 lightView : NORMAL2;
	float4 specular : COLOR;
	float2 tex : TEXCOORD0;
	float4 lightSpacePos : POSITION1;
};

struct VSPNCTInput
{
	float4 position : SV_POSITION;
	float3 norm : NORMAL;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};

struct VSPNCTBoneInput
{
	float4 position : SV_Position;
	float3 norm   : NORMAL;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	uint4  indices  : BLENDINDICES0;
	float4 weights  : BLENDWEIGHT0;
};


struct PSPNCTInput
{
	float4 position : SV_POSITION;
	float3 norm : NORMAL;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};

struct PSPNCTInputShadow
{
	float4 position : SV_Position;
	float3 norm   : NORMAL0;
	float3 lightRay		: NORMAL1;
	float3 lightView : NORMAL2;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float4 lightSpacePos : POSITION1;
};





