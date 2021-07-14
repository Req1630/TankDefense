#include "OutLine.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Resource\GameParamResource\GameParamResource.h"

namespace
{
	const char*	SHADER_NAME			= "Data\\Shader\\RenderTexture\\OutLine\\OutLine_PS.hlsl";	// �s�N�Z���V�F�[�_�[��.
	const char*	SHADER_ENTRY_NAME	= "PS_Main";	// �s�N�Z���V�F�[�_�[�G���g���[��.
};

COutLineRender::COutLineRender()
	: m_pConstantBufferFrame	( nullptr )
	, m_OutlineBuffer			()
{
}

COutLineRender::~COutLineRender()
{
}

//------------------------------------.
// ������.
//------------------------------------.
HRESULT COutLineRender::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitBase( pContext11 ) ))		return E_FAIL;
	if( FAILED( InitBufferTex() ))				return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, SHADER_ENTRY_NAME ) )) return E_FAIL;
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBufferFrame, sizeof(OUTLINE_CBUFFER) ))) return E_FAIL;
	m_OutlineBuffer = CGameParamResource::GetParam<OUTLINE_CBUFFER>();

	return S_OK;
}

//------------------------------------.
// �`��.
//------------------------------------.
void COutLineRender::Render( const int& srvCount, const std::vector<ID3D11ShaderResourceView*>& srvList )
{
	COutLineRender::SetBuffer();

	CImGuiManager::PushRenderProc( 
		[&]()
		{
			ImGui::Begin( "OutLineRender" );
			ImGui::SliderFloat( "NormalPower",		&m_OutlineBuffer.Sample.NormalPower,	0.0f, 4.0f );
			ImGui::SliderFloat( "NormalCompare",	&m_OutlineBuffer.Sample.NormalCompare,	0.0f, 1.0f );
			ImGui::SliderFloat( "DepthPower",		&m_OutlineBuffer.Sample.DepthPower,		0.0f, 4.0f );
			ImGui::SliderFloat( "DepthCompare",		&m_OutlineBuffer.Sample.DepthCompare,	0.0f, 1.0f );
			ImGui::ColorPicker4("Color",			(float*)&m_OutlineBuffer.Color );
			static CImGuiManager::SSuccess s_Success;
			if( ImGui::Button( "Save" ) ){
				s_Success.IsSucceeded = CGameParamResource::WritingParam( EParamNo::OutLine, m_OutlineBuffer, true );
			}
			if( ImGui::Button( "Load" ) ){
				m_OutlineBuffer = CGameParamResource::GetParam<OUTLINE_CBUFFER>();
				s_Success.IsSucceeded = true;
			}
			s_Success.Render();
			ImGui::End();
		});

	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_pContext11->Map( m_pConstantBufferFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
		// �������̈���R�s�[.
		memcpy_s( pData.pData, pData.RowPitch, (void*)(&m_OutlineBuffer), sizeof(m_OutlineBuffer) );
		m_pContext11->Unmap( m_pConstantBufferFrame, 0 );
	}

	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// �T���v���̃Z�b�g.

	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBufferInit );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBufferInit );	// �s�N�Z���V�F�[�_�[.
	m_pContext11->VSSetConstantBuffers( 1, 1, &m_pConstantBufferFrame );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 1, 1, &m_pConstantBufferFrame );	// �s�N�Z���V�F�[�_�[.

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	m_pContext11->PSSetShaderResources( 0, srvCount, &srvList[0] );

	m_pContext11->Draw( 4, 0 );

	std::vector<ID3D11ShaderResourceView*> resetSrvList(srvCount);
	m_pContext11->PSSetShaderResources( 0, srvCount, &resetSrvList[0] );
}

//------------------------------------.
// �o�b�t�@�̐ݒ�.
//------------------------------------.
void COutLineRender::SetBuffer()
{
	CRenderTexture::SetBuffer( EOutLineNo_MAX );
}

//------------------------------------.
// �e�N�X�`���̏�����.
//------------------------------------.
HRESULT COutLineRender::InitBufferTex()
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

	m_pRenderTargetViewList.resize( EOutLineNo_MAX );
	m_pShaderResourceViewList.resize( EOutLineNo_MAX );
	m_pTexture2DList.resize( EOutLineNo_MAX );

	for( int i = 0; i < EOutLineNo_MAX; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pRenderTargetViewList[i],
			&m_pShaderResourceViewList[i],
			&m_pTexture2DList[i] ))) return E_FAIL;
	}

	return S_OK;
}