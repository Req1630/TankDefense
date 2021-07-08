#ifndef MOVIE_DATA_LOADER_H
#define MOVIE_DATA_LOADER_H

#include "..\LoadCameraData\LoadCameraData.h"
#include "..\LoadWidgetData\LoadWidgetData.h"

#include <unordered_map>

/************************************
*	���[�r�[�f�[�^�ǂݍ��݃N���X.
**/
class CMovieDataLoader
{
public:
	CMovieDataLoader();
	~CMovieDataLoader();

	// ������.
	bool Init();

	// �f�[�^�̓ǂݍ���.
	static bool DataLoading();

	// �f�[�^�̏�������.
	static bool DataWriting(
		const std::vector<SMovieCamera>& stateList,
		const std::vector<SMovieWidget>& widgetList );

private:
	// ���[�r�[�f�[�^�p�X���X�g�̍쐬.
	bool InitDataPathList();

private:
	std::unordered_map<std::string, std::string>	m_DataPathList;
};

#endif	// #ifndef MOVIE_DATA_LOADER_H.
