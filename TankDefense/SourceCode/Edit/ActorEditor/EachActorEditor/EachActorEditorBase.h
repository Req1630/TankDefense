#ifndef EACH_ACTOR_EDITOR_BASE_H
#define EACH_ACTOR_EDITOR_BASE_H

#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Resource\GameParamResource\GameParamResource.h"

/***********************************
*	各アクターエディタ基底クラス.
**/
template<class T>
class CEachActorEditorBase
{
protected:
	// ドラッグ情報.
	struct stDragPrameter
	{
		std::string	Label;	// 項目名.
		float		Speed;	// 変化速度.
		float		Min;	// 最小値.
		float		Max;	// 最大値.
		std::string Format;	// float表示形式.
		std::string Msg;	// 項目の詳しい説明.

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

	// パラメータを表示.
	void EditParameterDraw( const int& maxIndex );

	// ドラッグ情報の読み込み.
	bool ReadingDragParameter( const char* fileName );

protected:
	std::vector<SDragPrameter>	m_DragParamList;
	T							m_Prameter;		// パラメータ.
	EParamNo					m_PrameterNo;	// パラメータ番号.
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

//------------------------------.
// パラメータを表示.
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
// ドラッグ情報の読み込み.
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

		// 仮改行文字を検索し、改行文字に置換.
		size_t pos = tempParam.Msg.find("@");
		if( pos != std::string::npos ) tempParam.Msg.replace( pos, 1, "\n" );

		m_DragParamList.emplace_back( tempParam );
	}

	return true;
}

#endif	// #ifndef EACH_ACTOR_EDITOR_BASE_H.