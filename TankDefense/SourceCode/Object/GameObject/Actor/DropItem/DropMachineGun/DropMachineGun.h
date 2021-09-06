/**
* @file		DropMachineGun.h.
* @brief	マシンガンのドロップクラス.
* @author	崎田友輝.
*/
#ifndef DROPMACHINEGUN_H
#define DROPMACHINEGUN_H

#include "..\DropItem.h"

/************************************************
*	マシンガンのドロップクラス.
**/
class CDropMachineGun final
	: public CDropItem
{
public:
	CDropMachineGun();
	~CDropMachineGun();

protected:
	// 作成関数.
	virtual void Create() override;

};
#endif	// #ifndef DROPMACHINEGUN_H.
