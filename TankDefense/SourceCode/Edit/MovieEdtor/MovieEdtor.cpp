#include "MovieEdtor.h"
#include "..\..\Object\Movie\Movie.h"
#include "CameraEditor/CameraEditor.h"
#include "ActorEditor/ActorEditor.h"
#include "WidgetEditor/WidgetEditor.h"
#include "..\StageEditor\StageRender\StageRender.h"
#include "..\..\Object\Movie\MovieDataLoader\MovieDataLoader.h"

CMovieEditor::CMovieEditor()
	: m_MovieNameList		()
	, m_pMovieDataLoader	( std::make_unique<CMovieDataLoader>() )
	, m_pMovie				( std::make_unique<CMovie>() )
	, m_pCameraEdit			( std::make_unique<CCameraEditor>() )
	, m_pActorEdit			( std::make_unique<CActorEditor>() )
	, m_pWidgetEditor		( std::make_unique<CWidgetEditor>() )
	, m_pStageRender		( std::make_unique<CStageRender>() )
	, m_NowSelectMovie		( EMovieNo::None )
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
	if( m_pStageRender->Init()		== false ) return false;
	if( m_pWidgetEditor->Init()		== false ) return false;
	if( m_pMovieDataLoader->Init()	== false ) return false; 

	m_MovieNameList		= m_pMovieDataLoader->GetMovieNameList();
	m_NowSelectMovie	= EMovieNo::EnemySpawn;

	const SMovieData data = m_pMovieDataLoader->GetMovieData( m_NowSelectMovie );

	m_MovieEndSecTime = data.MovieTime;
	m_pCameraEdit->SetMovieCameraState( data.CameraList );
	m_pWidgetEditor->SetWidgetStateList( data.WidgetList );

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
	
	SelectMovieDraw();
	SaveLoadDraw();
	ImGui::Separator();

	ImGui::Text( u8"�J�����I�u�W�F�N�g�̐� : %d", m_pCameraEdit->GetMovieCameraState().size() );
	ImGui::Text( u8"�E�B�W�F�b�g�I�u�W�F�N�g�̐� : %d", m_pWidgetEditor->GetWidgetStateList().size() );

	PlayDraw();

	ImGui::Separator();
	ImGui::DragFloat( u8"���o�Đ�����(�b)", &m_MovieEndSecTime, 0.5f, 1.0f, 180.0f );

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
	if( m_IsMoviePlaying == true ){
		
	} else {
		m_pActorEdit->ModelRender();
	}
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
	if( m_IsMoviePlaying == true ){
		m_pMovie->SpriteRender();
	} else {
		m_pWidgetEditor->SpriteRender();
	}
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
		m_pMovie->SetWidgetStateList( m_pWidgetEditor->GetWidgetStateList() );
		m_pMovie->Play();
	}
}

//-----------------------------------.
// ���[�r�[�̑I��\��.
//-----------------------------------.
void CMovieEditor::SelectMovieDraw()
{
	if( m_MovieNameList.empty() == true ) return;

	ImGui::PushItemWidth( 160.0f );
	std::string name = m_MovieNameList[m_NowSelectMovie];
	if( ImGui::BeginCombo( u8"�ҏW���������o��I��", name.c_str() ) ){
		int i = 0;
		for( auto& actorMesh : m_MovieNameList ){
			const bool isSelected = ( actorMesh.first == m_NowSelectMovie );
			name =  actorMesh.second;

			if( ImGui::Selectable( name.c_str(), isSelected ) ){ 
				m_NowSelectMovie = actorMesh.first;
				const SMovieData data = m_pMovieDataLoader->GetMovieData( m_NowSelectMovie );

				m_MovieEndSecTime = data.MovieTime;
				m_pCameraEdit->SetMovieCameraState( data.CameraList );
				m_pWidgetEditor->SetWidgetStateList( data.WidgetList );
			}
			if( isSelected ) ImGui::SetItemDefaultFocus();

			i++;
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
}

//-----------------------------------.
// �ۑ��A�Ǎ��{�^���̑I��.
//-----------------------------------.
void CMovieEditor::SaveLoadDraw()
{
	SaveButton( "" ); 	ImGui::SameLine();
	LoadButton( "" ); 	ImGui::SameLine();
	MessageRender();
}

//-----------------------------------.
// �p�����[�^�̏�������.
//-----------------------------------.
void CMovieEditor::ParameterWriting( const char* filePath )
{
	const SMovieData movieData = 
	{ 
		m_MovieEndSecTime,
		m_pCameraEdit->GetMovieCameraState(),
		m_pWidgetEditor->GetWidgetStateList() 
	};

	SetParameterWritingMsg( m_pMovieDataLoader->DataWriting( m_NowSelectMovie, movieData ));
	m_pMovieDataLoader->SetMovieData( m_NowSelectMovie, movieData );
}

//-----------------------------------.
// �p�����[�^�̓ǂݍ���.
//-----------------------------------.
void CMovieEditor::ParameterLoading( const char* filePath )
{
	const SMovieData data = m_pMovieDataLoader->GetMovieData( m_NowSelectMovie );

	m_MovieEndSecTime = data.MovieTime;
	m_pCameraEdit->SetMovieCameraState( data.CameraList );
	m_pWidgetEditor->SetWidgetStateList( data.WidgetList );

	SetParameterLoadingMsg( true );
}
