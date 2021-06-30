#ifndef MOVIE_CAMERA_H
#define MOVIE_CAMERA_H

#include "..\CameraBase.h"
#include "MovieCameraState.h"
#include <memory>


/****************************************
* ���o�p(Moive)�J����.
**/
class CMovieCamera : public CCameraBase
{
	// �Đ��p�h��J�������.
	struct stPlayShakeCamera
	{
		float	Time;			// �Đ�����.
		float	Attenuation;	// �����l.
		bool	IsAttenuation;	// �������邩.
	} typedef SPlayShakeCamera;
public:
	CMovieCamera();
	virtual ~CMovieCamera();

	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;

	// ���o�J�������̐ݒ�.
	inline void SetCameraState( const SMovieCamera& state ){ m_MovieCameraState = state; }

	// �Đ�����.
	inline bool IsPlaying() { return m_IsActive; }
	// �Đ�.
	void Play();
	// �l�̃��Z�b�g.
	void Reset();

private:
	// �J�����̈ړ�.
	void MoveCamera();
	// �J������h�炷.
	void ShakeCamera();

private:
	SMovieCamera					m_MovieCameraState;		// �J�������.
	std::vector<SPlayShakeCamera>	m_PlayShakeState;
	float				m_PosMoveTime;			// �ړ�����.
	float				m_LookPosMoveTime;		// �����ʒu�ړ�����.
	float				m_DeltaTime;			// �f���^����.
	bool				m_IsMoveCameraPlaying;	// �ړ��J���������삵�Ă��邩.
	bool				m_IsShakeCameraPlaying;	// �h��J���������삵�Ă��邩.
};

#endif	// #ifndef MOVIE_CAMERA_H.