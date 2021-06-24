#ifndef STAGE_STRUCT_H
#define STAGE_STRUCT_H

enum enActorList
{
	EActorNo_None,

	EActorNo_Player = 0,
	EActorNo_Ground,
	EActorNo_Ground_I,
	EActorNo_Ground_L,
	EActorNo_Tree,
	EActorNo_ItemSpeed,

	EActorNo_Max,

	EActorNo_Begin	= EActorNo_Player,
	EActorNo_End	= EActorNo_Max,
} typedef EActorNo;

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
