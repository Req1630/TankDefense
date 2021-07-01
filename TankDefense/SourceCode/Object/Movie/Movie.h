#ifndef MOVIE_H
#define MOVIE_H

#include "..\CameraBase\MovieCamera\MovieCameraState.h"

#include <queue>
#include <memory>

class CMovieCamera;	// ムービーカメラクラス.

/**********************************
*	ムービー(演出)クラス.
**/
class CMovie
{
public:
	CMovie();
	~CMovie();

	// 再生.
	void Play();
	// 更新処理.
	void Update();

	// 再生中か.
	inline bool IsPlaying(){ return m_PlayTime > 0.0f; }

	// カメラ情報の設定.
	inline void SetCameraQueue( const std::queue<SMovieCamera>& queue ){ m_CameraStateQueue = queue; }
	// カメラ情報の設定.
	void SetCameraQueue( const std::vector<SMovieCamera>& movieList );

	// 再生時間の設定.
	inline void SetPlayTime( const float& time )	{ m_PlayTime = time; }
private:
	// カメラの設定,
	void SettingCamera();

private:
	std::unique_ptr<CMovieCamera>	m_pCamera;
	std::queue<SMovieCamera>		m_CameraStateQueue;
	float							m_PlayTime;
};

#endif	// #ifndef MOVIE_H.