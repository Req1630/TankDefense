/**
* @file		DefaultWepon.h.
* @brief	主砲クラス.
* @author	崎田友輝.
*/
#ifndef DEFAULTWEPON_H
#define DEFAULTWEPON_H

#include "..\Weapon.h"

/************************************************
*	主砲クラス.
**/
class CDefaultWepon
	: public CWeapon
{
public:
	CDefaultWepon();
	~CDefaultWepon();

	// 初期化関数.
	virtual bool Init();

};

#endif // #ifndef DEFAULTWEPON_H.