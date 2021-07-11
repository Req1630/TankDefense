#include "LoadManager.h"
#include "..\MeshResource\MeshResource.h"
#include "..\SpriteResource\SpriteResource.h"
#include "..\FontResource\FontResource.h"
#include "..\EffectResource\EffectResource.h"
#include "..\GameParamResource\GameParamResource.h"
#include "..\..\Utility\XAudio2\SoundManager.h"
#include "..\..\Common\DebugText\DebugText.h"

CLoadManager::CLoadManager()
	: m_Thread				()
	, m_Mutex				()
	, m_isLoadEnd			( false )
	, m_isThreadJoined		( false )
	, m_isLoadFailed		( false )
{
}

CLoadManager::~CLoadManager()
{
	if( m_isLoadFailed == true ) m_Thread.join();
}

//------------------------.
// �ǂݍ���.
//------------------------.
void CLoadManager::LoadResource(  
	HWND hWnd, 
	ID3D11Device* pDevice11, 
	ID3D11DeviceContext* pContext11, 
	LPDIRECT3DDEVICE9 pDevice9 )
{
	CFontResource::Load( pDevice11, pContext11 );
	CDebugText::PushLog( "SoundData Load Begin" );
	CSoundManager::CreateSoundData();
	CDebugText::PushLog( "SoundData Load End" );
	CEffectResource::Load( pDevice11, pContext11 );

	auto load = [&]( 
		HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9 )
	{
		std::unique_lock<std::mutex> lock( m_Mutex );
		CLog::Print("------- �X���b�h���[�h�J�n ------");
		Sleep(200);
		if( FAILED( CSpriteResource::Load( pContext11 ) )){
			m_isLoadFailed = true;
			return;
		}
		Sleep(200);
		if( FAILED( CMeshResorce::Load( hWnd, pDevice11, pContext11, pDevice9 ) )){
			m_isLoadFailed = true;
			return;
		}
		Sleep(200);
		CGameParamResource::ReadAllParam();
		Sleep(200);

		m_isLoadEnd = true;
		CLog::Print("------- �X���b�h���[�h�I�� ------");
	};
	m_Thread = std::thread( load, hWnd, pDevice11, pContext11, pDevice9 );
	CDebugText::PushLog( "LoadManager Thread Start : ID[ ", m_Thread.get_id(), " ]" );
}

//------------------------.
// �X���b�h�̉��.
//------------------------.
bool CLoadManager::ThreadRelease()
{
	if( m_isThreadJoined == true ) return true;
	if( m_isLoadEnd == false ) return false;
	DWORD code = -1;
	GetExitCodeThread( m_Thread.native_handle(),&code );
	if( code == 0xffffffff ){
		m_isThreadJoined = true;
	}
	if( code == 0 ){
		m_Thread.join();
		while (1)
		{
			if( m_Thread.joinable() != true ){
				m_isThreadJoined = true;
				CDebugText::PushLog( "LoadManager Thread Joined" );
				break;
			}
		}
	}
	return true;
}