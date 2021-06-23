#include "StageEditor.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Utility\Input\Input.h"
#include "StageEditPlayer/StageEditPlayer.h"

namespace
{
	constexpr char ACOTR_MESH_LIST_PATH[]	= "Data\\Parameter\\StageObjectList.txt";
	constexpr char STAGE_OBJECT_LIST_PATH[]	= "Data\\Parameter\\StageObjectList.bin";
	constexpr char SELECT_COMBO_NAME[]		= u8"�z�u�������I�u�W�F�N�g��I�����Ă�������";
	constexpr char DELETE_COMBO_NAME[]		= u8"�폜�������I�u�W�F�N�g��I�����Ă�������";
};

CStageEditor::CStageEditor()
	: m_EditPlayer			( std::make_unique<CStageEditPlayer>() )
	, m_ActorList			()
	, m_ActorMeshList		()
	, m_NowSelectActor		()
	, m_DeleteActorNo		()
	, m_IsArrangementActive	( false )
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
	if( m_EditPlayer->Init()	== false ) return false;
	if( InitActorMeshList()		== false ) return false;
	return true;
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CStageEditor::Update()
{
	if( m_IsArrangementActive == false ) return;
	m_EditPlayer->Update();

	if( m_EditPlayer->IsPut() == true ){
		m_ActorList.emplace_back( m_NowSelectActor.ActorNo, m_EditPlayer->GetPutTranceform() );
	}
}

//------------------------------------.
// ImGui�`��.
//------------------------------------.
void CStageEditor::ImGuiRender()
{
	if( BeginTab("StageEdit") == false ) return;

	ActorMeshSelectDraw();		ImGui::SameLine();
	ChangeArrangement();
	DelteActorMeshSelectDraw();	ImGui::SameLine();
	DeleteActor();

	SaveButton( STAGE_OBJECT_LIST_PATH ); 	ImGui::SameLine();
	LoadButton( STAGE_OBJECT_LIST_PATH ); 	ImGui::SameLine();
	MessageRender();

	ControllerDraw();
	

	EndTab();
}

//------------------------------------.
// ���f���`��.
//------------------------------------.
void CStageEditor::ModelRender()
{
	m_EditPlayer->Render();

	for( auto& actor : m_ActorList ){
		m_ActorMeshList[actor.ActorNo].pStaticMesh->SetTranceform(actor.Tranceform);
		m_ActorMeshList[actor.ActorNo].pStaticMesh->Render();
	}

	if( m_NowSelectActor.MeshName.empty() == true ) return;
	m_NowSelectActor.pStaticMesh->SetTranceform( m_EditPlayer->GetPutTranceform() );
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
// �z�u�����̐؂�ւ�.
//------------------------------------.
void CStageEditor::ChangeArrangement()
{
	if( ImGui::Button( u8"�z�u" ) ){
		m_IsArrangementActive = true;
		// ImGui�ŃR���g���[���[����𖳌���.
		CImGuiManager::DisableGamepad();
	}
	if( CInput::IsMomentPress(EKeyBind::Edit_BackMenu) ){
		m_IsArrangementActive = false;
		// ImGui�ŃR���g���[���[�����L����.
		CImGuiManager::EnableGamepad();
	}
}

//------------------------------------.
// �A�N�^�[�̍폜����.
//------------------------------------.
void CStageEditor::DeleteActor()
{
	if( ImGui::Button( u8"�폜" ) ){
		if( m_ActorList.empty() == true ) return;
		m_ActorList.erase( m_ActorList.begin() + m_DeleteActorNo );
		m_DeleteActorNo = 0;
	}
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
	SetParameterWritingMsg( fileManager::BinaryVectorWriting( filePath, m_ActorList ) );
}

//------------------------------------.
// �p�����[�^�̓ǂݍ���.
//------------------------------------.
void CStageEditor::ParameterLoading( const char* filePath )
{
	SetParameterLoadingMsg( fileManager::BinaryVectorReading( filePath, m_ActorList ) );
}