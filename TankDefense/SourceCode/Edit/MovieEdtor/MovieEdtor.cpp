#include "MovieEdtor.h"
#include "CameraEditor/CameraEditor.h"
#include "..\StageEditor\StageRender\StageRender.h"

CMovieEditor::CMovieEditor()
	: m_pCameraEdit		( std::make_unique<CCameraEditor>() )
	, m_pStageRender	( std::make_unique<CStageRender>() )
	, m_MovieEndSecTime	( 1.0f )
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
	m_pCameraEdit->Update();
}

//-----------------------------------.
// ImGui�`��.
//-----------------------------------.
bool CMovieEditor::ImGuiRender()
{
	if( BeginTab("MovieEdit") == false ) return false;
	
	ImGui::DragFloat( u8"���o�Đ�����(�b)", &m_MovieEndSecTime, 0.5f, 1.0f, 180.0f );

	if( ImGui::TreeNode( u8"�J�����̒ǉ�" ) ){
		m_pCameraEdit->ImGuiRender();
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
