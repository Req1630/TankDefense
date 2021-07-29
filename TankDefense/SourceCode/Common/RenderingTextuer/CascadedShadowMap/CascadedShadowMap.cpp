#include "CascadedShadowMap.h"
#include "..\..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\..\Object\LightBase\LightManager\LightManager.h"
#include "..\..\Shader\Shader.h"

namespace
{
	constexpr char	SHADER_NAME[]			= "Data\\Shader\\ShadowMap.hlsl";	// �s�N�Z���V�F�[�_�[��.
	constexpr char	PS_SHADER_ENTRY_NAME[]	= "PS_Main";	// �s�N�Z���V�F�[�_�[�G���g���[��.
	
	const D3DXVECTOR3 VIEW_UP_VECTOR	= { 0.0f, 1.0f, 0.0f };	// ������x�N�g��.
	const D3DXMATRIX SHADOW_BIAS = 
	{
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f 
	};
};

CCascadedShadowMap::CCascadedShadowMap()
	: m_pSkinVertexShader		( nullptr )
	, m_pConstantBufferFrame	( nullptr )
	, m_ShadowMatrix			( CASCADED_NUM )
	, m_SplitPos				( CASCADED_NUM )
	, m_ProjMatrix				()
	, m_ViewMatrix				()
	, m_MaxClipDistance			( 1000.0f )
	, m_MinClipDistance			( 0.01f )
	, m_AspectWindow			( 0.0f )
	, m_FovCamera				( 0.0f )
	, m_IsEndRender				( false )
{
}

CCascadedShadowMap::~CCascadedShadowMap()
{
	SAFE_RELEASE(m_pSkinVertexShader);
	SAFE_RELEASE(m_pConstantBufferFrame);
}

//------------------------------------.
// �C���X�^���X�̎擾.
//------------------------------------.
CCascadedShadowMap* CCascadedShadowMap::GetInstance()
{
	static std::unique_ptr<CCascadedShadowMap> pInstance = 
		std::make_unique<CCascadedShadowMap>();
	return pInstance.get();
}

//------------------------------------.
// ������.
//------------------------------------.
HRESULT CCascadedShadowMap::Init( ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitBase( pContext11 ) ))		return E_FAIL;
	if( FAILED( InitBufferTex() ))				return E_FAIL;
	if( FAILED( InitVertexShader() ))			return E_FAIL;
	if( FAILED( InitPixelShader( SHADER_NAME, PS_SHADER_ENTRY_NAME ) )) return E_FAIL;
	if( FAILED( shader::CreateConstantBuffer( m_pDevice11, &m_pConstantBufferFrame, sizeof(C_CBUFFER) ))) return E_FAIL;

	m_AspectWindow = static_cast<float>(m_WndWidth)/static_cast<float>(m_WndHeight);
	m_FovCamera = static_cast<float>(3.141519/4.0);

	return S_OK;
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CCascadedShadowMap::Update()
{
	m_IsEndRender = false;

	D3DXVECTOR3 pos = CLightManager::GetPosition();
	D3DXVECTOR3 lookPos = CLightManager::GetLookPosition();

	// �r���[(�J����)�ϊ�.
	D3DXMatrixLookAtLH( 
		&m_ViewMatrix,		// (out)viewMarix.
		&pos,				// �J�������W.
		&lookPos,			// �J�����������W.
		&VIEW_UP_VECTOR );	// �J�����x�N�g��.

	const float size = D3DXVec3Length( &(lookPos-pos) );
	m_MinClipDistance = size * 0.5f;
	m_MaxClipDistance = size + m_MinClipDistance;

	// �v���W�F�N�V����(�ˉe)�ϊ�.
	D3DXMatrixOrthoLH(
		&m_ProjMatrix,				// (out)ProjMatrix.
		size,						// ����p.
		size,						// ��ʃA�X�y�N�g��.
		m_MinClipDistance,			// �ŏ��`�拗��.
		m_MaxClipDistance );		// �ő�`�拗��.

	// ���s��������.
	std::vector<float> spritPos( CASCADED_NUM + 1 );
	ComputeSplitPositions( CASCADED_NUM, 0.84f, 0.01f, 1000.0f, spritPos );

	// �J�X�P�[�h����.
	for( int i = 0; i < CASCADED_NUM; i++ ){
		// ���C�g�̃r���[�ˉe�s��.
		m_ShadowMatrix[i] = m_ViewMatrix * m_ProjMatrix;

		// ���������������8�p�����Ƃ߂āC
		//	���C�g�̃r���[�ˉe��Ԃ�AABB�����߂�.
		SBBox box = CalculateFrustum(
			spritPos[ i + 0 ],
			spritPos[ i + 1 ],
			m_ShadowMatrix[ i ] );

		// �N���b�v�s������߂�.
		D3DXMATRIX crop = CreateCropMatrix( box );

		// �V���h�E�}�b�v�s��ƕ����ʒu��ݒ�.
		m_ShadowMatrix[i]	= m_ShadowMatrix[i] * crop;
		m_SplitPos[i]		= spritPos[i+1];
	}

	this->SetBuffer();
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.

	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBufferFrame );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBufferFrame );	// �s�N�Z���V�F�[�_�[.
}

//------------------------------------.
// �`��.
//------------------------------------.
bool CCascadedShadowMap::Render( const bool& isSkin, const D3DXMATRIX& mW, std::function<void()>& func )
{
	if( m_IsEndRender == true ) return false;

	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( isSkin == true ? m_pSkinVertexShader : m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.

	for( int i = 0; i < CASCADED_NUM; i++ ){
		// �����_�[�^�[�Q�b�g�̐ݒ�.
		m_pContext11->OMSetRenderTargets( 1, &m_pRenderTargetViewList[i], CDirectX11::GetDepthSV() );
		// �f�v�X�X�e���V���o�b�t�@.
		m_pContext11->ClearDepthStencilView( CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );

		// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
		D3D11_MAPPED_SUBRESOURCE pData;
		if( SUCCEEDED( m_pContext11->Map( m_pConstantBufferFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
			C_CBUFFER cb;

			cb.ShadowVP = mW * m_ShadowMatrix[i];
			D3DXMatrixTranspose( &cb.ShadowVP, &cb.ShadowVP );

			// �������̈���R�s�[.
			memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
			m_pContext11->Unmap( m_pConstantBufferFrame, 0 );
		}

		func();
	}
	return true;
}

//------------------------------------.
// �o�b�t�@�̐ݒ�.
//------------------------------------.
void CCascadedShadowMap::SetBuffer()
{
	// G-Buffer�e�N�X�`���̃N���A.
	for( auto& rtv : m_pRenderTargetViewList ){
		m_pContext11->ClearRenderTargetView( rtv, CLEAR_BACK_COLOR );
	}
}

//------------------------------------.
// �e�N�X�`���̏�����.
//------------------------------------.
HRESULT CCascadedShadowMap::InitBufferTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= m_WndWidth;								// ��.
	texDesc.Height				= m_WndHeight;								// ����.
	texDesc.MipLevels			= 1;								// �~�b�v�}�b�v���x��:1.
	texDesc.ArraySize			= 1;								// �z��:1.
	texDesc.Format				= DXGI_FORMAT_R32_FLOAT;			// 32�r�b�g�t�H�[�}�b�g.
	texDesc.SampleDesc.Count	= 1;								// �}���`�T���v���̐�.
	texDesc.SampleDesc.Quality	= 0;								// �}���`�T���v���̃N�I���e�B.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// �g�p���@:�f�t�H���g.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// �����_�[�^�[�Q�b�g�A�V�F�[�_�[���\�[�X.
	texDesc.CPUAccessFlags		= 0;								// CPU����̓A�N�Z�X���Ȃ�.
	texDesc.MiscFlags			= 0;								// ���̑��̐ݒ�Ȃ�.

	m_pRenderTargetViewList.resize( CASCADED_NUM );
	m_pShaderResourceViewList.resize( CASCADED_NUM );
	m_pTexture2DList.resize( CASCADED_NUM );

	for( int i = 0; i < CASCADED_NUM; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pRenderTargetViewList[i],
			&m_pShaderResourceViewList[i],
			&m_pTexture2DList[i] ))) return E_FAIL;
	}
	return S_OK;
}

//------------------------------------.
// ���_�V�F�[�_�[�̍쐬.
//------------------------------------.
HRESULT CCascadedShadowMap::InitVertexShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

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

	// HLSL����o�[�e�b�N�X�V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		shader::InitShader(
		SHADER_NAME,		// �V�F�[�_�[�t�@�C����.
		"VS_SkinMain",		// �V�F�[�_�[�G���g���[�|�C���g.
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
		&m_pSkinVertexShader ))) {
		ERROR_MESSAGE("���_�V�F�[�_�[�쐬 : ���s");
		return E_FAIL;
	}
	return S_OK;
}

//------------------------------------.
// ���s��������.
//------------------------------------.
void CCascadedShadowMap::ComputeSplitPositions(
	const int splitCount, const float lamda, 
	const float nearClip, const float farClip,
	std::vector<float>& spritPos )
{
	// ���������P�̏ꍇ�́C���ʂ̃V���h�E�}�b�v�Ɠ���.
	if ( splitCount == 1 )
	{
		m_SplitPos[0] = nearClip;
		m_SplitPos[1] = farClip;
		return;
	}

	float inv_m = 1.0f / float( splitCount );    // splitCount���[���łȂ����Ƃ͕ۏ؍ς�.

	// �[�����Z�΍�.
	_ASSERT( nearClip != 0.0f );

	// (f/n)���v�Z.
	float f_div_n = farClip / nearClip;

	// (f-n)���v�Z.
	float f_sub_n = farClip - nearClip;

	// ���p�����X�L�[����K�p.
	// �� GPU Gems 3, Chapter 10. Parallel-Split Shadow Maps on Programmable GPUs.
	//    http://http.developer.nvidia.com/GPUGems3/gpugems3_ch10.html ���Q��.
	for( int i=1; i<splitCount + 1; ++i )
	{
		// �ΐ������X�L�[���Ōv�Z.
		float Ci_log = nearClip * powf( f_div_n, inv_m * i );

		// ��l�����X�L�[���Ōv�Z.
		float Ci_uni = nearClip + f_sub_n * i * inv_m;

		// ��L�̂Q�̉��Z���ʂ���`��Ԃ���.
		spritPos[i] = lamda * Ci_log + Ci_uni * ( 1.0f - lamda );
	}

	// �ŏ���, �j�A���ʂ܂ł̋�����ݒ�.
	spritPos[ 0 ] = nearClip;

	// �Ō��, �t�@�[���ʂ܂ł̋�����ݒ�.
	spritPos[ splitCount ] = farClip;
}

//------------------------------------.
// ���������������8�p�����Ƃ߂āC
//	���C�g�̃r���[�ˉe��Ԃ�AABB�����߂�.
//------------------------------------.
CCascadedShadowMap::SBBox CCascadedShadowMap::CalculateFrustum( float nearClip, float farClip, D3DXMATRIX& viewProj )
{
	D3DXVECTOR3 Dir = CCameraManager::GetLookPosition() - CCameraManager::GetPosition();
	D3DXVECTOR3 vZ;
	D3DXVec3Normalize( &vZ,&Dir );
	D3DXVECTOR3 vX;
	D3DXVec3Cross( &vX, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &vZ );
	D3DXVec3Normalize( &vX, &vX );
	D3DXVECTOR3 vY;
	D3DXVec3Cross( &vY, &vZ, &vX );
	D3DXVec3Normalize( &vY, &vY );

	float nearPlaneHalfHeight = tanf( m_FovCamera * 0.5f ) * nearClip;
	float nearPlaneHalfWidth  = nearPlaneHalfHeight * m_AspectWindow;

	float farPlaneHalfHeight = tanf( m_FovCamera * 0.5f ) * farClip;
	float farPlaneHalfWidth  = farPlaneHalfHeight * m_AspectWindow;

	D3DXVECTOR3 nearPlaneCenter = CCameraManager::GetPosition() + vZ * nearClip;
	D3DXVECTOR3 farPlaneCenter  = CCameraManager::GetPosition() + vZ * farClip;;

	D3DXVECTOR3 corners[8];

	corners[0] = D3DXVECTOR3( nearPlaneCenter - vX * nearPlaneHalfWidth - vY * nearPlaneHalfHeight );
	corners[1] = D3DXVECTOR3( nearPlaneCenter - vX * nearPlaneHalfWidth + vY * nearPlaneHalfHeight );
	corners[2] = D3DXVECTOR3( nearPlaneCenter + vX * nearPlaneHalfWidth + vY * nearPlaneHalfHeight );
	corners[3] = D3DXVECTOR3( nearPlaneCenter + vX * nearPlaneHalfWidth - vY * nearPlaneHalfHeight );

	corners[4] = D3DXVECTOR3( farPlaneCenter - vX * farPlaneHalfWidth - vY * farPlaneHalfHeight );
	corners[5] = D3DXVECTOR3( farPlaneCenter - vX * farPlaneHalfWidth + vY * farPlaneHalfHeight );
	corners[6] = D3DXVECTOR3( farPlaneCenter + vX * farPlaneHalfWidth + vY * farPlaneHalfHeight );
	corners[7] = D3DXVECTOR3( farPlaneCenter + vX * farPlaneHalfWidth - vY * farPlaneHalfHeight );


	D3DXVECTOR3 point;
	D3DXVec3TransformCoord( &point, &corners[0], &viewProj );
	D3DXVECTOR3 mini = point;
	D3DXVECTOR3 maxi = point;
	for( int i=1; i<8; ++i )
	{
		D3DXVec3TransformCoord( &point, &corners[i], &viewProj );
		D3DXVec3Minimize( &mini, &point, &mini );
		D3DXVec3Maximize( &maxi, &point, &maxi );
	}

	return SBBox( mini, maxi );
}

//------------------------------------.
// �N���b�v�s������߂�.
//------------------------------------.
D3DXMATRIX CCascadedShadowMap::CreateCropMatrix( SBBox& box )
{
	/* �قڒP�ʃL���[�u�N���b�s���O�Ɠ������� */
	float scaleX  = 1.0f;
	float scaleY  = 1.0f;
	float scaleZ  = 1.0f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float offsetZ = 0.0f;

	D3DXVECTOR3 mini = box.Min;
	D3DXVECTOR3 maxi = box.Max;

	scaleX = 2.0f / ( maxi.x - mini.x );
	scaleY = 2.0f / ( maxi.y - mini.y );

	offsetX = -0.5f * ( maxi.x + mini.x ) * scaleX;
	offsetY = -0.5f * ( maxi.y + mini.y ) * scaleY;

	// 1.0�������ƃV���h�E�}�b�v�Ɉڂ镔�����������Ȃ��Ă��܂��̂ŁC
	// ������������.
	scaleX = max( 1.0f, scaleX );
	scaleY = max( 1.0f, scaleY );

	return D3DXMATRIX(
		scaleX,  0.0f,    0.0f,    0.0f,
		0.0f,    scaleY,  0.0f,    0.0f,
		0.0f,    0.0f,    scaleZ,  0.0f,
		offsetX, offsetY, offsetZ, 1.0f );
}
