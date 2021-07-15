#include "StageEditor.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Edit\EditPlayer\EditPlayer.h"
#include "..\..\Object\GameObject\Actor\Stage\StageLoader\StageLoader.h"

namespace
{
	constexpr char	ACOTR_MESH_LIST_PATH[]		= "Data\\Parameter\\Stage\\StageObjectList.txt";
	constexpr char	SELECT_COMBO_NAME[]			= u8"配置したいオブジェクトを選択してください";
	constexpr char	DELETE_COMBO_NAME[]			= u8"削除したいオブジェクトを選択してください";
	constexpr float	DELETE_ACTOR_COLOR[]		= { 2.5f, 1.5f, 1.5f, 1.0f };
	constexpr float	NORMAL_ACTOR_COLOR[]		= { 1.0f, 1.0f, 1.0f, 1.0f };
};

CStageEditor::CStageEditor()
	: m_pEditPlayer			( nullptr )
	, m_pStageLoader		( std::make_unique<CStageLoader>())
	, m_pUndoRedo			( std::make_unique<CUndoRedo<SActorParam>>( &m_ActorList ))
	, m_ActorList			()
	, m_ActorMeshList		()
	, m_NowSelectActor		()
	, m_NowStageNo			( EStageNo_Enemy )
	, m_DeleteActorNo		()
{
}

CStageEditor::~CStageEditor()
{
}

//------------------------------------.
// 初期化.
//------------------------------------.
bool CStageEditor::Init()
{
	if( InitActorMeshList()		== false ) return false;
	if( m_pStageLoader->Init()	== false ) return false;

	m_ActorList = m_pStageLoader->GetActorList( m_NowStageNo );

	return true;
}

//------------------------------------.
// 更新.
//------------------------------------.
void CStageEditor::Update()
{
	if( m_IsImGuiGamepad == true ) return;
	if( m_pEditPlayer == nullptr ) return;

	m_pEditPlayer->SetIsRotController( true );
	m_pEditPlayer->Update();

	if( m_pEditPlayer->IsPut() == true ){
		const int listSize = static_cast<int>(m_ActorList.size());
		const SActorParam actorParam = { m_NowSelectActor.ActorNo, m_pEditPlayer->GetPutTranceform() };
		m_ActorList.insert( m_ActorList.begin()+listSize, actorParam );
		m_pUndoRedo->PushUndo( listSize, false, actorParam );
	}
}

//------------------------------------.
// ImGui描画.
//------------------------------------.
bool CStageEditor::ImGuiRender()
{
	if( BeginTab("StageEdit") == false ) return false;

	StageSelect();

	ImGui::TextWrapped( u8"配置しているオブジェクトの数 : %d", m_ActorList.size() );
	ActorMeshSelectDraw();		ImGui::SameLine();
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
void CStageEditor::ModelRender()
{
	if( m_pEditPlayer == nullptr ) return;
	m_pEditPlayer->Render();

	int i = 0;
	for( auto& actor : m_ActorList ){
		CDX9StaticMesh*	pStaticMesh = m_ActorMeshList[actor.ActorNo].pStaticMesh;
		const D3DXVECTOR4 color = m_DeleteActorNo == i ? DELETE_ACTOR_COLOR : NORMAL_ACTOR_COLOR;
		pStaticMesh->SetColor( color );
		pStaticMesh->SetTranceform(actor.Tranceform);
		pStaticMesh->Render();
		i++;
	}

	if( m_NowSelectActor.MeshName.empty() == true ) return;
	m_NowSelectActor.pStaticMesh->SetTranceform( m_pEditPlayer->GetPutTranceform() );
	m_NowSelectActor.pStaticMesh->SetRasterizerState( ERS_STATE::Wire );
	m_NowSelectActor.pStaticMesh->Render();
	m_NowSelectActor.pStaticMesh->SetRasterizerState( ERS_STATE::None );
}

//------------------------------------.
// エフェクト描画.
//------------------------------------.
void CStageEditor::EffectRneder()
{
}

//------------------------------------.
// ウィジェット描画.
//------------------------------------.
void CStageEditor::WidgetRender()
{
}

//------------------------------------.
// ステージ選択.
//------------------------------------.
void CStageEditor::StageSelect()
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
				m_ActorList = m_pStageLoader->GetActorList( m_NowStageNo );
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
void CStageEditor::ChangeArrangement()
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
void CStageEditor::DeleteActor()
{
	if( ImGui::Button( u8"削除" ) ){
		if( m_ActorList.empty() == true ) return;
		const SActorParam parm = m_ActorList[m_DeleteActorNo];
		m_ActorList.erase( m_ActorList.begin() + m_DeleteActorNo );
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
// アクターメッシュ選択の表示.
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
// 削除するアクターメッシュ選択の表示.
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
// アンドゥ/リドゥの表示.
//------------------------------------.
void CStageEditor::UndoRedoDraw()
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
void CStageEditor::ControllerDraw()
{
	if( ImGui::CollapsingHeader( u8"操作説明 : GamePad" ) ){
		ImGui::TextWrapped( u8"  左スティック ---------  プレイヤー移動" );
		ImGui::TextWrapped( u8"  右スティック ---------  カメラ移動" );
		ImGui::TextWrapped( u8"  Ｘボタン -------------  オブジェクト配置" );
		ImGui::TextWrapped( u8"  Ｂボタン -------------  プレイヤー上昇" );
		ImGui::TextWrapped( u8"  Ａボタン -------------  プレイヤー下降" );
		ImGui::TextWrapped( u8"  Ｒボタン -------------  オブジェクト右回転" );
		ImGui::TextWrapped( u8"  Ｌボタン -------------  オブジェクト左回転" );
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
		ImGui::TextWrapped( u8"  Ｅキー ---------------  オブジェクト右回転" );
		ImGui::TextWrapped( u8"  Ｑキー ---------------  オブジェクト左回転" );
		ImGui::TextWrapped( u8"  Ｘキー ---------------  ズームアウト" );
		ImGui::TextWrapped( u8"  Ｚキー ---------------  ズームイン" );
		ImGui::TextWrapped( u8"  バックスペースキー ---  メニューに戻る" );
	}
}

//------------------------------------.
// アクターメッシュリストの初期化.
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
// パラメータの書き込み.
//------------------------------------.
void CStageEditor::ParameterWriting( const char* filePath )
{
	if( m_ActorList.empty() == true ){
		m_MessageText = u8"オブジェクトが配置されていません。";
		return;
	}
	SetParameterWritingMsg( m_pStageLoader->WritingActorLst( m_NowStageNo, m_ActorList ) );
}

//------------------------------------.
// パラメータの読み込み.
//------------------------------------.
void CStageEditor::ParameterLoading( const char* filePath )
{
	SetParameterLoadingMsg( true );
	m_DeleteActorNo = 0;
	m_ActorList = m_pStageLoader->GetActorList( m_NowStageNo );
	m_pUndoRedo->StackClear();
}