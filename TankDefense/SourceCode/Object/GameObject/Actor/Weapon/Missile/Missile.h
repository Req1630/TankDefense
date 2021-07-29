/**
* @file		Missile.h.
* @brief	ミサイルクラス.
* @author	崎田友輝.
*/
#ifndef MISSILE_H
#define MISSILE_H

#include "..\Weapon.h"

/************************************************
*	ミサイルクラス.
**/
class CMissile
	: public CWeapon
{
public:
	CMissile();
	~CMissile();

	// 初期化関数.
	virtual bool Init();

};

#endif	// #ifndef MISSILE_H.