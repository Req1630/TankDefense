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
// �Đ�.
//--------------------------.
void CMovie::Play()
{
	m_pCamera->SetCameraState( m_CameraStateQueue.front() );
	m_CameraStateQueue.pop();
	CCameraManager::ChangeCamera( m_pCamera.get() );
}

//--------------------------.
// �X�V����.
//--------------------------.
void CMovie::Update()
{
	const float deltaTime = GetDeltaTime();

	if( m_PlayTime <= 0.0f ) return;

	m_pCamera->Update( deltaTime );

	// �J�����̍Đ����I�������玟�̃J��������ݒ肵�čĐ�����.
	if( m_pCamera->IsPlaying() == false ){
		if( m_CameraStateQueue.empty() == false ){
			m_pCamera->SetCameraState( m_CameraStateQueue.front() );
			m_pCamera->Play();
			m_CameraStateQueue.pop();
		} else {
			// �J�����̍Đ��I��.
		}
	}

	m_PlayTime -= deltaTime;
}
