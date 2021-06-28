#ifndef MOVIE_CAMERA_H
#define MOVIE_CAMERA_H

#include "..\CameraBase.h"
#include <memory>

// 演出用カメラ情報.
struct stMovieCamera
{
	float						ActiveTime;			// 動作時間.
	CCameraBase::SCameraState	StartState;			// 開始カメラ情報.
	CCameraBase::SCameraState	EndState;			// 終了カメラ情報.
	float						PosMoveTime;		// 移動時間.
	float						LookPosMoveTime;	// 注視位置移動時間.
} typedef SMovieCamera;

/****************************************
* 演出用(Moive)カメラ.
**/
class CMoiveCamera : public CCameraBase
{
public:
	CMoiveCamera();
	virtual ~CMoiveCamera();

	// 更新関数.
	virtual void Update( const float& deltaTime ) override;

	// 演出カメラ情報の設定.
	inline void SetCameraState( const SMovieCamera& state ){ m_MovieCameraState = state; }

private:
	SMovieCamera	m_MovieCameraState;	// カメラ情報.
	float			m_PosMoveTime;		// 移動時間.
	float			m_LookPosMoveTime;	// 注視位置移動時間.
};

#endif	// #ifndef MOVIE_CAMERA_H.