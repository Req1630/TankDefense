/**
* @file		ShotStatus.h.
* @brief	�e�̃X�e�[�^�X�\����.
* @author	��c�F�P.
*/
#ifndef SHOTSTATUS_H
#define SHOTSTATUS_H

// �e�X�e�[�^�X.
struct stStatus
{
	int	  BulletsNum;	// �e��.
	float Attack;		// �U����.
	float Speed;		// ���x.
	float Interval;		// �Ԋu(�b).

	stStatus()
		: BulletsNum( 0 )
		, Attack( 0.0f )
		, Speed( 0.0f )
		, Interval( 0.0f )
	{}
} typedef SStatus;

#endif // #ifndef SHOTSTATUS_H.