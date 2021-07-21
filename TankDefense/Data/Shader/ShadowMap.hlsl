//-------------------------------------------------.
// �R���X�^���g�o�b�t�@.
//-------------------------------------------------.
// ���b�V����.
cbuffer per_mesh : register(b0)
{
	matrix g_LightWVP; // ���C�g�̃��[���h�A�r���[�A�v���W�F�N�V����.
};

//-------------------------------------------------.
// �\����.
//-------------------------------------------------.
// ���_�o�͗p.
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
};
// ���_���͗p(�X�^�e�B�b�N���b�V��).
struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float3 Tangent : TANGENT;
	float3 Binorm : BINORMAL;
};
// ���_���͗p(�X�L�����b�V��).
struct VSSkinIn
{
	float3 Pos : POSITION; //�ʒu.  
	float3 Norm : NORMAL; //���_�@��.
	float2 Tex : TEXCOORD; //�e�N�X�`���[���W.
	float3 Tangent : TANGENT;
	float3 Binorm : BINORMAL;
	uint4 Bones : BONE_INDEX; //�{�[���̃C���f�b�N�X.
	float4 Weights : BONE_WEIGHT; //�{�[���̏d��.
};
// �X�L�j���O��̒��_�E�@��������.
struct Skin
{
	float4 Pos;
	float4 Norm;
	float4 Tangent;
	float4 Binorm;
};

// �X�^�e�B�b�N���b�V���̃��C���֐�.
VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	
	// ���C�g�s��ƍ��W���v�Z.
	output.Pos = mul(float4(input.Pos, 1.0f), g_LightWVP);
	
	return output;
}

//-----------------------------------------.
// �X�L�����b�V��.
//-----------------------------------------.
//��`.
#define MAX_BONE_MATRICES (255)	// �{�[���ő吔.
// �{�[���̃|�[�Y�s�񂪓���.
cbuffer per_bones : register(b1)
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};
//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ�.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
matrix FetchBoneMatrix(uint iBone)
{
	return g_mConstBoneWorld[iBone];
}
//���_���X�L�j���O�i�{�[���ɂ��ړ��j����.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
Skin SkinVert(VSSkinIn Input)
{
	Skin Output = (Skin) 0;

	float4 Pos = float4(Input.Pos, 1);
	float4 Norm = float4(Input.Norm, 0);
	float4 Tangent = float4(Input.Tangent, 0);
	float4 Binorm = float4(Input.Binorm, 0);
	//�{�[��0.
	uint iBone = Input.Bones.x;
	float fWeight = Input.Weights.x;
	matrix m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, m);
	Output.Tangent += fWeight * mul(Tangent, m);
	Output.Binorm += fWeight * mul(Binorm, m);
	//�{�[��1.
	iBone = Input.Bones.y;
	fWeight = Input.Weights.y;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, m);
	Output.Tangent += fWeight * mul(Tangent, m);
	Output.Binorm += fWeight * mul(Binorm, m);
	//�{�[��2.
	iBone = Input.Bones.z;
	fWeight = Input.Weights.z;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, m);
	Output.Tangent += fWeight * mul(Tangent, m);
	Output.Binorm += fWeight * mul(Binorm, m);
	//�{�[��3.
	iBone = Input.Bones.w;
	fWeight = Input.Weights.w;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, m);
	Output.Tangent += fWeight * mul(Tangent, m);
	Output.Binorm += fWeight * mul(Binorm, m);

	return Output;
}

// �o�[�e�b�N�X�V�F�[�_.
VS_OUTPUT VS_SkinMain(VSSkinIn input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	Skin vSkinned = SkinVert(input);

	// ���C�g�s��ƍ��W���v�Z.
	output.Pos = mul(vSkinned.Pos, g_LightWVP);
	
	return output;
}

// �s�N�Z���V�F�[�_�[.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	return input.Pos.z / input.Pos.w;
}