#ifndef MESH_STRUCT_H
#define MESH_STRUCT_H

#include "..\Common.h"

// ���b�V���P�ʂœn�����.
struct CBUFFER_PER_MESH
{
	D3DXMATRIX	mW;				// ���[���h�s��.
	D3DXMATRIX	mWVP;			// ���[���h,�r���[,�v���W�F�N�V�����̍����ϊ��s��.
	D3DXVECTOR4 vColor;			// �F.
	D3DXVECTOR4 vShadowDepth;	// �e�̔Z��(x �̂ݎg�p).
};

// �}�e���A���P�ʂœn�����.
struct CBUFFER_PER_MATERIAL
{
	D3DXVECTOR4 vAmbient;	// �A���r�G���g�F(���F).
	D3DXVECTOR4	vDiffuse;	// �f�B�t���[�Y�F(�g�U���ːF).
	D3DXVECTOR4	vSpecular;	// �X�y�L�����F(���ʔ��ːF).
};

// �t���[���P�ʂœn�����.
struct CBUFFER_PER_FRAME
{
	D3DXVECTOR4	vCamPos;		// �J�����ʒu(���_�ʒu).
	D3DXVECTOR4	vLightDir;		// ���C�g����.
	D3DXVECTOR4 vIntensity;		// ���C�g���x(���邳 : x�̂ݎg�p).
};

#endif	// #ifndef MESH_STRUCT_H.