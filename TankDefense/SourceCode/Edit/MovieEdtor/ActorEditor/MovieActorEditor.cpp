#include "MovieActorEditor.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Utility\BitFlagManager\BitFlagManager.h"
#include "..\..\EditPlayer\EditPlayer.h"


namespace
{
	constexpr float SELECT_ACTOR_RED_COLOR			= 4.0f;
	constexpr float SELECT_ACTOR_COLOR_DOWN_SPEED	= 0.02f;
};

CMovieActorEditor::CMovieActorEditor()
	: m_pEditPlayer			( nullptr )
	, m_pEnemyList			()
	, m_ActorEditStateList	()
	, m_DeltaTime			( 0.0f )
	, m_SelectActorRedColor	( SELECT_ACTOR_RED_COLOR )
	, m_NowSelectIndex		( -1 )
	, m_NowSelectActorState	( ESelectStateFlag_None )
	, m_IsPushNodeOpen		( false )
	, m_IsImGuiGamepad		( true )
{
}

CMovieActorEditor::~CMovieActorEditor()
{
}

//---------------------------.
// 更新処理.
//---------------------------.
void CMovieActorEditor::Update()
{
	m_DeltaTime = GetDeltaTime();


	if( m_IsImGuiGamepad == true ) return;
	if( m_pEditPlayer == nullptr ) return;

	m_pEditPlayer->SetIsRotController( true );
	m_pEditPlayer->Update();

	SActorEditState& state = m_ActorEditStateList[m_NowSelectIndex];

	if( m_pEditPlayer->IsPut() == true ){
		SActorEditState& state = m_ActorEditStateList[m_NowSelectIndex];
		const STranceform t = m_pEditPlayer->GetPutTranceform();
		state.MovieState.StartPosition	= t.Position;
//		state.MovieState.EndPosition	= t.Position;
		state.MovieState.StartRotation	= t.Rotation;
		m_NowSelectActorState = ESelectStateFlag_AllOff;
		OnImGuiGamepad();
	}

	if( !CInput::IsMomentPress(EKeyBind::Edit_BackMenu) ) return;
	OnImGuiGamepad();
}

//---------------------------.
// ImGui描画.
//---------------------------.
bool CMovieActorEditor::ImGuiRender()
{
	ActorNodeDraw();
	ImGui::SameLine();
	PushActorNodeDraw();
	ImGui::Separator();
	ActorParameterDraw();

	return true;
}

//---------------------------.
// モデル描画.
//---------------------------.
void CMovieActorEditor::ModelRender()
{
	if( m_pEditPlayer == nullptr ) return;
	if( m_NowSelectIndex < 0 ) return;

	AllActorRender();
	NowSelectActorRender();
}

//---------------------------.
// エディタ用プレイヤーの設定.
//---------------------------.
void CMovieActorEditor::SetEditPlayer( CEditPlayer* pPlayer )
{ 
	m_pEditPlayer = pPlayer;

	if( m_pEditPlayer == nullptr ) return;

	if( m_ActorEditStateList.empty() == true ) return;
	m_NowSelectIndex = 0;

	SMovieActor& state = m_ActorEditStateList[m_NowSelectIndex].MovieState;
	m_pEditPlayer->SetPosition( state.StartPosition );
	m_pEditPlayer->SetRotation( state.StartRotation );
	m_pEditPlayer->Update();
}

// アクターリストの取得.
SMovieActorStateList CMovieActorEditor::GetActorStateList()
{
	SMovieActorStateList stateList;

	for( auto s : m_ActorEditStateList ){
		switch( s.ActorNo )
		{
		case EActorNo_Enemy:
			stateList.EnemyList.emplace_back( s.MovieState );
			break;
		}
	}

	return stateList;
}

// アクターリストの設定.
void CMovieActorEditor::SetActorStateList( const SMovieActorStateList stateList )
{
	CreateMovieActorList( stateList.EnemyList, m_pEnemyList, EActorNo_Enemy );
}

//---------------------------.
// 全部のアクターの描画.
//---------------------------.
void CMovieActorEditor::AllActorRender()
{
	CDX9SkinMesh* pSkinMesh = nullptr;
	int i = 0;
	for( auto& s : m_ActorEditStateList ){
		if( i++ == m_NowSelectIndex ) continue;

		pSkinMesh = GetSkinMesh( s.ActorNo, s.ListIndex );
		if( pSkinMesh == nullptr ) continue;

		pSkinMesh->SetPosition( s.MovieState.StartPosition );
		pSkinMesh->SetRotation( s.MovieState.StartRotation );
		pSkinMesh->SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } );
		pSkinMesh->SetAnimSpeed( m_DeltaTime );
		pSkinMesh->Render( &s.AC );
	}
}

//---------------------------.
// 現在選択されているアクターの描画.
//---------------------------.
void CMovieActorEditor::NowSelectActorRender()
{
	// 現在選択されているアクターの描画.
	SActorEditState& state = m_ActorEditStateList[m_NowSelectIndex];
	CDX9SkinMesh* pSkinMesh = GetSkinMesh( state.ActorNo, state.ListIndex );

	if( pSkinMesh == nullptr ) return;

	const STranceform t = m_pEditPlayer->GetPutTranceform();
//	t.Position = state.MovieState.EndPosition;
//	t.Rotation = state.MovieState.StartRotation;

	pSkinMesh->SetTranceform( t );
	pSkinMesh->SetRasterizerState( ERS_STATE::Wire );
	pSkinMesh->SetColor( { m_SelectActorRedColor, 0.7f, 0.7f, 1.0f } );
	pSkinMesh->SetAnimSpeed( m_DeltaTime*0.5f );	// 二体分描画するので速度を半分にする.
	pSkinMesh->Render( &state.AC );
	pSkinMesh->SetRasterizerState( ERS_STATE::None );
	
	m_SelectActorRedColor -= SELECT_ACTOR_COLOR_DOWN_SPEED;
	if( m_SelectActorRedColor < 1.0f ) m_SelectActorRedColor = SELECT_ACTOR_RED_COLOR;
}

//---------------------------.
// アクターノードの表示.
//---------------------------.
void CMovieActorEditor::ActorNodeDraw()
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

			if( ImGui::Selectable( noName.c_str(), isSelected ) ){
				m_NowSelectIndex = i;

				SMovieActor& state = m_ActorEditStateList[m_NowSelectIndex].MovieState;
				m_pEditPlayer->SetPosition( state.StartPosition );
				m_pEditPlayer->SetRotation( state.StartRotation );
				m_pEditPlayer->Update();
			}
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
void CMovieActorEditor::PushActorNodeDraw()
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

			CDX9SkinMesh* pSkinMesh = GetSkinMesh( state.ActorNo, state.ListIndex );
			state.AC.SetAnimController( pSkinMesh->GetAnimationController() );

			m_ActorEditStateList.emplace_back( state );

			m_IsPushNodeOpen = false;	// ツリーノードを閉じる.
			m_NowSelectIndex = m_ActorEditStateList.size()-1;

			m_pEditPlayer->SetPosition( { 0.0f, 0.0f, 0.0f } );
			m_pEditPlayer->SetRotation( { 0.0f, 0.0f, 0.0f } );
			m_pEditPlayer->Update();
		}

		ImGui::TreePop();
	} else {
		m_IsPushNodeOpen = false;
	}
}

//---------------------------.
// アクターパラメータの表示.
//---------------------------.
void CMovieActorEditor::ActorParameterDraw()
{
	if( m_NowSelectIndex < 0 ) return;
	SMovieActor& state = m_ActorEditStateList[m_NowSelectIndex].MovieState;

	ImGui::DragFloat( u8"開始時間", &state.AciveStartTime, 0.1f, 0.0f, 180.0f );

	if( ImGui::Button( u8"オブジェクトを配置" )){
		OffImGuiGamepad();
		bit::OnBitFlag( &m_NowSelectActorState, ESelectStateFlag_AllOn );
		m_pEditPlayer->SetPosition( state.StartPosition );
		m_pEditPlayer->SetRotation( state.StartRotation );
	}
	if( bit::IsBitFlag( m_NowSelectActorState, ESelectStateFlag_AllOn ) ){
		const STranceform t = m_pEditPlayer->GetPutTranceform();
		state.StartPosition = t.Position;
		state.StartRotation = t.Rotation;
	}
	PositionDraw( u8"  Position", state.StartPosition );
	ImGui::SameLine();
	PositionDraw( u8"  Rotation", state.StartRotation );

}

//---------------------------.
// 座標の表示.
//---------------------------.
void CMovieActorEditor::PositionDraw( const char* c, const D3DXVECTOR3& pos )
{
	std::string msg = c;
	msg += "\n x : %f\n y : %f\n z : %f\n";

	
	ImGui::TextWrapped( msg.c_str(), pos.x, pos.y, pos.z );
	
}

//---------------------------.
// リストの追加.
//---------------------------.
void CMovieActorEditor::PushList( const EActorNo& no )
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
int CMovieActorEditor::GetListSize( const EActorNo& no )
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
std::string CMovieActorEditor::ActorNoToString( const EActorNo& no )
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
CDX9SkinMesh* CMovieActorEditor::GetSkinMesh( const EActorNo& no, const int& index )
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

//------------------------------------.
// ImGuiでゲームパッド操作を有効にする.
//------------------------------------.
void CMovieActorEditor::OnImGuiGamepad()
{
	// ImGuiでコントローラー操作を有効化.
	CImGuiManager::EnableGamepad();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.9f;
	m_IsImGuiGamepad = true;
}

//------------------------------------.
// ImGuiでゲームパッド操作を無効にする.
//------------------------------------.
void CMovieActorEditor::OffImGuiGamepad()
{
	// ImGuiでコントローラー操作を無効化.
	CImGuiManager::DisableGamepad();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;
	m_IsImGuiGamepad = false;
}