#ifndef ENEMY_SPAWN_MANAGER_H
#define ENEMY_SPAWN_MANAGER_H

#include "EnemySpawnStruct.h"
#include "..\..\Stage\StageStruct.h"

/************************************
*	敵スポーンを管理するクラス.
**/
class CEnemySpawnManager
{
public:
	CEnemySpawnManager();
	~CEnemySpawnManager();

	// 初期化.
	bool Init();

	// スポーンリストの取得.
	inline std::vector<SSpawnBoxRange> GetActorList( const EStageNo& no ){ return m_SpawnList[no]; }
	// スポーンリストの設定.
	inline void SetActorList( const EStageNo& no, const std::vector<SSpawnBoxRange>& list ){ m_SpawnList[no] = list; }

	// スポーンリストの書き込み.
	bool WritingActorLst( const EStageNo& no, const std::vector<SSpawnBoxRange>& list );

private:
	// ステージリストの読み込み.
	bool InitStageList();

	// ステージの読み込み.
	bool StageLoad();

private:
	spawn_list_map								m_SpawnList;	// スポーンリスト.
	std::unordered_map<EStageNo, std::string>	m_StageList;	// ステージリスト.
	EStageNo									m_NowStageNo;	// 現在のステージ番号.

};

#endif	// #ifndef ENEMY_SPAWN_MANAGER_H.