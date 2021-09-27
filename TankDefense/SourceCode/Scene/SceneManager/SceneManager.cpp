#include "SceneManager.h"
#include "..\Scenes\SceneList.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Common\RenderingTextuer\RenderingTextuerManager\RenderingTextuerManager.h"
#include "..\..\Object\Collider\CollisionRender\CollisionRender.h"
#include "..\..\Object\LoadRender\LoadRender.h"
#include "..\..\Object\CameraBase\CameraManager\CameraManager.h"
#include "..\..\Object\LightBase\LightManager\LightManager.h"
#include "..\..\Object\Fade\Fade.h"

CSceneManager::CSceneManager()
	: m_hWnd					( nullptr )
	, m_pScene					( nullptr )
	, m_pLoadRender				( nullptr )
	, m_pRenderingTexManager	( nullptr )
	, m_ModelRenderFunction		( nullptr )
	, m_Sprite3DRenderFunction	( nullptr )
	, m_NowScene				( EScene::Start )
	, m_NextScene				( EScene::Start )
	, m_IsThreadLoadEnd			( false )
	, m_IsLoadEnd				( false )
	, m_IsOldLoadEnd			( false )
	, m_OnceStep				( false )
	, m_isThreadJoined			( false )
{
	m_pLoadRender = std::make_unique<CLoadRender>();
	m_pRenderingTexManager = std::make_unique<CRenderingTexterManager>();
	m_ModelRenderFunction = [&]()
	{ 
		m_pScene->ModelRender();
		CLightManager::PositionRender();		// ���C�g�̈ʒu�`��.
		CCollisionRender::Render();				// �����蔻��̕`��.
	};
	m_Sprite3DRenderFunction = [&]()
	{
		m_pScene->Sprite3DRender();
	};
}

CSceneManager::~CSceneManager()
{
}

//=================================.
// ������.
//=================================.
void CSceneManager::Init( const bool& isResourceLoadEnd )
{
	if( isResourceLoadEnd == false ) return;
	if( m_OnceStep == true ) return;
	if( m_pRenderingTexManager->Init( CDirectX11::GetContext() ) ) return;
	ChangeNextScene();
	m_OnceStep = true;
}

//=================================.
//	�X�V�֐�.
//=================================.
void CSceneManager::Update( const float& deltaTime, const bool& isResourceLoadEnd )
{
	// ���\�[�X�̓ǂݍ��݂��I�����ĂȂ��ꍇ,
	//	���[�h�p�̕`�������.
	LoadRender( deltaTime, isResourceLoadEnd );
	if( m_pScene == nullptr ) return;

	// �X���b�h�̉������.
	ThreadRelease();

	if( m_IsLoadEnd == false ){
		// �X���b�h�̃��[�h���I����āA�t�F�[�h���I��(�s���ĂȂ�)���Ă���ꍇ,
		if( m_IsThreadLoadEnd == true && CFade::IsFadeEnd() == true ){
			CFade::KeepRender();
			CFade::SetFadeIn( true );	// �t�F�[�h�C��.
			CFrameRate::ClearElapsedTime();
		}
		// �X���b�h�̃��[�h���I����āA�t�F�[�h������Out�̏ꍇ,
		//	���[�h�I���t���O�𗧂ĂāA�V�[���̍X�V���s��.
		if( m_IsThreadLoadEnd == true && CFade::IsFadeOut() == true ){
			m_IsLoadEnd = true;
		}
		// ���[�h�̕`��.
		LoadRender( deltaTime, false );

	} else {
		// �E�B���h�E�̃T�C�Y���ύX����Ă���΁A
		//	�e�N�X�`���̃T�C�Y��ύX����.
		m_pRenderingTexManager->ResizeTexture();

		// �V�[���̍X�V.
		SceneUpdate( deltaTime );
	}
}

//=================================.
// ���̃V�[���ɕύX.
//=================================.
void CSceneManager::ChangeNextScene()
{
	switch( m_NextScene )
	{
	case EScene::Title:
		m_pScene	= std::make_shared<CTitle>( this );
		m_NowScene	= m_NextScene;
		m_NextScene	= EScene::GameMain;
		break;
	case EScene::GameMain:
		m_pScene	= std::make_shared<CGame>( this );
		m_NowScene	= m_NextScene;
		m_NextScene	= EScene::GameMain;
		break;
	case EScene::Edit:
	{
		m_pScene	= std::make_shared<CEdit>( this );
		const EScene tempScene = m_NextScene;
		m_NextScene	= m_NowScene;
		m_NowScene	= tempScene;
	}
		break;
	default:
		return;
	}

	// ���ڈȍ~�̓t�F�[�h�A�E�g���s��.
	if( m_OnceStep == true ){
		CFade::KeepRender( false );
		CFade::SetFadeOut();
	}

	// �e�t���O�����낷.
	m_isThreadJoined	= false;
	m_IsThreadLoadEnd	= false;
	m_IsLoadEnd			= false;

	// ���[�h�֐����X���b�h�ɓn��.
	m_Thread = std::thread( 
		[&]()
		{
			// �X���b�h�̃��b�N.
			std::lock_guard<std::mutex> lock(m_Mutex);
			// �O�̂��ߏ����X���[�v���ď�����x�点��.
			Sleep( 1000 );
			m_IsThreadLoadEnd = m_pScene->Load();
		});
	CDebugText::PushLog( "Scene Thread Start : ID[ ", m_Thread.get_id(), " ]" );
}

//=================================.
// �G�f�B�g�V�[���ɕύX.
//=================================.
void CSceneManager::ChangeEditScene()
{
	m_NextScene = EScene::Edit;
}

//=================================.
// �ǂݍ��ݎ��̕`��.
//=================================.
void CSceneManager::LoadRender( const float& deltaTime, const bool& isLoad )
{
	if( isLoad == true || m_IsLoadEnd == true ) return;
	m_pLoadRender->Init();
	m_pLoadRender->Update( deltaTime );
	m_pLoadRender->Render();
}

//=================================.
// �V�[���̍X�V.
//=================================.
void CSceneManager::SceneUpdate( const float& deltaTime )
{
	m_pScene->Update();
	m_pScene->ChangeScene();

	// ��̍X�V�ŁA�V�[�����ύX���Ă��邩������Ȃ��̂�,
	//	���[�h�̊m�F������.
	if( m_IsThreadLoadEnd == false ) return;

	CCameraManager::Update( deltaTime );
	CLightManager::Update( deltaTime );

	m_pRenderingTexManager->Render( m_ModelRenderFunction, m_Sprite3DRenderFunction );
	m_pScene->SpriteRender();
}

// �X���b�h�̉��.
void CSceneManager::ThreadRelease()
{
	if( m_isThreadJoined == true ) return;
	if( m_IsThreadLoadEnd == true ){
		DWORD code = -1;
		GetExitCodeThread( m_Thread.native_handle(),&code );
		if( code == 0xffffffff ) m_isThreadJoined = true;
		if( code == 0 ){
			m_Thread.join();
			while (1)
			{
				if( m_Thread.joinable() != true ){
					m_isThreadJoined = true;
					CDebugText::PushLog( "Scene Thread Joined" );
					break;
				}
			}
		}
	}
}