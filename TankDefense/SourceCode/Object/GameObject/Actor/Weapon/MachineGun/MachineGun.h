/**
* @file		MachineGun.h.
* @brief	�}�V���K���N���X.
* @author	��c�F�P.
*/
#ifndef MACHINEGUN_H
#define MACHINEGUN_H

#include "..\Weapon.h"

/************************************************
*	�}�V���K���N���X.
**/
class CMachineGun
	: public CWeapon
{
public:
	CMachineGun();
	~CMachineGun();

	// �������֐�.
	virtual bool Init();

};

#endif	// #ifndef MACHINEGUN_H.