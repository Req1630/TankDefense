#include "MovieActorManager.h"

CMovieActorManager::CMovieActorManager()
	: m_pEnemyList	()
{
}

CMovieActorManager::~CMovieActorManager()
{
}

// �X�V����.
void CMovieActorManager::Update()
{
	m_DeltaTime = GetDeltaTime();

	UpdateMovieActorList<CMovieEnemy>( m_pEnemyList );
}

// ���f���`��.
void CMovieActorManager::ModelRender()
{
	RenderMovieActorList<CMovieEnemy>( m_pEnemyList );
}

// �A�N�^�[���̐ݒ�.
void CMovieActorManager::SetMovieActorState( const SMovieActorStateList& stateList )
{
	// �G���̐ݒ�.
	CreateMovieActorList( stateList.EnemyList, m_pEnemyList );
}
