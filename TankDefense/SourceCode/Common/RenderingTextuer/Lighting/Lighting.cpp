#include "Lighting.h"
#include "..\..\Shader\Shader.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\LightBase\LightManager\LightManager.h"

namespace
{
	const char*	SHADER_NAME			= "Data\\Shader\\RenderTexture\\Lighting\\Lighting_PS.hlsl";	// �s�N�Z���V�F�[�_�[��.
	const char*	SHADER_ENTRY_NAME	= "PS_Main";	// �s�N�Z���V�F�[�_�[�G���g���[��.
	const int	BUFFER_COUNT_MAX	= 1;			// �o�b�t�@�[�̍ő吔.
	const int	BUFFER_INDEX_COUNT	= 0;			// �o�b�t�@�[�̔z��̔ԍ�.
};

CLightingRender::CLightingRender()
	: m_pConstantBufferFrame	( nullptr )
{
}

CLightingRender::~CLightingRender()
{
}

//------------------------------------.
// ������.
//------------------------------------.
HRESULT CLightingRender::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitBase( pContext11 ) ))		return E_FAIL;
	if( FAILED( InitBufferTex() ))				return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, SHADER_ENTRY_NAME ) )) return E_FAIL;
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBufferFrame, sizeof(LIGHT_CBUFFER) ))) return E_FAIL;

	return S_OK;
}

//------------------------------------.
// �`��.
//------------------------------------.
void CLightingRender::Render( const int& srvCount, const std::vector<ID3D11ShaderResourceView*>& srvList )
{
	CLightingRender::SetBuffer();

#if 1
	SLightState lightState = SLightState();
	lightState.Position		= D3DXVECTOR4( 5.0f, 5.0f, 0.0f, 0.0f );
	lightState.Vector		= D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 0.0f );
	lightState.Color		= D3DXVECTOR4( 2.0f, 0.0f, 0.0f, 5.0f );
	lightState.ConeAngle	= D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f );
	lightState.Type			= SLightState::POINT_LIGHT;

	CLightManager::PushLightStateQueue( lightState );

	static float rot = 0.0f;
	rot += 0.01f;

	float color_r = fabsf(cosf(rot));
	float color_g = fabsf(sinf(rot));
	float color_b = fabsf(cosf(rot)*sinf(rot));
	lightState.Position		= D3DXVECTOR4( -9.0f, 5.0f, 0.0f, 0.0f );
	lightState.Vector		= D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 0.0f );
	lightState.Color		= D3DXVECTOR4( color_r, color_g, color_b, 10.0f );
	lightState.ConeAngle	= D3DXVECTOR4( 5.0f, 0.0f, 0.0f, 0.0f );
	lightState.Type			= SLightState::POINT_LIGHT;

	CLightManager::PushLightStateQueue( lightState );
#endif

	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_pContext11->Map( m_pConstantBufferFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
		LIGHT_CBUFFER cb;

		cb.CameraPos = D3DXVECTOR4( CCameraManager::GetPosition(), 0.0f );

		// �L���[�����擾����.
		std::queue<SLightState> lightStateQueue = CLightManager::GetLightStateQueue();
		// �L���[�����󂯎�����̂ł��N���A����.
		CLightManager::LightStateQueueClear();

		// ���C�g�̏����L���[�̐����ݒ�.
		for( int i = 0; i < LIGHT_MAX_COUNT; i++ ){
			SLightState state = {};
			// �L���[���󂶂�Ȃ����.
			if( lightStateQueue.empty() == false ){
				// �l���擾���A���o��.
				state = lightStateQueue.front();
				lightStateQueue.pop();
			}

			cb.LightPos[i]			= state.Position;	// ���W.
			cb.LightVec[i]			= state.Vector;		// ����.
			cb.LightColor[i]		= state.Color;		// �F.
			cb.LightConeAngle[i]	= state.ConeAngle;	// �R�[���p�x.
			cb.LightType[i].x		= static_cast<float>(state.Type);	// ���C�g���.
		}

		// �]�����L���[�̌�Еt��.
		const int queueSize = static_cast<int>(lightStateQueue.size());
		for( int i = 0; i < queueSize; i++ ){
			lightStateQueue.pop();
		}

		// �������̈���R�s�[.
		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
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
void CLightingRender::SetBuffer()
{
	CRenderTexture::SetBuffer( BUFFER_COUNT_MAX );
}

//------------------------------------.
// �e�N�X�`���̏�����.
//------------------------------------.
HRESULT CLightingRender::InitBufferTex()
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

	if( FAILED( CreateBufferTex(
		texDesc,
		&m_pRenderTargetViewList[BUFFER_INDEX_COUNT],
		&m_pShaderResourceViewList[BUFFER_INDEX_COUNT],
		&m_pTexture2DList[BUFFER_INDEX_COUNT] ))) return E_FAIL;

	return S_OK;
}