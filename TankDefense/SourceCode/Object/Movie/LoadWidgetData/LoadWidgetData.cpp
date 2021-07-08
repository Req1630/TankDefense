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
// �����ɕϊ�����.
//----------------------------------.
std::vector<std::string> CWidgetDataConverter::ToString( const std::vector<SMovieWidget>& stateList )
{
	std::vector<std::string> stringData;

	int count = 1;
	std::string boolMag;	// �u�[���^�����̏o�͗p.

	for( auto& state : stateList ){
		std::stringstream ss;
		// �ԍ�.
		ss << "\t//---------------------"<< std::endl;
		ss << "\t// WidgetState No."	<< count << std::endl;

		ss << "\t#SpriteName, " << state.SpriteName << std::endl;

		// ����J�n����.
		ss << "\t#AciveStartTime, " << state.AciveStartTime << std::endl;
		
		// ���W.
		ss << "\t#Position, ";
		ss << state.Position.x << ", ";
		ss << state.Position.y << std::endl;

		// �A�j���[�V�������̎擾.
		const SMovieAnimState animState = state.AnimState;
		ss << "\t//AnimationState" << std::endl;

		// X ������UV�X�N���[�����邩.
		boolMag = animState.IsUVScrollX ? "true" : "false";
		ss << "\t#IsUVScrollX, " << boolMag << std::endl;

		// Y ������UV�X�N���[�����邩.
		boolMag = animState.IsUVScrollY ? "true" : "false";
		ss << "\t#IsUVScrollY, " << boolMag << std::endl;

		// UV�X�N���[�����x.
		ss << "\t#UVScrollSpeedX, " << animState.UVScrollSpeed.x << std::endl;
		ss << "\t#UVScrollSpeedY, " << animState.UVScrollSpeed.y << std::endl;

		// �A�j���[�V�������邩.
		boolMag = animState.IsAnimation ? "true" : "false";
		ss << "\t#IsAnimation, " << boolMag << std::endl;

		// �A�j���[�V�������x.
		ss << "\t#FrameCountSpeed, " << animState.FrameCountSpeed << std::endl;


		// �t�F�[�h���̎擾.
		const SMovieFadeState fadeState = state.FadeState;
		ss << "\t//FadeState" << std::endl;

		// �t�F�[�h�t���O.
		ss << "\t#FadeFlag, " << fadeState.FadeFlag	<< std::endl;
		// �t�F�[�h���x.
		ss << "\t#FadeSpeed, " << fadeState.FadeSpeed	<< std::endl;
		// �ő�A���t�@�l.
		ss << "\t#AlphaMax, " << fadeState.AlphaMax	<< std::endl;
		// �ŏ��A���t�@�l.
		ss << "\t#AlphaMin, " << fadeState.AlphaMin	<< std::endl;

		ss << "\t//---------------------"<< std::endl;
		stringData.emplace_back( ss.str() );
		count++;
	}

	return stringData;
}

//----------------------------------.
// ���X�g�ɕϊ�����.
//----------------------------------.
std::vector<SMovieWidget> CWidgetDataConverter::ToList( const std::vector<std::string>& dataList )
{
	std::vector<SMovieWidget> stateList;

	for( size_t i = 0; i < dataList.size(); ){
		SMovieWidget state;

		state.SpriteName		= dataList[i]; i++;

		// ����J�n����.
		state.AciveStartTime	= std::stof( dataList[i] ); i++;

		// ���W.
		state.Position.x		= std::stof( dataList[i] ); i++;
		state.Position.y		= std::stof( dataList[i] ); i++;

		// �A�j���[�V�������̎擾.
		SMovieAnimState& animState = state.AnimState;

		// X ������UV�X�N���[�����邩.
		animState.IsUVScrollX	= dataList[i] == "true" ? true : false; i++;

		// Y ������UV�X�N���[�����邩.
		animState.IsUVScrollY	= dataList[i] == "true" ? true : false; i++;

		// UV�X�N���[�����x.
		animState.UVScrollSpeed.x	= std::stof( dataList[i] ); i++;
		animState.UVScrollSpeed.y	= std::stof( dataList[i] ); i++;

		// �A�j���[�V�������邩.
		animState.IsAnimation		= dataList[i] == "true" ? true : false; i++;

		// �A�j���[�V�������x.
		animState.FrameCountSpeed	= std::stoi( dataList[i] ); i++;


		// �t�F�[�h���̎擾.
		SMovieFadeState& fadeState = state.FadeState;

		// �t�F�[�h�t���O.
		fadeState.FadeFlag	= std::stoi( dataList[i] ); i++;
		// �t�F�[�h���x.
		fadeState.FadeSpeed	= std::stof( dataList[i] ); i++;
		// �ő�A���t�@�l.
		fadeState.AlphaMax	= std::stof( dataList[i] ); i++;
		// �ŏ��A���t�@�l.
		fadeState.AlphaMin	= std::stof( dataList[i] ); i++;
		

		stateList.emplace_back( state );
	}

	return stateList;
}
