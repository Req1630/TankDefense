#ifndef EACH_ACTOR_EDITOR_BASE_H
#define EACH_ACTOR_EDITOR_BASE_H

#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Resource\GameParamResource\GameParamResource.h"

/***********************************
*	�e�A�N�^�[�G�f�B�^���N���X.
**/
template<class T>
class CEachActorEditorBase
{
protected:
	// �h���b�O���.
	struct stDragPrameter
	{
		std::string	Label;	// ���ږ�.
		float		Speed;	// �ω����x.
		float		Min;	// �ŏ��l.
		float		Max;	// �ő�l.
		std::string Format;	// float�\���`��.
		std::string Msg;	// ���ڂ̏ڂ�������.

		stDragPrameter()
			: Label		( "" )
			, Speed		( 1.0f )
			, Min		( 0.0f )
			, Max		( 1.0f )
			, Format	( "%.4f" )
			, Msg		( "" )
		{}
	} typedef SDragPrameter;

public:
	CEachActorEditorBase()
		: m_Prameter	()
		, m_PrameterNo	( EParamNo::None )
	{}
	virtual ~CEachActorEditorBase()
	{}

	// ������.
	virtual bool Init() = 0;
	// �X�V.
	virtual void Update() = 0;
	// ImGui�`��.
	virtual bool ImGuiRender() = 0;
	// ���f���`��.
	virtual void ModelRender() = 0;

	// �p�����[�^�̏�������.
	bool WritingParam();
	// �p�����[�A�̓ǂݍ���.
	bool ReadingParam();

protected:
	// �h���b�OVector�̕\��.
	void DragVector3( D3DXVECTOR3* v, const SDragPrameter& p );
	// �h���b�OFloat�̕\��.
	void DragFloat( float* v, const SDragPrameter& p );

	// �p�����[�^��\��.
	void EditParameterDraw( const int& maxIndex );

	// �h���b�O���̓ǂݍ���.
	bool ReadingDragParameter( const char* fileName );

protected:
	std::vector<SDragPrameter>	m_DragParamList;
	T							m_Prameter;		// �p�����[�^.
	EParamNo					m_PrameterNo;	// �p�����[�^�ԍ�.
};

//------------------------------.
// �p�����[�^�̏�������.
//------------------------------.
template<typename T>
bool CEachActorEditorBase<T>::WritingParam()
{
	return CGameParamResource::WritingParam( m_PrameterNo, m_Prameter, true );
}

//------------------------------.
// �p�����[�A�̓ǂݍ���.
//------------------------------.
template<typename T>
bool CEachActorEditorBase<T>::ReadingParam()
{
	m_Prameter = CGameParamResource::ReadingParam<T>(m_PrameterNo);

	return true;
}

//------------------------------.
// �h���b�OVector�̕\��.
//------------------------------.
template<typename T>
void CEachActorEditorBase<T>::DragVector3( D3DXVECTOR3* v, const SDragPrameter& p )
{
	DragFloat( v.x, p );
	DragFloat( v.y, p );
	DragFloat( v.z, p );
}

//------------------------------.
// �h���b�OFloat�̕\��.
//------------------------------.
template<typename T>
void CEachActorEditorBase<T>::DragFloat( float* v, const SDragPrameter& p )
{
	ImGui::DragFloat( p.Label.c_str(), v, p.Speed, p.Min, p.Max, p.Format.c_str() );
	ImGui::SameLine();
	CImGuiManager::HelpMarker( p.Msg.c_str() );
}

//------------------------------.
// �p�����[�^��\��.
//------------------------------.
template<typename T>
void CEachActorEditorBase<T>::EditParameterDraw( const int& maxIndex )
{
	ImGui::PushItemWidth( 120.0f );
	for( int i = 0; i < maxIndex; i++ ){
		DragFloat( &m_Prameter[i], m_DragParamList[i] );
	}
	ImGui::PopItemWidth();
}

//------------------------------.
// �h���b�O���̓ǂݍ���.
//------------------------------.
template<typename T>
bool CEachActorEditorBase<T>::ReadingDragParameter( const char* fileName )
{
	std::vector<std::string> list = fileManager::TextLoading( fileName );

	if( list.empty() == true ) return false;
	const int listSize = list.size();
	for( int i = 0; i < listSize; ){
		SDragPrameter tempParam;

		tempParam.Label	= list[i];				i++;
		float initval	= std::stof(list[i]);	i++;
		tempParam.Min	= std::stof(list[i]);	i++;
		tempParam.Max	= std::stof(list[i]);	i++;
		tempParam.Msg	= list[i];				i++;
		tempParam.Speed	= std::stof(list[i]);	i++;

		// �����s�������������A���s�����ɒu��.
		size_t pos = tempParam.Msg.find("@");
		if( pos != std::string::npos ) tempParam.Msg.replace( pos, 1, "\n" );

		m_DragParamList.emplace_back( tempParam );
	}

	return true;
}

#endif	// #ifndef EACH_ACTOR_EDITOR_BASE_H.