#include "ActorEditor.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\EditPlayer\EditPlayer.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"

CActorEditor::CActorEditor()
	: m_pEditPlayer			( nullptr )
	, m_pEnemyList			()
	, m_ActorEditStateList	()
	, m_NowSelectIndex		( -1 )
	, m_IsPushNodeOpen		( false )
	, m_IsImGuiGamepad		( true )
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

	if( m_IsImGuiGamepad == true ) return;
	if( m_pEditPlayer == nullptr ) return;
	m_pEditPlayer->Update();
}

//---------------------------.
// ImGui描画.
//---------------------------.
bool CActorEditor::ImGuiRender()
{
	ActorNodeDraw();
	PushActorNodeDraw();
	ImGui::Separator();
	ActorParameterDraw();

	return true;
}

//---------------------------.
// モデル描画.
//---------------------------.
void CActorEditor::ModelRender()
{
	if( m_pEditPlayer == nullptr ) return;
	if( m_NowSelectIndex < 0 ) return;

	SActorEditState& state = m_ActorEditStateList[m_NowSelectIndex];
	CDX9SkinMesh* pSkinMesh = GetSkinMesh( state.ActorNo, state.ListIndex );

	if( pSkinMesh == nullptr ) return;

	pSkinMesh->SetTranceform( m_pEditPlayer->GetPutTranceform() );
	pSkinMesh->Render();
}

//---------------------------.
// アクターノードの表示.
//---------------------------.
void CActorEditor::ActorNodeDraw()
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
	} else {
		m_IsPushNodeOpen = false;
	}
}

//---------------------------.
// アクターパラメータの表示.
//---------------------------.
void CActorEditor::ActorParameterDraw()
{
	if( m_NowSelectIndex < 0 ) return;
	SMovieActor& state = m_ActorEditStateList[m_NowSelectIndex].MovieState;

	ImGui::DragFloat( u8"開始時間", &state.AciveStartTime, 0.1f, 0.0f, 180.0f );
	ImGui::BeginGroup();
	if( ImGui::Button( u8"開始位置を設定" )){

	}
	PositionDraw( u8"  Positon", state.StartPosition );
	ImGui::EndGroup(); ImGui::SameLine();

	ImGui::BeginGroup();
	if( ImGui::Button( u8"終了位置を設定" )){

	}
	PositionDraw( u8"  Positon", state.EndPosition );
	ImGui::EndGroup(); ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::PushStyleColor( ImGuiCol_Button,			ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	ImGui::PushStyleColor( ImGuiCol_ButtonHovered,	ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	ImGui::PushStyleColor( ImGuiCol_ButtonActive,	ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	ImGui::Button( u8"" );
	ImGui::PopStyleColor( 3 );
	PositionDraw( u8"  Rotation", state.StartRotation );
	ImGui::EndGroup();
}

//---------------------------.
// 座標の表示.
//---------------------------.
void CActorEditor::PositionDraw( const char* c, const D3DXVECTOR3& pos )
{
	std::string msg = c;
	msg += "\n x : %f\n y : %f\n z : %f\n";

	
	ImGui::TextWrapped( msg.c_str(), pos.x, pos.y, pos.z );
	
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

//---------------------------.
// 各メッシュの取得.
//---------------------------.
CDX9SkinMesh* CActorEditor::GetSkinMesh( const EActorNo& no, const int& index )
{
	switch( no )
	{
	case EActorNo_Enemy:	return m_pEnemyList[index]->GetSkinMesh();
	case EActorNo_Enemy1:	return m_pEnemyList[index]->GetSkinMesh();
	case EActorNo_Enemy2:	return m_pEnemyList[index]->GetSkinMesh();
	case EActorNo_Boss:		return m_pEnemyList[index]->GetSkinMesh();
	default:
		break;
	}
	return nullptr;
}
