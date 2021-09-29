#ifndef MOVIE_ACTOR_MANAGER_H
#define MOVIE_ACTOR_MANAGER_H

#include "..\MovieActor.h"

using CMovieEnemy = CMovieActor<CEnemy>;

struct stMovieActorStateList
{
	std::vector<SMovieActor>	EnemyList;

} typedef SMovieActorStateList;

/*********************************
*	����p�A�N�^�[�Ǘ��N���X.
**/
class CMovieActorManager
{
public:
	CMovieActorManager();
	~CMovieActorManager();

	// �X�V����.
	void Update();
	// ���f���`��.
	void ModelRender();

	// �A�N�^�[���̐ݒ�.
	void SetMovieActorState( const SMovieActorStateList& stateList );

private:
	// �A�N�^�[���X�g�̍쐬.
	template<class T> 
	void CreateMovieActorList( 
		const std::vector<SMovieActor>&		stateList,
		std::vector<std::unique_ptr<T>>&	actorList )
	{
		const int stateSize		= stateList.size();
		const int createSize	= stateSize - actorList.size();

		if( createSize > 0 ){
			// �쐬�������̐����Ȃ�ǉ����Ă���.
			for( int i = 0; i < createSize; i++ ){
				actorList.emplace_back( std::make_unique<T>() );
			}
		} else if( createSize < 0 ){
			// �쐬�������̐����Ȃ�����Ă���.
			for( int i = 0; i < abs(createSize); i++ ){
				actorList.pop_back();
			}
		}

		for( int i = 0; i < stateSize; i++ ){
			actorList[i]->SetMovieState( stateList[i] );
			actorList[i]->Init();
		}
	}
	// �A�N�^�[���X�g�̍X�V.
	template<class T> 
	void UpdateMovieActorList( std::vector<std::unique_ptr<T>>&	actorList )
	{
		for( auto& a : actorList ) a->Update( m_DeltaTime );
	}
	// �A�N�^�[���X�g�̕`��.
	template<class T> 
	void RenderMovieActorList( std::vector<std::unique_ptr<T>>&	actorList )
	{
		for( auto& a : actorList ) a->Render();
	}

private:
	float	m_DeltaTime;
	std::vector<std::unique_ptr<CMovieEnemy>>	m_pEnemyList;

};

#endif	// #ifndef MOVIE_ACTOR_MANAGER_H.