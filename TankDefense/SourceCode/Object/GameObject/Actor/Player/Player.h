/**
* @file		Player.h.
* @brief	プレイヤークラス.
* @author	崎田友輝.
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "..\Actor.h"

class CDX9SkinMesh;
class CRotLookAtCenter;
class CBulletManager;
class CWeapon;
class CDefaultWepon;
class CMachineGun;
class CMissile;
class CBeam;

/************************************************
*	プレイヤークラス.
**/
class CPlayer final
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
	CPlayer( const std::shared_ptr<CBulletManager> pBulletMng );
	~CPlayer();

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

	// 武器の変更関数.
	void ChangeWeapon( const std::shared_ptr<CWeapon> pWeapon );

private:
	// 作成関数.
	void Create();

	// 操作関数.
	void Controller();
	// カメラの操作関数.
	void CameraController();
	// 攻撃関数.
	void AttackController();

	// 移動関数.
	void Move();
	// カメラの更新関数.
	void CameraUpdate();
	
	// デバックの更新関数.
	void DebugUpdate();

protected:
	CDX9SkinMesh*						m_pSkinMesh;		// プレイヤーの車体モデル.
	std::unique_ptr<CRotLookAtCenter>	m_pLookCamera;		// カメラ.
	std::shared_ptr<CBulletManager>		m_pBulletMng;		// 弾マネージャー.
	std::shared_ptr<CWeapon>			m_pWeapon;			// 武器.
	std::shared_ptr<CDefaultWepon>		m_pDefaultWepon;	// 主砲.

	D3DXVECTOR3							m_CameraRot;		// カメラの回転.
	D3DXVECTOR3							m_MoveVec;			// 移動ベクトル.
	D3DXVECTOR3							m_DireVec;			// 向きベクトル.

	D3DXVECTOR3							m_AlignmentPos;		// 照準の位置.

	SStatus								m_Status;			// ステータス.
};

#endif	// #ifndef PLAYER_H.