/**
* @file		DropMissile.h.
* @brief	�~�T�C���̃h���b�v�N���X.
* @author	��c�F�P.
*/
#ifndef DROPMISSILE_H
#define DROPMISSILE_H

#include "..\DropItem.h"

/************************************************
*	�~�T�C���̃h���b�v�N���X.
**/
class CDropMissile final
	: public CDropItem
{
public:
	CDropMissile();
	~CDropMissile();

protected:
	// �쐬�֐�.
	virtual void Create() override;

};
#endif	// #ifndef DROPMISSILE_H.
