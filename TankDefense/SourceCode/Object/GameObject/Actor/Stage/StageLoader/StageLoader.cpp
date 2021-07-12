#include "StageLoader.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

namespace
{
	constexpr char STAGE_LIST_PATH[] = "Data\\Parameter\\Stage\\StageList.txt";
};

CStageLoader::CStageLoader()
	: m_ActorList	()
	, m_StageList	()
{
}

CStageLoader::~CStageLoader()
{
}

//-------------------------------------.
// 初期化.
//-------------------------------------.
bool CStageLoader::Init()
{
	if( InitStageList() == false ) return false;
	if( StageLoad() == false ) return false;

	return true;
}

//-------------------------------------.
// アクターリストの書き込み.
//-------------------------------------.
bool CStageLoader::WritingActorLst( const EStageNo& no, const std::vector<SActorParam>& list )
{
	fileManager::BinaryVectorWriting( m_StageList[no].c_str(), list );
	fileManager::BinaryVectorReading( m_StageList[no].c_str(), m_ActorList[no] );

	return true;
}

//-------------------------------------.
// ステージリストの読み込み.
//-------------------------------------.
bool CStageLoader::InitStageList()
{
	std::vector<std::string> list = fileManager::TextLoading( STAGE_LIST_PATH );

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

//-------------------------------------.
// ステージの読み込み.
//-------------------------------------.
bool CStageLoader::StageLoad()
{
	for( auto& p : m_StageList ){
		fileManager::BinaryVectorReading( p.second.c_str(), m_ActorList[p.first] );
	}
	return true;
}