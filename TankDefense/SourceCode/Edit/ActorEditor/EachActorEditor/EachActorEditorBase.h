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
	void DragVector3( D3DXVECTOR3* v, const SDragPrameter& p );
	// ドラッグFloatの表示.
	void DragFloat( float* v, const SDragPrameter& p );

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
void CEachActorEditorBase<T>::DragVector3( D3DXVECTOR3* v, const SDragPrameter& p )
{
	DragFloat( v.x, p );
	DragFloat( v.y, p );
	DragFloat( v.z, p );
}

//------------------------------.
// ドラッグFloatの表示.
//------------------------------.
template<typename T>
void CEachActorEditorBase<T>::DragFloat( float* v, const SDragPrameter& p )
{
	ImGui::DragFloat( p.Label.c_str(), v, p.Speed, p.Min, p.Max, p.Format.c_str() );
	ImGui::SameLine();
	CImGuiManager::HelpMarker( p.Msg.c_str() );
}

#endif	// #ifndef EACH_ACTOR_EDITOR_BASE_H.