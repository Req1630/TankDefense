/**
* @file		DropMachineGun.h.
* @brief	�}�V���K���̃h���b�v�N���X.
* @author	��c�F�P.
*/
#ifndef DROPMACHINEGUN_H
#define DROPMACHINEGUN_H

#include "..\DropItem.h"

/************************************************
*	�}�V���K���̃h���b�v�N���X.
**/
class CDropMachineGun final
	: public CDropItem
{
public:
	CDropMachineGun();
	~CDropMachineGun();

protected:
	// �쐬�֐�.
	virtual void Create() override;

};
#endif	// #ifndef DROPMACHINEGUN_H.
