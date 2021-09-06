/**
* @file		Beam.h.
* @brief	ビームクラス.
* @author	崎田友輝.
*/
#ifndef BEAM_H
#define BEAM_H

#include "..\Weapon.h"

/************************************************
*	主砲クラス.
**/
class CBeam final
	: public CWeapon
{
public:
	CBeam();
	~CBeam();

	// 初期化関数.
	virtual bool Init() override;

protected:
	// 作成関数.
	virtual void Create() override;
};

#endif // #ifndef BEAM_H.