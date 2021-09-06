/**
* @file		DefaultWepon.h.
* @brief	å–CƒNƒ‰ƒX.
* @author	è“c—F‹P.
*/
#ifndef DEFAULTWEPON_H
#define DEFAULTWEPON_H

#include "..\Weapon.h"

/************************************************
*	å–CƒNƒ‰ƒX.
**/
class CDefaultWepon final
	: public CWeapon
{
public:
	CDefaultWepon();
	~CDefaultWepon();

	// ‰Šú‰»ŠÖ”.
	virtual bool Init() override;

protected:
	// ì¬ŠÖ”.
	virtual void Create() override;
};

#endif // #ifndef DEFAULTWEPON_H.