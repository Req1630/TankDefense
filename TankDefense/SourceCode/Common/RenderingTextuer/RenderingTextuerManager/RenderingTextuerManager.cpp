#include "RenderingTextuerManager.h"
#include "..\GBuffer\GBuffer.h"
#include "..\Lighting\Lighting.h"
#include "..\OutLine\OutLine.h"
#include "..\Bloom\Bloom.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\BitFlagManager\BitFlagManager.h"

namespace
{
	constexpr char	SHADER_NAME[]		= "Data\\Shader\\RenderTexture\\RenderTexture_PS.hlsl";
	constexpr char	LAST_SHADER_NAME[]	= "Data\\Shader\\RenderTexture\\RenderTexture_PS.hlsl";
	constexpr char	SHADER_ENTRY_NAME[]	= "PS_Main";
	constexpr int	BUFFER_COUNT_MAX	= 1;
};

CRenderingTexterManager::CRenderingTexterManager()
	: m_pLastPixelShader	( nullptr )
	, m_pGBuffer			( nullptr )
	, m_pLighting			( nullptr )
	, m_pOutLine			( nullptr )
	, m_pBloom				( nullptr )
	, m_RenderFlag			( 0 )
{
	m_pGBuffer	= std::make_unique<CGBufferRender>();
	m_pLighting	= std::make_unique<CLightingRender>();
	m_pOutLine	= std::make_unique<COutLineRender>();
	m_pBloom	= std::make_unique<CBloomRender>();
	bit::OnBitFlag( &m_RenderFlag, ERenderFlag_OllOn );
}

CRenderingTexterManager::~CRenderingTexterManager()
{
}

// ������.
HRESULT CRenderingTexterManager::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitBase( pContext11 ) ))								return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, SHADER_ENTRY_NAME )) )	return E_FAIL;
	if( FAILED( InitLastPixelShader() ))								return E_FAIL;
	if( FAILED( InitBufferTex() ))										return E_FAIL;

	if( FAILED( m_pGBuffer->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( m_pLighting->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( m_pOutLine->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( m_pBloom->Init( pContext11 ) ))		return E_FAIL;

	return S_OK;
}

// ���.
void CRenderingTexterManager::Release()
{
	for( auto& r : m_pRenderTargetViewList )	SAFE_RELEASE( r );
	for( auto& s : m_pShaderResourceViewList )	SAFE_RELEASE( s );
	for( auto& t : m_pTexture2DList )			SAFE_RELEASE( t );

	SAFE_RELEASE( m_pLastPixelShader );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBufferInit );
	SAFE_RELEASE( m_pVertexBuffer );

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

// �`��֐�.
void CRenderingTexterManager::Render( std::function<void()>& modelRender )
{
	m_pGBuffer->SetBuffer();

	modelRender();	// ���f���̕`��.

	// ��������w�ʂ�`�悳���Ȃ�.
	SetRasterizerState( ERS_STATE::Back );

	m_pContext11->IASetInputLayout( m_pVertexLayout );
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	ID3D11ShaderResourceView* pLastSRV = m_pGBuffer->GetShaderResourceViewList()[m_pGBuffer->EGBufferNo_COLOR];

	// ���C�g�̕`��.
	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_Light ) == true ){
		m_pLighting->Render( m_pGBuffer->GetSRVCount(), m_pGBuffer->GetShaderResourceViewList() );
		pLastSRV = m_pLighting->GetShaderResourceViewList()[0];
	}
	
	// �u���[���̕`��.
	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_Bloom ) == true ){
		std::vector<ID3D11ShaderResourceView*> srvList( 1, pLastSRV );
		m_pBloom->Render( 1, srvList );
	}

	// �֊s���̕`��.
	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_OutLine ) == true ){
		std::vector<ID3D11ShaderResourceView*> srvList = m_pGBuffer->GetShaderResourceViewList();
		srvList[m_pGBuffer->EGBufferNo_COLOR] = pLastSRV;
		m_pOutLine->Render( m_pGBuffer->GetSRVCount(), srvList );
		pLastSRV = m_pOutLine->GetShaderResourceViewList()[0];
	}

	SynthesizeTexture( pLastSRV );
	pLastSRV = m_pShaderResourceViewList[0];

	LastRender( pLastSRV );

	// �w�ʔ�\�������ɖ߂�.
	SetRasterizerState( ERS_STATE::None );

	CImGuiManager::PushRenderProc( 
		[&]()
		{
			ImGui::Begin( "RenderingTexture" );
			const float size = ImGui::GetWindowWidth() / static_cast<float>(m_WndWidth)*0.9f;
			const ImVec2 imageSize = { static_cast<float>(m_WndWidth)*size, static_cast<float>(m_WndHeight)*size };
			if( ImGui::TreeNode("G-Buffer") ){
				for( int i = 0; i <  m_pGBuffer->GetSRVCount(); i++ ){
					ImGui::Image( m_pGBuffer->GetShaderResourceViewList()[i], imageSize );
				}
				ImGui::TreePop();
			}
			if( ImGui::TreeNode("Lighting") ){
				ImGui::Image( m_pLighting->GetShaderResourceViewList()[0], imageSize );
				ImGui::TreePop();
			}
			if( ImGui::TreeNode("OutLine") ){
				for( int i = 0; i <  m_pOutLine->GetSRVCount(); i++ ){
					ImGui::Image( m_pOutLine->GetShaderResourceViewList()[i], imageSize );
				}
				ImGui::TreePop();
			}
			if( ImGui::TreeNode("Bloom") ){
				for( int i = 0; i <  m_pBloom->GetSRVCount(); i++ ){
					ImGui::Image( m_pBloom->GetShaderResourceViewList()[i], imageSize );
				}
				ImGui::TreePop();
			}
			ImGui::End();

			ImGui::Begin( "RenderFlag" );
			ImGui::CheckboxFlags( u8"���C�g�`��", &m_RenderFlag, ERenderFlag_Light );
			ImGui::CheckboxFlags( u8"�֊s���`��", &m_RenderFlag, ERenderFlag_OutLine );
			ImGui::CheckboxFlags( u8"Bloom�`��", &m_RenderFlag, ERenderFlag_Bloom );
			ImGui::End();
		});
}

// �T�C�Y�ύX.
HRESULT CRenderingTexterManager::ResizeTexture()
{
	if( m_WndWidth	== CDirectX11::GetWndWidth() && 
		m_WndHeight	== CDirectX11::GetWndHeight() ) return S_OK;

	CRenderTexture* pTex[] =
		{
			this,
			m_pGBuffer.get(),
			m_pLighting.get(),
			m_pOutLine.get(),
			m_pBloom.get(),
		};
	for( auto& t : pTex ){
		if( FAILED( t->Resize() ) ) return E_FAIL;
	}
	return S_OK;
}

// �e�摜����������.
void CRenderingTexterManager::SynthesizeTexture( ID3D11ShaderResourceView* pSRV )
{
	SetBuffer();

	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// �T���v���̃Z�b�g.

	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBufferInit );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBufferInit );	// �s�N�Z���V�F�[�_�[.

	// ���C���e�N�X�`����ݒ�.
	m_pContext11->PSSetShaderResources( 0, 1, &pSRV );
	m_pContext11->PSSetShaderResources( 1, 1, &m_pGBuffer->GetShaderResourceViewList()[0] );

	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_Bloom ) == true ){
		// Bloom�p�̃e�N�X�`����ݒ�.
		m_pContext11->PSSetShaderResources( 2, m_pBloom->GetSRVCount(), &m_pBloom->GetShaderResourceViewList()[0] );
	}

	m_pContext11->Draw( 4, 0 );

	std::vector<ID3D11ShaderResourceView*> resetSrvList(m_pBloom->GetSRVCount());
	m_pContext11->PSSetShaderResources( 1, m_pBloom->GetSRVCount(), &resetSrvList[0] );
	
}

// �ŏI�`��.
void CRenderingTexterManager::LastRender( ID3D11ShaderResourceView* pSRV )
{
	CDirectX11::SetBackBuffer();

	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pLastPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// �T���v���̃Z�b�g.

	// ���C���e�N�X�`����ݒ�.
	m_pContext11->PSSetShaderResources( 0, 1, &pSRV );

	m_pContext11->Draw( 4, 0 );

	std::vector<ID3D11ShaderResourceView*> resetSrvList( BUFFER_COUNT_MAX );
	m_pContext11->PSSetShaderResources( 0, BUFFER_COUNT_MAX, &resetSrvList[0] );
}

// �o�b�t�@�̐ݒ�.
void CRenderingTexterManager::SetBuffer()
{
	CRenderTexture::SetBuffer( BUFFER_COUNT_MAX );
}

// �e�N�X�`���̏�����.
HRESULT CRenderingTexterManager::InitBufferTex()
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

	m_pRenderTargetViewList.resize( BUFFER_COUNT_MAX );
	m_pShaderResourceViewList.resize( BUFFER_COUNT_MAX );
	m_pTexture2DList.resize( BUFFER_COUNT_MAX );

	for( int i = 0; i < BUFFER_COUNT_MAX; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pRenderTargetViewList[i],
			&m_pShaderResourceViewList[i],
			&m_pTexture2DList[i] ))) return E_FAIL;
	}

	return S_OK;
}

// �s�N�Z���V�F�[�_�[�̏�����.
HRESULT CRenderingTexterManager::InitLastPixelShader()
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
		LAST_SHADER_NAME,	// �V�F�[�_�[�t�@�C����.
		SHADER_ENTRY_NAME,	// �V�F�[�_�[�G���g���[�|�C���g.
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
		&m_pLastPixelShader ))) {
		ERROR_MESSAGE("�s�N�Z���V�F�[�_�[�쐬 : ���s");
		return E_FAIL;
	}

	return S_OK;
}