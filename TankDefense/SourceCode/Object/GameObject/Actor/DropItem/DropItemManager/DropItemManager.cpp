#include "DropItemManager.h"
#include "..\DropMachineGun\DropMachineGun.h"
#include "..\DropMissile\DropMissile.h"
#include "..\DropBeam\DropBeam.h"
#include "..\..\Player\Player.h"
#include "..\..\..\..\.\..\Utility\Input\Input.h"
#include <random>

CDropItemManager::CDropItemManager()
	: CDropItemManager( nullptr )
{
}

CDropItemManager::CDropItemManager( const std::shared_ptr<CPlayer> pPlayer )
	: m_DropItem	()
	, m_pPlayer		( pPlayer )
{
}

CDropItemManager::~CDropItemManager()
{
}

// 初期化関数.
bool CDropItemManager::Init()
{
	InitCollision();
	return true;
}

// 更新関数.
void CDropItemManager::Update( const float& deltaTime )
{
	// ドロップアイテムの更新.
	for ( auto &i : m_DropItem ) i.pItem->Update( deltaTime );

	// ドロップアイテムの削除.
	const int EndIndex = m_DropItem.size() - 1;
	if ( EndIndex >= 0 ){	// 要素が0個(EndIndex = -1)の場合ではない時.
		if ( m_DropItem[EndIndex].pItem->GetDisp() == false )
			m_DropItem.pop_back();
	}

	// デバックの更新.
	DebugUpdate();

	UpdateCollision();
}

// 描画関数.
void CDropItemManager::Render()
{
	// ドロップアイテムの描画.
	for ( auto &i : m_DropItem ) i.pItem->Render();
}

// Sprite3D描画関数.
void CDropItemManager::Sprite3DRender()
{
	// アイテム名をビルボードで表示.
	for ( auto &i : m_DropItem ) i.pItem->Sprite3DRender();
}

// 当たり判定関数.
void CDropItemManager::Collision( CActor* pActor )
{
	// ドロップアイテムの当たり判定.
	for ( auto &i : m_DropItem ) i.pItem->Collision( pActor );
}

// 当たり判定の初期化.
void CDropItemManager::InitCollision()
{
}

// 当たり判定の座標や、半径などの更新.
void CDropItemManager::UpdateCollision()
{
}

// 指定した武器を落とす関数.
void CDropItemManager::Drop( const DropItem::EList& DropItem, const D3DXVECTOR3& DropPos )
{
	// 使用していないのがあれば使用する.
	for ( auto &i : m_DropItem ){
		if ( i.pItem->GetDisp() == false ){
			// 表示アイテムが違う場合変更する.
			if ( i.Tag != DropItem ) {
				i = SDropItem( CreateDropItem( DropItem ), DropItem );
				i.pItem->SetPlayer( m_pPlayer );
			}

			// アイテムをドロップ.
			i.pItem->Drop( DropPos );

			return;
		}
	}

	// 新しく作成.
	const int NewIndex = m_DropItem.size();
	m_DropItem.push_back( SDropItem( CreateDropItem( DropItem ), DropItem ) );
	m_DropItem[NewIndex].pItem->SetPlayer( m_pPlayer );

	// アイテムをドロップ.
	m_DropItem[NewIndex].pItem->Drop( DropPos );
}

// ランダムな武器を落とす関数.
void CDropItemManager::RandomDrop( const D3DXVECTOR3& DropPos, const DropItem::SProb& Prob  )
{
	// ランダムの設定.
	std::random_device	rnd;
	std::mt19937		mt( rnd() );
	std::uniform_int_distribution<>	Rand_Num( 1, 100 );
	DropItem::EList		RandItem;

	// 指定した確率の結果を返す.
	const int MissileProb = Prob.MachineGun + Prob.Missile;
	if		( Rand_Num( mt ) <= Prob.MachineGun )	RandItem = DropItem::EList::MachineGun;
	else if ( Rand_Num( mt ) <= MissileProb		)	RandItem = DropItem::EList::Missile;
	else											RandItem = DropItem::EList::Beam;

	// ドロップさせる.
	Drop( RandItem, DropPos );
}

// ドロップアイテムの作成関数(ファクトリーメソッド).
std::unique_ptr<CDropItem> CDropItemManager::CreateDropItem( const DropItem::EList& DropItem )
{
	// 対応したクラスを返す.
	switch ( DropItem ) {
	default:
	case DropItem::EList::MachineGun:	return std::make_unique<CDropMachineGun>();
	case DropItem::EList::Missile:		return std::make_unique<CDropMissile>();
	case DropItem::EList::Beam:			return std::make_unique<CDropBeam>();
	}
}

// デバックの更新関数.
void CDropItemManager::DebugUpdate()
{
	CDebugText::PushText( "DropItem", "--------------------" );
	CDebugText::PushText( "DropItem", "----  DropItem  ----" );
	CDebugText::PushText( "DropItem", "--------------------" );
	CDebugText::PushText( "DropItem", "L_Ctrl + " );
	CDebugText::PushText( "DropItem", "'R' RandomItemDrop" );
	CDebugText::PushText( "DropItem", "--------------------" );

	for ( auto &i : m_DropItem ){
		const size_t		Index = &i - &m_DropItem[0];
		const D3DXVECTOR3	IPos = i.pItem->GetPosition();
		CDebugText::PushTextF( "DropItem", "DropItem[%3d].Pos  : %f, %f, %f", Index, IPos.x, IPos.y, IPos.z );
		CDebugText::PushTextF( "DropItem", "DropItem[%3d].Disp : %s", Index, i.pItem->GetDisp() == true ? "true" : "false" );
		CDebugText::PushTextF( "DropItem", "DropItem[%3d].Tag  : %s", Index, i.Tag == DropItem::EList::MachineGun ? "MachineGun" : i.Tag == DropItem::EList::Missile ? "Missile" : "Beam" );
	}

	if ( CKeyInput::IsPress( VK_LCONTROL ) == false ) return;
	const D3DXVECTOR3 PPos		= m_pPlayer->GetPosition();
	const D3DXVECTOR3 DropPos	= { PPos.x, PPos.y, PPos.z - 10.0f };
	if ( CKeyInput::IsMomentPress( 'R' ) )
		RandomDrop( DropPos, DropItem::SProb( 50, 30 ) );

	// プレイヤーの武器変更用.
	if ( CKeyInput::IsMomentPress( '2' ) ) Drop( DropItem::EList::MachineGun, PPos );
	if ( CKeyInput::IsMomentPress( '3' ) ) Drop( DropItem::EList::Missile, PPos );
	if ( CKeyInput::IsMomentPress( '4' ) ) Drop( DropItem::EList::Beam, PPos );
}