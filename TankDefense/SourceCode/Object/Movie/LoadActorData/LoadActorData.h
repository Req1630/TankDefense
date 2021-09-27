#ifndef LOAD_ACTOR_DATA_H
#define LOAD_ACTOR_DATA_H

#include "..\..\GameObject\Actor\MovieActor\MovieActorManager\MovieActorManager.h"

/******************************************
*	���[�r�[�p�A�N�^�[�f�[�^�ƕ������ϊ�����N���X.
**/
class CActorDataConverter
{
public:
	CActorDataConverter();
	~CActorDataConverter();

	// �����ɕϊ�����.
	static std::vector<std::string> ToString( const SMovieActorStateList& stateList );

	// ���X�g�ɕϊ�����.
	static SMovieActorStateList ToList( const std::vector<std::string>& dataList );

private:
	// �A�N�^�[���𕶎��ɕϊ�����.
	static void ActorStateToString( 
		std::stringstream* pSs,
		const std::vector<SMovieActor>& stateList, 
		const char* tag );

	// ���������A�N�^�[���ɕϊ�����.
	static bool StringToActorState(
		int* pIndex,
		const std::vector<std::string>& dataList,
		std::vector<SMovieActor>* pStateList,
		const char* tag );
};

#endif	// #ifndef LOAD_ACTOR_DATA_H.