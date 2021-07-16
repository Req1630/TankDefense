#ifndef PLAYER_PARAMETER_H
#define PLAYER_PARAMETER_H

struct stPlayerParam
{
	float		AttackPower;
	float		HP;
	float		MoveSpeed;
	D3DXVECTOR3	InitPosition;

	static const size_t MAX_INDEX = 6;

	stPlayerParam()
		: AttackPower	( 0.0f )
		, HP			( 0.0f )
		, MoveSpeed		( 0.0f )
		, InitPosition	( 0.0f, 0.0f ,0.0f )
	{}

	// �\���̂�z��Ƃ��ăA�N�Z�X���� operator�֐�.
	// (&AttackPower) == �擪�A�h���X.
	//	MAX_INDEX == �\���̓��̕ϐ��̐�.
	//	�� "D3DXVECTOR3" �� "x,y,z" �� 3������̂�3���Ƃ��ăJ�E���g.
	float  operator[](const size_t& idx) const	{ _ASSERT(idx <= MAX_INDEX); return (&AttackPower)[idx]; }
	float& operator[](const size_t& idx)		{ _ASSERT(idx <= MAX_INDEX); return (&AttackPower)[idx]; }

} typedef SPlayerParam;

#endif	// #ifndef PLAYER_PARAMETER_H.