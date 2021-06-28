#include "EnemySpawnRangeEditor.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Utility\Input\Input.h"
#include "..\StageEditor\StageEditPlayer\StageEditPlayer.h"
#include "..\..\Common\Mesh\AuraMesh\AuraMesh.h"
#include "..\StageEditor\StageRender\StageRender.h"

namespace
{
	constexpr char	ACOTR_MESH_LIST_PATH[]			= "Data\\Parameter\\StageObjectList.txt";
	constexpr char	STAGE_OBJECT_LIST_PATH[]		= "Data\\Parameter\\StageObjectList.bin";
	constexpr char	ENEMY_SPAWN_RANGE_LIST_PATH[]	= "Data\\Parameter\\EnemySpawnRangeList.bin";
	constexpr char	SELECT_COMBO_NAME[]				= u8"�z�u�������o���͈͂�ύX���Ă�������";
	constexpr char	DELETE_COMBO_NAME[]				= u8"�폜�������I�u�W�F�N�g��I�����Ă�������";
	constexpr float	DELETE_ACTOR_COLOR[]			= { 2.5f, 1.5f, 1.5f, 1.0f };
	constexpr float	NORMAL_ACTOR_COLOR[]			= { 1.0f, 1.0f, 1.0f, 1.0f };
};

CEnemySpawnRangeEditor::CEnemySpawnRangeEditor()
	: m_EditPlayer			( std::make_unique<CStageEditPlayer>() )
	, m_pAuraMesh			( std::make_unique<CAuraMesh>() )
	, m_pUndoRedo			( std::make_unique<CUndoRedo<SBoxRange>>( &m_BoxRangeList ))
	, m_pStageRender		( std::make_unique<CStageRender>() )
	, m_BoxRangeList		()
	, m_NowSelectActor		()
	, m_DeleteActorNo		( 0 )
	, m_IsArrangementActive	( false )
{
}

CEnemySpawnRangeEditor::~CEnemySpawnRangeEditor()
{
}

//------------------------------------.
// ������.
//------------------------------------.
bool CEnemySpawnRangeEditor::Init()
{
	if( m_EditPlayer->Init()	== false )	return false;
	if( m_pStageRender->Init()	== false )	return false;
	if( FAILED( m_pAuraMesh->Init() ))		return false;
	return true;
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CEnemySpawnRangeEditor::Update()
{
	if( m_IsArrangementActive == false ) return;
	m_EditPlayer->Update();

	if( m_EditPlayer->IsPut() == true ){
		const int listSize = static_cast<int>(m_BoxRangeList.size());
		const SBoxRange actorParam = { m_NowSelectActor.Range, m_EditPlayer->GetPutTranceform() };
		m_BoxRangeList.insert( m_BoxRangeList.begin()+listSize, actorParam );
		m_pUndoRedo->PushUndo( listSize, false, actorParam );
	}

	m_pAuraMesh->Update();
}

//------------------------------------.
// ImGui�`��.
//------------------------------------.
bool CEnemySpawnRangeEditor::ImGuiRender()
{
	if( BeginTab("EnemySpawnEdit") == false ) return false;

	ImGui::TextWrapped( u8"�z�u���Ă���I�u�W�F�N�g�̐� : %d", m_BoxRangeList.size() );
	ChangeRangeDraw();
	ChangeArrangement();
	DelteActorMeshSelectDraw();	ImGui::SameLine();
	DeleteActor();
	UndoRedoDraw();

	SaveButton( ENEMY_SPAWN_RANGE_LIST_PATH ); 	ImGui::SameLine();
	LoadButton( ENEMY_SPAWN_RANGE_LIST_PATH ); 	ImGui::SameLine();
	MessageRender();
	ImGui::Separator();
	ControllerDraw();
	

	EndTab();
	return true;
}

//------------------------------------.
// ���f���`��.
//------------------------------------.
void CEnemySpawnRangeEditor::ModelRender()
{
	m_EditPlayer->Render();


	m_pStageRender->Render();

	
	for( auto& box : m_BoxRangeList ){
		const D3DXVECTOR3 scale = { box.Range.x, 1.0f, box.Range.y };
		m_pAuraMesh->SetTranceform( box.Tranceform );
		m_pAuraMesh->SetScale( scale );
		m_pAuraMesh->Render();
	}

	const D3DXVECTOR3 scale = { m_NowSelectActor.Range.x, 1.0f, m_NowSelectActor.Range.y };
	m_pAuraMesh->SetTranceform( m_EditPlayer->GetPutTranceform() );
	m_pAuraMesh->SetScale( scale );
	m_pAuraMesh->Render();


}

//------------------------------------.
// �G�t�F�N�g�`��.
//------------------------------------.
void CEnemySpawnRangeEditor::EffectRneder()
{
}

//------------------------------------.
// �z�u�����̐؂�ւ�.
//------------------------------------.
void CEnemySpawnRangeEditor::ChangeArrangement()
{
	if( ImGui::Button( u8"�z�u" ) ){
		m_IsArrangementActive = true;
		// ImGui�ŃR���g���[���[����𖳌���.
		CImGuiManager::DisableGamepad();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;
	}
	if( CInput::IsMomentPress(EKeyBind::Edit_BackMenu) ){
		m_IsArrangementActive = false;
		// ImGui�ŃR���g���[���[�����L����.
		CImGuiManager::EnableGamepad();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.9f;
	}
}

//------------------------------------.
// �A�N�^�[�̍폜����.
//------------------------------------.
void CEnemySpawnRangeEditor::DeleteActor()
{
	if( ImGui::Button( u8"�폜" ) ){
		if( m_BoxRangeList.empty() == true ) return;
		const SBoxRange parm = m_BoxRangeList[m_DeleteActorNo];
		m_BoxRangeList.erase( m_BoxRangeList.begin() + m_DeleteActorNo );
		m_pUndoRedo->PushUndo( m_DeleteActorNo, true, parm );
		m_DeleteActorNo = 0;
	}
}

//------------------------------------.
// �z�u����͈͂̕ύX�\��.
//------------------------------------.
void CEnemySpawnRangeEditor::ChangeRangeDraw()
{
	ImGui::Text( SELECT_COMBO_NAME );
	ImGui::DragFloat( u8"�w�����̃T�C�Y", &m_NowSelectActor.Range.x, 1.0f, 1.0f, 100.0f );
	ImGui::DragFloat( u8"�y�����̃T�C�Y", &m_NowSelectActor.Range.y, 1.0f, 1.0f, 100.0f );
}

//------------------------------------.
// �폜����A�N�^�[���b�V���I���̕\��.
//------------------------------------.
void CEnemySpawnRangeEditor::DelteActorMeshSelectDraw()
{
	ImGui::Text( DELETE_COMBO_NAME );

	int no = 0;
	std::string name = "range : " + std::to_string(m_DeleteActorNo);
	if( ImGui::BeginCombo( "##2", name.c_str() ) ){
		int i = 0;
		for( auto& actorMesh : m_BoxRangeList ){
			const bool isSelected = ( i == m_DeleteActorNo );

			name = "range : " + std::to_string(i);
			if( ImGui::Selectable( name.c_str(), isSelected ) ) m_DeleteActorNo = i;
			if( isSelected ) ImGui::SetItemDefaultFocus();

			i++;
		}

		ImGui::EndCombo();
	}
}

//------------------------------------.
// �A���h�D/���h�D�̕\��.
//------------------------------------.
void CEnemySpawnRangeEditor::UndoRedoDraw()
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
void CEnemySpawnRangeEditor::ControllerDraw()
{
	if( ImGui::CollapsingHeader( u8"������� : GamePad" ) ){
		ImGui::TextWrapped( u8"  ���X�e�B�b�N ---------  �v���C���[�ړ�" );
		ImGui::TextWrapped( u8"  �E�X�e�B�b�N ---------  �J�����ړ�" );
		ImGui::TextWrapped( u8"  �w�{�^�� -------------  �I�u�W�F�N�g�z�u" );
		ImGui::TextWrapped( u8"  �a�{�^�� -------------  �v���C���[�㏸" );
		ImGui::TextWrapped( u8"  �`�{�^�� -------------  �v���C���[���~" );
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
		ImGui::TextWrapped( u8"  �w�L�[ ---------------  �Y�[���A�E�g" );
		ImGui::TextWrapped( u8"  �y�L�[ ---------------  �Y�[���C��" );
		ImGui::TextWrapped( u8"  �o�b�N�X�y�[�X�L�[ ---  ���j���[�ɖ߂�" );
	}
}

//------------------------------------.
// �p�����[�^�̏�������.
//------------------------------------.
void CEnemySpawnRangeEditor::ParameterWriting( const char* filePath )
{
	if( m_BoxRangeList.empty() == true ){
		m_MessageText = u8"�I�u�W�F�N�g���z�u����Ă��܂���B";
		return;
	}
	SetParameterWritingMsg( fileManager::BinaryVectorWriting( filePath, m_BoxRangeList ) );
}

//------------------------------------.
// �p�����[�^�̓ǂݍ���.
//------------------------------------.
void CEnemySpawnRangeEditor::ParameterLoading( const char* filePath )
{
	SetParameterLoadingMsg( fileManager::BinaryVectorReading( filePath, m_BoxRangeList ) );
	m_DeleteActorNo = 0;
	m_pUndoRedo->StackClear();
}