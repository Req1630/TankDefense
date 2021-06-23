#ifndef STAGE_EDITOR_H
#define STAGE_EDITOR_H

#include "..\EditorBase.h"

enum enActorList
{
	EActorNo_None,

	EActorNo_Player = 0,
	EActorNo_Ground,
	EActorNo_Ground_I,
	EActorNo_Ground_L,
	EActorNo_Tree,
	EActorNo_ItemSpeed,

	EActorNo_Max,

	EActorNo_Begin	= EActorNo_Player,
	EActorNo_End	= EActorNo_Max,
} typedef EActorNo;

struct stActorParam
{
	EActorNo	ActorNo;
	STranceform	Tranceform;

	stActorParam() : ActorNo(), Tranceform() 
	{}
	stActorParam( const EActorNo& no, const STranceform& t ) 
		: ActorNo( no ), Tranceform( t )
	{}
} typedef SActorParam;

class CStageEditPlayer;

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
	int									m_DeleteActorNo;		// 削除するアクターの番号.
	bool								m_IsArrangementActive;	// 配置動作か.
};

#endif	// #ifndef STAGE_EDITOR_H.