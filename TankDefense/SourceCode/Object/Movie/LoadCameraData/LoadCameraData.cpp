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
// 文字に変換する.
//----------------------------------.
std::vector<std::string> CCameraDataConverter::ToString( const std::vector<SMovieCamera>& stateList )
{
	std::vector<std::string> stringData;

	int count = 1;
	for( auto& state : stateList ){
		const SMovieMoveCamera	moveState = state.MoveState;

		std::stringstream ss;
		// 番号.
		ss << "\t//---------------------"<< std::endl;
		ss << "\t// CameraState No."	<< count << std::endl;

		// 動作時間.
		ss << "\t#ActiveTime, " << moveState.ActiveTime << std::endl;

		// 開始カメラ情報.
		ss << "\t// StartState" << std::endl;
		ss << "\t#Position, ";
		ss << moveState.StartState.Position.x << ", ";
		ss << moveState.StartState.Position.y << ", ";
		ss << moveState.StartState.Position.z << std::endl;

		ss << "\t#Look Position, ";
		ss << moveState.StartState.LookPosition.x << ", ";
		ss << moveState.StartState.LookPosition.y << ", ";
		ss << moveState.StartState.LookPosition.z << std::endl;

		// 終了カメラ情報.
		ss << "\t// EndState" << std::endl;
		ss << "\t#Position, ";
		ss << moveState.EndState.Position.x << ", ";
		ss << moveState.EndState.Position.y << ", ";
		ss << moveState.EndState.Position.z << std::endl;

		ss << "\t#Look Position, ";
		ss << moveState.EndState.LookPosition.x << ", ";
		ss << moveState.EndState.LookPosition.y << ", ";
		ss << moveState.EndState.LookPosition.z << std::endl;
		
		// 座標移動時間.
		ss << "\t#PosMoveTime, "		<< moveState.PosMoveTime		<< std::endl;
		// 視点座標移動時間.
		ss << "\t#LookPosMoveTime, "	<< moveState.LookPosMoveTime	<< std::endl;

		const SMovieShakeCamera	shakeStatae = state.ShakeState;

		std::string boolMag;
		int shakeCount = 0;
		for( const auto& shake : shakeStatae.ShakeState ){
			ss << "\t// shakeState No."	<< shakeCount << std::endl;
			// 振れ幅.
			ss << "\t#Amplitube, " << shake.Amplitube << ", ";
			// 周波数
			ss << "\t#Frequency, " << shake.Frequency << ", ";
			// 動作時間.
			ss << "\t#Time, " << shake.Time << ", ";
			// 基底座標.
			ss << "\t#Base Position, ";
			ss << shake.BasePosition.x << ", ";
			ss << shake.BasePosition.y << ", ";
			ss << shake.BasePosition.z << ", ";

			// 減衰するか.
			boolMag = shake.IsAttenuation ? "true" : "false";
			ss << "\t#IsAttenuation, " << boolMag << ", ";

			// 動作するか.
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
// リストに変換する.
//----------------------------------.
std::vector<SMovieCamera> CCameraDataConverter::ToList( const std::vector<std::string>& dataList )
{
	std::vector<SMovieCamera> stateList;

	for( size_t i = 0; i < dataList.size(); ){
		SMovieMoveCamera moveState;

		// 動作時間.
		moveState.ActiveTime				= std::stof( dataList[i] ); i++;

		// 開始カメラ情報.
		moveState.StartState.Position.x		= std::stof( dataList[i] ); i++;
		moveState.StartState.Position.y		= std::stof( dataList[i] ); i++;
		moveState.StartState.Position.z		= std::stof( dataList[i] ); i++;

		moveState.StartState.LookPosition.x	= std::stof( dataList[i] ); i++;
		moveState.StartState.LookPosition.y	= std::stof( dataList[i] ); i++;
		moveState.StartState.LookPosition.z	= std::stof( dataList[i] ); i++;

		// 終了カメラ情報.
		moveState.EndState.Position.x		= std::stof( dataList[i] ); i++;
		moveState.EndState.Position.y		= std::stof( dataList[i] ); i++;
		moveState.EndState.Position.z		= std::stof( dataList[i] ); i++;

		moveState.EndState.LookPosition.x	= std::stof( dataList[i] ); i++;
		moveState.EndState.LookPosition.y	= std::stof( dataList[i] ); i++;
		moveState.EndState.LookPosition.z	= std::stof( dataList[i] ); i++;

		// 座標移動時間.
		moveState.PosMoveTime				= std::stof( dataList[i] ); i++;

		// 視点座標移動時間.
		moveState.LookPosMoveTime			= std::stof( dataList[i] ); i++;

		SMovieShakeCamera	shakeStatae;

		for( auto& shake : shakeStatae.ShakeState ){
			// 振れ幅.
			shake.Amplitube			= std::stof( dataList[i] ); i++;
			// 周波数.
			shake.Frequency			= std::stof( dataList[i] ); i++;
			// 動作時間.
			shake.Time				= std::stof( dataList[i] ); i++;

			// 基底座標.
			shake.BasePosition.x	= std::stof( dataList[i] ); i++;
			shake.BasePosition.y	= std::stof( dataList[i] ); i++;
			shake.BasePosition.z	= std::stof( dataList[i] ); i++;

			// 減衰するか.
			shake.IsAttenuation		= dataList[i] == "true" ? true : false; i++;
			// 動作するか.
			shake.IsPlaying			= dataList[i] == "true" ? true : false; i++;
		}

		stateList.emplace_back( moveState, shakeStatae );
	}

	return stateList;
}
