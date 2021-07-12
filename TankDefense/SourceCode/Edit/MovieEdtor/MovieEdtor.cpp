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
// 初期化.
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
	
	SelectMovieDraw();
	SaveLoadDraw();
	ImGui::Separator();

	ImGui::Text( u8"カメラオブジェクトの数 : %d", m_pCameraEdit->GetMovieCameraState().size() );
	ImGui::Text( u8"ウィジェットオブジェクトの数 : %d", m_pWidgetEditor->GetWidgetStateList().size() );

	PlayDraw();

	ImGui::Separator();
	ImGui::DragFloat( u8"演出再生時間(秒)", &m_MovieEndSecTime, 0.5f, 1.0f, 180.0f );

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
	if( m_IsMoviePlaying == true ){
		
	} else {
		m_pActorEdit->ModelRender();
	}
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
	if( m_IsMoviePlaying == true ){
		m_pMovie->SpriteRender();
	} else {
		m_pWidgetEditor->SpriteRender();
	}
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
		m_pMovie->SetWidgetStateList( m_pWidgetEditor->GetWidgetStateList() );
		m_pMovie->Play();
	}
}

//-----------------------------------.
// ムービーの選択表示.
//-----------------------------------.
void CMovieEditor::SelectMovieDraw()
{
	if( m_MovieNameList.empty() == true ) return;

	ImGui::PushItemWidth( 160.0f );
	std::string name = m_MovieNameList[m_NowSelectMovie];
	if( ImGui::BeginCombo( u8"編集したい演出を選択", name.c_str() ) ){
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
// 保存、読込ボタンの選択.
//-----------------------------------.
void CMovieEditor::SaveLoadDraw()
{
	SaveButton( "" ); 	ImGui::SameLine();
	LoadButton( "" ); 	ImGui::SameLine();
	MessageRender();
}

//-----------------------------------.
// パラメータの書き込み.
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
// パラメータの読み込み.
//-----------------------------------.
void CMovieEditor::ParameterLoading( const char* filePath )
{
	const SMovieData data = m_pMovieDataLoader->GetMovieData( m_NowSelectMovie );

	m_MovieEndSecTime = data.MovieTime;
	m_pCameraEdit->SetMovieCameraState( data.CameraList );
	m_pWidgetEditor->SetWidgetStateList( data.WidgetList );

	SetParameterLoadingMsg( true );
}
