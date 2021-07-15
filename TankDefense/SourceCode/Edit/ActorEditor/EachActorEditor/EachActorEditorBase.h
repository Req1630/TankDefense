#ifndef EACH_ACTOR_EDITOR_BASE_H
#define EACH_ACTOR_EDITOR_BASE_H

#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Resource\GameParamResource\GameParamResource.h"

/***********************************
*	�e�A�N�^�[�G�f�B�^���N���X.
**/
template<class T>
class CEachActorEditorBase
{
protected:
	struct stDragPrameter
	{
		std::string	Label;
		float		Speed;
		float		Min;
		float		Max;
		std::string Format;
		std::string Msg;

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

protected:
	T			m_Prameter;		// �p�����[�^.
	EParamNo	m_PrameterNo;	// �p�����[�^�ԍ�.
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

#endif	// #ifndef EACH_ACTOR_EDITOR_BASE_H.