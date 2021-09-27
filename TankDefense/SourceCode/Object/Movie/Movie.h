#ifndef MOVIE_H
#define MOVIE_H

#include "..\CameraBase\MovieCamera\MovieCameraState.h"
#include "MovieDataLoader/MovieDataLoader.h"

#include <queue>
#include <memory>

class CMovieCamera;			// ムービーカメラクラス.
class CMovieWidget;			// ウィジェットクラス.
class CMovieActorManager;	// ムービーアクタークラス.

/**********************************
*	ムービー(演出)クラス.
**/
class CMovie
{
public:
	CMovie();
	~CMovie();

	// 初期化関数.
	bool Init( const EMovieNo& no );

	// 再生.
	void Play();
	// 更新処理.
	void Update();
	// モデルの描画.
	void ModelRender();
	// 画像の描画.
	void SpriteRender();

	// 再生中か.
	inline bool IsPlaying(){ return m_PlayTime > 0.0f; }

	// カメラ情報の設定.
	inline void SetCameraQueue( const std::queue<SMovieCamera>& queue ){ m_CameraStateQueue = queue; }
	// カメラ情報の設定.
	void SetCameraQueue( const std::vector<SMovieCamera>& movieList );
	// カメラ情報の設定.
	inline void SetCameraList( const std::vector<SMovieCamera>& movieList ){ m_CameraStateList = movieList; }

	// ウィジェット情報の取得.
	void SetWidgetStateList( const std::vector<SMovieWidget>& stateList );

	// アクターリストの設定.
	void SetActorList( const SMovieActorStateList& );

	// 再生時間の設定.
	inline void SetPlayTime( const float& time )	{ m_PlayTime = time; }

private:
	// カメラの設定,
	void SettingCamera();

private:
	std::unique_ptr<CMovieDataLoader>			m_pMovieDataLoader;
	std::unique_ptr<CMovieCamera>				m_pCamera;
	std::unique_ptr<CMovieActorManager>			m_pActorManager;
	std::queue<SMovieCamera>					m_CameraStateQueue;
	std::vector<SMovieCamera>					m_CameraStateList;
	std::vector<std::unique_ptr<CMovieWidget>>	m_pWidgetList;
	float										m_PlayTime;
};

#endif	// #ifndef MOVIE_H.