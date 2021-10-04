#include "MovieDataLoader.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Utility\BitFlagManager\BitFlagManager.h"

#include <iostream>
#include <fstream>

namespace
{
	constexpr char MOVIE_LIST_PATH[]	= "Data\\Parameter\\Movie\\MovieList.txt";	// ムービーリストのパス.

	// カメラ情報の開始文字、終了文字.
	constexpr char CAERA_STATE_START[]	= "CameraState Start {";
	constexpr char CAERA_STATE_END[]	= "} CameraState End";

	// ウィジェット情報の開始文字、終了文字.
	constexpr char WIDGET_STATE_START[]	= "WidgetState Start {";
	constexpr char WIDGET_STATE_END[]	= "} WidgetState End";

	// アクター情報の開始文字、終了文字.
	constexpr char ACTOR_STATE_START[]	= "ActorState Start {";
	constexpr char ACTOR_STATE_END[]	= "} ActorState End";
};

CMovieDataLoader::CMovieDataLoader()
	: m_MovieNameList	()
	, m_DataPathList	()
	, m_EachLoadEndFlag	( ELoadedFlag_None )
{
}

CMovieDataLoader::~CMovieDataLoader()
{
}

//------------------------------.
// 初期化.
//------------------------------.
bool CMovieDataLoader::Init( const bool& isAllData )
{
	if( InitDataPathList() == false ) return false;
//	if( isAllData == false ) return false;
 	if( InitDataList() == false ) return false;

	return true;
}

//------------------------------.
// データの読み込み.
//------------------------------.
bool CMovieDataLoader::DataLoading( const char* filePath, SMovieData* pMovieData )
{
	const std::vector<std::string> dataList = fileManager::TextLoading( filePath );

	std::vector<std::string>	cameraDataList;
	std::vector<std::string>	widgetDataList;
	std::vector<std::string>	actorDataList;

	bool isTimeLoad = false;

	for( auto& s : dataList ){

		if( isTimeLoad == false ){
			pMovieData->MovieTime = std::stof(s);
			isTimeLoad = true;
		}

		// カメラ情報の読み込み.
		LoadingEachData<CCameraDataConverter>( 
			s,
			cameraDataList, 
			pMovieData->CameraList,
			ELoadedFlag_Camera,
			CAERA_STATE_START,
			CAERA_STATE_END );

		// 画像情報の読み込み.
		LoadingEachData<CWidgetDataConverter>( 
			s,
			widgetDataList, 
			pMovieData->WidgetList,
			ELoadedFlag_Widget,
			WIDGET_STATE_START,
			WIDGET_STATE_END );

		// アクター情報の読み込み.
		LoadingEachData<CActorDataConverter>( 
			s,
			actorDataList, 
			pMovieData->ActorList,
			ELoadedFlag_Actor,
			ACTOR_STATE_START,
			ACTOR_STATE_END );
	}

	m_EachLoadEndFlag, ELoadedFlag_None;

	return true;
}

//------------------------------.
// データの書き込み.
//------------------------------.
bool CMovieDataLoader::DataWriting( const EMovieNo& movieNo, const SMovieData& movieData )
{
	const std::string n = m_MovieNameList.at(movieNo);

	// ファイルを開く.
	std::fstream fileStream( m_DataPathList[n].first, std::ios::out, std::ios::trunc );

	if( fileStream.is_open() == false ) return false;

	const std::vector<std::string> cameraDataList	= CCameraDataConverter::ToString( movieData.CameraList );
	const std::vector<std::string> widgetDataList	= CWidgetDataConverter::ToString( movieData.WidgetList );
	const std::vector<std::string> actorDataList	= CActorDataConverter::ToString( movieData.ActorList );

	// 時間の書き込み.
	WritingTimeData( fileStream, movieData.MovieTime );

	// カメラ情報の書き込み.
	WritingEachData( fileStream, cameraDataList, CAERA_STATE_START, CAERA_STATE_END );

	// ウィジェット情報の書き込み.
	WritingEachData( fileStream, widgetDataList, WIDGET_STATE_START, WIDGET_STATE_END );

	// アクター情報の書き込み.
	WritingEachData( fileStream, actorDataList, ACTOR_STATE_START, ACTOR_STATE_END );

	// ファイルを閉じる.
	fileStream.close();

	return true;
}

//------------------------------.
// ムービーデータの取得.
//------------------------------.
SMovieData CMovieDataLoader::GetMovieData( const EMovieNo& movieNo )
{
	const std::string n = m_MovieNameList.at(movieNo);
	return m_DataPathList[n].second;
}

//------------------------------.
// ムービーデータの設定.
//------------------------------.
void CMovieDataLoader::SetMovieData( const EMovieNo& movieNo, const SMovieData& movieData )
{
	const std::string n = m_MovieNameList.at(movieNo);
	m_DataPathList[n].second = movieData;
}

//------------------------------.
// ムービーデータパスリストの作成.
//------------------------------.
bool CMovieDataLoader::InitDataPathList()
{
	std::vector<std::string> list = fileManager::TextLoading(MOVIE_LIST_PATH);

	if( list.empty() == true ) return true;

	int listSize = list.size();
	for( int i = 0; i < listSize; i+=2 ){
		m_MovieNameList[static_cast<EMovieNo>(i)] = list[i];

		m_DataPathList[list[i]].first = list[i+1];
	}

	return true;
}

//------------------------------.
// データリストの作成.
//------------------------------.
bool CMovieDataLoader::InitDataList()
{
	if( m_DataPathList.empty() == true ) return false;

	for( auto& l : m_DataPathList ){
		data_pair& p = l.second;
		if( DataLoading( p.first.c_str(), &p.second ) == false ) return false;
	}

	return true;
}

//------------------------------.
// 各データを読み込む.
//------------------------------.
template<typename LoadCalas, typename State>
void CMovieDataLoader::LoadingEachData( 
	const std::string& s,
	std::vector<std::string>& dataList,
	State& stateList,
	const ELoadedFlag& flag,
	const char* startTag,
	const char* endTag )
{
	// 終了タグを見つけたらデータリストを変換する.
	if( s.find( endTag ) != std::string::npos ){
		stateList = LoadCalas::ToList( dataList );
		bit::OffBitFlag( &m_EachLoadEndFlag, flag );
	}

	// フラグが立っていればデータを追加していく.
	if( bit::IsBitFlag( m_EachLoadEndFlag, flag ) ){
		dataList.emplace_back( s );
	}

	// 開始タグを見つけたらフラグを立てる.
	if( s.find( startTag ) != std::string::npos ){
		bit::OnBitFlag( &m_EachLoadEndFlag, flag );
	}
}

//------------------------------.
// 各データを書き込む.
//------------------------------.
void CMovieDataLoader::WritingEachData( 
	std::fstream& fs,
	const std::vector<std::string>& dataList,
	const char* startTag, 
	const char* endTag )
{
	fs << startTag << std::endl;
	for( auto& s : dataList ) fs << s << std::endl;
	fs << endTag << std::endl;
	fs << std::endl;
}

//------------------------------.
// 時間を書き込む.
//------------------------------.
void CMovieDataLoader::WritingTimeData( std::fstream& fs, const float& time )
{
	fs << "//---------------------" << std::endl;
	fs << "#Time," << time << std::endl;
	fs << "//---------------------" << std::endl;
	fs << std::endl;
}