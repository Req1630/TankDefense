/**
* @file		MachineGun.h.
* @brief	マシンガンクラス.
* @author	崎田友輝.
*/
#ifndef MACHINEGUN_H
#define MACHINEGUN_H

#include "..\Weapon.h"

/************************************************
*	マシンガンクラス.
**/
class CMachineGun
	: public CWeapon
{
public:
	CMachineGun();
	~CMachineGun();

	// 初期化関数.
	virtual bool Init();

};

#endif	// #ifndef MACHINEGUN_H.