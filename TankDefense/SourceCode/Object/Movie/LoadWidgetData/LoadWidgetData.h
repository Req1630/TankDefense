#ifndef LOAD_WIDGET_DATA_H
#define LOAD_WIDGET_DATA_H

#include "..\..\GameObject\Widget\MovieWidget\MovieWidget.h"

/******************************************
*	ムービー用ウィジェットデータと文字列を変換するクラス.
**/
class CWidgetDataConverter
{
public:
	CWidgetDataConverter();
	~CWidgetDataConverter();

	// 文字に変換する.
	static std::vector<std::string> ToString( const std::vector<SMovieWidget>& stateList );

	// リストに変換する.
	static std::vector<SMovieWidget> ToList( const std::vector<std::string>& dataList );
};

#endif	// #ifndef LOAD_WIDGET_DATA_H.
