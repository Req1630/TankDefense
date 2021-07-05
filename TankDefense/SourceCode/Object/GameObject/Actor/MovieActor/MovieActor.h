#ifndef MOVIE_ACTOR_H
#define MOVIE_ACTOR_H

#include "..\EnemyBase\EnemyBase.h"
#include "..\EnemyBase\Enemy\Enemy.h"
#include "..\Actor.h"

// ���[�r�[(���o�p)�A�N�^�[���.
struct stMovieActor
{
	D3DXVECTOR3	StartPosition;	// �J�n���W.
	D3DXVECTOR3	EndPosition;	// �I�����W.
	D3DXVECTOR3	StartRotation;	// �J�n��]�l.
	float		AciveStartTime;	// ����J�n����(�b).

	stMovieActor()
		: StartPosition		( 0.0f, 0.0f, 0.0f )
		, EndPosition		( 0.0f, 0.0f, 0.0f )
		, StartRotation		( 0.0f, 0.0f, 0.0f )
		, AciveStartTime	( 0.0f )
	{}
	stMovieActor( 
		const D3DXVECTOR3&	startPos,
		const D3DXVECTOR3&	endPos,
		const D3DXVECTOR3&	rot,
		const float&		startTime )
		: StartPosition		( startPos )
		, EndPosition		( endPos )
		, StartRotation		( rot )
		, AciveStartTime	( startTime )
	{}

} typedef SMovieActor;

/********************************************
*	���[�r�[�p�A�N�^�[�N���X.
*	 �C���X�^���X�쐬���Ɏg�p�������A�N�^�[��ݒ肷��.
**/
template<typename T>
class CMovieActor : public T
{
public:
	CMovieActor();
	virtual ~CMovieActor();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;
	// �`��֐�.
	virtual void Render() override;

	// ���[�r�[���̐ݒ�.
	inline void SetMovieState( const SMovieActor& state ){ m_MovieState = state; }

	// ���f���\��.
	void ModelRender( const bool& isWire = false );

	// ���b�V���̃|���^�̎擾.
	inline CDX9SkinMesh* GetSkinMesh(){ return T::m_pSkinMesh; }

private:
	// ����ł��邩�m�F����.
	void ConfirmAcive();

private:
	SMovieActor	m_MovieState;	// ���[�r�[���.
	float		m_NowTime;		// ����J�n����(�b).
	bool		m_IsAcive;		// �����Ԃ�.
};

#endif	// #ifndef MOVIE_ACTOR_H.