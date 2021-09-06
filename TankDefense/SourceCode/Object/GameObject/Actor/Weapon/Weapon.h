/**
* @file		Weapon.h.
* @brief	武器ベースクラス.
* @author	崎田友輝.
*/
#ifndef WEAPON_H
#define WEAPON_H

#include "..\Actor.h"
#include "ShotStatus.h"

class CDX9SkinMesh;
class CBulletManager;

/************************************************
*	武器ベースクラス.
**/
class CWeapon
	: public CActor
{
public:
	// 武器の種類.
	enum enType : unsigned char
	{
		DefaultWepon,
		MachineGun,
		Missile,
		Beam
	} typedef EType;

public:
	CWeapon();
	virtual ~CWeapon();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update( const float& deltaTime ) override;
	// 描画関数.
	virtual void Render() override;

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

	// 当たり判定の初期化.
	virtual void InitCollision() override;
	// 当たり判定の座標や、半径などの更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

	// 弾の発射関数.
	virtual bool Shot( D3DXVECTOR3 MoveVec ) final;

	// 移動関数.
	virtual void Move( D3DXVECTOR3 BodyPos, D3DXVECTOR3 Rot ) final;

	// マネージャーの設定.
	void SetBulletMng( const std::shared_ptr<CBulletManager> pBulletMng ) { m_pBulletMng = pBulletMng; }

protected:
	// デバックの更新関数.
	void DebugUpdate();

	// 作成関数.
	virtual void Create() = 0;

protected:
	CDX9SkinMesh*					m_pSkinMesh_Body;	// 車体モデル.
	CDX9SkinMesh*					m_pSkinMesh_Weapon;	// 武器モデル.
	EType							m_Type;				// 武器のタイプ.
	SStatus							m_Status;			// ステータス.
	std::string						m_BoneName;			// ボーン名前.
	std::string						m_BulletName;		// 弾のモデルの名前.
	std::string						m_ShotName;			// 弾の発射場所のボーンの名前.

private:
	std::shared_ptr<CBulletManager>	m_pBulletMng;		// 弾.
	float							m_ShotCnt;			// 弾間隔用カウント.
};

#endif	// #ifndef WEAPON_H.