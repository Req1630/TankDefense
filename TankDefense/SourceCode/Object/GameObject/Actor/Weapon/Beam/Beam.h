/**
* @file		Beam.h.
* @brief	�r�[���N���X.
* @author	��c�F�P.
*/
#ifndef BEAM_H
#define BEAM_H

#include "..\Weapon.h"

/************************************************
*	��C�N���X.
**/
class CBeam final
	: public CWeapon
{
public:
	CBeam();
	~CBeam();

	// �������֐�.
	virtual bool Init() override;

protected:
	// �쐬�֐�.
	virtual void Create() override;
};

#endif // #ifndef BEAM_H.