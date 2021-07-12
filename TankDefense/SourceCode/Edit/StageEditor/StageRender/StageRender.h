#ifndef STAGE_RENDER_H
#define STAGE_RENDER_H

#include "..\StageEditor.h"
#include "..\..\..\Object\GameObject\Actor\Stage\StageLoader\StageLoader.h"

/************************************
*	ステージの描画クラス.
**/
class CStageRender
{
public:
	CStageRender();
	~CStageRender();

	// 初期化.
	bool Init();
	// 描画.
	void Render();

	// ステージの設定.
	void SetStage( const EStageNo& no );

private:
	std::unique_ptr<CStageLoader>	m_pStageLoader;		// ステージの読み込みクラス.
	actor_mesh_list					m_ActorMeshList;	// アクターメッシュリスト.
	std::vector<SActorParam>		m_ActorList;		// 保存用のアクターリスト.
};

#endif	// #ifndef STAGE_RENDER_H.