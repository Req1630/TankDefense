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
	void DragVector3( const char* label, D3DXVECTOR3* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f" );
	
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
void CEachActorEditorBase<T>::DragVector3( const char* label, D3DXVECTOR3* v, float v_speed, float v_min, float v_max, const char* format )
{
	ImGui::DragFloat( label, v.x, v_speed, v_min, v_max, format );
	ImGui::DragFloat( label, v.y, v_speed, v_min, v_max, format );
	ImGui::DragFloat( label, v.z, v_speed, v_min, v_max, format );
}

#endif	// #ifndef EACH_ACTOR_EDITOR_BASE_H.