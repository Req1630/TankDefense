#ifndef INPUT_ENUMS_H
#define INPUT_ENUMS_H

// �L�[�o�C���h.
// �Q�[�����ɕύX���Ďg�p����.
enum class enKeyBind : unsigned char
{
	None,

	Decision,		// ����.
	Cancel,			// �L�����Z��.
	Skip,			// �X�L�b�v.

	Attack,			// �U��.
	SpecialAbility,	// ����\��.
	Avoidance,		// ���.
	STGAttack,		// �V���[�e�B���O���̍U��.

	Up,			// ��.
	Down,		// ��.
	Right,		// �E.
	Left,		// ��.

	Edit_ActorPut,	// �G�f�B�^�p�A�N�^�[�ǉ�.
	Edit_BackMenu,	// �G�f�B�^�p���j���[�ɖ߂�.
	Edit_Up,		// �G�f�B�^�p�㏸.
	Edit_Down,		// �G�f�B�^�p���~.
	Edit_RightRot,	// �G�f�B�^�p�E��].
	Edit_LeftRot,	// �G�f�B�^�p����].

	UpAxis,		// �㎲.
	RightAxis,	// �E��.

	Start,		// Xbox : Start,	DS3 : Start.
	Back,		// Xbox : Back,		DS3 : Select.

} typedef EKeyBind;

enum class enAxisBind : unsigned char
{
	None,

	L_Forward,	// ���X�e�B�b�N�̑O��.
	L_Right,	// ���X�e�B�b�N�̍��E.

	R_Forward,	// �E�X�e�B�b�N�̑O��.
	R_Right,	// �E�X�e�B�b�N�̍��E.
} typedef EAxisBind;

// ���͏��.
enum enInputState : unsigned char
{
	EInputState_NotPress	= 1 << 0,	// �����Ă��Ȃ�.
	EInputState_MomentPress	= 1 << 1,	// �������u��.
	EInputState_Hold		= 1 << 2,	// ������.
	EInputState_Release		= 1 << 3,	// �������u��.
} typedef EInputState;

#endif	// #ifndef INPUT_ENUMS_H.