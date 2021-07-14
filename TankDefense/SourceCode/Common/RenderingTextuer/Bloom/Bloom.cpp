#include "Bloom.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\LightBase\LightManager\LightManager.h"
#include "..\DownSampling\DownSampling.h"

namespace
{
	const char*	SHADER_NAME			= "Data\\Shader\\RenderTexture\\Bloom\\Bloom.hlsl";	// �s�N�Z���V�F�[�_�[��.
	const char*	SHADER_ENTRY_NAME	= "PS_Main";		// �s�N�Z���V�F�[�_�[�G���g���[��.
	const int	BLUR_SAMPLE_NUM		= 5;				// �ڂ����̃T���v����.
	const int	BUFFER_COUNT_MAX	= BLUR_SAMPLE_NUM;	// �o�b�t�@�[�̍ő吔.
};

CBloomRender::CBloomRender()
	: m_pDownSampling	( nullptr )
{
	m_pDownSampling = std::make_unique<CDownSamplingRender>();
}

CBloomRender::~CBloomRender()
{
}

//------------------------------------.
// ������.
//------------------------------------.
HRESULT CBloomRender::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitBase( pContext11 ) ))		return E_FAIL;
	if( FAILED( InitBufferTex() ))				return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, SHADER_ENTRY_NAME ) )) return E_FAIL;
	if( FAILED( CBloomRender::InitVertexBuffer() ))		return E_FAIL;
	if( FAILED( CBloomRender::InitConstantBuffer() ))	return E_FAIL;
	if( FAILED( m_pDownSampling->Init( pContext11 ) ))	return E_FAIL;

	return S_OK;
}

//------------------------------------.
// �`��.
//------------------------------------.
void CBloomRender::Render( const int& srvCount, const std::vector<ID3D11ShaderResourceView*>& srvList )
{
	m_pDownSampling->Render( srvCount, srvList );

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// �T���v���̃Z�b�g.

	ID3D11ShaderResourceView* srv = m_pDownSampling->GetShaderResourceViewList()[0];
	for( int  i = 0; i < BLUR_SAMPLE_NUM; i++ ){
		// �^�[�Q�b�g�r���[�̃N���A.
		m_pContext11->ClearRenderTargetView( m_pRenderTargetViewList[i], CLEAR_BACK_COLOR );
		// �����_�[�^�[�Q�b�g�̐ݒ�.
		m_pContext11->OMSetRenderTargets( 1, &m_pRenderTargetViewList[i], CDirectX11::GetDepthSV() );
		// �f�v�X�X�e���V���o�b�t�@.
		m_pContext11->ClearDepthStencilView( CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );


		m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffersInit[i] );	// ���_�V�F�[�_.
		m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffersInit[i] );	// �s�N�Z���V�F�[�_�[.

		m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffers[i], &stride, &offset );

		m_pContext11->PSSetShaderResources( 0, 1, &srv );
		m_pContext11->Draw( 4, 0 );
		std::vector<ID3D11ShaderResourceView*> resetSrvList(1);
		m_pContext11->PSSetShaderResources( 0, 1, &resetSrvList[0] );

		srv = m_pShaderResourceViewList[i];
	}
}

//------------------------------------.
// �o�b�t�@�̐ݒ�.
//------------------------------------.
void CBloomRender::SetBuffer()
{
	CRenderTexture::SetBuffer( BUFFER_COUNT_MAX );
}

//------------------------------------.
// �e�N�X�`���̏�����.
//------------------------------------.
HRESULT CBloomRender::InitBufferTex()
{
	UINT width	= m_WndWidth / 2;
	UINT height	= m_WndHeight / 2;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= width;							// ��.
	texDesc.Height				= height;							// ����.
	texDesc.MipLevels			= 1;								// �~�b�v�}�b�v���x��:1.
	texDesc.ArraySize			= 1;								// �z��:1.
	texDesc.Format				= DXGI_FORMAT_R16G16B16A16_FLOAT;	// 32�r�b�g�t�H�[�}�b�g.
	texDesc.SampleDesc.Count	= 1;								// �}���`�T���v���̐�.
	texDesc.SampleDesc.Quality	= 0;								// �}���`�T���v���̃N�I���e�B.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// �g�p���@:�f�t�H���g.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// �����_�[�^�[�Q�b�g�A�V�F�[�_�[���\�[�X.
	texDesc.CPUAccessFlags		= 0;								// CPU����̓A�N�Z�X���Ȃ�.
	texDesc.MiscFlags			= 0;								// ���̑��̐ݒ�Ȃ�.

	m_pRenderTargetViewList.resize( BUFFER_COUNT_MAX );
	m_pShaderResourceViewList.resize( BUFFER_COUNT_MAX );
	m_pTexture2DList.resize( BUFFER_COUNT_MAX );

	for( int i = 0; i < BUFFER_COUNT_MAX; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pRenderTargetViewList[i],
			&m_pShaderResourceViewList[i],
			&m_pTexture2DList[i] ))) return E_FAIL;
		texDesc.Width	>>= 1;	if( texDesc.Width	<= 0 ) texDesc.Width	= 1;
		texDesc.Height	>>= 1;	if( texDesc.Height	<= 0 ) texDesc.Height	= 1;
	}

	return S_OK;
}

// ���_�o�b�t�@�[�̍쐬.
HRESULT CBloomRender::InitVertexBuffer()
{
	UINT width	= m_WndWidth / 2;
	UINT height	= m_WndHeight / 2;
	m_pVertexBuffers.resize( BLUR_SAMPLE_NUM );
	for( int i = 0; i < BUFFER_COUNT_MAX; i++ ){
		// �|��(�l�p�`)�̒��_���쐬.
		VERTEX vertices[]=
		{
			// ���_���W(x,y,z)				 
			D3DXVECTOR3( 0.0f,						static_cast<float>(height),	0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
			D3DXVECTOR3( 0.0f,						0.0f,						0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
			D3DXVECTOR3( static_cast<float>(width),	static_cast<float>(height),	0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
			D3DXVECTOR3( static_cast<float>(width),	0.0f,						0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),
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
			&m_pVertexBuffers[i]))){
			ERROR_MESSAGE( "���_�ޯ̧�쐬���s" );
			return E_FAIL;
		}

		width	>>= 1;	if( width	<= 0 ) width	= 1;
		height	>>= 1;	if( height	<= 0 ) height	= 1;
	}
	return S_OK;
}

// �R���X�^���g�o�b�t�@�̍쐬.
HRESULT CBloomRender::InitConstantBuffer()
{

	UINT width = m_WndWidth / 2;
	UINT height = m_WndHeight / 2;
	m_pConstantBuffersInit.resize( BLUR_SAMPLE_NUM );
	for( int  i = 0; i < BLUR_SAMPLE_NUM; i++ ){
		// �R���X�^���g�o�b�t�@�̍쐬.
		shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBuffersInit[i], sizeof(C_BUFFER_PER_INIT) );

		// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
		D3D11_MAPPED_SUBRESOURCE pData;
		// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
		if( SUCCEEDED( m_pContext11->Map( m_pConstantBuffersInit[i], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
			C_BUFFER_PER_INIT cb;	// �R���X�^���g�o�b�t�@.
			D3DXMatrixIdentity( &cb.mW );
			D3DXMatrixTranspose( &cb.mW, &cb.mW );
			// �r���[�|�[�g�̕�,������n��.
			cb.vViewPort.x = static_cast<float>(width);
			cb.vViewPort.y = static_cast<float>(height);
			// �s�N�Z���̃T�C�Y��n��.
			cb.vPixelSize.x = 1.0f / static_cast<float>(width);
			cb.vPixelSize.y = 1.0f / static_cast<float>(height);
			// �E�B���h�E�䗦��n��.
			cb.vWndRatio.x = static_cast<float>(m_WndWidth/width);
			cb.vWndRatio.y = static_cast<float>(m_WndHeight/height);

			// �������̈���R�s�[.
			memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
			m_pContext11->Unmap( m_pConstantBuffersInit[i], 0 );
		}

		width	>>= 1;	if( width	<= 0 ) width	= 1;
		height	>>= 1;	if( height	<= 0 ) height	= 1;
	}

	return S_OK;
}