#include "GameActorManager.h"
#include "..\Actor\Player\Player.h"
#include "..\Actor\EnemyBase\Enemy\Enemy.h"
#include "..\Actor\EnemyBase\SpecialEnemy_1\SpecialEnemy_1.h"
#include "..\Actor\EnemyBase\SpecialEnemy_2\SpecialEnemy_2.h"

CGameActorManager::CGameActorManager()
	: m_pPlayer				( nullptr )
	, m_pEnemy				( nullptr )
	, m_pSpecialEnemy_1		( nullptr )
	, m_pSpecialEnemy_2		( nullptr )
{
	m_pPlayer		  = std::make_shared<CPlayer>();
	m_pEnemy		  = std::make_shared<CEnemy>();
	m_pSpecialEnemy_1 = std::make_shared<CSpecialEnemy_1>();
	m_pSpecialEnemy_2 = std::make_shared<CSpecialEnemy_2>();
}

CGameActorManager::~CGameActorManager()
{
}

// 初期化関数.
bool CGameActorManager::Init()
{
	if ( m_pPlayer->Init()		   == false ) return false;	// プレイヤーの初期化.
	if ( m_pEnemy->Init()		   == false ) return false;	// 雑魚敵の初期化.
	if ( m_pSpecialEnemy_1->Init() == false ) return false;	// 特殊な敵1の初期化.
	if ( m_pSpecialEnemy_2->Init() == false ) return false;	// 特殊な敵2の初期化.

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


	SetPositionList( m_pPlayer.get() );			// プレイヤー座標を設定.
	m_ObjPositionList.shrink_to_fit();

}

// 描画関数.
void CGameActorManager::Render()
{
	m_pPlayer->Render();			// プレイヤーの描画.
	m_pEnemy->Render();				// 雑魚敵の描画.
	m_pSpecialEnemy_1->Render();	// 特殊な敵1の描画.
	m_pSpecialEnemy_2->Render();	// 特殊な敵2の描画.

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

