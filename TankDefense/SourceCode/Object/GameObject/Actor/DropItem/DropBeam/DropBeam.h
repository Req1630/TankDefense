/**
* @file		DropBeam.h.
* @brief	ビーム砲のドロップクラス.
* @author	崎田友輝.
*/
#ifndef DROPBEAM_H
#define DROPBEAM_H

#include "..\DropItem.h"

/************************************************
*	ビーム砲のドロップクラス.
**/
class CDropBeam final
	: public CDropItem
{
public:
	CDropBeam();
	~CDropBeam();

protected:
	// 作成関数.
	virtual void Create() override;

};
#endif	// #ifndef DROPBEAM_H.
