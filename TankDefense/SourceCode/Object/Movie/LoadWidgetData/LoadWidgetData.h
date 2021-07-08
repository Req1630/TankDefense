#ifndef LOAD_WIDGET_DATA_H
#define LOAD_WIDGET_DATA_H

#include "..\..\GameObject\Widget\MovieWidget\MovieWidget.h"

/******************************************
*	���[�r�[�p�E�B�W�F�b�g�f�[�^�ƕ������ϊ�����N���X.
**/
class CWidgetDataConverter
{
public:
	CWidgetDataConverter();
	~CWidgetDataConverter();

	// �����ɕϊ�����.
	static std::vector<std::string> ToString( const std::vector<SMovieWidget>& stateList );

	// ���X�g�ɕϊ�����.
	static std::vector<SMovieWidget> ToList( const std::vector<std::string>& dataList );
};

#endif	// #ifndef LOAD_WIDGET_DATA_H.
