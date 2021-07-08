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
		ss << "\t//---------------------"<< std::endl;
		ss << "\t// CameraState No."	<< count << std::endl;

		// ���쎞��.
		ss << "\t#ActiveTime, " << moveState.ActiveTime << std::endl;

		// �J�n�J�������.
		ss << "\t// StartState" << std::endl;
		ss << "\t#Position, ";
		ss << moveState.StartState.Position.x << ", ";
		ss << moveState.StartState.Position.y << ", ";
		ss << moveState.StartState.Position.z << std::endl;

		ss << "\t#Look Position, ";
		ss << moveState.StartState.LookPosition.x << ", ";
		ss << moveState.StartState.LookPosition.y << ", ";
		ss << moveState.StartState.LookPosition.z << std::endl;

		// �I���J�������.
		ss << "\t// EndState" << std::endl;
		ss << "\t#Position, ";
		ss << moveState.EndState.Position.x << ", ";
		ss << moveState.EndState.Position.y << ", ";
		ss << moveState.EndState.Position.z << std::endl;

		ss << "\t#Look Position, ";
		ss << moveState.EndState.LookPosition.x << ", ";
		ss << moveState.EndState.LookPosition.y << ", ";
		ss << moveState.EndState.LookPosition.z << std::endl;
		
		// ���W�ړ�����.
		ss << "\t#PosMoveTime, "		<< moveState.PosMoveTime		<< std::endl;
		// ���_���W�ړ�����.
		ss << "\t#LookPosMoveTime, "	<< moveState.LookPosMoveTime	<< std::endl;

		const SMovieShakeCamera	shakeStatae = state.ShakeState;

		std::string boolMag;
		int shakeCount = 0;
		for( const auto& shake : shakeStatae.ShakeState ){
			ss << "\t// shakeState No."	<< shakeCount << std::endl;
			// �U�ꕝ.
			ss << "\t#Amplitube, " << shake.Amplitube << ", ";
			// ���g��
			ss << "\t#Frequency, " << shake.Frequency << ", ";
			// ���쎞��.
			ss << "\t#Time, " << shake.Time << ", ";
			// �����W.
			ss << "\t#Base Position, ";
			ss << shake.BasePosition.x << ", ";
			ss << shake.BasePosition.y << ", ";
			ss << shake.BasePosition.z << ", ";

			// �������邩.
			boolMag = shake.IsAttenuation ? "true" : "false";
			ss << "\t#IsAttenuation, " << boolMag << ", ";

			// ���삷�邩.
			boolMag = shake.IsPlaying ? "true" : "false";
			ss << "\t#IsPlaying, " << boolMag << std::endl;

			shakeCount++;
		}


		ss << "\t//---------------------"<< std::endl;
		stringData.emplace_back( ss.str() );
		count++;
	}

	return stringData;
}

//----------------------------------.
// ���X�g�ɕϊ�����.
//----------------------------------.
std::vector<SMovieCamera> CCameraDataConverter::ToList( const std::vector<std::string>& dataList )
{
	std::vector<SMovieCamera> stateList;

	for( size_t i = 0; i < dataList.size(); ){
		SMovieMoveCamera moveState;

		// ���쎞��.
		moveState.ActiveTime				= std::stof( dataList[i] ); i++;

		// �J�n�J�������.
		moveState.StartState.Position.x		= std::stof( dataList[i] ); i++;
		moveState.StartState.Position.y		= std::stof( dataList[i] ); i++;
		moveState.StartState.Position.z		= std::stof( dataList[i] ); i++;

		moveState.StartState.LookPosition.x	= std::stof( dataList[i] ); i++;
		moveState.StartState.LookPosition.y	= std::stof( dataList[i] ); i++;
		moveState.StartState.LookPosition.z	= std::stof( dataList[i] ); i++;

		// �I���J�������.
		moveState.EndState.Position.x		= std::stof( dataList[i] ); i++;
		moveState.EndState.Position.y		= std::stof( dataList[i] ); i++;
		moveState.EndState.Position.z		= std::stof( dataList[i] ); i++;

		moveState.EndState.LookPosition.x	= std::stof( dataList[i] ); i++;
		moveState.EndState.LookPosition.y	= std::stof( dataList[i] ); i++;
		moveState.EndState.LookPosition.z	= std::stof( dataList[i] ); i++;

		// ���W�ړ�����.
		moveState.PosMoveTime				= std::stof( dataList[i] ); i++;

		// ���_���W�ړ�����.
		moveState.LookPosMoveTime			= std::stof( dataList[i] ); i++;

		SMovieShakeCamera	shakeStatae;

		for( auto& shake : shakeStatae.ShakeState ){
			// �U�ꕝ.
			shake.Amplitube			= std::stof( dataList[i] ); i++;
			// ���g��.
			shake.Frequency			= std::stof( dataList[i] ); i++;
			// ���쎞��.
			shake.Time				= std::stof( dataList[i] ); i++;

			// �����W.
			shake.BasePosition.x	= std::stof( dataList[i] ); i++;
			shake.BasePosition.y	= std::stof( dataList[i] ); i++;
			shake.BasePosition.z	= std::stof( dataList[i] ); i++;

			// �������邩.
			shake.IsAttenuation		= dataList[i] == "true" ? true : false; i++;
			// ���삷�邩.
			shake.IsPlaying			= dataList[i] == "true" ? true : false; i++;
		}

		stateList.emplace_back( moveState, shakeStatae );
	}

	return stateList;
}
