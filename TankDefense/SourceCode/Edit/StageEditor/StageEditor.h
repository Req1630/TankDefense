#ifndef STAGE_EDITOR_H
#define STAGE_EDITOR_H

#include "..\EditorBase.h"
#include "..\..\Object\GameObject\Actor\Stage\StageStruct.h"
#include "..\UndoRedo\UndoRedo.h"

class CStageLoader;

struct stActorMesh
{
	EActorNo		ActorNo;
	std::string		MeshName;
	CDX9StaticMesh*	pStaticMesh;
	stActorMesh() : ActorNo ( EActorNo_None ), MeshName ("") , pStaticMesh ( nullptr )
	{}
	stActorMesh( const EActorNo& no, const std::string& name, CDX9StaticMesh* pMesh) 
		: ActorNo ( no ), MeshName ( name ) , pStaticMesh ( pMesh )
	{}

} typedef SActorMesh;
using actor_mesh_list = std::vector<SActorMesh>;

/*****************************************
*	ステージエディタ.
**/
class CStageEditor : public CEditorBase
{
public:
	CStageEditor();
	virtual ~CStageEditor();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update() override;
	// ImGui描画.
	virtual bool ImGuiRender() override;
	// モデル描画.
	virtual void ModelRender() override;
	// エフェクト描画.
	virtual void EffectRneder() override;
	// ウィジェット描画.
	virtual void WidgetRender() override;

	// エディタ用プレイヤーの設定.
	virtual void SetEditPlayer( CEditPlayer* pPlayer ) override { m_pEditPlayer = pPlayer; }

private:
	// ステージ選択.
	void StageSelect();
	// 配置処理の切り替え.
	void ChangeArrangement();
	// アクターの削除処理.
	void DeleteActor();
	// アクターメッシュ選択の表示.
	void ActorMeshSelectDraw();
	// 削除するアクターメッシュ選択の表示.
	void DelteActorMeshSelectDraw();
	// アンドゥ/リドゥの表示.
	void UndoRedoDraw();
	// 操作説明の表示.
	void ControllerDraw();
	// アクターメッシュリストの初期化.
	bool InitActorMeshList();

	// パラメータの書き込み.
	virtual void ParameterWriting( const char* filePath ) override;
	// パラメータの読み込み.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	CEditPlayer*							m_pEditPlayer;			// エディタ用プレイヤー.
	std::unique_ptr<CStageLoader>			m_pStageLoader;			// ステージ読み込みクラス.
	std::unique_ptr<CUndoRedo<SActorParam>>	m_pUndoRedo;			// 元に戻す操作クラス.
	std::vector<SActorParam>				m_ActorList;			// 保存用のアクターリスト.
	actor_mesh_list							m_ActorMeshList;		// アクターメッシュリスト.
	SActorMesh								m_NowSelectActor;		// 現在選択しているアクター.
	EStageNo								m_NowStageNo;			// 現在のステージ番号.
	int										m_DeleteActorNo;		// 削除するアクターの番号.
};

#endif	// #ifndef STAGE_EDITOR_H.