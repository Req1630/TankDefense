#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

class CActorEditor
{
public:
	CActorEditor();
	~CActorEditor();

	// 更新処理.
	void Update();
	// ImGui描画.
	bool ImGuiRender();
	// モデル描画.
	void ModelRender();

};

#endif	// #ifndef ACTOR_EDITOR_H.
