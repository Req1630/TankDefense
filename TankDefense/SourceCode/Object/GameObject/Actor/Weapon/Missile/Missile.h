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
class CMissile final
	: public CWeapon
{
public:
	CMissile();
	~CMissile();

	// 初期化関数.
	virtual bool Init() override;

protected:
	// 作成関数.
	virtual void Create() override;
};

#endif	// #ifndef MISSILE_H.