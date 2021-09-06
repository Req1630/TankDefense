/**
* @file		DropItemManager.h.
* @brief	ドロップアイテムマネージャークラス.
* @author	崎田友輝.
*/
#ifndef DROPITEMMANAGER_H
#define DROPITEMMANAGER_H

#include "..\..\Actor.h"
#include "..\DropItem.h"

class CPlayer;

namespace DropItem{
	// ドロップリスト.
	enum class enDropItemList : unsigned char {
		MachineGun,
		Missile,
		Beam,

		Max
	} typedef EList;

	// ドロップ確率(%).
	struct stProbability
	{
		int MachineGun;
		int Missile;
		int Beam;

		stProbability()
			: stProbability( 33, 33 )
		{}
		stProbability( const int MachProb, const int MissProb, const int BeamProb = 0 )
			: MachineGun	( MachProb )
			, Missile		( MissProb )
			, Beam			( BeamProb )
		{}
	} typedef SProb;
}

/************************************************
*	ドロップアイテムマネージャー.
**/
class CDropItemManager final
	: public CActor
{
private:
	struct stDropItem {
		std::unique_ptr<CDropItem>	pItem;
		DropItem::EList				Tag;

		stDropItem()
			: stDropItem( nullptr, DropItem::EList::MachineGun )
		{}
		stDropItem( std::unique_ptr<CDropItem> pNewItem, const DropItem::EList& NewTag )
			: pItem	( std::move( pNewItem ) )
			, Tag	( NewTag )
		{}

	} typedef SDropItem;

public:
	CDropItemManager();
	CDropItemManager( const std::shared_ptr<CPlayer> pPlayer );
	~CDropItemManager();

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

	// 指定した武器を落とす関数.
	void Drop( const DropItem::EList& DropItem, const D3DXVECTOR3& DropPos );

	// ランダムな武器を落とす関数.
	void RandomDrop( const D3DXVECTOR3& DropPos, const DropItem::SProb& Prob = DropItem::SProb() );
	
private:
	// ドロップアイテムの作成関数(ファクトリーメソッド).
	std::unique_ptr<CDropItem> CreateDropItem( const DropItem::EList& DropItem );

	// デバックの更新関数.
	void DebugUpdate();

private:
	std::vector<SDropItem>		m_DropItem;	// ドロップアイテム.
	std::shared_ptr<CPlayer>	m_pPlayer;	// プレイヤー.

};
#endif	// #ifndef DROPITEMMANAGER_H.
