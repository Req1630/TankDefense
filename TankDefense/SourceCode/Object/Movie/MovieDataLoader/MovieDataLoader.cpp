#include "MovieDataLoader.h"
#include "..\..\..\Utility\FileManager\FileManager.h"

#include <iostream>
#include <fstream>

namespace
{
	constexpr char FILE_NAME[] = "Data\\TestData.txt";

	constexpr char CAERA_STATE_START[]	= "CameraState Start {";
	constexpr char CAERA_STATE_END[]	= "} CameraState End";
};

CMovieDataLoader::CMovieDataLoader()
{
}

CMovieDataLoader::~CMovieDataLoader()
{
}

//------------------------------.
// データの読み込み.
//------------------------------.
bool CMovieDataLoader::DataLoading()
{
	const std::vector<std::string> dataList = fileManager::TextLoading( FILE_NAME );

	std::vector<std::string>	cameraDataList;
	std::vector<SMovieCamera>	cameraStateList;
	bool isCameraLoadStart = false;
	for( auto& s : dataList ){

		// カメラ情報の読み込み.
		if( s.find( CAERA_STATE_END ) != std::string::npos ){
			cameraStateList = CCameraDataConverter::ToList( cameraDataList );
			isCameraLoadStart = false;
		}
		if( isCameraLoadStart == true ){
			cameraDataList.emplace_back( s );
		}
		if( s.find( CAERA_STATE_START ) != std::string::npos ){
			isCameraLoadStart = true;
		}

	}

	return true;
}

//------------------------------.
// データの書き込み.
//------------------------------.
bool CMovieDataLoader::DataWriting( const std::vector<SMovieCamera>& stateList )
{
	// ファイルを開く.
	std::fstream fileStream( FILE_NAME, std::ios::out, std::ios::trunc );

	const std::vector<std::string> cameraDataList = CCameraDataConverter::ToString( stateList );
	// カメラ情報の書き込み.
	fileStream << CAERA_STATE_START << std::endl;
	for( auto& s : cameraDataList ){
		fileStream << s << std::endl;
	}
	fileStream << CAERA_STATE_END << std::endl;


	// ファイルを閉じる.
	fileStream.close();

	return true;
}