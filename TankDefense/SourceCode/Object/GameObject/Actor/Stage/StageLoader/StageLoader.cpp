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
// ������.
//-------------------------------------.
bool CStageLoader::Init()
{
	if( InitStageList() == false ) return false;
	if( StageLoad() == false ) return false;

	return true;
}

//-------------------------------------.
// �A�N�^�[���X�g�̏�������.
//-------------------------------------.
bool CStageLoader::WritingActorLst( const EStageNo& no, const std::vector<SActorParam>& list )
{
	fileManager::BinaryVectorWriting( m_StageList[no].c_str(), list );
	fileManager::BinaryVectorReading( m_StageList[no].c_str(), m_ActorList[no] );

	return true;
}

//-------------------------------------.
// �X�e�[�W���X�g�̓ǂݍ���.
//-------------------------------------.
bool CStageLoader::InitStageList()
{
	std::vector<std::string> list = fileManager::TextLoading( STAGE_LIST_PATH );

	if( list.empty() == true ) return false; 
	if( list.size() > EStageNo_Max ) return false;

	EStageNo no = EStageNo_Begin;
	for( auto& p : list ){
		// �X�e�[�W�̃p�X���擾.
		m_StageList[no] = p;

		int i = static_cast<int>(no)+1;
		no = static_cast<EStageNo>(i);
	}

	return true;
}

//-------------------------------------.
// �X�e�[�W�̓ǂݍ���.
//-------------------------------------.
bool CStageLoader::StageLoad()
{
	for( auto& p : m_StageList ){
		fileManager::BinaryVectorReading( p.second.c_str(), m_ActorList[p.first] );
	}
	return true;
}