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
	constexpr char SELECT_COMBO_NAME[]		= u8"配置したいオブジェクトを選択してください";
	constexpr char DELETE_COMBO_NAME[]		= u8"削除したいオブジェクトを選択してください";
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
// 初期化.
//------------------------------------.
bool CStageEditor::Init()
{
	if( m_EditPlayer->Init()	== false ) return false;
	if( InitActorMeshList()		== false ) return false;
	return true;
}

//------------------------------------.
// 更新.
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
// ImGui描画.
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
// モデル描画.
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
// エフェクト描画.
//------------------------------------.
void CStageEditor::EffectRneder()
{
}

//------------------------------------.
// 配置処理の切り替え.
//------------------------------------.
void CStageEditor::ChangeArrangement()
{
	if( ImGui::Button( u8"配置" ) ){
		m_IsArrangementActive = true;
		// ImGuiでコントローラー操作を無効化.
		CImGuiManager::DisableGamepad();
	}
	if( CInput::IsMomentPress(EKeyBind::Edit_BackMenu) ){
		m_IsArrangementActive = false;
		// ImGuiでコントローラー操作を有効化.
		CImGuiManager::EnableGamepad();
	}
}

//------------------------------------.
// アクターの削除処理.
//------------------------------------.
void CStageEditor::DeleteActor()
{
	if( ImGui::Button( u8"削除" ) ){
		if( m_ActorList.empty() == true ) return;
		m_ActorList.erase( m_ActorList.begin() + m_DeleteActorNo );
		m_DeleteActorNo = 0;
	}
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
	SetParameterWritingMsg( fileManager::BinaryVectorWriting( filePath, m_ActorList ) );
}

//------------------------------------.
// パラメータの読み込み.
//------------------------------------.
void CStageEditor::ParameterLoading( const char* filePath )
{
	SetParameterLoadingMsg( fileManager::BinaryVectorReading( filePath, m_ActorList ) );
}