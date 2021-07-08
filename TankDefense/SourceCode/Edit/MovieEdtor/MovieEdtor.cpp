#include "MovieEdtor.h"
#include "..\..\Object\Movie\Movie.h"
#include "CameraEditor/CameraEditor.h"
#include "ActorEditor/ActorEditor.h"
#include "WidgetEditor/WidgetEditor.h"
#include "..\StageEditor\StageRender\StageRender.h"
#include "..\..\Object\Movie\MovieDataLoader\MovieDataLoader.h"

CMovieEditor::CMovieEditor()
	: m_pMovie				( std::make_unique<CMovie>() )
	, m_pCameraEdit			( std::make_unique<CCameraEditor>() )
	, m_pActorEdit			( std::make_unique<CActorEditor>() )
	, m_pWidgetEditor		( std::make_unique<CWidgetEditor>() )
	, m_pStageRender		( std::make_unique<CStageRender>() )
	, m_MovieEndSecTime		( 1.0f )
	, m_IsMoviePlaying		( false )
{
}

CMovieEditor::~CMovieEditor()
{
}

//-----------------------------------.
// ������.
//-----------------------------------.
bool CMovieEditor::Init()
{
	if( m_pStageRender->Init() == false ) return false;
	if( m_pWidgetEditor->Init() == false ) return false;

	return true;
}

//-----------------------------------.
// �X�V.
//-----------------------------------.
void CMovieEditor::Update()
{

	if( m_IsMoviePlaying == true ){
		m_pMovie->Update();
		m_IsMoviePlaying = m_pMovie->IsPlaying();
		if( m_IsMoviePlaying == false ){
			// ImGui�ŃR���g���[���[�����L����.
			CImGuiManager::EnableGamepad();
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.9f;
		}
	} else {
		m_pCameraEdit->Update();
		m_pActorEdit->Update();
		m_pWidgetEditor->Update();
	}
}

//-----------------------------------.
// ImGui�`��.
//-----------------------------------.
bool CMovieEditor::ImGuiRender()
{
	if( BeginTab("MovieEdit") == false ) return false;
	
	ImGui::Text( u8"�J�����I�u�W�F�N�g�̐� : %d", m_pCameraEdit->GetMovieCameraState().size() );

	PlayDraw();

	ImGui::Separator();
	ImGui::DragFloat( u8"���o�Đ�����(�b)", &m_MovieEndSecTime, 0.5f, 1.0f, 180.0f );

#if 1
	static std::vector<std::string> m;
	if( ImGui::Button("TEST") ){
		CMovieDataLoader::DataWriting( 
			m_pCameraEdit->GetMovieCameraState(),
			m_pWidgetEditor->GetWidgetStateList() );
		CMovieDataLoader::DataLoading();
	}
	if( m.empty() == false ){
		for( auto& a : m )
			ImGui::TextWrapped(a.c_str());
	}
#endif


	if( ImGui::TreeNode( u8"�J�����̕ҏW" ) ){
		m_pCameraEdit->ImGuiRender();

		ImGui::TreePop();
	}

	if( ImGui::TreeNode( u8"�L�����N�^�[�̕ҏW" ) ){
		m_pActorEdit->ImGuiRender();

		ImGui::TreePop();
	}

	if( ImGui::TreeNode( u8"�E�B�W�F�b�g�̕ҏW" ) ){
		m_pWidgetEditor->ImGuiRender();

		ImGui::TreePop();
	}

	EndTab();
	return true;
}

//-----------------------------------.
// ���f���`��.
//-----------------------------------.
void CMovieEditor::ModelRender()
{
	m_pStageRender->Render();
	m_pActorEdit->ModelRender();
}

//-----------------------------------.
// �G�t�F�N�g�`��.
//-----------------------------------.
void CMovieEditor::EffectRneder()
{
}

//-----------------------------------.
// �E�B�W�F�b�g�`��.
//-----------------------------------.
void CMovieEditor::WidgetRender()
{
	m_pWidgetEditor->SpriteRender();
}

//-----------------------------------.
// �G�f�B�^�p�v���C���[�̐ݒ�.
//-----------------------------------.
void CMovieEditor::SetEditPlayer( CEditPlayer* pPlayer )
{
	m_pActorEdit->SetEditPlayer( pPlayer );
}

//-----------------------------------.
// �Đ��̕\��.
//-----------------------------------.
void CMovieEditor::PlayDraw()
{
	if( ImGui::Button( u8"�Đ�") ){
		m_IsMoviePlaying = true;
		// ImGui�ŃR���g���[���[����𖳌���.
		CImGuiManager::DisableGamepad();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;

		m_pMovie->SetPlayTime( m_MovieEndSecTime );
		m_pMovie->SetCameraQueue( m_pCameraEdit->GetMovieCameraState() );
		m_pMovie->Play();
	}
}

//-----------------------------------.
// �p�����[�^�̏�������.
//-----------------------------------.
void CMovieEditor::ParameterWriting( const char* filePath )
{
}

//-----------------------------------.
// �p�����[�^�̓ǂݍ���.
//-----------------------------------.
void CMovieEditor::ParameterLoading( const char* filePath )
{
}
