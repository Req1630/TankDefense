#include "EnemySpawnManager.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

namespace
{
	constexpr char SPAWN_LIST_PATH[] = "Data\\Parameter\\Spawn\\SpawnList.txt";
};

CEnemySpawnManager::CEnemySpawnManager()
	: m_SpawnList	()
	, m_NowStageNo	( EStageNo_Enemy )
{
}

CEnemySpawnManager::~CEnemySpawnManager()
{
}

//------------------------------------.
// 初期化.
//------------------------------------.
bool CEnemySpawnManager::Init()
{
	if( InitStageList() == false ) return false;
	if( StageLoad() == false ) return false; 
	return true;
}

//------------------------------------.
// アクターリストの書き込み.
//------------------------------------.
bool CEnemySpawnManager::WritingActorLst( const EStageNo& no, const std::vector<SSpawnBoxRange>& list )
{
	fileManager::BinaryVectorWriting( m_StageList[no].c_str(), list );
	fileManager::BinaryVectorReading( m_StageList[no].c_str(), m_SpawnList[no] );

	return true;
}

//------------------------------------.
// ステージリストの読み込み.
//------------------------------------.
bool CEnemySpawnManager::InitStageList()
{
	std::vector<std::string> list = fileManager::TextLoading( SPAWN_LIST_PATH );

	if( list.empty() == true ) return false; 
	if( list.size() > EStageNo_Max ) return false;

	EStageNo no = EStageNo_Begin;
	for( auto& p : list ){
		// ステージのパスを取得.
		m_StageList[no] = p;

		int i = static_cast<int>(no)+1;
		no = static_cast<EStageNo>(i);
	}
	return true;
}

//------------------------------------.
// ステージの読み込み.
//------------------------------------.
bool CEnemySpawnManager::StageLoad()
{
	for( auto& p : m_StageList ){
		fileManager::BinaryVectorReading( p.second.c_str(), m_SpawnList[p.first] );
	}
	return true;
}

