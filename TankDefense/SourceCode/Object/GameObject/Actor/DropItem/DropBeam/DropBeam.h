/**
* @file		DropBeam.h.
* @brief	�r�[���C�̃h���b�v�N���X.
* @author	��c�F�P.
*/
#ifndef DROPBEAM_H
#define DROPBEAM_H

#include "..\DropItem.h"

/************************************************
*	�r�[���C�̃h���b�v�N���X.
**/
class CDropBeam final
	: public CDropItem
{
public:
	CDropBeam();
	~CDropBeam();

protected:
	// �쐬�֐�.
	virtual void Create() override;

};
#endif	// #ifndef DROPBEAM_H.
