#include "RenderingTextuerManager.h"
#include "..\GBuffer\GBuffer.h"
#include "..\Lighting\Lighting.h"
#include "..\OutLine\OutLine.h"
#include "..\Bloom\Bloom.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\BitFlagManager\BitFlagManager.h"

namespace
{
	constexpr char SHADER_NAME[]		= "Data\\Shader\\RenderTexture\\RenderTexture_PS.hlsl";
	constexpr char SHADER_ENTRY_NAME[]	= "PS_Main";
};

CRenderingTexterManager::CRenderingTexterManager()
	: m_pGBuffer	( nullptr )
	, m_pLighting	( nullptr )
	, m_pOutLine	( nullptr )
	, m_pBloom		( nullptr )
	, m_RenderFlag	( 0 )
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
	if( FAILED( InitBase( pContext11 ) ))			return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, SHADER_ENTRY_NAME )) )	return E_FAIL;
	if( FAILED( m_pGBuffer->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( m_pLighting->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( m_pOutLine->Init( pContext11 ) ))	return E_FAIL;
	if( FAILED( m_pBloom->Init( pContext11 ) ))	return E_FAIL;

	return S_OK;
}

// �`��֐�.
void CRenderingTexterManager::Render( std::function<void()>& func )
{
	m_pGBuffer->SetBuffer();
	func();	// ���f���̕`��.

	ID3D11ShaderResourceView* pLastSRV = m_pGBuffer->GetShaderResourceViewList()[m_pGBuffer->EGBufferNo_COLOR];

	// ���C�g�̕`��.
	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_Light ) == true ){
		m_pLighting->Render( m_pGBuffer->GetSRVCount(), m_pGBuffer->GetShaderResourceViewList() );
		pLastSRV = m_pLighting->GetShaderResourceViewList()[0];
	}
	
	// �u���[���̕`��.
	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_Bloom ) == true ){
		std::vector<ID3D11ShaderResourceView*> srvList(1, pLastSRV);
		m_pBloom->Render( 1, srvList );
	}

	// �֊s���̕`��.
	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_OutLine ) == true ){
		std::vector<ID3D11ShaderResourceView*> srvList = m_pGBuffer->GetShaderResourceViewList();
		srvList[m_pGBuffer->EGBufferNo_COLOR] = pLastSRV;
		m_pOutLine->Render( m_pGBuffer->GetSRVCount(), srvList );
		pLastSRV = m_pOutLine->GetShaderResourceViewList()[0];
	}


	CDirectX11::SetBackBuffer();
	LastRender( pLastSRV );

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

// �ŏI�`��.
void CRenderingTexterManager::LastRender( ID3D11ShaderResourceView* pSRV )
{
	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// �T���v���̃Z�b�g.

	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBufferInit );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBufferInit );	// �s�N�Z���V�F�[�_�[.

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// ���C���e�N�X�`����ݒ�.
	m_pContext11->PSSetShaderResources( 0, 1, &pSRV );
	m_pContext11->PSSetShaderResources( 1, 1, &m_pGBuffer->GetShaderResourceViewList()[0] );

	if( bit::IsBitFlag( m_RenderFlag, ERenderFlag_Bloom ) == true ){
		// Bloom�p�̃e�N�X�`����ݒ�.
		m_pContext11->PSSetShaderResources( 2, m_pBloom->GetSRVCount(), &m_pBloom->GetShaderResourceViewList()[0] );
	}

	SetRasterizerState( ERS_STATE::Back );
	m_pContext11->Draw( 4, 0 );
	SetRasterizerState( ERS_STATE::None );

	std::vector<ID3D11ShaderResourceView*> resetSrvList(m_pBloom->GetSRVCount());
	m_pContext11->PSSetShaderResources( 1, m_pBloom->GetSRVCount(), &resetSrvList[0] );
}