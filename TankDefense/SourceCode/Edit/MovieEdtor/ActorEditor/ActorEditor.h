#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

class CActorEditor
{
public:
	CActorEditor();
	~CActorEditor();

	// XVˆ—.
	void Update();
	// ImGui•`‰æ.
	bool ImGuiRender();
	// ƒ‚ƒfƒ‹•`‰æ.
	void ModelRender();

};

#endif	// #ifndef ACTOR_EDITOR_H.
