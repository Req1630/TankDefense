/**
* @file		DropMissile.h.
* @brief	ミサイルのドロップクラス.
* @author	崎田友輝.
*/
#ifndef DROPMISSILE_H
#define DROPMISSILE_H

#include "..\DropItem.h"

/************************************************
*	ミサイルのドロップクラス.
**/
class CDropMissile final
	: public CDropItem
{
public:
	CDropMissile();
	~CDropMissile();

protected:
	// 作成関数.
	virtual void Create() override;

};
#endif	// #ifndef DROPMISSILE_H.
