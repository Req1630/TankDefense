#ifndef MOVIE_DATA_LOADER_H
#define MOVIE_DATA_LOADER_H

#include "..\LoadCameraData\LoadCameraData.h"
#include "..\LoadWidgetData\LoadWidgetData.h"

/************************************
*	ムービーデータ読み込みクラス.
**/
class CMovieDataLoader
{
public:
	CMovieDataLoader();
	~CMovieDataLoader();

	// データの読み込み.
	static bool DataLoading();

	// データの書き込み.
	static bool DataWriting( 
		const std::vector<SMovieCamera>& stateList,
		const std::vector<SMovieWidget>& widgetList );
};

#endif	// #ifndef MOVIE_DATA_LOADER_H.
