/**
* @file FrameRate.h.
* @brief �Q�[�����[�v�̃t���[�����[�g���v�Z����N���X.
* @author ���c���.
*/
#ifndef FRAME_RATE_H
#define FRAME_RATE_H

// �x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)

#include <Windows.h>

#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.

// ���C�u�����ǂݍ���.
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx10.lib" )	//�uD3DX�`�v�̒�`�g�p���ɕK�v.

/********************************************
*	�t���[�����[�g�̌v���N���X.
**/
class CFrameRate
{
public:
	CFrameRate();
	CFrameRate( const double& fps = 60.0 );
	~CFrameRate();

	// �ҋ@�֐�.
	//	true �̏ꍇ���̃t���[�����I������.
	bool Wait();

	// FPS�̎擾.
	inline double GetFPS() const { return m_FPS; }
	// �f���^�^�C���̎擾.
	static inline double		GetDeltaTime(){ return m_FrameTime; }
	// �o�ߎ��Ԃ̎擾.
	//	�X�e�[�W���[�h����̌o�ߎ���(t/20,t,t*2,t*3).
	static inline D3DXVECTOR4	GetElapsedTime(){ return m_Time; }
	// �o�ߎ��Ԃ̏�����.
	//	�V�[���̃��[�h�I�����ȂǂɌĂ�.
	static inline void ClearElapsedTime(){ m_Time = { 0.0f, 0.0f, 0.0f, 0.0f }; }

private:
	const double	FRAME_RATE;		// �t���[�����[�g.
	const double	MIN_FRAME_TIME;	// �ŏ��t���[���^�C��.

private:
	static double		m_FrameTime;	// �t���[���^�C��(�f���^�^�C��).
	static D3DXVECTOR4	m_Time;			// �X�e�[�W���[�h����̌o�ߎ���(t/20,t,t*2,t*3).
	double				m_FPS;			// ���݂�FPS.
	LARGE_INTEGER		m_StartTime;	// �J�n����.
	LARGE_INTEGER		m_NowTime;		// ���݂̎���.
	LARGE_INTEGER		m_FreqTime;		// �N����������.
};

// �f���^�^�C���̎擾.
template<class T = float>
static inline T GetDeltaTime()
{
	return static_cast<T>(CFrameRate::GetDeltaTime());
}

// �o�ߎ��Ԃ̎擾.
//	�X�e�[�W���[�h����̌o�ߎ���(t/20,t,t*2,t*3).
template<class T = D3DXVECTOR4>
static inline T GetElapsedTime()
{
	return static_cast<T>(CFrameRate::GetElapsedTime());
}

#endif	// #ifndef FRAME_RATE_H.