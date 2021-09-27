#ifndef MOVIE_H
#define MOVIE_H

#include "..\CameraBase\MovieCamera\MovieCameraState.h"
#include "MovieDataLoader/MovieDataLoader.h"

#include <queue>
#include <memory>

class CMovieCamera;			// ���[�r�[�J�����N���X.
class CMovieWidget;			// �E�B�W�F�b�g�N���X.
class CMovieActorManager;	// ���[�r�[�A�N�^�[�N���X.

/**********************************
*	���[�r�[(���o)�N���X.
**/
class CMovie
{
public:
	CMovie();
	~CMovie();

	// �������֐�.
	bool Init( const EMovieNo& no );

	// �Đ�.
	void Play();
	// �X�V����.
	void Update();
	// ���f���̕`��.
	void ModelRender();
	// �摜�̕`��.
	void SpriteRender();

	// �Đ�����.
	inline bool IsPlaying(){ return m_PlayTime > 0.0f; }

	// �J�������̐ݒ�.
	inline void SetCameraQueue( const std::queue<SMovieCamera>& queue ){ m_CameraStateQueue = queue; }
	// �J�������̐ݒ�.
	void SetCameraQueue( const std::vector<SMovieCamera>& movieList );
	// �J�������̐ݒ�.
	inline void SetCameraList( const std::vector<SMovieCamera>& movieList ){ m_CameraStateList = movieList; }

	// �E�B�W�F�b�g���̎擾.
	void SetWidgetStateList( const std::vector<SMovieWidget>& stateList );

	// �A�N�^�[���X�g�̐ݒ�.
	void SetActorList( const SMovieActorStateList& );

	// �Đ����Ԃ̐ݒ�.
	inline void SetPlayTime( const float& time )	{ m_PlayTime = time; }

private:
	// �J�����̐ݒ�,
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