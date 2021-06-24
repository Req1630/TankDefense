#ifndef STAGE_EDITOR_H
#define STAGE_EDITOR_H

#include "..\EditorBase.h"
#include "StageStruct.h"
#include <queue>
#include <functional>

class CStageEditPlayer;

class CUndoRedo
{
	struct stPram
	{
		int						VecotPos;
		SActorParam				ActorParam;
		std::function<void()>	Func;
	} typedef SPram;

	void Undo()
	{
		m_UndoQueue.back().Func();
	}
	void Redo()
	{
	}

	void PushUndo( const SPram& param )
	{
		m_UndoQueue.push(param);
	}
	void PushRedo( const SPram& param )
	{
		m_RedoQueue.push(param);
	}

private:
	std::queue<SPram> m_UndoQueue;
	std::queue<SPram> m_RedoQueue;
};

/*****************************************
*	ステージエディタ.
**/
class CStageEditor : public CEditorBase
{
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

public:
	CStageEditor();
	virtual ~CStageEditor();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update() override;
	// ImGui描画.
	virtual void ImGuiRender() override;
	// モデル描画.
	virtual void ModelRender() override;
	// エフェクト描画.
	virtual void EffectRneder() override;

private:
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
	std::unique_ptr<CStageEditPlayer>	m_EditPlayer;			// エディタ用プレイヤー.
	std::vector<SActorParam>			m_ActorList;			// 保存用のアクターリスト.
	actor_mesh_list						m_ActorMeshList;		// アクターメッシュリスト.
	SActorMesh							m_NowSelectActor;		// 現在選択しているアクター.
	int									m_ArrangementCount;		// アクターを追加した数.
	int									m_DeleteActorNo;		// 削除するアクターの番号.
	bool								m_IsArrangementActive;	// 配置動作か.
};

#endif	// #ifndef STAGE_EDITOR_H.