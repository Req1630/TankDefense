#ifndef ACTOR_EDITOR_H
#define ACTOR_EDITOR_H

class CActorEditor
{
public:
	CActorEditor();
	~CActorEditor();

	// �X�V����.
	void Update();
	// ImGui�`��.
	bool ImGuiRender();
	// ���f���`��.
	void ModelRender();

};

#endif	// #ifndef ACTOR_EDITOR_H.
