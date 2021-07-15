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
// �X�V����.
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
		switch( m_NowSelectActorState )
		{
		case ESelectStateFlag_StartPos:
			state.MovieState.StartPosition = t.Position;
			break;
		case ESelectStateFlag_EndPos:
			state.MovieState.EndPosition = t.Position;
			break;
		case ESelectStateFlag_Rot:
			break;
		default:
			break;
		}
		m_NowSelectActorState = ESelectStateFlag_None;
		OnImGuiGamepad();
	}

	if( !CInput::IsMomentPress(EKeyBind::Edit_BackMenu) ) return;
	OnImGuiGamepad();
}

//---------------------------.
// ImGui�`��.
//---------------------------.
bool CMovieActorEditor::ImGuiRender()
{
	ActorNodeDraw();
	PushActorNodeDraw();
	ImGui::Separator();
	ActorParameterDraw();

	return true;
}

//---------------------------.
// ���f���`��.
//---------------------------.
void CMovieActorEditor::ModelRender()
{
	if( m_pEditPlayer == nullptr ) return;
	if( m_NowSelectIndex < 0 ) return;

	AllActorRender();
	NowSelectActorRender();
}

//---------------------------.
// �S���̃A�N�^�[�̕`��.
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
// ���ݑI������Ă���A�N�^�[�̕`��.
//---------------------------.
void CMovieActorEditor::NowSelectActorRender()
{
	// ���ݑI������Ă���A�N�^�[�̕`��.
	SActorEditState& state = m_ActorEditStateList[m_NowSelectIndex];
	CDX9SkinMesh* pSkinMesh = GetSkinMesh( state.ActorNo, state.ListIndex );

	if( pSkinMesh == nullptr ) return;
	for( int i = 0; i < 2; i++ ){
		bool isWire = false;
		STranceform t = m_pEditPlayer->GetPutTranceform();
		t.Rotation = state.MovieState.StartRotation;
		if( i == 0 ){
			// �I������Ă���΃��C���[�\������.
			isWire = bit::IsBitFlag( m_NowSelectActorState, ESelectStateFlag_StartPos );
			if( isWire == false ) t.Position = state.MovieState.StartPosition;
		} else {
			// �I������Ă���΃��C���[�\������.
			isWire = bit::IsBitFlag( m_NowSelectActorState, ESelectStateFlag_EndPos );
			if( isWire == false ) t.Position = state.MovieState.EndPosition;
		}

		pSkinMesh->SetTranceform( t );
		pSkinMesh->SetRasterizerState( isWire ? ERS_STATE::Wire : ERS_STATE::None );
		pSkinMesh->SetColor( { m_SelectActorRedColor, 0.7f, 0.7f, 1.0f } );
		pSkinMesh->SetAnimSpeed( m_DeltaTime*0.5f );	// ��̕��`�悷��̂ő��x�𔼕��ɂ���.
		pSkinMesh->Render( &state.AC );
		pSkinMesh->SetRasterizerState( ERS_STATE::None );
	}
	m_SelectActorRedColor -= SELECT_ACTOR_COLOR_DOWN_SPEED;
	if( m_SelectActorRedColor < 1.0f ) m_SelectActorRedColor = SELECT_ACTOR_RED_COLOR;
}

//---------------------------.
// �A�N�^�[�m�[�h�̕\��.
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

			if( ImGui::Selectable( noName.c_str(), isSelected ) ) m_NowSelectIndex = i;
			if( isSelected ) ImGui::SetItemDefaultFocus();

			i++;
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
}

//---------------------------.
// �ǉ��A�N�^�[�m�[�h�̕\��.
//---------------------------.
void CMovieActorEditor::PushActorNodeDraw()
{
	ImGui::SetNextTreeNodeOpen( m_IsPushNodeOpen );
	if( ImGui::TreeNode( u8"�V�����ǉ�����" ) ){
		m_IsPushNodeOpen = true;
		ImGui::Text( u8"�ǉ��������I�u�W�F�N�g��I�����Ă�������" );

		for( int i = EActorNo_Begin; i < EActorNo_End; i++ ){
			const EActorNo no = static_cast<EActorNo>(i);

			if( ImGui::Button( ActorNoToString( no ).c_str() ) == false ) continue;

			// �A�N�^�[�N���X�̒ǉ�.
			PushList( no );


			// �e�l��ݒ肵�Ēǉ�.
			SActorEditState state;
			state.ActorNo	= no;
			state.ListIndex	= GetListSize(no)-1;

			CDX9SkinMesh* pSkinMesh = GetSkinMesh( state.ActorNo, state.ListIndex );
			state.AC.SetAnimController( pSkinMesh->GetAnimationController() );

			m_ActorEditStateList.emplace_back( state );

			m_IsPushNodeOpen = false;	// �c���[�m�[�h�����.
			m_NowSelectIndex = m_ActorEditStateList.size()-1;
		}

		ImGui::TreePop();
	} else {
		m_IsPushNodeOpen = false;
	}
}

//---------------------------.
// �A�N�^�[�p�����[�^�̕\��.
//---------------------------.
void CMovieActorEditor::ActorParameterDraw()
{
	if( m_NowSelectIndex < 0 ) return;
	SMovieActor& state = m_ActorEditStateList[m_NowSelectIndex].MovieState;

	ImGui::DragFloat( u8"�J�n����", &state.AciveStartTime, 0.1f, 0.0f, 180.0f );
	ImGui::BeginGroup();
	if( ImGui::Button( u8"�J�n�ʒu��ݒ�" )){
		OffImGuiGamepad();
		bit::OnBitFlag( &m_NowSelectActorState, ESelectStateFlag_StartPos );
		m_pEditPlayer->SetPosition( state.StartPosition );
	}
	PositionDraw( u8"  Positon", state.StartPosition );
	ImGui::EndGroup(); 
	ImGui::SameLine();
	ImGui::SeparatorEx( ImGuiSeparatorFlags_Vertical );
	ImGui::SameLine();

	ImGui::BeginGroup();
	if( ImGui::Button( u8"�I���ʒu��ݒ�" )){
		OffImGuiGamepad();
		bit::OnBitFlag( &m_NowSelectActorState, ESelectStateFlag_EndPos );
		m_pEditPlayer->SetPosition( state.EndPosition );
	}
	PositionDraw( u8"  Positon", state.EndPosition );
	ImGui::EndGroup();

	//ImGui::BeginGroup();
	//ImGui::PushStyleColor( ImGuiCol_Button,			ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	//ImGui::PushStyleColor( ImGuiCol_ButtonHovered,	ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	//ImGui::PushStyleColor( ImGuiCol_ButtonActive,	ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	//ImGui::Button( u8"" );
	//ImGui::PopStyleColor( 3 );
	PositionDraw( u8"  Rotation", state.StartRotation );
	//ImGui::EndGroup();
}

//---------------------------.
// ���W�̕\��.
//---------------------------.
void CMovieActorEditor::PositionDraw( const char* c, const D3DXVECTOR3& pos )
{
	std::string msg = c;
	msg += "\n x : %f\n y : %f\n z : %f\n";

	
	ImGui::TextWrapped( msg.c_str(), pos.x, pos.y, pos.z );
	
}

//---------------------------.
// ���X�g�̒ǉ�.
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
// ���X�g�̃T�C�Y���擾.
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
// �A�N�^�[�ԍ��𕶎��ɕϊ�.
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
// �e���b�V���̎擾.
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
// ImGui�ŃQ�[���p�b�h�����L���ɂ���.
//------------------------------------.
void CMovieActorEditor::OnImGuiGamepad()
{
	// ImGui�ŃR���g���[���[�����L����.
	CImGuiManager::EnableGamepad();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.9f;
	m_IsImGuiGamepad = true;
}

//------------------------------------.
// ImGui�ŃQ�[���p�b�h����𖳌��ɂ���.
//------------------------------------.
void CMovieActorEditor::OffImGuiGamepad()
{
	// ImGui�ŃR���g���[���[����𖳌���.
	CImGuiManager::DisableGamepad();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;
	m_IsImGuiGamepad = false;
}