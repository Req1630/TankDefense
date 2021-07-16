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

	// 構造体を配列としてアクセスする operator関数.
	// (&AttackPower) == 先頭アドレス.
	//	MAX_INDEX == 構造体内の変数の数.
	//	※ "D3DXVECTOR3" は "x,y,z" と 3つ分あるので3つ分としてカウント.
	float  operator[](const size_t& idx) const	{ _ASSERT(idx <= MAX_INDEX); return (&AttackPower)[idx]; }
	float& operator[](const size_t& idx)		{ _ASSERT(idx <= MAX_INDEX); return (&AttackPower)[idx]; }

} typedef SPlayerParam;

#endif	// #ifndef PLAYER_PARAMETER_H.