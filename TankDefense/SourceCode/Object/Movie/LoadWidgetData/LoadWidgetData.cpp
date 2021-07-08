#include "LoadWidgetData.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include <iostream>
#include <fstream>

CWidgetDataConverter::CWidgetDataConverter()
{
}

CWidgetDataConverter::~CWidgetDataConverter()
{
}

//----------------------------------.
// 文字に変換する.
//----------------------------------.
std::vector<std::string> CWidgetDataConverter::ToString( const std::vector<SMovieWidget>& stateList )
{
	std::vector<std::string> stringData;

	int count = 1;
	std::string boolMag;	// ブール型文字の出力用.

	for( auto& state : stateList ){
		std::stringstream ss;
		// 番号.
		ss << "\t//---------------------"<< std::endl;
		ss << "\t// WidgetState No."	<< count << std::endl;

		ss << "\t#SpriteName, " << state.SpriteName << std::endl;

		// 動作開始時間.
		ss << "\t#AciveStartTime, " << state.AciveStartTime << std::endl;
		
		// 座標.
		ss << "\t#Position, ";
		ss << state.Position.x << ", ";
		ss << state.Position.y << std::endl;

		// アニメーション情報の取得.
		const SMovieAnimState animState = state.AnimState;
		ss << "\t//AnimationState" << std::endl;

		// X 方向にUVスクロールするか.
		boolMag = animState.IsUVScrollX ? "true" : "false";
		ss << "\t#IsUVScrollX, " << boolMag << std::endl;

		// Y 方向にUVスクロールするか.
		boolMag = animState.IsUVScrollY ? "true" : "false";
		ss << "\t#IsUVScrollY, " << boolMag << std::endl;

		// UVスクロール速度.
		ss << "\t#UVScrollSpeedX, " << animState.UVScrollSpeed.x << std::endl;
		ss << "\t#UVScrollSpeedY, " << animState.UVScrollSpeed.y << std::endl;

		// アニメーションするか.
		boolMag = animState.IsAnimation ? "true" : "false";
		ss << "\t#IsAnimation, " << boolMag << std::endl;

		// アニメーション速度.
		ss << "\t#FrameCountSpeed, " << animState.FrameCountSpeed << std::endl;


		// フェード情報の取得.
		const SMovieFadeState fadeState = state.FadeState;
		ss << "\t//FadeState" << std::endl;

		// フェードフラグ.
		ss << "\t#FadeFlag, " << fadeState.FadeFlag	<< std::endl;
		// フェード速度.
		ss << "\t#FadeSpeed, " << fadeState.FadeSpeed	<< std::endl;
		// 最大アルファ値.
		ss << "\t#AlphaMax, " << fadeState.AlphaMax	<< std::endl;
		// 最小アルファ値.
		ss << "\t#AlphaMin, " << fadeState.AlphaMin	<< std::endl;

		ss << "\t//---------------------"<< std::endl;
		stringData.emplace_back( ss.str() );
		count++;
	}

	return stringData;
}

//----------------------------------.
// リストに変換する.
//----------------------------------.
std::vector<SMovieWidget> CWidgetDataConverter::ToList( const std::vector<std::string>& dataList )
{
	std::vector<SMovieWidget> stateList;

	for( size_t i = 0; i < dataList.size(); ){
		SMovieWidget state;

		state.SpriteName		= dataList[i]; i++;

		// 動作開始時間.
		state.AciveStartTime	= std::stof( dataList[i] ); i++;

		// 座標.
		state.Position.x		= std::stof( dataList[i] ); i++;
		state.Position.y		= std::stof( dataList[i] ); i++;

		// アニメーション情報の取得.
		SMovieAnimState& animState = state.AnimState;

		// X 方向にUVスクロールするか.
		animState.IsUVScrollX	= dataList[i] == "true" ? true : false; i++;

		// Y 方向にUVスクロールするか.
		animState.IsUVScrollY	= dataList[i] == "true" ? true : false; i++;

		// UVスクロール速度.
		animState.UVScrollSpeed.x	= std::stof( dataList[i] ); i++;
		animState.UVScrollSpeed.y	= std::stof( dataList[i] ); i++;

		// アニメーションするか.
		animState.IsAnimation		= dataList[i] == "true" ? true : false; i++;

		// アニメーション速度.
		animState.FrameCountSpeed	= std::stoi( dataList[i] ); i++;


		// フェード情報の取得.
		SMovieFadeState& fadeState = state.FadeState;

		// フェードフラグ.
		fadeState.FadeFlag	= std::stoi( dataList[i] ); i++;
		// フェード速度.
		fadeState.FadeSpeed	= std::stof( dataList[i] ); i++;
		// 最大アルファ値.
		fadeState.AlphaMax	= std::stof( dataList[i] ); i++;
		// 最小アルファ値.
		fadeState.AlphaMin	= std::stof( dataList[i] ); i++;
		

		stateList.emplace_back( state );
	}

	return stateList;
}
