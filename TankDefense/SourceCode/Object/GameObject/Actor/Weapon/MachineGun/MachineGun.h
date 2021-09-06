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
class CMachineGun final
	: public CWeapon
{
public:
	CMachineGun();
	~CMachineGun();

	// 初期化関数.
	virtual bool Init() override;

protected:
	// 作成関数.
	virtual void Create() override;
};

#endif	// #ifndef MACHINEGUN_H.