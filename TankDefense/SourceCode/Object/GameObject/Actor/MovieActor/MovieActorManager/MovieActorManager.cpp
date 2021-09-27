#include "MovieActorManager.h"

CMovieActorManager::CMovieActorManager()
	: m_pEnemyList	()
{
}

CMovieActorManager::~CMovieActorManager()
{
}

// 更新処理.
void CMovieActorManager::Update()
{
	m_DeltaTime = GetDeltaTime();

	UpdateMovieActorList<CMovieEnemy>( m_pEnemyList );
}

// モデル描画.
void CMovieActorManager::ModelRender()
{
	RenderMovieActorList<CMovieEnemy>( m_pEnemyList );
}

// アクター情報の設定.
void CMovieActorManager::SetMovieActorState( const SMovieActorStateList& stateList )
{
	// 敵情報の設定.
	CreateMovieActorList( stateList.EnemyList, m_pEnemyList );
}
