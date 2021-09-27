#include "LoadActorData.h"

namespace
{
	constexpr char ENEMY_TAG[] = "Enemy";
};

CActorDataConverter::CActorDataConverter()
{
}

CActorDataConverter::~CActorDataConverter()
{
}

//----------------------------------.
// �����ɕϊ�����.
//----------------------------------.
std::vector<std::string> CActorDataConverter::ToString( const SMovieActorStateList& stateList )
{
	std::vector<std::string> stringData;

	std::stringstream ss;
	ActorStateToString( &ss, stateList.EnemyList, ENEMY_TAG );

	stringData.emplace_back( ss.str() );
	return stringData;
}

//----------------------------------.
// ���X�g�ɕϊ�����.
//----------------------------------.
SMovieActorStateList CActorDataConverter::ToList( const std::vector<std::string>& dataList )
{
	SMovieActorStateList stateList;

	for( int i = 0; i < static_cast<int>(dataList.size()); ){
		if( StringToActorState( &i, dataList, &stateList.EnemyList, ENEMY_TAG ) ) continue;
	}

	return stateList;
}

//----------------------------------.
// �A�N�^�[���𕶎��ɕϊ�����.
//----------------------------------.
void CActorDataConverter::ActorStateToString( 
	std::stringstream* pSs,
	const std::vector<SMovieActor>& stateList, 
	const char* tag )
{
	*pSs << "\t//---------------------"<< std::endl;

	int count = 1;
	for( const auto& state : stateList ){
		*pSs << "\t// " << tag << " No."	<< count << std::endl;

		*pSs << "\t#TagName," << tag << std::endl;

		// ����J�n����.
		*pSs << "\t#ActiveStartTime," << state.AciveStartTime << std::endl;

		// �J�n���W.
		*pSs << "\t// StartPosition" << std::endl;
		*pSs << "\t#Position,";
		*pSs << state.StartPosition.x << ", ";
		*pSs << state.StartPosition.y << ", ";
		*pSs << state.StartPosition.z << std::endl;

		// �J�n��]���W.
		*pSs << "\t// StartRotationn" << std::endl;
		*pSs << "\t#Position,";
		*pSs << state.StartRotation.x << ", ";
		*pSs << state.StartRotation.y << ", ";
		*pSs << state.StartRotation.z << std::endl;

		count++;
	}

	*pSs << "\t//---------------------"<< std::endl;
}

//----------------------------------.
// ���������A�N�^�[���ɕϊ�����.
//----------------------------------.
bool CActorDataConverter::StringToActorState(
	int* pIndex,
	const std::vector<std::string>& dataList,
	std::vector<SMovieActor>* pStateList,
	const char* tag )
{
	if( dataList[*pIndex] != tag ) return false;

	(*pIndex)++;

	SMovieActor tmpState;

	tmpState.AciveStartTime	= std::stof( dataList[*pIndex] ); (*pIndex)++;

	tmpState.StartPosition.x = std::stof( dataList[*pIndex] ); (*pIndex)++;
	tmpState.StartPosition.y = std::stof( dataList[*pIndex] ); (*pIndex)++;
	tmpState.StartPosition.z = std::stof( dataList[*pIndex] ); (*pIndex)++;
	tmpState.StartRotation.x = std::stof( dataList[*pIndex] ); (*pIndex)++;
	tmpState.StartRotation.y = std::stof( dataList[*pIndex] ); (*pIndex)++;
	tmpState.StartRotation.z = std::stof( dataList[*pIndex] ); (*pIndex)++;

	pStateList->emplace_back( tmpState );

	return true;
}