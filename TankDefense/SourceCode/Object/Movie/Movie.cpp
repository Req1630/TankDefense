#include "Movie.h"
#include "..\CameraBase\MovieCamera\MovieCamera.h"
#include "..\CameraBase\CameraManager\CameraManager.h"

CMovie::CMovie()
	: m_pCamera				( std::make_unique<CMovieCamera>() )
	, m_CameraStateQueue	()
	, m_PlayTime			( 0.0f )
{
}

CMovie::~CMovie()
{
}

//--------------------------.
// 再生.
//--------------------------.
void CMovie::Play()
{
	m_pCamera->SetCameraState( m_CameraStateQueue.front() );
	m_CameraStateQueue.pop();
	CCameraManager::ChangeCamera( m_pCamera.get() );
}

//--------------------------.
// 更新処理.
//--------------------------.
void CMovie::Update()
{
	const float deltaTime = GetDeltaTime();

	if( m_PlayTime <= 0.0f ) return;

	m_pCamera->Update( deltaTime );

	// カメラの再生が終了したら次のカメラ情報を設定して再生する.
	if( m_pCamera->IsPlaying() == false ){
		if( m_CameraStateQueue.empty() == false ){
			m_pCamera->SetCameraState( m_CameraStateQueue.front() );
			m_pCamera->Play();
			m_CameraStateQueue.pop();
		} else {
			// カメラの再生終了.
		}
	}

	m_PlayTime -= deltaTime;
}
