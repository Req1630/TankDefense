#include "D3DX11.h"
#include "..\..\Common\DebugText\DebugText.h"

namespace
{
	constexpr float CLEAR_BACK_COLOR[4] = { 0.6f, 0.6f, 0.6f, 1.0f };	// �o�b�N�J���[.
};

CDirectX11::CDirectX11()
	: m_hWnd					( nullptr )
	, m_pDevice11				( nullptr )
	, m_pContext11				( nullptr )
	, m_pSwapChain				( nullptr )
	, m_pBackBuffer_TexRTV		( nullptr )
	, m_pBackBuffer_DSTex		( nullptr )
	, m_pBackBuffer_DSTexDSV	( nullptr )
	, m_pDepthStencilState		( nullptr )
	, m_pDepthStencilStateOff	( nullptr )
	, m_pAlphaBlend				( nullptr )
	, m_pNoAlphaBlend			( nullptr )
	, m_pAlphaToCoverage		( nullptr )
	, m_pRsSoldAndNone			( nullptr )
	, m_pRsSoldAndBack			( nullptr )
	, m_pRsSoldAndFront			( nullptr )
	, m_pRsWireFrame			( nullptr )
	, m_WndWidth				( 0 )
	, m_WndHeight				( 0 )
{
}

CDirectX11::~CDirectX11()
{
}

//-----------------------------------.
// �C���X�^���X�̎擾.
//-----------------------------------.
CDirectX11* CDirectX11::GetInstance()
{
	static std::unique_ptr<CDirectX11> pInstance = 
		std::make_unique<CDirectX11>();
	return pInstance.get();
}

//-----------------------------------.
// DirectX11�\�z�֐�.
//-----------------------------------.
HRESULT CDirectX11::Create( HWND hWnd )
{
	GetInstance()->m_hWnd = hWnd;

	GetInstance()->m_WndWidth = static_cast<UINT>(WND_W);
	GetInstance()->m_WndHeight = static_cast<UINT>(WND_H);

	CDebugText::PushLog( "DirectX11 Device Create Start" );

	if( FAILED(GetInstance()->InitDevice11()) )			return E_FAIL;
	if( FAILED(GetInstance()->InitTexRTV()) )			return E_FAIL;
	if( FAILED(GetInstance()->InitDSTex()) )			return E_FAIL;
	if( FAILED(GetInstance()->InitViewports()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitBlend()) )			return E_FAIL;
	if( FAILED(GetInstance()->InitDeprh()) )			return E_FAIL;
	if( FAILED(GetInstance()->InitRasterizerState()) )	return E_FAIL;

	CDebugText::PushLog( "DirectX11 Device Create End [Succeeded]" );
	CLog::Print( "DirectX11�f�o�C�X�쐬 : ����" );

	return S_OK;
}

//-----------------------------------.
// DirectX11����֐�.
//-----------------------------------.
HRESULT CDirectX11::Release()
{

	SAFE_RELEASE( GetInstance()->m_pDepthStencilState );
	SAFE_RELEASE( GetInstance()->m_pDepthStencilStateOff );
	SAFE_RELEASE( GetInstance()->m_pAlphaBlend );
	SAFE_RELEASE( GetInstance()->m_pNoAlphaBlend );
	SAFE_RELEASE( GetInstance()->m_pAlphaToCoverage );
	SAFE_RELEASE( GetInstance()->m_pRsSoldAndNone );
	SAFE_RELEASE( GetInstance()->m_pRsSoldAndBack );
	SAFE_RELEASE( GetInstance()->m_pRsSoldAndFront );
	SAFE_RELEASE( GetInstance()->m_pRsWireFrame );

	SAFE_RELEASE( GetInstance()->m_pBackBuffer_DSTexDSV );
	SAFE_RELEASE( GetInstance()->m_pBackBuffer_DSTex );
	SAFE_RELEASE( GetInstance()->m_pBackBuffer_TexRTV );
	SAFE_RELEASE( GetInstance()->m_pSwapChain );
	SAFE_RELEASE( GetInstance()->m_pContext11 );
	SAFE_RELEASE( GetInstance()->m_pDevice11 );

	return S_OK;
}

//-----------------------------------.
// �N���A�o�b�N�o�b�t�@.
//-----------------------------------.
void CDirectX11::ClearBackBuffer()
{
	// �J���[�o�b�N�o�b�t�@.
	GetInstance()->m_pContext11->ClearRenderTargetView( 
		GetInstance()->m_pBackBuffer_TexRTV, CLEAR_BACK_COLOR );
}

//-----------------------------------.
// �X���b�v�`�F�[���v���[���g.
//-----------------------------------.
void CDirectX11::SwapChainPresent()
{
	GetInstance()->m_pSwapChain->Present( 0, 0 );
}

//-----------------------------------.
// BackBuffer�̐ݒ�.
//-----------------------------------.
void CDirectX11::SetBackBuffer()
{
	// �����_�[�^�[�Q�b�g�̐ݒ�.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		1, 
		&GetInstance()->m_pBackBuffer_TexRTV,
		GetInstance()->m_pBackBuffer_DSTexDSV );
	// �f�v�X�X�e���V���o�b�t�@.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );
}

//--------------------------------------------.
// �u�����h��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CDirectX11::SetBlend( bool EnableAlpha )
{
	// �u�����h�X�e�[�g�̐ݒ�.
	UINT mask = 0xffffffff;	// �}�X�N�l.
	ID3D11BlendState* blend = 
		EnableAlpha == true ? GetInstance()->m_pAlphaBlend : GetInstance()->m_pNoAlphaBlend;
	GetInstance()->m_pContext11->OMSetBlendState( blend, nullptr, mask );
}

//--------------------------------------------.
// �A���t�@�J�o���[�W��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CDirectX11::SetCoverage( bool EnableCoverage )
{
	// �u�����h�X�e�[�g�̐ݒ�.
	UINT mask = 0xffffffff;	// �}�X�N�l.
	ID3D11BlendState* blend = 
		EnableCoverage == true ? GetInstance()->m_pAlphaToCoverage : GetInstance()->m_pNoAlphaBlend;
	GetInstance()->m_pContext11->OMSetBlendState( blend, nullptr, mask );
}

//--------------------------------------------.
// �[�x�e�X�g��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CDirectX11::SetDeprh( bool flag )
{
	ID3D11DepthStencilState* pTmp
		= ( flag == true ) ? GetInstance()->m_pDepthStencilState : GetInstance()->m_pDepthStencilStateOff;
	// �[�x�ݒ���Z�b�g.
	GetInstance()->m_pContext11->OMSetDepthStencilState( pTmp, 1 );
}

//--------------------------------------------.
// ���X�^���C�U�X�e�[�g�ݒ�.
//--------------------------------------------.
void CDirectX11::SetRasterizerState( const ERS_STATE& rsState )
{
	switch( rsState )
	{
	case enRS_STATE::None:	// ���w�ʕ`��.
		GetInstance()->m_pContext11->RSSetState( GetInstance()->m_pRsSoldAndNone );
		break;
	case enRS_STATE::Back:	// �w�ʂ�`�悵�Ȃ�.
		GetInstance()->m_pContext11->RSSetState( GetInstance()->m_pRsSoldAndBack );
		break;
	case enRS_STATE::Front:	// ���ʂ�`�悵�Ȃ�.
		GetInstance()->m_pContext11->RSSetState( GetInstance()->m_pRsSoldAndFront );
		break;
	case enRS_STATE::Wire:	// ���C���[�t���[���`��.
		GetInstance()->m_pContext11->RSSetState( GetInstance()->m_pRsWireFrame );
		break;
	default:
		break;
	}
}

//-----------------------------------.
// �t���X�N���[���̐ݒ�.
//-----------------------------------.
bool CDirectX11::SetFullScreen( const bool& isOn )
{
	// ���݂̃X�N���[�������擾.
	BOOL isState = FALSE;
	GetInstance()->m_pSwapChain->GetFullscreenState( &isState, nullptr );
	if( isOn == true ){
		// ���݂̏�Ԃ��E�B���h�E��ԂȂ�.
		if( isState == FALSE ){
			// �t���X�N���[���ɕύX.
			GetInstance()->m_pSwapChain->SetFullscreenState( TRUE, nullptr );
			ShowCursor( FALSE );	// �}�E�X���\���ɂ���.
		}
		return true;
	} else {
		// ���݂̏�Ԃ��t���X�N���[���Ȃ�.
		if( isState == TRUE ){
			// �E�B���h�E�ɕύX.
			GetInstance()->m_pSwapChain->SetFullscreenState( FALSE, nullptr );
			ShowCursor( TRUE );		// �}�E�X��\������.
		}
		return false;
	}
	return true;
}

//-----------------------------------.
// �t���X�N���[����Ԃ��擾.
//-----------------------------------.
bool CDirectX11::IsFullScreen()
{
	if( GetInstance()->m_pSwapChain == nullptr ) return false;
	// ���݂̃X�N���[�������擾.
	BOOL isState = FALSE;
	GetInstance()->m_pSwapChain->GetFullscreenState( &isState, nullptr );
	return static_cast<bool>(isState);
}

//-----------------------------------.
// �E�B���h�E�T�C�Y���ύX���ꂽ���ɌĂ�.
//-----------------------------------.
void CDirectX11::Resize()
{
	if( GetInstance()->m_pContext11 == nullptr ) return;

	// �Z�b�g���Ă��郌���_�[�^�[�Q�b�g���O��.
	GetInstance()->m_pContext11->OMSetRenderTargets( 0, nullptr, nullptr );

	// �g�p���Ă����o�b�N�o�b�t�@���������.
	SAFE_RELEASE( GetInstance()->m_pBackBuffer_TexRTV );
	SAFE_RELEASE( GetInstance()->m_pBackBuffer_DSTex );
	SAFE_RELEASE( GetInstance()->m_pBackBuffer_DSTexDSV );

	// �X���b�v�`�F�[�������T�C�Y����.
	// width, height ���w�肵�Ȃ��ꍇ�AhWnd���Q�Ƃ��A�����Ōv�Z���Ă����.
	if( FAILED( GetInstance()->m_pSwapChain->ResizeBuffers( 0, 0, 0, DXGI_FORMAT_UNKNOWN, 0 ) )){
		ERROR_MESSAGE( "�f�v�X�X�e���V���r���[�쐬���s" );
		return;
	}

	// �X���b�v�`�F�[���̃o�b�t�@�̎擾.
	ID3D11Texture2D* pBuufer = nullptr;
	if( FAILED( GetInstance()->m_pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBuufer ) )){
		ERROR_MESSAGE( "�f�v�X�X�e���V���r���[�쐬���s" );
		return;
	}

	// �e�N�X�`�����̎擾.
	D3D11_TEXTURE2D_DESC texDesc = {0};
	pBuufer->GetDesc( &texDesc );
	GetInstance()->m_WndWidth	= texDesc.Width;
	GetInstance()->m_WndHeight	= texDesc.Height;
	SAFE_RELEASE( pBuufer );

	if( FAILED( GetInstance()->InitTexRTV() )){
		ERROR_MESSAGE( "�f�v�X�X�e���V���r���[�쐬���s" );
		return;
	}
	if( FAILED( GetInstance()->InitDSTex() )){
		ERROR_MESSAGE( "�f�v�X�X�e���V���r���[�쐬���s" );
		return;
	}

	// �����_�[�^�[�Q�b�g�̐ݒ�.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		1, 
		&GetInstance()->m_pBackBuffer_TexRTV,
		GetInstance()->m_pBackBuffer_DSTexDSV );
	// �f�v�X�X�e���V���o�b�t�@.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );

	// �r���[�|�[�g�̐ݒ�.
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT)GetInstance()->m_WndWidth;
	vp.Height	= (FLOAT)GetInstance()->m_WndHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	GetInstance()->m_pContext11->RSSetViewports( 1, &vp );
}

//-----------------------------------.
// �f�o�C�X11�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitDevice11()
{
// MSAA(�A���`�G�C���A�X)���g�p���悤�Ǝv������,
//	�֊s���Ƒ������������ߖ����ɂ��Ă���.
//	���݃V�F�[�_�[��FXAA���g�p���ăA���`�G�C���A�X���s���Ă���.
#if 0
	//�f�o�C�X�̐���
	HRESULT hr;
	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&m_pDevice11,
		NULL,
		&m_pContext11);
	if(FAILED(hr)){
		MessageBoxW(m_hWnd, L"D3D11CreateDevice", L"Err", MB_ICONSTOP);
	}

	for(int i=0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++){
		UINT Quality;
		if( SUCCEEDED(
			m_pDevice11->CheckMultisampleQualityLevels(
				DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality ))){
			if(0 < Quality){
				m_MSAASampleDesc.Count = i;
				m_MSAASampleDesc.Quality = Quality - 1;
			}
		}
	}

	//�C���^�[�t�F�[�X�擾
	IDXGIDevice1* hpDXGI = NULL;
	if(FAILED(m_pDevice11->QueryInterface(__uuidof(IDXGIDevice1), (void**)&hpDXGI))){
		MessageBoxW(m_hWnd, L"QueryInterface", L"Err", MB_ICONSTOP);
	}

	//�A�_�v�^�[�擾
	IDXGIAdapter* hpAdapter = NULL;
	if(FAILED(hpDXGI->GetAdapter(&hpAdapter))){
		MessageBoxW(m_hWnd, L"GetAdapter", L"Err", MB_ICONSTOP);
	}

	//�t�@�N�g���[�擾
	IDXGIFactory* hpDXGIFactory = NULL;
	hpAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&hpDXGIFactory);
	if(hpDXGIFactory == NULL){
		MessageBoxW(m_hWnd, L"GetParent", L"Err", MB_ICONSTOP);
	}

	//�X���b�v�`�F�C���쐬
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width	= WND_W;
	swapChainDesc.BufferDesc.Height	= WND_H;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling			= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc	= m_MSAASampleDesc;
	swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount	= 1;
	swapChainDesc.OutputWindow	= m_hWnd;
	swapChainDesc.Windowed		= TRUE;
	swapChainDesc.SwapEffect	= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags			= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if(FAILED(hpDXGIFactory->CreateSwapChain(m_pDevice11, &swapChainDesc, &m_pSwapChain))){
		MessageBoxW(m_hWnd, L"CreateSwapChain", L"Err", MB_ICONSTOP);
	}
#else
	// �X���b�v�`�F�[���\����.
	DXGI_SWAP_CHAIN_DESC sd = {0};
	sd.BufferCount			= 1;								// �o�b�N�o�b�t�@�̐�.
	sd.BufferDesc.Width		= GetInstance()->m_WndWidth ;		// �o�b�N�o�b�t�@�̕�.
	sd.BufferDesc.Height	= GetInstance()->m_WndHeight;		// �o�b�N�o�b�t�@�̍���.
	sd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;		// �t�H�[�}�b�g(32�ޯĶװ).
	sd.BufferDesc.RefreshRate.Numerator		= 60;				// ���t���b�V�����[�g(����) ��FPS:60.
	sd.BufferDesc.RefreshRate.Denominator	= 1;				// ���t���b�V�����[�g(���q).
	sd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �g����(�\����).
	sd.OutputWindow			= m_hWnd;							// �E�B���h�E�n���h��.
	sd.SampleDesc.Count		= 1;								// �}���`�T���v���̐�.
	sd.SampleDesc.Quality	= 0;								// �}���`�T���v���̃N�I���e�B.
	sd.Windowed				= TRUE;								// �E�B���h�E���[�h(�t���X�N���[������FALSE).

	//�쐬�����݂�@�\���x���̗D����w��.
	// (GPU���T�|�[�g����@�\��Ă̒�`).
	// D3D_FEATURE_LEVEL�񋓌^�̔z��.
	// D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 �� GPU���x��.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;	// �z��̗v�f��.

	auto createDevice = [&]( const D3D_DRIVER_TYPE& type ) -> HRESULT
	{
		return D3D11CreateDeviceAndSwapChain(
			nullptr,			// �r�f�I�A�_�v�^�ւ̃|�C���^.
			type,				// �쐬����Ãf�o�C�X�̎��.
			nullptr,			// �\�t�g�E�F�A ���X�^���C�U����������DLL�̃n���h��.
			0,					// �L���ɂ��郉���^�C�����C���[.
			&pFeatureLevels,	// �쐬�����݂�@�\���x���̏������w�肷��z��ւ̃|�C���^.
			1,					// ���̗v�f��.
			D3D11_SDK_VERSION,	// SDK�̃o�[�W����.
			&sd,				// �X���b�v�`�F�[���̏������p�����[�^�̃|�C���^.
			&m_pSwapChain,		// (out) �����_�����O�Ɏg�p����X���b�v�`�F�[��.
			&m_pDevice11,		// (out) �쐬���ꂽ�f�o�C�X.
			pFeatureLevel,		// �@�\���ق̔z��ɂ���ŏ��̗v�f��\���|�C���^.
			&m_pContext11 );	// (out) �f�o�C�X�R���e�L�X�g.
	};

	if( FAILED(createDevice(D3D_DRIVER_TYPE_HARDWARE)) ){
		if( FAILED(createDevice(D3D_DRIVER_TYPE_WARP)) ){
			if( FAILED(createDevice(D3D_DRIVER_TYPE_REFERENCE)) ){
				ERROR_MESSAGE( "�f�o�C�X�ƃX���b�v�`�F�[���쐬 : ���s" );
				return E_FAIL;
			}
		}
	}
	pFeatureLevel = nullptr;

	// ALT + Enter�Ńt���X�N���[���𖳌�������.
	IDXGIFactory* pFactory = nullptr;
	// ��ō����IDXGISwapChain���g��.
	m_pSwapChain->GetParent( __uuidof(IDXGIFactory), (void**)&pFactory );
	// �]�v�ȋ@�\�𖳌��ɂ���ݒ������.
	pFactory->MakeWindowAssociation( m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER );
	SAFE_RELEASE( pFactory );
#endif

	return S_OK;
}

//-----------------------------------.
// �����_�[�^�[�Q�b�g�r���[�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitTexRTV()
{
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	if( FAILED( m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:���Ɋ֘A�t�����ꂽGUID���擾.
		(LPVOID*)&pBackBuffer_Tex ))){	//(out)�o�b�N�o�b�t�@�e�N�X�`��.
		ERROR_MESSAGE( "�o�b�N�o�b�t�@�e�N�X�`���쐬 : ���s" );
	}
	// ���̃e�N�X�`���ɑ΂��ă����_�[�^�[�Q�b�g�r���[(RTV)���쐬.
	if( FAILED( m_pDevice11->CreateRenderTargetView(
		pBackBuffer_Tex,
		nullptr,
		&m_pBackBuffer_TexRTV ))){	//(out)RTV.
		// �o�b�N�o�b�t�@�e�N�X�`�������.
		SAFE_RELEASE(pBackBuffer_Tex);
		ERROR_MESSAGE( "�����_�[�^�[�Q�b�g�r���[�쐬 : ���s" );
	}

	// �o�b�N�o�b�t�@�e�N�X�`�������.
	SAFE_RELEASE(pBackBuffer_Tex);

	return S_OK;
}

//-----------------------------------.
// �X�e���V���r���[�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitDSTex()
{
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= GetInstance()->m_WndWidth ;	// ��.
	descDepth.Height				= GetInstance()->m_WndHeight;	// ����.
	descDepth.MipLevels				= 1;						// �~�b�v�}�b�v���x��:1.
	descDepth.ArraySize				= 1;						// �z��:1.
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// 32�r�b�g�t�H�[�}�b�g.
	descDepth.SampleDesc.Count		= 1;						// �}���`�T���v���̐�.
	descDepth.SampleDesc.Quality	= 0;						// �}���`�T���v���̃N�I���e�B.
	descDepth.Usage					= D3D11_USAGE_DEFAULT;		// �g�p���@:�f�t�H���g.
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// �[�x(�X�e���V���Ƃ��Ďg�p).
	descDepth.CPUAccessFlags		= 0;						// CPU����̓A�N�Z�X���Ȃ�.
	descDepth.MiscFlags				= 0;						// ���̑��̐ݒ�Ȃ�.
//	descDepth.SampleDesc			= m_MSAASampleDesc;


	// ���̃e�N�X�`���ɑ΂��ăf�v�X�X�e���V��(DSTex)���쐬.
	if( FAILED( m_pDevice11->CreateTexture2D( &descDepth, nullptr, &m_pBackBuffer_DSTex )) ){
		ERROR_MESSAGE( "�f�v�X�X�e���V���쐬 : ���s" );
		return E_FAIL;
	}
	// ���̃e�N�X�`���ɑ΂��ăf�u�X�X�e���V���r���[(DSV)���쐬.
	if( FAILED( m_pDevice11->CreateDepthStencilView( m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV)) ){
		ERROR_MESSAGE( "�f�v�X�X�e���V���r���[�쐬 : ���s" );
		return E_FAIL;
	}
	// �����_�[�^�[�Q�b�g�����޽�X�e���V���r���[���p�C�v���C���ɃZ�b�g.
	m_pContext11->OMSetRenderTargets( 1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV );
	return S_OK;
}

// �r���[�|�[�g�̍쐬.
HRESULT CDirectX11::InitViewports( const D3D11_VIEWPORT& vp )
{
	m_pContext11->RSSetViewports( 1, &vp );
	return S_OK;
}

//-----------------------------------.
// �r���[�|�[�g�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitViewports()
{
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT)GetInstance()->m_WndWidth ;	// ��.
	vp.Height	= (FLOAT)GetInstance()->m_WndHeight;	// ����.
	vp.MinDepth = 0.0f;			// �ŏ��[�x(��O).
	vp.MaxDepth = 1.0f;			// �ő�[�x(��).
	vp.TopLeftX = 0.0f;			// ����ʒux.
	vp.TopLeftY = 0.0f;			// ����ʒuy.

	m_pContext11->RSSetViewports( 1, &vp );
	return S_OK;
}

//-----------------------------------.
// ���X�^���C�U�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitRasterizer()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode				= D3D11_FILL_SOLID;	// �h��Ԃ�(�\���b�h).
	rdc.CullMode				= D3D11_CULL_NONE;	// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	rdc.FrontCounterClockwise	= FALSE;			// �|���S���̕\�������肷��t���O.
	rdc.DepthClipEnable			= FALSE;			// �����ɂ��ẴN���b�s���O�L��.

	ID3D11RasterizerState* pRs = nullptr;
	if( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &pRs )) ){
		SAFE_RELEASE( pRs );
		ERROR_MESSAGE( "���X�^���C�U�[�쐬 : ���s" );
		return E_FAIL;
	}
	m_pContext11->RSSetState( pRs );
	SAFE_RELEASE( pRs );

	return S_OK;
}

//--------------------------------------------.
// �u�����h�쐬.
//--------------------------------------------.
HRESULT CDirectX11::InitBlend()
{
	// �A���t�@�u�����h�p�u�����h�X�e�[�g�\����.
	// �摜�t�@�C�����ɃA���t�@��񂪂���̂ŁA���߂���悤�Ƀu�����h�X�e�[�g�Őݒ肷��.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory( &BlendDesc, sizeof( BlendDesc ) );

	BlendDesc.IndependentBlendEnable				= false;
	BlendDesc.AlphaToCoverageEnable					= false;
	for( int i = 0; i < 8; i++ ){
		BlendDesc.RenderTarget[i].BlendEnable			= true;
		BlendDesc.RenderTarget[i].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[i].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
		BlendDesc.RenderTarget[i].BlendOp				= D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[i].SrcBlendAlpha			= D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[i].DestBlendAlpha		= D3D11_BLEND_ZERO;
		BlendDesc.RenderTarget[i].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[i].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// �u�����h�X�e�[�g�쐬.
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaBlend ) ) ){
		ERROR_MESSAGE( "�u�����h�X�e�[�g�쐬 : ���s" );
		return E_FAIL;
	}

	// �A���t�@�g�D�[�J�o���[�W���I���ɂ��č쐬.
	BlendDesc.AlphaToCoverageEnable = true;
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaToCoverage ) ) ){
		ERROR_MESSAGE( "�A���t�@�g�D�[�J�o���[�W�쐬 : ���s" );
		return E_FAIL;
	}

	// �u�����h�X�e�[�g������Ԃ̍쐬.
	for( int i = 0; i < 8; i++ ){
		BlendDesc.RenderTarget[i].BlendEnable	= false;
	}
	BlendDesc.AlphaToCoverageEnable			= false;
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pNoAlphaBlend ) ) ){
		ERROR_MESSAGE( "�u�����h�X�e�[�g������ԍ쐬 : ���s" );
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------.
// �[�x�e�X�g�쐬.
//--------------------------------------------.
HRESULT CDirectX11::InitDeprh()
{
	// �[�x�e�X�g(z�e�X�g)��L���ɂ���.
	D3D11_DEPTH_STENCIL_DESC dsDesc = D3D11_DEPTH_STENCIL_DESC();

	dsDesc.DepthEnable		= TRUE;	// �L��.
	dsDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc		= D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable	= FALSE;
	dsDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// �[�x�e�X�g�L����Ԃ̍쐬.
	if( FAILED( m_pDevice11->CreateDepthStencilState(
		&dsDesc, &m_pDepthStencilState ))){
		ERROR_MESSAGE( "�[�x�e�X�g�쐬 : ���s" );
		return E_FAIL;
	}

	// �[�x�e�X�g������Ԃ̍쐬.
	dsDesc.DepthEnable = FALSE;	// ����.
	// �[�x�ݒ�쐬.
	if( FAILED( m_pDevice11->CreateDepthStencilState(
		&dsDesc, &m_pDepthStencilStateOff ))){
		ERROR_MESSAGE( "�[�x�e�X�g�쐬 : ���s" );
		return E_FAIL;
	}
	return S_OK;
}

//--------------------------------------------.
// ���X�^���C�U�X�e�[�g�쐬.
//--------------------------------------------.
HRESULT CDirectX11::InitRasterizerState()
{
	auto createRasterizerState = [&]( const D3D11_RASTERIZER_DESC& rdc, ID3D11RasterizerState** ppRs )
	{
		if( FAILED( m_pDevice11->CreateRasterizerState( &rdc, ppRs )) ){
			ERROR_MESSAGE("���X�^���C�U�[�쐬 : ���s");
			return E_FAIL;
		}
		return S_OK;
	};

	D3D11_RASTERIZER_DESC rdc = {};
	rdc.FillMode				= D3D11_FILL_SOLID;	// �h��Ԃ�(�\���b�h).
	rdc.CullMode				= D3D11_CULL_NONE;	// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	rdc.FrontCounterClockwise	= FALSE;			// �|���S���̕\�������肷��t���O.
	rdc.DepthClipEnable			= FALSE;			// �����ɂ��ẴN���b�s���O�L��.

	if( FAILED( createRasterizerState( rdc, &m_pRsSoldAndNone ) )){
		ERROR_MESSAGE("���X�^���C�U�[�쐬 : ���s");
		return E_FAIL;
	}

	rdc.FillMode = D3D11_FILL_SOLID;// �h��Ԃ�(�\���b�h).
	rdc.CullMode = D3D11_CULL_BACK;	// BACK:�w�ʂ�`�悵�Ȃ�,
	if( FAILED( createRasterizerState( rdc, &m_pRsSoldAndBack ) )){
		ERROR_MESSAGE("���X�^���C�U�[�쐬 : ���s");
		return E_FAIL;
	}

	rdc.FillMode = D3D11_FILL_SOLID;// �h��Ԃ�(�\���b�h).
	rdc.CullMode = D3D11_CULL_FRONT;// FRONT:���ʂ�`�悵�Ȃ�.
	if( FAILED( createRasterizerState( rdc, &m_pRsSoldAndFront ) )){
		ERROR_MESSAGE("���X�^���C�U�[�쐬 : ���s");
		return E_FAIL;
	}

	rdc.FillMode = D3D11_FILL_WIREFRAME;// ���C���[�t���[��.
	rdc.CullMode = D3D11_CULL_NONE;		// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	if( FAILED( createRasterizerState( rdc, &m_pRsWireFrame ) )){
		ERROR_MESSAGE("���X�^���C�U�[�쐬 : ���s");
		return E_FAIL;
	}

	m_pContext11->RSSetState( m_pRsSoldAndNone );

	return S_OK;
}