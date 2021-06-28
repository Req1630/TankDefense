#ifndef STAGE_RENDER_H
#define STAGE_RENDER_H

#include "..\StageEditor.h"

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

private:
	actor_mesh_list				m_ActorMeshList;	// アクターメッシュリスト.
	std::vector<SActorParam>	m_ActorList;		// 保存用のアクターリスト.
};

#endif	// #ifndef STAGE_RENDER_H.