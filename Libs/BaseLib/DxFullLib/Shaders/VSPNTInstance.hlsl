
#include "INCStructs.hlsli"
#include "INCParameters.hlsli"

struct VSPNTInstanceInput
{
	float4 position : SV_Position;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float4x4 mat : MATRIX;          // �C���X�^���X���Ƃɐݒ肳���s��
	uint InstanceId           : SV_InstanceID;   // �C���X�^���X�h�c
};


PSPNTInput main(VSPNTInstanceInput input)
{
	PSPNTInput result;
	//���_�̈ʒu��ϊ�
	float4 pos = float4(input.position.xyz, 1.0f);
	//���[���h�ϊ�
	pos = mul(pos, input.mat);
	//�r���[�ϊ�
	pos = mul(pos, View);
	//�ˉe�ϊ�
	pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	result.position = pos;
	//���C�e�B���O
	result.norm = mul(input.norm, (float3x3)input.mat);
	result.norm = normalize(result.norm);
	//�X�y�L�����[
	float3 H = normalize(normalize(-LightDir.xyz) + normalize(EyePos.xyz - pos.xyz));
	result.specular = Specular * dot(result.norm, H);
	//�e�N�X�`��UV
	result.tex = input.tex;
	return result;
}