#ifndef MOVIE_H
#define MOVIE_H

#include "..\CameraBase\MovieCamera\MovieCameraState.h"

#include <queue>
#include <memory>

class CMovieCamera;	// ���[�r�[�J�����N���X.

/**********************************
*	���[�r�[(���o)�N���X.
**/
class CMovie
{
public:
	CMovie();
	~CMovie();

	// �Đ�.
	void Play();
	// �X�V����.
	void Update();

	// �Đ�����.
	inline bool IsPlaying(){ return m_PlayTime > 0.0f; }

	// �J�������̐ݒ�.
	inline void SetCameraQueue( const std::queue<SMovieCamera>& queue ){ m_CameraStateQueue = queue; }
	// �J�������̐ݒ�.
	void SetCameraQueue( const std::vector<SMovieCamera>& movieList );

	// �Đ����Ԃ̐ݒ�.
	inline void SetPlayTime( const float& time )	{ m_PlayTime = time; }
private:
	// �J�����̐ݒ�,
	void SettingCamera();

private:
	std::unique_ptr<CMovieCamera>	m_pCamera;
	std::queue<SMovieCamera>		m_CameraStateQueue;
	float							m_PlayTime;
};

#endif	// #ifndef MOVIE_H.