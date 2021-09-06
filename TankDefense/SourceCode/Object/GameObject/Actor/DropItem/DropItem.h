/**
* @file		DropItem.h.
* @brief	ドロップアイテムベースクラス.
* @author	崎田友輝.
*/
#ifndef DROPITEM_H
#define DROPITEM_H

#include "..\Actor.h"

class CDX9StaticMesh;
class CPlayer;
class CWeapon;

/************************************************
*	ドロップアイテムクラス.
**/
class CDropItem
	: public CActor
{
public:
	CDropItem();
	~CDropItem();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update( const float& deltaTime ) override;
	// 描画関数.
	virtual void Render() override;
	// Sprite3D描画関数.
	virtual void Sprite3DRender() override;

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

	// 当たり判定の初期化.
	virtual void InitCollision() override;
	// 当たり判定の座標や、半径などの更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

	// ドロップ関数.
	virtual void Drop( D3DXVECTOR3 DropPos ) final;

	// プレイヤーを設定.
	void SetPlayer( const std::shared_ptr<CPlayer> pPlayer ) { m_pPlayer = pPlayer; }

	// 表示を取得.
	bool GetDisp() const { return m_Disp; }

protected:
	// 作成関数.
	virtual void Create() = 0;

protected:
	std::shared_ptr<CWeapon>	m_pGetWeapom;		// 入手できる武器.
	CDX9StaticMesh*				m_pStaticMesh;		// ボックスのモデル.
	CSprite*					m_pBillboardSprite;	// 武器の名前.

private:
	std::shared_ptr<CPlayer>	m_pPlayer;			// プレイヤー.
	bool						m_Disp;				// 表示状態.

};
#endif	// #ifndef DROPITEM_H.
