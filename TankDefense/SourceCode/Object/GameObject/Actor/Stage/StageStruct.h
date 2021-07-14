#ifndef STAGE_STRUCT_H
#define STAGE_STRUCT_H

#include "..\..\..\..\Utility\Tranceform\Tranceform.h"

// ステージリスト.
enum enStageList
{
	EStageNo_None,

	EStageNo_Enemy=0,
	EStageNo_Boss,

	EStageNo_Max,

	EStageNo_Begin = EStageNo_Enemy,
} typedef EStageNo;

// アクターリスト.
enum enActorList
{
	EActorNo_None,

	EActorNo_Player = 0,
	EActorNo_Ground,
	EActorNo_Ground_I,
	EActorNo_Ground_L,
	EActorNo_Tree,
	EActorNo_ItemSpeed,
	EActorNo_Light,

	EActorNo_Max,

	EActorNo_Begin	= EActorNo_Player,
	EActorNo_End	= EActorNo_Max,
} typedef EActorNo;

// ステージアクター情報.
struct stActorParam
{
	EActorNo	ActorNo;
	STranceform	Tranceform;

	stActorParam() : ActorNo(), Tranceform() 
	{}
	stActorParam( const EActorNo& no, const STranceform& t ) 
		: ActorNo( no ), Tranceform( t )
	{}
} typedef SActorParam;

#endif	// #ifndef STAGE_STRUCT_H.
