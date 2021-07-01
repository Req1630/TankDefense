#include "LoadCameraData.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include <iostream>
#include <fstream>

CCameraDataConverter::CCameraDataConverter()
{
}

CCameraDataConverter::~CCameraDataConverter()
{
}

//----------------------------------.
// �����ɕϊ�����.
//----------------------------------.
std::vector<std::string> CCameraDataConverter::ToString( const std::vector<SMovieCamera>& stateList )
{
	std::vector<std::string> stringData;

	int count = 1;
	for( auto& state : stateList ){
		const SMovieMoveCamera	moveState = state.MoveState;

		std::stringstream ss;
		// �ԍ�.
		ss << "//---------------------"<< std::endl;
		ss << "// CameraState No."	<< count << std::endl;

		// ���쎞��.
		ss << "#ActiveTime, " << moveState.ActiveTime << std::endl;

		// �J�n�J�������.
		ss << "// StartState" << std::endl;
		ss << "#Position, ";
		ss << moveState.StartState.Position.x << ", ";
		ss << moveState.StartState.Position.y << ", ";
		ss << moveState.StartState.Position.z << std::endl;

		ss << "#Look Position, ";
		ss << moveState.StartState.LookPosition.x << ", ";
		ss << moveState.StartState.LookPosition.y << ", ";
		ss << moveState.StartState.LookPosition.z << std::endl;

		// �I���J�������.
		ss << "// EndState" << std::endl;
		ss << "#Position, ";
		ss << moveState.EndState.Position.x << ", ";
		ss << moveState.EndState.Position.y << ", ";
		ss << moveState.EndState.Position.z << std::endl;

		ss << "#Look Position, ";
		ss << moveState.EndState.LookPosition.x << ", ";
		ss << moveState.EndState.LookPosition.y << ", ";
		ss << moveState.EndState.LookPosition.z << std::endl;
		
		// ���W�ړ�����.
		ss << "#PosMoveTime, "		<< moveState.PosMoveTime		<< std::endl;
		// ���_���W�ړ�����.
		ss << "#LookPosMoveTime, "	<< moveState.LookPosMoveTime	<< std::endl;

		const SMovieShakeCamera	shakeStatae = state.ShakeState;

		std::string boolMag;
		int shakeCount = 0;
		for( const auto& shake : shakeStatae.ShakeState ){
			ss << "// shakeState No."	<< shakeCount << std::endl;
			// �U�ꕝ.
			ss << "#Amplitube, " << shake.Amplitube << ", ";
			// ���g��
			ss << "#Frequency, " << shake.Frequency << ", ";
			// ���쎞��.
			ss << "#Time, " << shake.Time << ", ";
			// �����W.
			ss << "#Base Position, ";
			ss << shake.BasePosition.x << ", ";
			ss << shake.BasePosition.y << ", ";
			ss << shake.BasePosition.z << ", ";

			// �������邩.
			boolMag = shake.IsAttenuation ? "true" : "false";
			ss << "#IsAttenuation, " << boolMag << ", ";

			// ���삷�邩.
			boolMag = shake.IsPlaying ? "true" : "false";
			ss << "#IsPlaying, " << boolMag << std::endl;

			shakeCount++;
		}


		ss << "//---------------------"<< std::endl;
		stringData.emplace_back( ss.str() );
		count++;
	}
#if 0
	// �t�@�C�����J��.
	std::fstream fileStream( "Data\\TestData.txt", std::ios::out, std::ios::trunc );

	for( auto& s : stringData ){
		fileStream << s << std::endl;
	}

	fileStream.close();
	
#endif

	return stringData;
}

//----------------------------------.
// ���X�g�ɕϊ�����.
//----------------------------------.
std::vector<SMovieCamera> CCameraDataConverter::ToList( const std::vector<std::string>& dataList )
{
	std::vector<std::string> list = fileManager::TextLoading("Data\\TestData.txt");

	std::vector<SMovieCamera> stateList;

	for( size_t i = 0; i < list.size(); ){
		SMovieMoveCamera moveState;

		// ���쎞�Ԃ̏o��.
		moveState.ActiveTime				= std::stof( list[i] ); i++;

		// �J�n�J�������̏o��.
		moveState.StartState.Position.x		= std::stof( list[i] ); i++;
		moveState.StartState.Position.y		= std::stof( list[i] ); i++;
		moveState.StartState.Position.z		= std::stof( list[i] ); i++;

		moveState.StartState.LookPosition.x	= std::stof( list[i] ); i++;
		moveState.StartState.LookPosition.y	= std::stof( list[i] ); i++;
		moveState.StartState.LookPosition.z	= std::stof( list[i] ); i++;

		// �I���J�������̏o��.
		moveState.EndState.Position.x		= std::stof( list[i] ); i++;
		moveState.EndState.Position.y		= std::stof( list[i] ); i++;
		moveState.EndState.Position.z		= std::stof( list[i] ); i++;

		moveState.EndState.LookPosition.x	= std::stof( list[i] ); i++;
		moveState.EndState.LookPosition.y	= std::stof( list[i] ); i++;
		moveState.EndState.LookPosition.z	= std::stof( list[i] ); i++;

		// ���W�ړ����Ԃ̏o��.
		moveState.PosMoveTime				= std::stof( list[i] ); i++;

		// ���_���W�ړ����Ԃ̏o��.
		moveState.LookPosMoveTime			= std::stof( list[i] ); i++;

		SMovieShakeCamera	shakeStatae;

		for( auto& shake : shakeStatae.ShakeState ){
			// �U�ꕝ.
			shake.Amplitube			= std::stof( list[i] ); i++;
			// ���g��.
			shake.Frequency			= std::stof( list[i] ); i++;
			// ���쎞��.
			shake.Time				= std::stof( list[i] ); i++;

			// �����W.
			shake.BasePosition.x	= std::stof( list[i] ); i++;
			shake.BasePosition.y	= std::stof( list[i] ); i++;
			shake.BasePosition.z	= std::stof( list[i] ); i++;

			// �������邩.
			shake.IsAttenuation		= list[i] == "true" ? true : false; i++;
			// ���삷�邩.
			shake.IsPlaying			= list[i] == "true" ? true : false; i++;
		}

		stateList.emplace_back( moveState, shakeStatae );
	}

	return stateList;
}
