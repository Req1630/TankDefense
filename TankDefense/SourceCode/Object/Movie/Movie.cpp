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
// 初期化関数.
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
// 再生.
//--------------------------.
void CMovie::Play()
{
	SettingCamera();
	for( auto& w : m_pWidgetList ) w->Play();
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

	// 画像の更新.
	for( auto& w : m_pWidgetList ) w->Update( deltaTime );

	m_PlayTime -= deltaTime;
}

//--------------------------.
// 画像の描画.
//--------------------------.
void CMovie::SpriteRender()
{
	for( auto& w : m_pWidgetList ) w->Render();
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
// ウィジェット情報の取得.
//--------------------------.
void CMovie::SetWidgetStateList( const std::vector<SMovieWidget>& stateList )
{
	const int stateSize		= stateList.size();
	const int widgetSize	= m_pWidgetList.size();
	const int diff			= stateSize - widgetSize;
	if( diff == 0 ){

		// stateSize の方が多い.
	} else if( diff >= 1 ){
		for( int i = 0; i < diff; i++ ){
			m_pWidgetList.emplace_back( std::make_unique<CMovieWidget>() );
		}

		// widgetSize の方が多い.
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

