/**
* @file		Player.h.
* @brief	プレイヤークラス.
* @author	崎田友輝.
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "..\Actor.h"

class CDX9StaticMesh;
class CRotLookAtCenter;
class CWeapon;
class CDefaultWepon;
class CMachineGun;
class CMissile;

/************************************************
*	プレイヤークラス.
**/
class CPlayer
	: public CActor
{
public:
	// ステータス.
	struct stStatus
	{
		float Hp;
		float Speed;

		stStatus()
			: Hp	( 0.0f )
			, Speed	( 0.0f )
		{}
	} typedef SStatus;

public:
	CPlayer();
	~CPlayer();

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

private:
	// 操作関数.
	void Controller();
	// カメラの操作関数.
	void CameraController();
	// 攻撃関数.
	void AttackController();

	// 移動関数.
	void Move();
	// カメラの更新.
	void CameraUpdate();
	
protected:
	CDX9StaticMesh*						m_pStaticMesh;		// プレイヤーの車体モデル.
	std::unique_ptr<CRotLookAtCenter>	m_pLookCamera;		// カメラ.
	std::shared_ptr<CWeapon>			m_pWeapon;			// 武器.
	std::shared_ptr<CDefaultWepon>		m_pDefaultWepon;	// 主砲.
	std::shared_ptr<CMachineGun>		m_pMachineGun;		// マシンガン.
	std::shared_ptr<CMissile>			m_pMissile;			// ミサイル.

	D3DXVECTOR3							m_CameraRot;		// カメラの回転.
	D3DXVECTOR3							m_MoveVec;			// 移動ベクトル.

	SStatus								m_Status;			// ステータス.
};

#endif	// #ifndef PLAYER_H.