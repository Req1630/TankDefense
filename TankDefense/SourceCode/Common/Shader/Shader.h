/**
* @file Shader.h.
* @brief �V�F�[�_�[�쐬�p�֐����܂Ƃ߂��w�b�_�[.
* @author ���c���.
*/
#ifndef SHADER_H
#define SHADER_H

#include "..\Common.h"

namespace shader
{
// �V�F�[�_�[�Ǎ��֐�.
HRESULT InitShader(
	const char* filePath,
	const char* entryPoint,
	const char* shaderModel,
	UINT compileFlag,
	ID3DBlob** ppCompiledShader,
	ID3DBlob** ppErrors );

// �o�[�e�b�N�X�V�F�[�_�[�쐬�֐�.
HRESULT CreateVertexShader(
	ID3D11Device*			pDevice11,
	ID3DBlob*				pCompiledShader,
	ID3D11VertexShader**	pVertexShader );

// �s�N�Z���X�V�F�[�_�[�쐬�֐�.
HRESULT CreatePixelShader(
	ID3D11Device*		pDevice11,
	ID3DBlob*			pCompiledShader,
	ID3D11PixelShader**	pPixelShader );

// �C���v�b�g���C�A�E�g�쐬�֐�.
HRESULT CreateInputLayout(
	ID3D11Device*				pDevice11,
	D3D11_INPUT_ELEMENT_DESC*	pLayout,
	UINT						numElements,
	ID3DBlob*					pCompiledShader, 
	ID3D11InputLayout**			m_pVertexLayout );

// �R���X�^���g�o�b�t�@�쐬�֐�.
HRESULT CreateConstantBuffer( 
	ID3D11Device*	pDevice11,
	ID3D11Buffer**	pConstantBuffer,
	UINT			size );

// POSITION�̎擾.
D3D11_INPUT_ELEMENT_DESC GetPositionInputElement();
// TEXCOORD�̎擾.
D3D11_INPUT_ELEMENT_DESC GetTexcoordInputElement();

// �u���u��ǂݍ��񂾍ۂ̃G���[���b�Z�[�W���擾.
const char* GetBlobErrorMsg( ID3DBlob* pBlob );

};
#endif	// #ifndef SHADER_H.