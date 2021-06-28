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
// 初期化.
//-----------------------------------.
bool CMovieEditor::Init()
{
	if( m_pStageRender->Init() == false ) return false;
	return true;
}

//-----------------------------------.
// 更新.
//-----------------------------------.
void CMovieEditor::Update()
{
	m_pCameraEdit->Update();
}

//-----------------------------------.
// ImGui描画.
//-----------------------------------.
bool CMovieEditor::ImGuiRender()
{
	if( BeginTab("MovieEdit") == false ) return false;
	
	ImGui::DragFloat( u8"演出再生時間(秒)", &m_MovieEndSecTime, 0.5f, 1.0f, 180.0f );

	if( ImGui::TreeNode( u8"カメラの追加" ) ){
		m_pCameraEdit->ImGuiRender();
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
}

//-----------------------------------.
// エフェクト描画.
//-----------------------------------.
void CMovieEditor::EffectRneder()
{
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
