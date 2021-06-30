#include "MovieEdtor.h"
#include "CameraEditor/CameraEditor.h"
#include "..\StageEditor\StageRender\StageRender.h"

CMovieEditor::CMovieEditor()
	: m_pCameraEdit			( std::make_unique<CCameraEditor>() )
	, m_pStageRender		( std::make_unique<CStageRender>() )
	, m_pMovieCameraList	()
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
	return true;
}

//-----------------------------------.
// �X�V.
//-----------------------------------.
void CMovieEditor::Update()
{

	if( m_IsMoviePlaying == true ){
	} else {
		m_pCameraEdit->Update();
	}
}

//-----------------------------------.
// ImGui�`��.
//-----------------------------------.
bool CMovieEditor::ImGuiRender()
{
	if( BeginTab("MovieEdit") == false ) return false;
	
	ImGui::Text( u8"�J�����I�u�W�F�N�g�̐� : %d", m_pMovieCameraList.size() );

	PlayDraw();

	ImGui::Separator();
	ImGui::DragFloat( u8"���o�Đ�����(�b)", &m_MovieEndSecTime, 0.5f, 1.0f, 180.0f );

	if( ImGui::TreeNode( u8"�J�����̒ǉ�" ) ){
		m_pCameraEdit->ImGuiRender(); ImGui::SameLine();
		if( ImGui::Button( u8"�J������ǉ�" ) ){
			m_pMovieCameraList.emplace_back( m_pCameraEdit->GetMovieCameraState() );
		}

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
}

//-----------------------------------.
// �G�t�F�N�g�`��.
//-----------------------------------.
void CMovieEditor::EffectRneder()
{
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
