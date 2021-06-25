#ifndef ENEMY_SPAEN_RANGE_EDITOR_H
#define ENEMY_SPAEN_RANGE_EDITOR_H

#include "..\EditorBase.h"
#include "..\UndoRedo\UndoRedo.h"
#include "..\StageEditor\StageEditor.h"

class CStageEditPlayer;	// ステージエディタ用のプレイヤー.
class CAuraMesh;		// オーラメッシュクラス.

/***************************************************
* 敵スポーン範囲エディタ.
**/
class CEnemySpawnRangeEditor : public CEditorBase
{
	struct stBoxRange
	{
		D3DXVECTOR2 Range;		// 範囲,
		STranceform	Tranceform;
	} typedef SBoxRange;
public:
	CEnemySpawnRangeEditor();
	virtual ~CEnemySpawnRangeEditor();

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

private:
	// 配置処理の切り替え.
	void ChangeArrangement();
	// アクターの削除処理.
	void DeleteActor();
	// 配置する範囲の変更表示.
	void ChangeRangeDraw();
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
	std::unique_ptr<CStageEditPlayer>		m_EditPlayer;			// エディタ用プレイヤー.
	std::unique_ptr<CAuraMesh>				m_pAuraMesh;
	std::unique_ptr<CUndoRedo<SBoxRange>>	m_pUndoRedo;			// 元に戻す操作クラス.
	std::vector<SBoxRange>					m_BoxRangeList;
	std::vector<SActorParam>				m_ActorList;			// 保存用のアクターリスト.
	actor_mesh_list							m_ActorMeshList;		// アクターメッシュリスト.
	SBoxRange								m_NowSelectActor;		// 現在選択しているアクター.
	int										m_DeleteActorNo;		// 削除するアクターの番号.
	bool									m_IsArrangementActive;	// 配置動作か.
};

#endif	// #ifndef ENEMY_SPAEN_RANGE_EDITOR_H.