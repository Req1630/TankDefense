#ifndef MOVIE_DATA_LOADER_H
#define MOVIE_DATA_LOADER_H

#include "..\LoadCameraData\LoadCameraData.h"

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
	static bool DataWriting( const std::vector<SMovieCamera>& stateList );
};

#endif	// #ifndef MOVIE_DATA_LOADER_H.
