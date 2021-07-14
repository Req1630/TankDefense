/**
* @file SceneList.h.
* @brief �e�V�[�����܂Ƃ߂����w�b�_�[.
* @author ���c���.
*/
#ifndef SCENE_LIST_H
#define SCENE_LIST_H

/****************************************
*	�e�V�[���̃w�b�_�[���܂Ƃ߂�w�b�_�[.
*
*/

#include "..\SceneManager\SceneManager.h"
#include "Title/Title.h"
#include "Game/Game.h"
#include "Edit/Edit.h"

// �V�[���̎��.
enum class enSceneNo : unsigned char
{
	None,

	Title,		// �^�C�g��.
	GameMain,	// �Q�[�����C��.

	Edit,
	EnemyBattle,// �G���탁�C��.
	BossBattle,	// �{�X�탁�C��.
	GameClear,	// �Q�[���N���A.
	GameOver,	// �Q�[���I�[�o�[.
	Ending,		// �G���f�B���O.

	Max,

	ResultCheck,		// �Q�[���N���A���Q�[���I�[�o�[�̊m�F.
	Start = Title,		// ���߂̃V�[��.
};

#endif	// #ifndef SCENE_LIST_H.