#include "AuraMesh.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Utility\DebugConsole\DebugConsole.h"

namespace
{
	constexpr char SHADER_NAME[]	= "Data\\Shader\\Aura.hlsl";	// �V�F�[�_�[��.
};

CAuraMesh::CAuraMesh()
	: m_pVertexBuffer	( nullptr )
	, m_pIndexBuffer	( nullptr )
	, m_pVertexShader	( nullptr )
	, m_pPixelShader	( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pConstantBuffer	( nullptr )
	, m_ScrollTime		( 0.0f )
{
}

CAuraMesh::~CAuraMesh()
{
	Release();
}

// ������.
HRESULT CAuraMesh::Init()
{
	// �p�����[�^�̏�����.
	if( FAILED( SetDevice( CDirectX11::GetContext() )) ) return E_FAIL;
	if( FAILED( InitModel()))	return E_FAIL;	// ���f���̓ǂݍ���.
	if( FAILED( InitShader()))	return E_FAIL;	// �V�F�[�_�[�̏�����.
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBuffer, sizeof(C_BUFFER) ))) return E_FAIL;

	return S_OK;
}

//����֐�.
void CAuraMesh::Release()
{
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pIndexBuffer );
	
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBuffer );

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

// �X�V.
void CAuraMesh::Update()
{
	m_ScrollTime -= GetDeltaTime() * 0.25f;
}

// �`��.
void CAuraMesh::Render()
{
	D3DXMATRIX mWorld = m_Tranceform.GetWorldMatrix();

	// �V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData)))
	{
		C_BUFFER cb;

		cb.mW = mWorld;
		D3DXMatrixTranspose( &cb.mW, &cb.mW );//�s���]�u����.
		cb.mWVP = mWorld * CCameraManager::GetViewProjMatrix();
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );//�s���]�u����.

		cb.vColor	= { 0.7f, 0.2f, 0.2f, 0.7f };
		cb.vUV.x	= m_ScrollTime;
		cb.vUV.y	= m_ScrollTime;

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		// �o�b�t�@���̃f�[�^�̏��������I������Unmap.
		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}

	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );

	// ���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�p���邩�H.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );

	// ���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pContext11->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	SetBlend( true );
	SetRasterizerState( ERS_STATE::Front );
	m_pContext11->DrawIndexed( 30, 0, 0 );
	SetRasterizerState( ERS_STATE::Back );
	m_pContext11->DrawIndexed( 30, 0, 0 );
	SetRasterizerState( ERS_STATE::None );
	SetBlend( false );
}

// ���b�V���ǂݍ���.
HRESULT CAuraMesh::InitModel()
{
	const float boxSize = 1.0f;
	const float height = 20.0f;
	//�����(�l�p�`)�̒��_���쐬.
	VERTEX vertices[]=
	{
		//���_���W(x,y,z)				 
		D3DXVECTOR3( boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		 boxSize ),

		D3DXVECTOR3( boxSize, height,	-boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3( boxSize, height,	 boxSize ),

		D3DXVECTOR3( boxSize, height,	 boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3(-boxSize, height,	 boxSize ),

		D3DXVECTOR3(-boxSize, height,	 boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		 boxSize ),
		D3DXVECTOR3(-boxSize, height,	-boxSize ),

		D3DXVECTOR3(-boxSize, height,	-boxSize ),
		D3DXVECTOR3( boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3(-boxSize, 0.0f,		-boxSize ),
		D3DXVECTOR3( boxSize, height,	-boxSize ),

	};
	WORD indexList[]{
		0,	1,	2,		1,	0,	3,
		4,	5,	6,		5,	4,	7,
		8,	9,	10,		9,	8,	11,
		12,	13,	14,		13,	12,	15,
		16,	17,	18,		17,	16,	19,
	};

	// �ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices)/sizeof(vertices[0]);
	UINT uIndexMax = sizeof(indexList)/sizeof(indexList[0]);

	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// �g�p���@(�f�t�H���g).
	bd.ByteWidth			= sizeof(VERTEX)*uVerMax;	// ���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;						// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;						// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;						// �\���̂̃T�C�Y(���g�p).

	//���ؿ���ް��\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	//����؂̒��_���.

	//���_�ޯ̧�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) ){
		_ASSERT_EXPR( false, L"���_�ޯ̧�쐬���s" );
		return E_FAIL;
	}

	D3D11_BUFFER_DESC id;
	id.Usage				= D3D11_USAGE_DEFAULT;	   // �g�p���@(�f�t�H���g).
	id.ByteWidth			= sizeof(WORD)*uIndexMax;  // ���_�̃T�C�Y.
	id.BindFlags			= D3D11_BIND_INDEX_BUFFER; // ���_�o�b�t�@�Ƃ��Ĉ���.
	id.CPUAccessFlags		= 0;					   // CPU����̓A�N�Z�X���Ȃ�.
	id.MiscFlags			= 0;					   // ���̑��̃t���O(���g�p).
	id.StructureByteStride	= 0;					   // �\���̂̃T�C�Y(���g�p).

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem			= indexList;
	IndexData.SysMemPitch		= 0;
	IndexData.SysMemSlicePitch	= 0;

	if( FAILED( m_pDevice11->CreateBuffer(
		&id, &IndexData, &m_pIndexBuffer ))){
		_ASSERT_EXPR( false, L"�C���f�b�N�X�ޯ̧�쐬���s" );
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAuraMesh::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

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
	SAFE_RELEASE(pCompiledShader);

	return S_OK;
}