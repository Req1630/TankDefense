#ifndef MOVIE_CAMERA_H
#define MOVIE_CAMERA_H

#include "..\CameraBase.h"
#include "MovieCameraState.h"
#include <memory>


/****************************************
* 演出用(Moive)カメラ.
**/
class CMovieCamera : public CCameraBase
{
	// 再生用揺れカメラ情報.
	struct stPlayShakeCamera
	{
		float	Time;			// 再生時間.
		float	Attenuation;	// 減衰値.
		bool	IsAttenuation;	// 減衰するか.
	} typedef SPlayShakeCamera;
public:
	CMovieCamera();
	virtual ~CMovieCamera();

	// 更新関数.
	virtual void Update( const float& deltaTime ) override;

	// 演出カメラ情報の設定.
	inline void SetCameraState( const SMovieCamera& state ){ m_MovieCameraState = state; }

	// 再生中か.
	inline bool IsPlaying() { return m_IsActive; }
	// 再生.
	void Play();
	// 値のリセット.
	void Reset();

private:
	// カメラの移動.
	void MoveCamera();
	// カメラを揺らす.
	void ShakeCamera();

private:
	SMovieCamera					m_MovieCameraState;		// カメラ情報.
	std::vector<SPlayShakeCamera>	m_PlayShakeState;
	float				m_PosMoveTime;			// 移動時間.
	float				m_LookPosMoveTime;		// 注視位置移動時間.
	float				m_DeltaTime;			// デルタ時間.
	bool				m_IsMoveCameraPlaying;	// 移動カメラが動作しているか.
	bool				m_IsShakeCameraPlaying;	// 揺れカメラが動作しているか.
};

#endif	// #ifndef MOVIE_CAMERA_H.