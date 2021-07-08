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
// 初期化.
//-----------------------------------.
bool CMovieEditor::Init()
{
	if( m_pStageRender->Init() == false ) return false;
	if( m_pWidgetEditor->Init() == false ) return false;

	return true;
}

//-----------------------------------.
// 更新.
//-----------------------------------.
void CMovieEditor::Update()
{

	if( m_IsMoviePlaying == true ){
		m_pMovie->Update();
		m_IsMoviePlaying = m_pMovie->IsPlaying();
		if( m_IsMoviePlaying == false ){
			// ImGuiでコントローラー操作を有効化.
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
// ImGui描画.
//-----------------------------------.
bool CMovieEditor::ImGuiRender()
{
	if( BeginTab("MovieEdit") == false ) return false;
	
	ImGui::Text( u8"カメラオブジェクトの数 : %d", m_pCameraEdit->GetMovieCameraState().size() );

	PlayDraw();

	ImGui::Separator();
	ImGui::DragFloat( u8"演出再生時間(秒)", &m_MovieEndSecTime, 0.5f, 1.0f, 180.0f );

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


	if( ImGui::TreeNode( u8"カメラの編集" ) ){
		m_pCameraEdit->ImGuiRender();

		ImGui::TreePop();
	}

	if( ImGui::TreeNode( u8"キャラクターの編集" ) ){
		m_pActorEdit->ImGuiRender();

		ImGui::TreePop();
	}

	if( ImGui::TreeNode( u8"ウィジェットの編集" ) ){
		m_pWidgetEditor->ImGuiRender();

		ImGui::TreePop();
	}

	EndTab();
	return true;
}

//-----------------------------------.
// モデル描画.
//-----------------------------------.
void CMovieEditor::ModelRender()
{
	m_pStageRender->Render();
	m_pActorEdit->ModelRender();
}

//-----------------------------------.
// エフェクト描画.
//-----------------------------------.
void CMovieEditor::EffectRneder()
{
}

//-----------------------------------.
// ウィジェット描画.
//-----------------------------------.
void CMovieEditor::WidgetRender()
{
	m_pWidgetEditor->SpriteRender();
}

//-----------------------------------.
// エディタ用プレイヤーの設定.
//-----------------------------------.
void CMovieEditor::SetEditPlayer( CEditPlayer* pPlayer )
{
	m_pActorEdit->SetEditPlayer( pPlayer );
}

//-----------------------------------.
// 再生の表示.
//-----------------------------------.
void CMovieEditor::PlayDraw()
{
	if( ImGui::Button( u8"再生") ){
		m_IsMoviePlaying = true;
		// ImGuiでコントローラー操作を無効化.
		CImGuiManager::DisableGamepad();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;

		m_pMovie->SetPlayTime( m_MovieEndSecTime );
		m_pMovie->SetCameraQueue( m_pCameraEdit->GetMovieCameraState() );
		m_pMovie->Play();
	}
}

//-----------------------------------.
// パラメータの書き込み.
//-----------------------------------.
void CMovieEditor::ParameterWriting( const char* filePath )
{
}

//-----------------------------------.
// パラメータの読み込み.
//-----------------------------------.
void CMovieEditor::ParameterLoading( const char* filePath )
{
}
