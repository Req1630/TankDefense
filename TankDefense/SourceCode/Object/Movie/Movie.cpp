#include "Movie.h"
#include "..\CameraBase\MovieCamera\MovieCamera.h"
#include "..\CameraBase\CameraManager\CameraManager.h"
#include "..\GameObject\Widget\MovieWidget\MovieWidget.h"

CMovie::CMovie()
	: m_pMovieDataLoader	( std::make_unique<CMovieDataLoader>() )
	, m_pCamera				( std::make_unique<CMovieCamera>() )
	, m_CameraStateQueue	()
	, m_CameraStateList		()
	, m_pWidgetList			()
	, m_PlayTime			( 0.0f )
{
}

CMovie::~CMovie()
{
}

//--------------------------.
// �������֐�.
//--------------------------.
bool CMovie::Init( const EMovieNo& no )
{
	if( m_pMovieDataLoader->Init(false) == false ) return false;

	const SMovieData data = m_pMovieDataLoader->GetMovieData( no );

	m_PlayTime = data.MovieTime;
	SetCameraQueue( data.CameraList );
	SetWidgetStateList( data.WidgetList );

	return true;
}

//--------------------------.
// �Đ�.
//--------------------------.
void CMovie::Play()
{
	SettingCamera();
	for( auto& w : m_pWidgetList ) w->Play();
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

	// �摜�̍X�V.
	for( auto& w : m_pWidgetList ) w->Update( deltaTime );

	m_PlayTime -= deltaTime;
}

//--------------------------.
// �摜�̕`��.
//--------------------------.
void CMovie::SpriteRender()
{
	for( auto& w : m_pWidgetList ) w->Render();
}

//--------------------------.
// �J�������̐ݒ�.
//--------------------------.
void CMovie::SetCameraQueue( const std::vector<SMovieCamera>& movieList )
{
	// ���L���[�̒��g�����o��.
	int size = m_CameraStateQueue.size();
	for( int i = 0; i < size; i++ ) m_CameraStateQueue.pop();

	// �L���[�ɒǉ����Ă���.
	for( auto& m : movieList ) m_CameraStateQueue.push( m );
}

//--------------------------.
// �E�B�W�F�b�g���̎擾.
//--------------------------.
void CMovie::SetWidgetStateList( const std::vector<SMovieWidget>& stateList )
{
	const int stateSize		= stateList.size();
	const int widgetSize	= m_pWidgetList.size();
	const int diff			= stateSize - widgetSize;
	if( diff == 0 ){

		// stateSize �̕�������.
	} else if( diff >= 1 ){
		for( int i = 0; i < diff; i++ ){
			m_pWidgetList.emplace_back( std::make_unique<CMovieWidget>() );
		}

		// widgetSize �̕�������.
	} else if( diff <= -1 ){
		for( int i = 0; i < abs(diff); i++ ){
			m_pWidgetList.pop_back();
		}
	}
	m_pWidgetList.shrink_to_fit();

	for( int i = 0; i < stateSize; i++ ){
		m_pWidgetList[i]->SetMovieWidgetState( stateList[i] );
		m_pWidgetList[i]->Init();
	}
}

//--------------------------.
// �J�����̐ݒ�,
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

