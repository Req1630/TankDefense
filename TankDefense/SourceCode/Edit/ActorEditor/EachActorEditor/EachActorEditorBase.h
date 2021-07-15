#ifndef EACH_ACTOR_EDITOR_BASE_H
#define EACH_ACTOR_EDITOR_BASE_H

#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Resource\GameParamResource\GameParamResource.h"

/***********************************
*	各アクターエディタ基底クラス.
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

	// 初期化.
	virtual bool Init() = 0;
	// 更新.
	virtual void Update() = 0;
	// ImGui描画.
	virtual bool ImGuiRender() = 0;
	// モデル描画.
	virtual void ModelRender() = 0;

	// パラメータの書き込み.
	bool WritingParam();
	// パラメーアの読み込み.
	bool ReadingParam();

protected:
	// ドラッグVectorの表示.
	void DragVector3( const char* label, D3DXVECTOR3* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f" );
	
protected:
	T			m_Prameter;		// パラメータ.
	EParamNo	m_PrameterNo;	// パラメータ番号.
};

//------------------------------.
// パラメータの書き込み.
//------------------------------.
template<typename T>
bool CEachActorEditorBase<T>::WritingParam()
{
	return CGameParamResource::WritingParam( m_PrameterNo, m_Prameter, true );
}

//------------------------------.
// パラメーアの読み込み.
//------------------------------.
template<typename T>
bool CEachActorEditorBase<T>::ReadingParam()
{
	m_Prameter = CGameParamResource::ReadingParam<T>(m_PrameterNo);

	return true;
}

//------------------------------.
// ドラッグVectorの表示.
//------------------------------.
template<typename T>
void CEachActorEditorBase<T>::DragVector3( const char* label, D3DXVECTOR3* v, float v_speed, float v_min, float v_max, const char* format )
{
	ImGui::DragFloat( label, v.x, v_speed, v_min, v_max, format );
	ImGui::DragFloat( label, v.y, v_speed, v_min, v_max, format );
	ImGui::DragFloat( label, v.z, v_speed, v_min, v_max, format );
}

#endif	// #ifndef EACH_ACTOR_EDITOR_BASE_H.