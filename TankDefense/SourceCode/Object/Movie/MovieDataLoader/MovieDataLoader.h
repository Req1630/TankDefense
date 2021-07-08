#ifndef MOVIE_DATA_LOADER_H
#define MOVIE_DATA_LOADER_H

#include "..\LoadCameraData\LoadCameraData.h"
#include "..\LoadWidgetData\LoadWidgetData.h"

/************************************
*	���[�r�[�f�[�^�ǂݍ��݃N���X.
**/
class CMovieDataLoader
{
public:
	CMovieDataLoader();
	~CMovieDataLoader();

	// �f�[�^�̓ǂݍ���.
	static bool DataLoading();

	// �f�[�^�̏�������.
	static bool DataWriting( 
		const std::vector<SMovieCamera>& stateList,
		const std::vector<SMovieWidget>& widgetList );
};

#endif	// #ifndef MOVIE_DATA_LOADER_H.
