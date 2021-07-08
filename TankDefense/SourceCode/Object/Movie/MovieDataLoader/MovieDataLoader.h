#ifndef MOVIE_DATA_LOADER_H
#define MOVIE_DATA_LOADER_H

#include "..\LoadCameraData\LoadCameraData.h"
#include "..\LoadWidgetData\LoadWidgetData.h"

#include <unordered_map>

/************************************
*	ムービーデータ読み込みクラス.
**/
class CMovieDataLoader
{
public:
	CMovieDataLoader();
	~CMovieDataLoader();

	// 初期化.
	bool Init();

	// データの読み込み.
	static bool DataLoading();

	// データの書き込み.
	static bool DataWriting(
		const std::vector<SMovieCamera>& stateList,
		const std::vector<SMovieWidget>& widgetList );

private:
	// ムービーデータパスリストの作成.
	bool InitDataPathList();

private:
	std::unordered_map<std::string, std::string>	m_DataPathList;
};

#endif	// #ifndef MOVIE_DATA_LOADER_H.
