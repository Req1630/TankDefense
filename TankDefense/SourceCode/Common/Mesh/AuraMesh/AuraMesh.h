#ifndef AURA_MESH_H
#define AURA_MESH_H

#include "..\..\Common.h"

/********************************************
* �I�[�����b�V��.
**/
class CAuraMesh : public CCommon
{
	// �R���X�^���g�o�b�t�@�̃A�v�����̒�`.
	//���V�F�[�_�[���̃R���X�^���g�o�b�t�@�ƈ�v���Ă���K�v����.
	struct C_BUFFER
	{
		D3DXMATRIX	mWVP;		// ���[���h,�r���[,�v���W�F�N�V�����̍����ϊ��s��.
		D3DXMATRIX	mW;			// ���[���h�s��.
		D3DXVECTOR4	vColor;		// �J���[(RGBA�̌^�ɍ��킹��).
		D3DXVECTOR4	vUV;		// UV.
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
	};
public:
	CAuraMesh();
	virtual ~CAuraMesh();

	// ������.
	HRESULT Init();
	// ����֐�.
	void Release();

	// �X�V.
	void Update();
	// �`��.
	void Render();


private:
	// ���f���̍쐬.
	HRESULT InitModel();
	// �V�F�[�_�쐬.
	HRESULT InitShader();

private:
	ID3D11Buffer*		m_pVertexBuffer;	// ���_�o�b�t�@.
	ID3D11Buffer*		m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@.

	ID3D11VertexShader*	m_pVertexShader;	// ���_�V�F�[�_�[.
	ID3D11PixelShader*	m_pPixelShader;		// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*	m_pVertexLayout;	// ���_���C�A�E�g.
	ID3D11Buffer*		m_pConstantBuffer;	// �R���X�^���g�o�b�t�@.

	float				m_ScrollTime;
};

#endif	// #ifndef AURA_MESH_H.