#include "MovieDataLoader.h"
#include "..\..\..\Utility\FileManager\FileManager.h"

#include <iostream>
#include <fstream>

namespace
{
	constexpr char MOVIE_LIST_PATH[]	= "Data\\Parameter\\Movie\\MovieList.txt";	// ���[�r�[���X�g�̃p�X.
	constexpr char FILE_NAME[]			= "Data\\TestData.txt";

	// �J�������̊J�n�����A�I������.
	constexpr char CAERA_STATE_START[]	= "CameraState Start {";
	constexpr char CAERA_STATE_END[]	= "} CameraState End";

	// �E�B�W�F�b�g���̊J�n�����A�I������.
	constexpr char WIDGET_STATE_START[]	= "WidgetState Start {";
	constexpr char WIDGET_STATE_END[]	= "} WidgetState End";
};

CMovieDataLoader::CMovieDataLoader()
	: m_DataPathList	()
{
}

CMovieDataLoader::~CMovieDataLoader()
{
}

//------------------------------.
// ������.
//------------------------------.
bool CMovieDataLoader::Init()
{
	if( InitDataPathList() == false ) return false;

	return true;
}

//------------------------------.
// �f�[�^�̓ǂݍ���.
//------------------------------.
bool CMovieDataLoader::DataLoading()
{
	const std::vector<std::string> dataList = fileManager::TextLoading( FILE_NAME );

	std::vector<std::string>	cameraDataList;
	std::vector<SMovieCamera>	cameraStateList;
	bool isCameraLoadStart = false;

	std::vector<std::string>	widgetDataList;
	std::vector<SMovieWidget>	widgetStateList;
	bool isWidgetLoadStart = false;

	for( auto& s : dataList ){

		// �J�������̓ǂݍ���.
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

		// �E�B�W�F�b�g���̓ǂݍ���.
		if( s.find( WIDGET_STATE_END ) != std::string::npos ){
			widgetStateList = CWidgetDataConverter::ToList( widgetDataList );
			isWidgetLoadStart = false;
		}
		if( isWidgetLoadStart == true ){
			widgetDataList.emplace_back( s );
		}
		if( s.find( WIDGET_STATE_START ) != std::string::npos ){
			isWidgetLoadStart = true;
		}

	}

	return true;
}

//------------------------------.
// �f�[�^�̏�������.
//------------------------------.
bool CMovieDataLoader::DataWriting(
	const std::vector<SMovieCamera>& stateList,
	const std::vector<SMovieWidget>& widgetList )
{
	// �t�@�C�����J��.
	std::fstream fileStream( FILE_NAME, std::ios::out, std::ios::trunc );

	const std::vector<std::string> cameraDataList = CCameraDataConverter::ToString( stateList );
	const std::vector<std::string> widgetDataList = CWidgetDataConverter::ToString( widgetList );

	// �J�������̏�������.
	fileStream << CAERA_STATE_START << std::endl;
	for( auto& s : cameraDataList ){
		fileStream << s << std::endl;
	}
	fileStream << CAERA_STATE_END << std::endl;

	fileStream << std::endl;

	// �E�B�W�F�b�g���̏�������.
	fileStream << WIDGET_STATE_START << std::endl;
	for( auto& s : widgetDataList ){
		fileStream << s << std::endl;
	}
	fileStream << WIDGET_STATE_END << std::endl;


	// �t�@�C�������.
	fileStream.close();

	return true;
}

//------------------------------.
// ���[�r�[�f�[�^�p�X���X�g�̍쐬.
//------------------------------.
bool CMovieDataLoader::InitDataPathList()
{
	std::vector<std::string> list = fileManager::TextLoading(MOVIE_LIST_PATH);

	if( list.empty() == true ) return true;

	int listSize = list.size();
	for( int i = 0; i < listSize; i+=2 ){
		m_DataPathList[list[i]] = list[i+1];
	}

	return true;
}