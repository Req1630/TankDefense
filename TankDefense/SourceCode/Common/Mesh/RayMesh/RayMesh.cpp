#include "RayMesh.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Utility\Math\Math.h"

namespace
{
	const char* SHADER_NAME = "Data\\Shader\\RayMesh.hlsl";
};

CRayMesh::CRayMesh()
	: m_pVertexShader	( nullptr )
	, m_pPixelShader	( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pConstantBuffer	( nullptr )
	, m_pVertexBuffer	( nullptr )
	, m_Vector			( 0.0f, 0.0f, 0.0f )
{
}

CRayMesh::~CRayMesh()
{
	Release();
}

// �������֐�.
HRESULT CRayMesh::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( SetDevice( pContext11 ) ))	return E_FAIL;
	if( FAILED( InitShader() ))				return E_FAIL;
	if( FAILED( InitVertexBuffer() ))		return E_FAIL;
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBuffer, sizeof(C_BUFFER_PER_FRAME) ))) return E_FAIL;

	return S_OK;
}

// ���.
void CRayMesh::Release()
{
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pVertexBuffer );

	
	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;
}

// �`��֐�.
void CRayMesh::Render()
{
	D3DXMATRIX mWorld, mScale, mRot, mTran;

	// �g��k���s��쐬.
	D3DXMatrixScaling( &mScale, m_Tranceform.Scale.x, m_Tranceform.Scale.y, m_Tranceform.Scale.z );
	// ���s�ړ��s��.
	D3DXMatrixTranslation( &mTran, m_Tranceform.Position.x, m_Tranceform.Position.y, m_Tranceform.Position.z );
	//��]�s��.
	D3DXQUATERNION q = Math::GetRotationBetween( Math::Z_AXIS, m_Vector );
	D3DXMatrixRotationQuaternion( &mRot, &q );

	// ���[���h�s��쐬.
	mWorld = mScale * mRot * mTran;

	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER_PER_FRAME cb;	// �R���X�^���g�o�b�t�@.

	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData ))){

		cb.mWVP	= mWorld * CCameraManager::GetViewProjMatrix();
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );

		cb.vColor = m_Color;

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}

	// �V�F�[�_�[�̐ݒ�.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );

	// �R���X�^���g�o�b�t�@�̐ݒ�.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );

	// ���_���̐ݒ�.
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

	// �`��.
	m_pContext11->Draw( 2, 0 );
}

// �V�F�[�_�[�̍쐬.
HRESULT CRayMesh::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	//===========================.
	// ���_�V�F�[�_�[�̍쐬.
	//===========================.
	// HLSL����o�[�e�b�N�X�V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		shader::InitShader(
			SHADER_NAME,		// �V�F�[�_�[�t�@�C����.
			"VS_Main",			// �V�F�[�_�[�G���g���[�|�C���g.
			"vs_5_0",			// �V�F�[�_�[���f��.
			uCompileFlag,		// �V�F�[�_�[�R���p�C���t���O.
			&pCompiledShader,	// �u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors ))) {		// �G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		ERROR_MESSAGE( shader::GetBlobErrorMsg( pErrors ) );
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// ��L�ō쐬�����u���u����u���_�V�F�[�_�[�v���쐬.
	if( FAILED( shader::CreateVertexShader(
		m_pDevice11,
		pCompiledShader, 
		&m_pVertexShader ))) {
		ERROR_MESSAGE("���_�V�F�[�_�[�쐬 : ���s");
		return E_FAIL;
	}

	// ���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		shader::GetPositionInputElement(),
	};

	// ���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// ���_�C���v�b�g���C�A�E�g���쐬.
	if( FAILED(
		shader::CreateInputLayout(
			m_pDevice11,
			layout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout))) {
		ERROR_MESSAGE("���_�C���v�b�g���C�A�E�g�쐬 : ���s");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);


	//===========================.
	// �s�N�Z���V�F�[�_�[�̍쐬.
	//===========================.
	// HLSL����s�N�Z���V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		shader::InitShader(
			SHADER_NAME,		// �V�F�[�_�[�t�@�C����.
			"PS_Main",			// �V�F�[�_�[�G���g���[�|�C���g.
			"ps_5_0",			// �V�F�[�_�[���f��.
			uCompileFlag,		// �V�F�[�_�[�R���p�C���t���O.
			&pCompiledShader,	// �u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors ))) {		// �G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		ERROR_MESSAGE( shader::GetBlobErrorMsg( pErrors ) );
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// ��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�[�v���쐬.
	if( FAILED( shader::CreatePixelShader(
		m_pDevice11,
		pCompiledShader, 
		&m_pPixelShader ))) {
		ERROR_MESSAGE("�s�N�Z���V�F�[�_�[�쐬 : ���s");
		return E_FAIL;
	}

	return S_OK;
}

// ���_�o�b�t�@�̍쐬.
HRESULT CRayMesh::InitVertexBuffer()
{
	// ���C�̃o�[�e�b�N�X�o�b�t�@�쐬.
	CD3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.ByteWidth		= sizeof(VERTEX)*2;// ��_��.
	bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags	= 0;
	bd.MiscFlags		= 0;

	// �Ƃ肠����z�x�N�g���̃��C���쐬.
	D3DXVECTOR3 pos[2] =
	{
		{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }
	};

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pos;	// ���C�̍��W���Z�b�g.
	if( FAILED( m_pDevice11->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ))){	
		ERROR_MESSAGE( "�o�[�e�b�N�X�o�b�t�@�쐬���s" );
		return E_FAIL;
	}

	return S_OK;
}