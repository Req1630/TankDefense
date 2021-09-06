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
class CMissile final
	: public CWeapon
{
public:
	CMissile();
	~CMissile();

	// �������֐�.
	virtual bool Init() override;

protected:
	// �쐬�֐�.
	virtual void Create() override;
};

#endif	// #ifndef MISSILE_H.