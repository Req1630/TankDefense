#include "ActorEditor.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\Input\Input.h"

CActorEditor::CActorEditor()
	: m_pEnemyList			()
	, m_ActorEditStateList	()
	, m_NowSelectIndex		( -1 )
	, m_IsPushNodeOpen		( false )
{
}

CActorEditor::~CActorEditor()
{
}

//---------------------------.
// 更新処理.
//---------------------------.
void CActorEditor::Update()
{
	const float deltaTime = GetDeltaTime();
}

//---------------------------.
// ImGui描画.
//---------------------------.
bool CActorEditor::ImGuiRender()
{
	ImGui::PushItemWidth( 160.0f );
	std::string noName = "None";
	if( m_NowSelectIndex >= 0 ){
		const EActorNo no	= m_ActorEditStateList[m_NowSelectIndex].ActorNo;
		const int index		= m_ActorEditStateList[m_NowSelectIndex].ListIndex;

		noName = ActorNoToString(no) + "_" + std::to_string(index);
	}
	if( ImGui::BeginCombo( "##2", noName.c_str() ) ){
		int i = 0;
		for( auto& actorMesh : m_ActorEditStateList ){
			const bool isSelected = ( i == m_NowSelectIndex );
			noName = ActorNoToString(actorMesh.ActorNo) + "_" + std::to_string(actorMesh.ListIndex);

			if( ImGui::Selectable( noName.c_str(), isSelected ) ) m_NowSelectIndex = i;
			if( isSelected ) ImGui::SetItemDefaultFocus();

			i++;
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	
	PushActorNodeDraw();

	return true;
}

//---------------------------.
// モデル描画.
//---------------------------.
void CActorEditor::ModelRender()
{
	if( m_NowSelectIndex < 0 ) return;
	SActorEditState& state = m_ActorEditStateList[m_NowSelectIndex];

	SingleModelRender( state.ActorNo, state.ListIndex, false );
}

//---------------------------.
// 単体モデルの描画.
//---------------------------.
void CActorEditor::SingleModelRender( const EActorNo& no, const int& index, const bool& isWire )
{
	switch( no )
	{
	case EActorNo_Enemy:
		m_pEnemyList[index]->ModelRender( isWire );
		break;
	case EActorNo_Enemy1:
		m_pEnemyList[index]->ModelRender( isWire );
		break;
	case EActorNo_Enemy2:
		m_pEnemyList[index]->ModelRender( isWire );
		break;
	case EActorNo_Boss:
		m_pEnemyList[index]->ModelRender( isWire );
		break;
	default:
		break;
	}
}

//---------------------------.
// 追加アクターノードの表示.
//---------------------------.
void CActorEditor::PushActorNodeDraw()
{
	ImGui::SetNextTreeNodeOpen( m_IsPushNodeOpen );
	if( ImGui::TreeNode( u8"新しく追加する" ) ){
		m_IsPushNodeOpen = true;
		ImGui::Text( u8"追加したいオブジェクトを選択してください" );

		for( int i = EActorNo_Begin; i < EActorNo_End; i++ ){
			const EActorNo no = static_cast<EActorNo>(i);

			if( ImGui::Button( ActorNoToString( no ).c_str() ) == false ) continue;

			// アクタークラスの追加.
			PushList( no );

			// 各値を設定して追加.
			SActorEditState state;
			state.ActorNo	= no;
			state.ListIndex	= GetListSize(no)-1;
			m_ActorEditStateList.emplace_back( state );

			m_IsPushNodeOpen = false;	// ツリーノードを閉じる.
			if( m_NowSelectIndex < 0 ) m_NowSelectIndex++;
		}

		ImGui::TreePop();
	}
}

//---------------------------.
// リストの追加.
//---------------------------.
void CActorEditor::PushList( const EActorNo& no )
{
	switch( no )
	{
	case EActorNo_Enemy:
		m_pEnemyList.emplace_back( std::make_unique<CMovieActor<CEnemy>>() );
		m_pEnemyList.back()->Init();
		break;
	case EActorNo_Enemy1:
		m_pEnemyList.emplace_back( std::make_unique<CMovieActor<CEnemy>>() );
		m_pEnemyList.back()->Init();
		break;
	case EActorNo_Enemy2:
		m_pEnemyList.emplace_back( std::make_unique<CMovieActor<CEnemy>>() );
		m_pEnemyList.back()->Init();
		break;
	case EActorNo_Boss:
		m_pEnemyList.emplace_back( std::make_unique<CMovieActor<CEnemy>>() );
		m_pEnemyList.back()->Init();
		break;
	default:
		break;
	}
}

//---------------------------.
// リストのサイズを取得.
//---------------------------.
int CActorEditor::GetListSize( const EActorNo& no )
{
	switch( no )
	{
	case EActorNo_Enemy:	return m_pEnemyList.size();
	case EActorNo_Enemy1:	return m_pEnemyList.size();
	case EActorNo_Enemy2:	return m_pEnemyList.size();
	case EActorNo_Boss:		return m_pEnemyList.size();
	default:
		break;
	}
	return -1;
}

//---------------------------.
// アクター番号を文字に変換.
//---------------------------.
std::string CActorEditor::ActorNoToString( const EActorNo& no )
{
	switch( no )
	{
	case EActorNo_Enemy:	return u8"Enemy";
	case EActorNo_Enemy1:	return u8"Enemy1";
	case EActorNo_Enemy2:	return u8"Enemy2";
	case EActorNo_Boss:		return u8"Boss";
	default:
		break;
	}

	return "";
}
