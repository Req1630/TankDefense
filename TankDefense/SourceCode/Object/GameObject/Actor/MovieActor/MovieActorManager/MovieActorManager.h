#ifndef MOVIE_ACTOR_MANAGER_H
#define MOVIE_ACTOR_MANAGER_H

#include "..\MovieActor.h"

using CMovieEnemy = CMovieActor<CEnemy>;

struct stMovieActorStateList
{
	std::vector<SMovieActor>	EnemyList;

} typedef SMovieActorStateList;

/*********************************
*	動画用アクター管理クラス.
**/
class CMovieActorManager
{
public:
	CMovieActorManager();
	~CMovieActorManager();

	// 更新処理.
	void Update();
	// モデル描画.
	void ModelRender();

	// アクター情報の設定.
	void SetMovieActorState( const SMovieActorStateList& stateList );

private:
	// アクターリストの作成.
	template<class T> 
	void CreateMovieActorList( 
		const std::vector<SMovieActor>&		stateList,
		std::vector<std::unique_ptr<T>>&	actorList )
	{
		const int stateSize		= stateList.size();
		const int createSize	= stateSize - actorList.size();

		if( createSize > 0 ){
			// 作成数が正の整数なら追加していく.
			for( int i = 0; i < createSize; i++ ){
				actorList.emplace_back( std::make_unique<T>() );
			}
		} else if( createSize < 0 ){
			// 作成数が負の整数なら消していく.
			for( int i = 0; i < abs(createSize); i++ ){
				actorList.pop_back();
			}
		}

		for( int i = 0; i < stateSize; i++ ){
			actorList[i]->SetMovieState( stateList[i] );
			actorList[i]->Init();
		}
	}
	// アクターリストの更新.
	template<class T> 
	void UpdateMovieActorList( std::vector<std::unique_ptr<T>>&	actorList )
	{
		for( auto& a : actorList ) a->Update( m_DeltaTime );
	}
	// アクターリストの描画.
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