/**
* @file		Missile.h.
* @brief	�~�T�C���N���X.
* @author	��c�F�P.
*/
#ifndef MISSILE_H
#define MISSILE_H

#include "..\Weapon.h"

/************************************************
*	�~�T�C���N���X.
**/
class CMissile
	: public CWeapon
{
public:
	CMissile();
	~CMissile();

	// �������֐�.
	virtual bool Init();

};

#endif	// #ifndef MISSILE_H.