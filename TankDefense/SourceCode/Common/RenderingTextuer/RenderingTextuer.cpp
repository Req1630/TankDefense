#include "RenderingTextuer.h"
#include "..\Shader\Shader.h"
#include "..\D3DX\D3DX11.h"

namespace
{
	const char* SHADER_NAME			= "Data\\Shader\\RenderTexture\\RenderTexture_VS.hlsl";
	const char* SHADER_ENTRY_NAME	= "VS_Main";
};

CRenderTexture::CRenderTexture()
	: m_pRenderTargetViewList	()
	, m_pShaderResourceViewList	()
	, m_pTexture2DList			()
	, m_pVertexShader			( nullptr )
	, m_pPixelShader			( nullptr )
	, m_pVertexLayout			( nullptr )
	, m_pConstantBufferInit		( nullptr )
	, m_pVertexBuffer			( nullptr )
	, m_pSampleLinear			( nullptr )
	, m_WndWidth				( 1 )
	, m_WndHeight				( 1 )
{
}

CRenderTexture::~CRenderTexture()
{
}

//----------------------------------.
// ������.
//----------------------------------.
HRESULT CRenderTexture::InitBase( ID3D11DeviceContext* pContext11 )
{
	if( m_pDevice11 == nullptr ){
		if( FAILED( SetDevice( pContext11 ) )) return E_FAIL;
	}

	// �E�B���h�E�T�C�Y�̎擾.
	m_WndWidth	= CDirectX11::GetWndWidth();
	m_WndHeight	= CDirectX11::GetWndHeight();

	if( FAILED( InitVertexShader( SHADER_NAME, SHADER_ENTRY_NAME )) )	return E_FAIL;
	if( FAILED( InitVertexBuffer() ))	return E_FAIL;
	if( FAILED( InitConstantBuffer()) )	return E_FAIL;
	if( FAILED( InitSampleLinear() ))	return E_FAIL;
	return S_OK;
}

//----------------------------------.
//----------------------------------.
// ���_�o�b�t�@�[�̍쐬.
HRESULT CRenderTexture::InitVertexBuffer()
{
	const float wnd_w = static_cast<float>(m_WndWidth);
	const float wnd_h = static_cast<float>(m_WndHeight);
	// �|��(�l�p�`)�̒��_���쐬.
	VERTEX vertices[]=
	{
		// ���_���W(x,y,z)				 
		D3DXVECTOR3( 0.0f,	wnd_h,	0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
		D3DXVECTOR3( 0.0f,	0.0f,	0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
		D3DXVECTOR3( wnd_w,	wnd_h,	0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
		D3DXVECTOR3( wnd_w,	0.0f,	0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),
	};
	// �ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// �g�p���@(�f�t�H���g).
	bd.ByteWidth			= sizeof(VERTEX) * uVerMax;	// ���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;	// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;	// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;	// �\���̂̃T�C�Y(���g�p).

	// �T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	// �|���̒��_���Z�b�g.

	// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, 
		&InitData, 
		&m_pVertexBuffer))){
		ERROR_MESSAGE( "���_�ޯ̧�쐬���s" );
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------.
// �R���X�^���g�o�b�t�@�̍쐬.
//----------------------------------.
HRESULT CRenderTexture::InitConstantBuffer()
{
	shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBufferInit, sizeof(C_BUFFER_PER_INIT) );

	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;

	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( FAILED( m_pContext11->Map( m_pConstantBufferInit, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
		ERROR_MESSAGE( "�R���X�^���g�o�b�t�@�������� : ���s" );
		return E_FAIL;
	}

	C_BUFFER_PER_INIT cb;	// �R���X�^���g�o�b�t�@.
	D3DXMatrixIdentity( &cb.mW );
	D3DXMatrixTranspose( &cb.mW, &cb.mW );
	// �r���[�|�[�g�̕�,������n��.
	cb.vViewPort.x = static_cast<float>(m_WndWidth);
	cb.vViewPort.y = static_cast<float>(m_WndHeight);

	cb.vPixelSize.x = 1.0f / static_cast<float>(m_WndWidth);
	cb.vPixelSize.y = 1.0f / static_cast<float>(m_WndHeight);

	// �������̈���R�s�[.
	memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
	m_pContext11->Unmap( m_pConstantBufferInit, 0 );

	return S_OK;
}

//----------------------------------.
// ���.
//----------------------------------.
void CRenderTexture::Release()
{
	for( auto& r : m_pRenderTargetViewList )	SAFE_RELEASE( r );
	for( auto& s : m_pShaderResourceViewList )	SAFE_RELEASE( s );
	for( auto& t : m_pTexture2DList )			SAFE_RELEASE( t );

	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBufferInit );
	SAFE_RELEASE( m_pVertexBuffer );

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

//----------------------------------.
// �T�C�Y�̕ύX.
//----------------------------------.
HRESULT CRenderTexture::Resize()
{
	Release();
	if( FAILED( Init( CDirectX11::GetContext() ) )) return E_FAIL;
	return S_OK;
}

//----------------------------------.
// �o�b�t�@�̐ݒ�.
//----------------------------------.
void CRenderTexture::SetBuffer( const int& numViews )
{
	// G-Buffer�e�N�X�`���̃N���A.
	for( auto& rtv : m_pRenderTargetViewList ){
		m_pContext11->ClearRenderTargetView( rtv, CLEAR_BACK_COLOR );
	}
	// �����_�[�^�[�Q�b�g�̐ݒ�.
	m_pContext11->OMSetRenderTargets( 
		numViews,
		&m_pRenderTargetViewList[0],
		CDirectX11::GetDepthSV() );
	// �f�v�X�X�e���V���o�b�t�@.
	m_pContext11->ClearDepthStencilView(
		CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

//----------------------------------.
// ���_�V�F�[�_�[�쐬.
//----------------------------------.
HRESULT CRenderTexture::InitVertexShader( const char* filePath, const char* entryName )
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSL����o�[�e�b�N�X�V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		shader::InitShader(
			filePath,			// �V�F�[�_�[�t�@�C����.
			entryName,			// �V�F�[�_�[�G���g���[�|�C���g.
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
		shader::GetTexcoordInputElement(),
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
	return S_OK;
}

//----------------------------------.
// �s�N�Z���V�F�[�_�[�쐬.
//----------------------------------.
HRESULT CRenderTexture::InitPixelShader( const char* filePath, const char* entryName )
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSL����s�N�Z���V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		shader::InitShader(
			filePath,			// �V�F�[�_�[�t�@�C����.
			entryName,			// �V�F�[�_�[�G���g���[�|�C���g.
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

//----------------------------------.
// �T���v���[�̍쐬.
//----------------------------------.
HRESULT CRenderTexture::InitSampleLinear()
{
	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU		= D3D11_TEXTURE_ADDRESS_CLAMP;		// UV�l��[0,1]�𒴂�����,[0,1]�ɐݒ肷��.
	samDesc.AddressV		= D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	samDesc.MinLOD			= 0;
	samDesc.MaxLOD			= D3D11_FLOAT32_MAX;
	// �T���v���쐬.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear ) ) ){
		ERROR_MESSAGE( "SamplerState creation failed" );
		return E_FAIL;
	}
	return S_OK;
}

//----------------------------------.
// �o�b�t�@�̍쐬.
//----------------------------------.
HRESULT CRenderTexture::CreateBufferTex(
	const D3D11_TEXTURE2D_DESC	texDesc,
	ID3D11RenderTargetView**	ppRTV,
	ID3D11ShaderResourceView**	ppSRV,
	ID3D11Texture2D**			ppTex )
{
	// ���̃e�N�X�`���ɑ΂��ăf�v�X�X�e���V��(DSTex)���쐬.
	if( FAILED( m_pDevice11->CreateTexture2D( &texDesc, nullptr, ppTex )) ){
		_ASSERT_EXPR( false, L"�e�N�X�`���f�X�N�쐬���s" );
		return E_FAIL;
	}
	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset( &rtvDesc, 0, sizeof( rtvDesc ) );
	rtvDesc.Format             = texDesc.Format;
	rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	// RenderTargetView�쐬.
	if( FAILED( m_pDevice11->CreateRenderTargetView( *ppTex, &rtvDesc, ppRTV ) )){
		ERROR_MESSAGE( "�����_�[�^�[�Q�b�g�r���[�쐬���s" );
		return E_FAIL;
	}

	// �V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset( &srvDesc, 0, sizeof( srvDesc ) );
	srvDesc.Format              = rtvDesc.Format;
	srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	// �e�N�X�`���쐬���Ɠ����t�H�[�}�b�g
	if( FAILED( m_pDevice11->CreateShaderResourceView( *ppTex, &srvDesc, ppSRV ) )){
		ERROR_MESSAGE( "�f�v�X�X�e���V���쐬���s" );
		return E_FAIL;
	}
	return S_OK;
}
