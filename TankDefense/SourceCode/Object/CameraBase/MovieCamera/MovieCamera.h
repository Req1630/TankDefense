#ifndef MOVIE_CAMERA_H
#define MOVIE_CAMERA_H

#include "..\CameraBase.h"
#include <memory>

// ���o�p�J�������.
struct stMovieCamera
{
	float						ActiveTime;			// ���쎞��.
	CCameraBase::SCameraState	StartState;			// �J�n�J�������.
	CCameraBase::SCameraState	EndState;			// �I���J�������.
	float						PosMoveTime;		// �ړ�����.
	float						LookPosMoveTime;	// �����ʒu�ړ�����.
} typedef SMovieCamera;

/****************************************
* ���o�p(Moive)�J����.
**/
class CMoiveCamera : public CCameraBase
{
public:
	CMoiveCamera();
	virtual ~CMoiveCamera();

	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;

	// ���o�J�������̐ݒ�.
	inline void SetCameraState( const SMovieCamera& state ){ m_MovieCameraState = state; }

private:
	SMovieCamera	m_MovieCameraState;	// �J�������.
	float			m_PosMoveTime;		// �ړ�����.
	float			m_LookPosMoveTime;	// �����ʒu�ړ�����.
};

#endif	// #ifndef MOVIE_CAMERA_H.