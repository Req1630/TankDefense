/**
* @file		DefaultWepon.h.
* @brief	��C�N���X.
* @author	��c�F�P.
*/
#ifndef DEFAULTWEPON_H
#define DEFAULTWEPON_H

#include "..\Weapon.h"

/************************************************
*	��C�N���X.
**/
class CDefaultWepon
	: public CWeapon
{
public:
	CDefaultWepon();
	~CDefaultWepon();

	// �������֐�.
	virtual bool Init();

};

#endif // #ifndef DEFAULTWEPON_H.