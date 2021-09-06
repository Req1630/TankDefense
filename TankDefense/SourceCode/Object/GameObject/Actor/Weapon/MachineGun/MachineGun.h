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
class CMachineGun final
	: public CWeapon
{
public:
	CMachineGun();
	~CMachineGun();

	// �������֐�.
	virtual bool Init() override;

protected:
	// �쐬�֐�.
	virtual void Create() override;
};

#endif	// #ifndef MACHINEGUN_H.