#ifndef LOAD_CAMERA_DATA_H
#define LOAD_CAMERA_DATA_H

#include "..\..\CameraBase\MovieCamera\MovieCameraState.h"

/******************************************
*	���[�r�[�p�J�����f�[�^�ƕ������ϊ�����N���X.
**/
class CCameraDataConverter
{
public:
	CCameraDataConverter();
	~CCameraDataConverter();

	// �����ɕϊ�����.
	static std::vector<std::string> ToString( const std::vector<SMovieCamera>& stateList );

	// ���X�g�ɕϊ�����.
	static std::vector<SMovieCamera> ToList( const std::vector<std::string>& dataList );
};

#endif	// #ifndef LOAD_CAMERA_DATA_H.
