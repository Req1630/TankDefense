#include "GBuffer.h"
#include "..\CascadedShadowMap\CascadedShadowMap.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\LightBase\LightManager\LightManager.h"
#include "..\..\Mesh\MeshStruct.h"
#include "..\..\Shader\Shader.h"

namespace
{
	constexpr char	SHADER_NAME[]			= "Data\\Shader\\MeshPS.hlsl";	// �s�N�Z���V�F�[�_�[��.
	constexpr char	PS_SHADER_ENTRY_NAME[]	= "PS_Main";	// �s�N�Z���V�F�[�_�[�G���g���[��.

	const D3DXMATRIX SHADOW_BIAS = 
	{
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f 
	};
};

CGBufferRender::CGBufferRender()
	: m_pConstantBufferFrame	( nullptr )
{
	m_pShadowMap = CCascadedShadowMap::GetInstance();
}

CGBufferRender::~CGBufferRender()
{
	SAFE_RELEASE( m_pConstantBufferFrame );
}

//------------------------------------.
// ������.
//------------------------------------.
HRESULT CGBufferRender::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitBase( pContext11 ) ))		return E_FAIL;
	if( FAILED( InitBufferTex() ))				return E_FAIL;
	if( FAILED( InitShadowSampler() ))			return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, PS_SHADER_ENTRY_NAME ) )) return E_FAIL;
	if( FAILED( shader::CreateConstantBuffer( 
		m_pDevice11,
		&m_pConstantBufferFrame,
		sizeof(CBUFFER_PER_FRAME) ))) return E_FAIL;
	if( FAILED( shader::CreateConstantBuffer(
		m_pDevice11,
		&m_pConstantBufferShadow,
		sizeof(CCascadedShadowMap::CBUFFER_PER_SHADOW) ))) return E_FAIL;
	
	return S_OK;
}

//------------------------------------.
// �o�b�t�@�̐ݒ�.
//------------------------------------.
void CGBufferRender::SetBuffer()
{
	CRenderTexture::SetBuffer( EGBufferNo_MAX );

	Update();
}

//------------------------------------.
// �e�N�X�`���̏�����.
//------------------------------------.
HRESULT CGBufferRender::InitBufferTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= m_WndWidth ;						// ��.
	texDesc.Height				= m_WndHeight;						// ����.
	texDesc.MipLevels			= 1;								// �~�b�v�}�b�v���x��:1.
	texDesc.ArraySize			= 1;								// �z��:1.
	texDesc.Format				= DXGI_FORMAT_R16G16B16A16_FLOAT;	// 32�r�b�g�t�H�[�}�b�g.
	texDesc.SampleDesc.Count	= 1;								// �}���`�T���v���̐�.
	texDesc.SampleDesc.Quality	= 0;								// �}���`�T���v���̃N�I���e�B.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// �g�p���@:�f�t�H���g.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// �����_�[�^�[�Q�b�g�A�V�F�[�_�[���\�[�X.
	texDesc.CPUAccessFlags		= 0;								// CPU����̓A�N�Z�X���Ȃ�.
	texDesc.MiscFlags			= 0;								// ���̑��̐ݒ�Ȃ�.

	m_pRenderTargetViewList.resize( EGBufferNo_MAX );
	m_pShaderResourceViewList.resize( EGBufferNo_MAX );
	m_pTexture2DList.resize( EGBufferNo_MAX );

	for( int i = 0; i < EGBufferNo_MAX; i++ ){
		if( i == EGBufferNo_Z_DEPTH ){
			texDesc.Format = DXGI_FORMAT_R32_FLOAT;;
		} else {
			texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pRenderTargetViewList[i],
			&m_pShaderResourceViewList[i],
			&m_pTexture2DList[i] ))) return E_FAIL;
	}
	return S_OK;
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CGBufferRender::Update()
{
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.
	m_pContext11->PSSetShaderResources( 2, m_pShadowMap->CASCADED_NUM, &m_pShadowMap->GetShaderResourceViewList()[0] );
	m_pContext11->PSSetSamplers( 1, 1, &m_pShadowMapSampler );

	UpdateFrame();
	UpdateShadow();
}

//------------------------------------.
// �t���[�����n���l�̍X�V.
//------------------------------------.
void CGBufferRender::UpdateFrame()
{
	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBufferFrame, 
		0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData ))) {
		CBUFFER_PER_FRAME cb;	// �R���X�^���g�o�b�t�@.

		// �J�����ʒu.
		D3DXVECTOR3 camPos = CCameraManager::GetPosition();
		cb.vCamPos = D3DXVECTOR4( camPos.x, camPos.y, camPos.z, 1.0f );

		//----- ���C�g��� -----.
		// ���C�g����.
		D3DXVECTOR3 lightDir = CLightManager::GetDirection();
		cb.vLightDir = D3DXVECTOR4( lightDir.x, lightDir.y, lightDir.z, 1.0f );
		// ���C�g���x(���邳).
		cb.vIntensity.x = CLightManager::GetIntensity();
		// ���C�g�����̐��K��(�m�[�}���C�Y).
		D3DXVec4Normalize( &cb.vLightDir, &cb.vLightDir );

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		// �o�b�t�@���̃f�[�^�̏��������I������Unmap.
		m_pContext11->Unmap( m_pConstantBufferFrame, 0 );
	}

	m_pContext11->VSSetConstantBuffers( 2, 1, &m_pConstantBufferFrame );
	m_pContext11->PSSetConstantBuffers( 2, 1, &m_pConstantBufferFrame );
}

//------------------------------------.
// �e����n���l�̍X�V.
//------------------------------------.
void CGBufferRender::UpdateShadow()
{
	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBufferShadow, 
		0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData ))) {
		CCascadedShadowMap::CBUFFER_PER_SHADOW cb;	// �R���X�^���g�o�b�t�@.

		std::vector<D3DXMATRIX>& shadowMat	= m_pShadowMap->GetShadowMatrix();
		std::vector<float>& spritPos		= m_pShadowMap->GetSpritPosition();

		for( int i = 0; i < m_pShadowMap->CASCADED_NUM; i++ ){
			cb.ShadowVP[i]		= shadowMat[i] * SHADOW_BIAS;
			cb.SpritPos[i].x	= spritPos[i];
			D3DXMatrixTranspose( &cb.ShadowVP[i], &cb.ShadowVP[i] );
		}

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		// �o�b�t�@���̃f�[�^�̏��������I������Unmap.
		m_pContext11->Unmap( m_pConstantBufferShadow, 0 );
	}

	m_pContext11->VSSetConstantBuffers( 4, 1, &m_pConstantBufferShadow );
	m_pContext11->PSSetConstantBuffers( 4, 1, &m_pConstantBufferShadow );
}

//------------------------------------.
// �V���h�E�p�T���v���[�̍쐬.
//------------------------------------.
HRESULT CGBufferRender::InitShadowSampler()
{
	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));

	// �V���h�E�}�b�v�p.
	samDesc.Filter			= D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU		= D3D11_TEXTURE_ADDRESS_BORDER;
	samDesc.AddressV		= D3D11_TEXTURE_ADDRESS_BORDER;
	samDesc.AddressW		= D3D11_TEXTURE_ADDRESS_BORDER;
	samDesc.BorderColor[0]	= 1.0f;
	samDesc.BorderColor[1]	= 1.0f;
	samDesc.BorderColor[2]	= 1.0f;
	samDesc.BorderColor[3]	= 1.0f;
	samDesc.ComparisonFunc	= D3D11_COMPARISON_LESS_EQUAL;
	samDesc.MaxAnisotropy	= 1;
	samDesc.MipLODBias		= 0.0f;
	samDesc.MinLOD			= -D3D11_FLOAT32_MAX;
	samDesc.MaxLOD			=  D3D11_FLOAT32_MAX;
	// �T���v���쐬.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pShadowMapSampler ))){
		ERROR_MESSAGE( "�T���v���[�쐬���s" );
		return E_FAIL;
	}
	return S_OK;
}