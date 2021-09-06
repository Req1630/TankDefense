/**
* @file GameObjctList.h.
* @brief �Q�[�����ŏo�Ă���I�u�W�F�N�g���X�g���܂Ƃ߂��w�b�_�[.
* @author ���c���.
*/
#ifndef GAME_OBJECT_LIST_H
#define GAME_OBJECT_LIST_H

// �I�u�W�F�N�g�^�O���X�g.
enum class enObjectTagList : unsigned int
{
	None,

	Player,			// �v���C���[.
	Enemy,			// �G���G.
	SpecialEnemy_1,	// ����ȓG1.
	SpecialEnemy_2, // ����ȓG2.
	Boss,			// �{�X.
	PlayerBullet,	// �v���C���[�̒e.
	EnemyBullet,	// �G�̒e.
	BossBullet,		// �{�X�̒e.
	DropItem,		// �h���b�v�A�C�e��.
	DefaultWepon,	// ��C.
	MachineGun,		// �}�V���K��.
	Missile,		// �~�T�C��.
	Beam,			// �r�[���C.

	Max,

} typedef EObjectTag;

#endif	// #ifndef GAME_OBJECT_LIST_H.
