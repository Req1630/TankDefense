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
	SettingCamera();
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

//--------------------------.
// カメラ情報の設定.
//--------------------------.
void CMovie::SetCameraQueue( const std::vector<SMovieCamera>& movieList )
{
	// 一回キューの中身を取り出す.
	int size = m_CameraStateQueue.size();
	for( int i = 0; i < size; i++ ) m_CameraStateQueue.pop();

	// キューに追加していく.
	for( auto& m : movieList ) m_CameraStateQueue.push( m );
}

//--------------------------.
// カメラの設定,
//--------------------------.
void CMovie::SettingCamera()
{
	if( m_CameraStateQueue.empty() == false ){
		m_pCamera->SetCameraState( m_CameraStateQueue.front() );
		m_pCamera->Play();
		m_CameraStateQueue.pop();
	}
	CCameraManager::ChangeCamera( m_pCamera.get() );
}
