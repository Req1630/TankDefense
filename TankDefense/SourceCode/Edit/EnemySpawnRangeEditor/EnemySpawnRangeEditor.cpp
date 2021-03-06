#include "EnemySpawnRangeEditor.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Utility\Input\Input.h"
#include "..\EditPlayer\EditPlayer.h"
#include "..\..\Common\Mesh\AuraMesh\AuraMesh.h"
#include "..\StageEditor\StageRender\StageRender.h"
#include "..\..\Object\GameObject\Actor\EnemyBase\EnemySpawnManager\EnemySpawnManager.h"

namespace
{
	constexpr char	STAGE_OBJECT_LIST_PATH[]		= "Data\\Parameter\\Stage\\StageObjectList.bin";
	constexpr char	SELECT_COMBO_NAME[]				= u8"配置したい出現範囲を変更してください";
	constexpr char	DELETE_COMBO_NAME[]				= u8"削除したいオブジェクトを選択してください";
	constexpr float	DELETE_ACTOR_COLOR[]			= { 2.5f, 1.5f, 1.5f, 1.0f };
	constexpr float	NORMAL_ACTOR_COLOR[]			= { 1.0f, 1.0f, 1.0f, 1.0f };
};

CEnemySpawnRangeEditor::CEnemySpawnRangeEditor()
	: m_pEditPlayer			( nullptr )
	, m_pAuraMesh			( std::make_unique<CAuraMesh>() )
	, m_pUndoRedo			( std::make_unique<CUndoRedo<SSpawnBoxRange>>( &m_BoxRangeList ))
	, m_pStageRender		( std::make_unique<CStageRender>() )
	, m_pEnemySpawnManager	( std::make_unique<CEnemySpawnManager>() )
	, m_BoxRangeList		()
	, m_NowSelectActor		()
	, m_NowStageNo			( EStageNo_Enemy )
	, m_DeleteActorNo		( 0 )
{
}

CEnemySpawnRangeEditor::~CEnemySpawnRangeEditor()
{
}

//------------------------------------.
// 初期化.
//------------------------------------.
bool CEnemySpawnRangeEditor::Init()
{
	if( m_pStageRender->Init()			== false )	return false;
	if( m_pEnemySpawnManager->Init()	== false )	return false;
	if( FAILED( m_pAuraMesh->Init() ))				return false;

	m_BoxRangeList = m_pEnemySpawnManager->GetActorList( m_NowStageNo );

	return true;
}

//------------------------------------.
// 更新.
//------------------------------------.
void CEnemySpawnRangeEditor::Update()
{
	if( m_IsImGuiGamepad == true ) return;
	if( m_pEditPlayer == nullptr ) return; 

	m_pEditPlayer->SetIsRotController( false );
	m_pEditPlayer->Update();

	if( m_pEditPlayer->IsPut() == true ){
		const int listSize = static_cast<int>(m_BoxRangeList.size());
		const SSpawnBoxRange actorParam = { m_NowSelectActor.Range, m_pEditPlayer->GetPutTranceform() };
		m_BoxRangeList.insert( m_BoxRangeList.begin()+listSize, actorParam );
		m_pUndoRedo->PushUndo( listSize, false, actorParam );
	}

	m_pAuraMesh->Update();
}

//------------------------------------.
// ImGui描画.
//------------------------------------.
bool CEnemySpawnRangeEditor::ImGuiRender()
{
	if( BeginTab("EnemySpawnEdit") == false ) return false;

	StageSelect();

	ImGui::TextWrapped( u8"配置しているオブジェクトの数 : %d", m_BoxRangeList.size() );
	ChangeRangeDraw();
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
// モデル描画.
//------------------------------------.
void CEnemySpawnRangeEditor::ModelRender()
{
	if( m_pEditPlayer == nullptr ) return;
	m_pEditPlayer->Render();


	m_pStageRender->Render();

	
	for( auto& box : m_BoxRangeList ){
		const D3DXVECTOR3 scale = { box.Range.x, 1.0f, box.Range.y };
		m_pAuraMesh->SetTranceform( box.Tranceform );
		m_pAuraMesh->SetScale( scale );
		m_pAuraMesh->Render();
	}

	const D3DXVECTOR3 scale = { m_NowSelectActor.Range.x, 1.0f, m_NowSelectActor.Range.y };
	m_pAuraMesh->SetTranceform( m_pEditPlayer->GetPutTranceform() );
	m_pAuraMesh->SetScale( scale );
	m_pAuraMesh->Render();


}

//------------------------------------.
// エフェクト描画.
//------------------------------------.
void CEnemySpawnRangeEditor::EffectRneder()
{
}

//------------------------------------.
// ウィジェット描画.
//------------------------------------.
void CEnemySpawnRangeEditor::WidgetRender()
{
}

//------------------------------------.
// ステージ選択.
//------------------------------------.
void CEnemySpawnRangeEditor::StageSelect()
{
	const char* stageNameList[] =
	{
		"Enemy",
		"Boss"
	};

	if( ImGui::BeginCombo( u8"ステージの選択", stageNameList[m_NowStageNo] ) ){

		for( int i = 0; i < EStageNo_Max; i++ ){
			const bool isSelected = ( i == m_NowStageNo );
			if( ImGui::Selectable( stageNameList[i], isSelected ) ){
				m_NowStageNo = static_cast<EStageNo>(i);
				m_DeleteActorNo = 0;
				m_pStageRender->SetStage( m_NowStageNo );
				m_BoxRangeList = m_pEnemySpawnManager->GetActorList( m_NowStageNo );
				m_pUndoRedo->StackClear();
			}

			if( isSelected ) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( u8"編集したいステージを選択することができる" );
}

//------------------------------------.
// 配置処理の切り替え.
//------------------------------------.
void CEnemySpawnRangeEditor::ChangeArrangement()
{
	if( ImGui::Button( u8"配置" ) ){
		OffImGuiGamepad();
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( 
		u8"配置を押すと、プレイヤーを動かして、\n"
		u8"オブジェクトを配置することができる"
	);
	if( CInput::IsMomentPress(EKeyBind::Edit_BackMenu) ){
		OnImGuiGamepad();
	}
}

//------------------------------------.
// アクターの削除処理.
//------------------------------------.
void CEnemySpawnRangeEditor::DeleteActor()
{
	if( ImGui::Button( u8"削除" ) ){
		if( m_BoxRangeList.empty() == true ) return;
		const SSpawnBoxRange parm = m_BoxRangeList[m_DeleteActorNo];
		m_BoxRangeList.erase( m_BoxRangeList.begin() + m_DeleteActorNo );
		m_pUndoRedo->PushUndo( m_DeleteActorNo, true, parm );
		m_DeleteActorNo = 0;
	}
	ImGui::SameLine();
	CImGuiManager::HelpMarker( 
		u8"左のコンボボックスから削除したいオブジェクトを選択して\n"
		u8"削除ボタンを押すと、そのオブジェクトを削除することができる"
	);
}

//------------------------------------.
// 配置する範囲の変更表示.
//------------------------------------.
void CEnemySpawnRangeEditor::ChangeRangeDraw()
{
	ImGui::Text( SELECT_COMBO_NAME );
	ImGui::SameLine();
	CImGuiManager::HelpMarker( u8"設定した範囲に敵がスポーンする" );

	ImGui::PushItemWidth( 160.0 );
	ImGui::DragFloat( u8"Ｘ方向のサイズ", &m_NowSelectActor.Range.x, 1.0f, 1.0f, 100.0f );
	ImGui::DragFloat( u8"Ｚ方向のサイズ", &m_NowSelectActor.Range.y, 1.0f, 1.0f, 100.0f );
	ImGui::PopItemWidth();
}

//------------------------------------.
// 削除するアクターメッシュ選択の表示.
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
// アンドゥ/リドゥの表示.
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
	if( ImGui::Button( u8"<- 元に戻す" ) ){
		m_pUndoRedo->Undo();
	}
	ImGui::PopStyleColor(size);

	size = 0;
	if( m_pUndoRedo->IsRedo() == false )  setInvalidButtonColor();
	ImGui::SameLine();
	if( ImGui::Button( u8"やり直し ->" ) ){
		m_pUndoRedo->Redo();
	}
	ImGui::PopStyleColor(size);
}

//------------------------------------.
// 操作説明の表示.
//------------------------------------.
void CEnemySpawnRangeEditor::ControllerDraw()
{
	if( ImGui::CollapsingHeader( u8"操作説明 : GamePad" ) ){
		ImGui::TextWrapped( u8"  左スティック ---------  プレイヤー移動" );
		ImGui::TextWrapped( u8"  右スティック ---------  カメラ移動" );
		ImGui::TextWrapped( u8"  Ｘボタン -------------  オブジェクト配置" );
		ImGui::TextWrapped( u8"  Ｂボタン -------------  プレイヤー上昇" );
		ImGui::TextWrapped( u8"  Ａボタン -------------  プレイヤー下降" );
		ImGui::TextWrapped( u8"  Ｒトリガー -----------  ズームアウト" );
		ImGui::TextWrapped( u8"  Ｌトリガー -----------  ズームイン" );
		ImGui::TextWrapped( u8"  セレクトボタン -------  メニューに戻る" );
	}
	if( ImGui::CollapsingHeader( u8"操作説明 : KeyBorad" ) ){
		ImGui::TextWrapped( u8"  ＷＡＳＤキー ---------  プレイヤー移動" );
		ImGui::TextWrapped( u8"  矢印キー -------------  カメラ移動" );
		ImGui::TextWrapped( u8"  スペースキー ---------  オブジェクト配置" );
		ImGui::TextWrapped( u8"  Ｒキー ---------------  プレイヤー上昇" );
		ImGui::TextWrapped( u8"  Ｆキー ---------------  プレイヤー下降" );
		ImGui::TextWrapped( u8"  Ｘキー ---------------  ズームアウト" );
		ImGui::TextWrapped( u8"  Ｚキー ---------------  ズームイン" );
		ImGui::TextWrapped( u8"  バックスペースキー ---  メニューに戻る" );
	}
}

//------------------------------------.
// パラメータの書き込み.
//------------------------------------.
void CEnemySpawnRangeEditor::ParameterWriting( const char* filePath )
{
	if( m_BoxRangeList.empty() == true ){
		m_MessageText = u8"オブジェクトが配置されていません。";
		return;
	}
	SetParameterWritingMsg( m_pEnemySpawnManager->WritingActorLst( m_NowStageNo, m_BoxRangeList ) );
}

//------------------------------------.
// パラメータの読み込み.
//------------------------------------.
void CEnemySpawnRangeEditor::ParameterLoading( const char* filePath )
{
	SetParameterLoadingMsg( true );
	m_DeleteActorNo = 0;
	m_BoxRangeList = m_pEnemySpawnManager->GetActorList( m_NowStageNo );
	m_pUndoRedo->StackClear();
}