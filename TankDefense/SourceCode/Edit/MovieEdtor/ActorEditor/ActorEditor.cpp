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
// �X�V����.
//---------------------------.
void CActorEditor::Update()
{
	const float deltaTime = GetDeltaTime();
}

//---------------------------.
// ImGui�`��.
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
// ���f���`��.
//---------------------------.
void CActorEditor::ModelRender()
{
	if( m_NowSelectIndex < 0 ) return;
	SActorEditState& state = m_ActorEditStateList[m_NowSelectIndex];

	SingleModelRender( state.ActorNo, state.ListIndex, false );
}

//---------------------------.
// �P�̃��f���̕`��.
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
// �ǉ��A�N�^�[�m�[�h�̕\��.
//---------------------------.
void CActorEditor::PushActorNodeDraw()
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
			m_ActorEditStateList.emplace_back( state );

			m_IsPushNodeOpen = false;	// �c���[�m�[�h�����.
			if( m_NowSelectIndex < 0 ) m_NowSelectIndex++;
		}

		ImGui::TreePop();
	}
}

//---------------------------.
// ���X�g�̒ǉ�.
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
// ���X�g�̃T�C�Y���擾.
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
// �A�N�^�[�ԍ��𕶎��ɕϊ�.
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
