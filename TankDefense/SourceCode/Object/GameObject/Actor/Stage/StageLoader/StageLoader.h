#ifndef STAGE_LOADER_H
#define STAGE_LOADER_H

#include "..\StageStruct.h"

#include <vector>
#include <unordered_map>

/****************************
*	ステージ読み込みクラス.
**/
class CStageLoader
{
public:
	CStageLoader();
	~CStageLoader();

	// 初期化.
	bool Init();

	// アクターリストの取得.
	inline std::vector<SActorParam> GetActorList( const EStageNo& no ){ return m_ActorList[no]; }
	// アクターリストの設定.
	inline void SetActorList( const EStageNo& no, const std::vector<SActorParam>& list ){ m_ActorList[no] = list; }

	// アクターリストの書き込み.
	bool WritingActorLst( const EStageNo& no, const std::vector<SActorParam>& list );

private:
	// ステージリストの読み込み.
	bool InitStageList();

	// ステージの読み込み.
	bool StageLoad();

private:
	std::unordered_map<EStageNo, std::vector<SActorParam>>	m_ActorList;
	std::unordered_map<EStageNo, std::string>				m_StageList;
};

#endif	// #ifndef STAGE_LOADER_H.