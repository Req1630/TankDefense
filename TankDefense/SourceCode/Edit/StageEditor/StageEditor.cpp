#include "StageEditor.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Edit\EditPlayer\EditPlayer.h"
#include "..\..\Object\GameObject\Actor\Stage\StageLoader\StageLoader.h"

namespace
{
	constexpr char	ACOTR_MESH_LIST_PATH[]		= "Data\\Parameter\\Stage\\StageObjectList.txt";
	constexpr char	SELECT_COMBO_NAME[]			= u8"�z�u�������I�u�W�F�N�g��I�����Ă�������";
	constexpr char	DELETE_COMBO_NAME[]			= u8"�폜�������I�u�W�F�N�g��I�����Ă�������";
	constexpr float	DELETE_ACTOR_COLOR[]		= { 2.5f, 1.5f, 1.5f, 1.0f };
	constexpr float	NORMAL_ACTOR_COLOR[]		= { 1.0f, 1.0f, 1.0f, 1.0f };
};

CStageEditor::CStageEditor()
	: m_pEditPlayer			( nullptr )
	, m_pStageLoader		( std::make_unique<CStageLoader>())
	, m_pUndoRedo			( std::make_unique<CUndoRedo<SActorParam>>( &m_ActorList ))
	, m_ActorList			()
	, m_ActorMeshList		()
	, m_NowSelectActor		()
	, m_NowStageNo			( EStageNo_Enemy )
	, m_DeleteActorNo		()
{
}

CStageEditor::~CStageEditor()
{
}

//------------------------------------.
// ������.
//------------------------------------.
bool CStageEditor::Init()
{
	if( InitActorMeshList()		== false ) return false;
	if( m_pStageLoader->Init()	== false ) return false;

	m_ActorList = m_pStageLoader->GetActorList( m_NowStageNo );

	return true;
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CStageEditor::Update()
{
	if( m_IsImGuiGamepad == true ) return;
	if( m_pEditPlayer == nullptr ) return;

	m_pEditPlayer->SetIsRotController( true );
	m_pEditPlayer->Update();

	if( m_pEditPlayer->IsPut() == true ){
		const int listSize = static_cast<int>(m_ActorList.size());
		const SActorParam actorParam = { m_NowSelectActor.ActorNo, m_pEditPlayer->GetPutTranceform() };
		m_ActorList.insert( m_ActorList.begin()+listSize, actorParam );
		m_pUndoRedo->PushUndo( listSize, false, actorParam );
	}
}

//------------------------------------.
// ImGui�`��.
//------------------------------------.
bool CStageEditor::ImGuiRender()
{
	if( BeginTab("StageEdit") == false ) return false;

	StageSelect();

	ImGui::TextWrapped( u8"�z�u���Ă���I�u�W�F�N�g�̐� : %d", m_ActorList.size() );
	ActorMeshSelectDraw();		ImGui::SameLine();
	ChangeArrangement();
	DelteActorMeshSelectDraw();	ImGui::SameLine();
	DeleteActor();
	UndoRedoDraw();

	SaveButton( "" ); 	ImGui::SameLine();
	LoadButton( "" ); 	ImGui::SameLine();
	MessageRender();
	ImGui::Separator();
	ControllerDraw();
	

	EndTab();
	return true;
}

//------------------------------------.
// ���f���`��.
//------------------------------------.
void CStageEditor::ModelRender()
{
	if( m_pEditPlayer == nullptr ) return;
	m_pEditPlayer->Render();

	int i = 0;
	for( auto& actor : m_ActorList ){
		CDX9StaticMesh*	pStaticMesh = m_ActorMeshList[actor.ActorNo].pStaticMesh;
		const D3DXVECTOR4 color = m_DeleteActorNo == i ? DELETE_ACTOR_COLOR : NORMAL_ACTOR_COLOR;
		pStaticMesh->SetColor( color );
		pStaticMesh->SetTranceform(actor.Tranceform);
		pStaticMesh->Render();
		i++;
	}

	if( m_NowSelectActor.MeshName.empty() == true ) return;
	m_NowSelectActor.pStaticMesh->SetTranceform( m_pEditPlayer->GetPutTranceform() );
	m_NowSelectActor.pStaticMesh->SetRasterizerState( ERS_STATE::Wire );
	m_NowSelectActor.pStaticMesh->Render();
	m_NowSelectActor.pStaticMesh->SetRasterizerState( ERS_STATE::None );
}

//------------------------------------.
// �G�t�F�N�g�`��.
//------------------------------------.
void CStageEditor::EffectRneder()
{
}

//------------------------------------.
// �E�B�W�F�b�g�`��.
//------------------------------------.
void CStageEditor::WidgetRender()
{
}

//------------------------------------.
// �X�e�[�W�I��.
//------------------------------------.
void CStageEditor::StageSelect()
{
	const char* stageNameList[] =
	{
		"Enemy",
		"Boss"
	};

	if( ImGui::BeginCombo( u8"�X�e�[�W�̑I��", stageNameList[m_NowStageNo] ) ){

		for( int i = 0; i < EStageNo_Max; i++ ){
			const bool isSelected = ( i == m_NowStageNo );
			if( ImGui::Selectable( stageNameList[i], isSelected ) ){
				m_NowStageNo = static_cast<EStageNo>(i);
				m_DeleteActorNo = 0;
				m_ActorList = m_pStageLoader->GetActorList( m_NowStageNo );
				m_pUndoRedo->StackClear();
			}

			if( isSelected ) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( u8"�ҏW�������X�e�[�W��I�����邱�Ƃ��ł���" );
}

//------------------------------------.
// �z�u�����̐؂�ւ�.
//------------------------------------.
void CStageEditor::ChangeArrangement()
{
	if( ImGui::Button( u8"�z�u" ) ){
		OffImGuiGamepad();
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( 
		u8"�z�u�������ƁA�v���C���[�𓮂����āA\n"
		u8"�I�u�W�F�N�g��z�u���邱�Ƃ��ł���"
	);
	if( CInput::IsMomentPress(EKeyBind::Edit_BackMenu) ){
		OnImGuiGamepad();
	}
}

//------------------------------------.
// �A�N�^�[�̍폜����.
//------------------------------------.
void CStageEditor::DeleteActor()
{
	if( ImGui::Button( u8"�폜" ) ){
		if( m_ActorList.empty() == true ) return;
		const SActorParam parm = m_ActorList[m_DeleteActorNo];
		m_ActorList.erase( m_ActorList.begin() + m_DeleteActorNo );
		m_pUndoRedo->PushUndo( m_DeleteActorNo, true, parm );
		m_DeleteActorNo = 0;
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( 
		u8"���̃R���{�{�b�N�X����폜�������I�u�W�F�N�g��I������\n"
		u8"�폜�{�^���������ƁA���̃I�u�W�F�N�g���폜���邱�Ƃ��ł���"
	);
}

//------------------------------------.
// �A�N�^�[���b�V���I���̕\��.
//------------------------------------.
void CStageEditor::ActorMeshSelectDraw()
{
	ImGui::Text( SELECT_COMBO_NAME );
	if( ImGui::BeginCombo( "##1", m_NowSelectActor.MeshName.c_str() ) ){

		for( auto& actorMesh : m_ActorMeshList ){
			const bool isSelected = ( actorMesh.MeshName == m_NowSelectActor.MeshName );
			if( ImGui::Selectable( actorMesh.MeshName.c_str(), isSelected ) )
				m_NowSelectActor = actorMesh;

			if( isSelected ) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
}

//------------------------------------.
// �폜����A�N�^�[���b�V���I���̕\��.
//------------------------------------.
void CStageEditor::DelteActorMeshSelectDraw()
{
	ImGui::Text( DELETE_COMBO_NAME );

	EActorNo no = EActorNo_None;
	std::string meshName = "";
	if( m_ActorList.empty() == false ){
		no = m_ActorList[m_DeleteActorNo].ActorNo;
		meshName =  std::to_string(m_DeleteActorNo) + "_" + m_ActorMeshList[no].MeshName;
	}
	if( ImGui::BeginCombo( "##2", meshName.c_str() ) ){
		int i = 0;
		for( auto& actorMesh : m_ActorList ){
			const bool isSelected = ( i == m_DeleteActorNo );
			no = m_ActorList[i].ActorNo;
			meshName =  std::to_string(i) + "_" + m_ActorMeshList[no].MeshName;

			if( ImGui::Selectable( meshName.c_str(), isSelected ) ) m_DeleteActorNo = i;
			if( isSelected ) ImGui::SetItemDefaultFocus();

			i++;
		}

		ImGui::EndCombo();
	}
}

//------------------------------------.
// �A���h�D/���h�D�̕\��.
//------------------------------------.
void CStageEditor::UndoRedoDraw()
{
	int size = 0;
	auto setInvalidButtonColor = [&size]()
	{
		ImGui::PushStyleColor(ImGuiCol_Text,			ImVec4( 0.50f, 0.50f, 0.50f, 0.40f  ) );
		ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4( 0.26f/2.0f, 0.59f/2.0f, 0.98f/2.0f, 0.40f  ) );
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4( 0.26f/2.0f, 0.59f/2.0f, 0.98f/2.0f, 0.40f  ) );
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4( 0.26f/2.0f, 0.59f/2.0f, 0.98f/2.0f, 0.40f  ) );
		size = 4;
	};
	if( m_pUndoRedo->IsUndo() == false ) setInvalidButtonColor();
	if( ImGui::Button( u8"<- ���ɖ߂�" ) ){
		m_pUndoRedo->Undo();
	}
	ImGui::PopStyleColor(size);

	size = 0;
	if( m_pUndoRedo->IsRedo() == false )  setInvalidButtonColor();
	ImGui::SameLine();
	if( ImGui::Button( u8"��蒼�� ->" ) ){
		m_pUndoRedo->Redo();
	}
	ImGui::PopStyleColor(size);
}

//------------------------------------.
// ��������̕\��.
//------------------------------------.
void CStageEditor::ControllerDraw()
{
	if( ImGui::CollapsingHeader( u8"������� : GamePad" ) ){
		ImGui::TextWrapped( u8"  ���X�e�B�b�N ---------  �v���C���[�ړ�" );
		ImGui::TextWrapped( u8"  �E�X�e�B�b�N ---------  �J�����ړ�" );
		ImGui::TextWrapped( u8"  �w�{�^�� -------------  �I�u�W�F�N�g�z�u" );
		ImGui::TextWrapped( u8"  �a�{�^�� -------------  �v���C���[�㏸" );
		ImGui::TextWrapped( u8"  �`�{�^�� -------------  �v���C���[���~" );
		ImGui::TextWrapped( u8"  �q�{�^�� -------------  �I�u�W�F�N�g�E��]" );
		ImGui::TextWrapped( u8"  �k�{�^�� -------------  �I�u�W�F�N�g����]" );
		ImGui::TextWrapped( u8"  �q�g���K�[ -----------  �Y�[���A�E�g" );
		ImGui::TextWrapped( u8"  �k�g���K�[ -----------  �Y�[���C��" );
		ImGui::TextWrapped( u8"  �Z���N�g�{�^�� -------  ���j���[�ɖ߂�" );
	}
	if( ImGui::CollapsingHeader( u8"������� : KeyBorad" ) ){
		ImGui::TextWrapped( u8"  �v�`�r�c�L�[ ---------  �v���C���[�ړ�" );
		ImGui::TextWrapped( u8"  ���L�[ -------------  �J�����ړ�" );
		ImGui::TextWrapped( u8"  �X�y�[�X�L�[ ---------  �I�u�W�F�N�g�z�u" );
		ImGui::TextWrapped( u8"  �q�L�[ ---------------  �v���C���[�㏸" );
		ImGui::TextWrapped( u8"  �e�L�[ ---------------  �v���C���[���~" );
		ImGui::TextWrapped( u8"  �d�L�[ ---------------  �I�u�W�F�N�g�E��]" );
		ImGui::TextWrapped( u8"  �p�L�[ ---------------  �I�u�W�F�N�g����]" );
		ImGui::TextWrapped( u8"  �w�L�[ ---------------  �Y�[���A�E�g" );
		ImGui::TextWrapped( u8"  �y�L�[ ---------------  �Y�[���C��" );
		ImGui::TextWrapped( u8"  �o�b�N�X�y�[�X�L�[ ---  ���j���[�ɖ߂�" );
	}
}

//------------------------------------.
// �A�N�^�[���b�V�����X�g�̏�����.
//------------------------------------.
bool CStageEditor::InitActorMeshList()
{
	const std::vector<std::string> meshNameList = fileManager::TextLoading(ACOTR_MESH_LIST_PATH);

	if( meshNameList.empty() == true ) return false;

	EActorNo actorNo = EActorNo_Begin;
	for( auto& meshName : meshNameList ){
		m_ActorMeshList.emplace_back( actorNo, meshName, CMeshResorce::GetStatic(meshName) );

		if( m_ActorMeshList[actorNo].pStaticMesh == nullptr ){
			m_ActorMeshList.clear();
			return false;
		}
		const int no = static_cast<int>(actorNo)+1;
		actorNo = static_cast<EActorNo>(no);
	}
	m_NowSelectActor = m_ActorMeshList[EActorNo_Player];

	return true;
}

//------------------------------------.
// �p�����[�^�̏�������.
//------------------------------------.
void CStageEditor::ParameterWriting( const char* filePath )
{
	if( m_ActorList.empty() == true ){
		m_MessageText = u8"�I�u�W�F�N�g���z�u����Ă��܂���B";
		return;
	}
	SetParameterWritingMsg( m_pStageLoader->WritingActorLst( m_NowStageNo, m_ActorList ) );
}

//------------------------------------.
// �p�����[�^�̓ǂݍ���.
//------------------------------------.
void CStageEditor::ParameterLoading( const char* filePath )
{
	SetParameterLoadingMsg( true );
	m_DeleteActorNo = 0;
	m_ActorList = m_pStageLoader->GetActorList( m_NowStageNo );
	m_pUndoRedo->StackClear();
}