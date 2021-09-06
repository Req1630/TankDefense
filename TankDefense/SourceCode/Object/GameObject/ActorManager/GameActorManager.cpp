#include "GameActorManager.h"
#include "..\Actor\Player\Player.h"
#include "..\Actor\Weapon\Bullet\BulletManager\BulletManager.h"
#include "..\Actor\EnemyBase\Enemy\Enemy.h"
#include "..\Actor\EnemyBase\SpecialEnemy_1\SpecialEnemy_1.h"
#include "..\Actor\EnemyBase\SpecialEnemy_2\SpecialEnemy_2.h"
#include "..\Actor\Boss\Boss.h"
#include "..\Actor\DropItem\DropItemManager\DropItemManager.h"

CGameActorManager::CGameActorManager()
	: m_pPlayer				( nullptr )
	, m_pBulletMng			( nullptr )
	, m_pEnemy				( nullptr )
	, m_pSpecialEnemy_1		( nullptr )
	, m_pSpecialEnemy_2		( nullptr )
	, m_pBoss				( nullptr )
	, m_pDropItemMng		( nullptr )
{
	m_pBulletMng		= std::make_shared<CBulletManager>();
	m_pPlayer			= std::make_shared<CPlayer>( m_pBulletMng );
	m_pEnemy			= std::make_shared<CEnemy>();
	m_pSpecialEnemy_1	= std::make_shared<CSpecialEnemy_1>();
	m_pSpecialEnemy_2	= std::make_shared<CSpecialEnemy_2>();
	m_pBoss				= std::make_shared<CBoss>();
	m_pDropItemMng		= std::make_shared<CDropItemManager>( m_pPlayer );
}

CGameActorManager::~CGameActorManager()
{
}

// 初期化関数.
bool CGameActorManager::Init()
{
	if ( m_pBulletMng->Init()		== false ) return false;	// 弾の初期化.
	if ( m_pPlayer->Init()			== false ) return false;	// プレイヤーの初期化.
	if ( m_pEnemy->Init()			== false ) return false;	// 雑魚敵の初期化.
	if ( m_pSpecialEnemy_1->Init()	== false ) return false;	// 特殊な敵1の初期化.
	if ( m_pSpecialEnemy_2->Init()	== false ) return false;	// 特殊な敵2の初期化.
	if ( m_pBoss->Init()			== false ) return false;	// ボスの初期化.
	if ( m_pDropItemMng->Init()		== false ) return false;	// ドロップアイテムの初期化.

	return true;
}

// 更新関数.
void CGameActorManager::Update()
{
	m_DeltaTime = GetDeltaTime();

	m_ObjPosListCount = 0;		// カウントの初期化.
	m_ObjPositionList.clear();	// リストの初期化.

	// プレイヤーの更新.
	m_pPlayer->Update( m_DeltaTime );			// 更新.

	// 雑魚敵の更新.
	m_pEnemy->SetTargetPos( *m_pPlayer.get() );					// プレイヤーの座標を取得.
	//m_pEnemy->SetEnemyPos( *m_pEnemy.get() );
	m_pEnemy->Update( m_DeltaTime );							// 更新.
	// 当たり判定.
	m_pEnemy->Collision( m_pPlayer.get() );
	m_pEnemy->Collision( m_pSpecialEnemy_1.get() );
	m_pEnemy->Collision( m_pSpecialEnemy_2.get() );

	// 特殊な敵1の更新.
	m_pSpecialEnemy_1->SetTargetPos( *m_pPlayer.get() );		// プレイヤーの座標を取得.
	m_pSpecialEnemy_1->Update( m_DeltaTime );					// 更新.
	// 当たり判定.
	m_pSpecialEnemy_1->Collision( m_pPlayer.get() );
	m_pSpecialEnemy_1->Collision( m_pEnemy.get() );
	m_pSpecialEnemy_1->Collision( m_pSpecialEnemy_2.get() );

	// 特殊な敵2の更新.
	m_pSpecialEnemy_2->SetTargetPos( *m_pPlayer.get() );		// プレイヤーの座標を取得.
	m_pSpecialEnemy_2->Update( m_DeltaTime );					// 更新.
	// 当たり判定.
	m_pSpecialEnemy_2->Collision( m_pPlayer.get() );
	m_pSpecialEnemy_2->Collision( m_pSpecialEnemy_1.get() );

	// ボスの更新.
	m_pBoss->SetTargetPos( *m_pPlayer.get() );					// プレイヤーの座標を取得.
	m_pBoss->Update( m_DeltaTime );								// 更新.
	// 当たり判定.
	m_pBoss->Collision( m_pPlayer.get() );
	m_pBoss->Collision( m_pEnemy.get() );
	m_pBoss->Collision( m_pSpecialEnemy_1.get() );
	m_pBoss->Collision( m_pSpecialEnemy_2.get() );

	// 弾の更新.
	m_pBulletMng->Update( m_DeltaTime );		// 更新.

	SetPositionList( m_pPlayer.get() );			// プレイヤー座標を設定.
	m_ObjPositionList.shrink_to_fit();

	// ドロップアイテムの更新.
	m_pDropItemMng->Update( m_DeltaTime );
	m_pDropItemMng->Collision( m_pPlayer.get() );
}

// 描画関数.
void CGameActorManager::Render()
{
	m_pPlayer->Render();			// プレイヤーの描画.
	m_pEnemy->Render();				// 雑魚敵の描画.
	m_pSpecialEnemy_1->Render();	// 特殊な敵1の描画.
	m_pBoss->Render();				// ボスの描画.
	m_pSpecialEnemy_2->Render();	// 特殊な敵2の描画.
	m_pBulletMng->Render();			// 弾の描画.

	m_pDropItemMng->Render();		// ドロップアイテムの描画.
}

// Sprite3D描画関数.
void CGameActorManager::Sprite3DRender()
{
	m_pPlayer->Sprite3DRender();			// プレイヤーの描画.
	m_pEnemy->Sprite3DRender();				// 雑魚敵の描画.
	m_pSpecialEnemy_1->Sprite3DRender();	// 特殊な敵1の描画.
	m_pBoss->Sprite3DRender();				// ボスの描画.
	m_pSpecialEnemy_2->Sprite3DRender();	// 特殊な敵2の描画.
	m_pBulletMng->Sprite3DRender();			// 弾の描画.

	m_pDropItemMng->Sprite3DRender();
}

// スプライトの描画.
void CGameActorManager::SpriteRender()
{
}

// 座標リストの設定.
void CGameActorManager::SetPositionList( CGameObject* pObj )
{
	if ( pObj == nullptr ) return;
	m_ObjPosListCount++;	// オブジェクト数の加算.
	m_ObjPositionList.emplace_back(
		pObj->GetObjectTag(),	// タグの取得.
		// 座標、回転値 の取得..
		std::pair<D3DXVECTOR3, float>( pObj->GetPosition(), pObj->GetRotation().y ));
}

