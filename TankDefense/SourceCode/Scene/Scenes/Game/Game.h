#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

/**********************************
*	�Q�[���V�[���N���X.
*/
class CGame : public CSceneBase
{
private:

public:
	CGame( CSceneManager* pSceneManager );
	virtual ~CGame();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// ���f���`��֐�.
	virtual void ModelRender() override;
	// �摜�`��֐�.
	virtual void SpriteRender() override;
	// �X�v���C�g3D�`��֐�.
	virtual void Sprite3DRender() override;

private:

private:
};

#endif	// #ifndef GAME_H.