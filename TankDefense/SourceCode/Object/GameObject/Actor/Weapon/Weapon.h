/**
* @file		Weapon.h.
* @brief	武器ベースクラス.
* @author	崎田友輝.
*/
#ifndef WEAPON_H
#define WEAPON_H

#include "..\Actor.h"

class CDX9StaticMesh;
class CBullet;

/************************************************
*	武器ベースクラス.
**/
class CWeapon
	: public CActor
{
public:
	// 武器の種類.
	enum enType
	{
		DefaultWepon,
		MachineGun,
		Missile,
		Beam
	} typedef EType;

	// ステータス.
	struct stStatus
	{
		int	  BulletsNum;	// 弾数.
		float Attack;		// 攻撃力.
		float Speed;		// 速度.
		float Interval;		// 間隔(秒).

		stStatus()
			: BulletsNum	( 0		)
			, Attack		( 0.0f	)
			, Speed			( 0.0f	)
			, Interval		( 0.0f	)
		{}
	} typedef SStatus;

public:
	CWeapon();
	virtual ~CWeapon();

	// 初期化関数.
	virtual bool Init();
	// 更新関数.
	virtual void Update( const float& deltaTime );
	// 描画関数.
	virtual void Render();

	// 当たり判定関数.
	virtual void Collision( CActor* pActor );

	// 当たり判定の初期化.
	virtual void InitCollision();
	// 当たり判定の座標や、半径などの更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision();

	// 弾の発射.
	void Shot( D3DXVECTOR3 MoveVec );

	// 移動.
	void Move( D3DXVECTOR3 Pos, D3DXVECTOR3 Rot );

protected:
	CDX9StaticMesh*							m_pStaticMesh;	// 武器モデル.

	std::vector<std::unique_ptr<CBullet>>	m_pBullet;		// 弾.
	EType									m_Type;			// 武器のタイプ.
	int										m_ListSize;		// 配列のサイズ.

	SStatus									m_Status;		// ステータス.
	float									m_ShotCnt;		// 弾間隔用カウント.

};

#endif	// #ifndef WEAPON_H.