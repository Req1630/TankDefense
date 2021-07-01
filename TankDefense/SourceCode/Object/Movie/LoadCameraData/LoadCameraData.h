#ifndef LOAD_CAMERA_DATA_H
#define LOAD_CAMERA_DATA_H

#include "..\..\CameraBase\MovieCamera\MovieCameraState.h"

/******************************************
*	ムービー用カメラデータと文字列を変換するクラス.
**/
class CCameraDataConverter
{
public:
	CCameraDataConverter();
	~CCameraDataConverter();

	// 文字に変換する.
	static std::vector<std::string> ToString( const std::vector<SMovieCamera>& stateList );

	// リストに変換する.
	static std::vector<SMovieCamera> ToList( const std::vector<std::string>& dataList );
};

#endif	// #ifndef LOAD_CAMERA_DATA_H.
