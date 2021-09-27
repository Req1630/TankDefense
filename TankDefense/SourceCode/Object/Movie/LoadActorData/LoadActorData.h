#ifndef LOAD_ACTOR_DATA_H
#define LOAD_ACTOR_DATA_H

#include "..\..\GameObject\Actor\MovieActor\MovieActorManager\MovieActorManager.h"

/******************************************
*	ムービー用アクターデータと文字列を変換するクラス.
**/
class CActorDataConverter
{
public:
	CActorDataConverter();
	~CActorDataConverter();

	// 文字に変換する.
	static std::vector<std::string> ToString( const SMovieActorStateList& stateList );

	// リストに変換する.
	static SMovieActorStateList ToList( const std::vector<std::string>& dataList );

private:
	// アクター情報を文字に変換する.
	static void ActorStateToString( 
		std::stringstream* pSs,
		const std::vector<SMovieActor>& stateList, 
		const char* tag );

	// 文字情報をアクター情報に変換する.
	static bool StringToActorState(
		int* pIndex,
		const std::vector<std::string>& dataList,
		std::vector<SMovieActor>* pStateList,
		const char* tag );
};

#endif	// #ifndef LOAD_ACTOR_DATA_H.