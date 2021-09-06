#include "SpecialEnemy_1.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\Explosion\Explosion.h"

CSpecialEnemy_1::CSpecialEnemy_1()
	: m_pEnemyBase		()
	, m_pExplosion		( nullptr )

{
	m_pExplosion	= std::make_shared<CExplosion>();
	m_ObjectTag		= EObjectTag::SpecialEnemy_1;
	Init();
}

CSpecialEnemy_1::~CSpecialEnemy_1()
{
}

// 初期化関数.
bool CSpecialEnemy_1::Init()
{
	if ( m_pExplosion->Init() == false ) return false;	// 爆発の初期化.

	m_pSkinMesh = CMeshResorce::GetSkin( "enemy_bomb_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	
	Spawn( D3DXVECTOR3( 0.0f, 0.0f ,-20.0f ) );
	m_IsDelete = false;

	InitCollision();	// 当たり判定の初期化.
	
	return true;
}

// 更新関数.
void CSpecialEnemy_1::Update( const float& deltaTime )
{

	m_DeltaTime = deltaTime;

	CurrentStateUpdate();	// 現在の状態更新.

	// スポーン.
	if ( CKeyInput::IsMomentPress( 'T' ) == true ) m_IsDelete = false;
	// 死亡.
	if ( CKeyInput::IsMomentPress( 'G' ) == true ) m_IsDelete = true;

	DebugUpdate();			// デバック更新.

	UpdateCollision();	// 当たり判定の更新.
}

// 描画関数.
void CSpecialEnemy_1::Render()
{
	// 画面の外なら終了.
	if ( IsDisplayOut() == true ) return;

	m_pSkinMesh->SetTranceform( m_Tranceform );
	if ( m_IsDelete == false ) m_pSkinMesh->Render();
	
}

// Sprite3D描画関数.
void CSpecialEnemy_1::Sprite3DRender()
{
	m_pExplosion->Render();
}

// 当たり判定関数.
void CSpecialEnemy_1::Collision( CActor* pActor )
{
	if ( pActor == nullptr ) return;
	if ( m_pCollisions == nullptr ) return;
	if ( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;
	if ( m_pExplosion->GetCollision<CSphere>() == nullptr ) return;

	PlayerCollsion( pActor );
	ExplosionCollsion( pActor );
	ExplosionEnemyCollsion( pActor );
	ExplosionSEnemy2Collsion( pActor );
}

// 当たり判定の初期化.
void CSpecialEnemy_1::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// 当たり判定の座標や、半径などの更新.
//	Update関数の最後に呼ぶ.
void CSpecialEnemy_1::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( D3DXVECTOR3( m_Tranceform.Position.x, m_Tranceform.Position.y + 4.0f, m_Tranceform.Position.z ) );
}

// スポーン.
bool CSpecialEnemy_1::Spawn( const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if ( m_NowState != Enemy::EEnemyState::None ) return true;
	m_Tranceform.Position = spawnPos;			// スポーン座標の設定.
	//m_LifePoint = pPARAMETER->LifeMax;		// 体力の設定.
	m_NowState = Enemy::EEnemyState::Spawn;		// 現在の状態をスポーンに変更.
	return true;
}

// 相手座標の設定.
void CSpecialEnemy_1::SetTargetPos( CActor& actor )
{
	// プレイヤーの座標を取得.
	CEnemyBase::SetTargetPos( actor );
}

// スポーン中.
void CSpecialEnemy_1::Spawning()
{
	CEnemyBase::Spawning();
}

// 移動.
void CSpecialEnemy_1::Move()
{
	// プレイヤーを追跡.
	SetMoveVector( m_TargetPosition );									// 移動ベクトルの設定.
	TargetRotation( m_MoveSpeed / 2 );									// 回転.
	CEnemyBase::VectorMove( m_MoveSpeed / 2, EXPLOSION_ATTACK_LENGTH );	// 移動.
	Attack();															// 攻撃.
	CEnemyBase::WaitMove();												// 待機.
}

// 怯み.
void CSpecialEnemy_1::Fright()
{
	CEnemyBase::Fright();
}

// 死亡.
void CSpecialEnemy_1::Death()
{
	CEnemyBase::Death();
}

// 攻撃関数.
void CSpecialEnemy_1::Attack()
{
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;

	if ( m_pExplosion->GetIsExplosion() == false ) m_pExplosion->SetIsExplosion( true );
	m_pExplosion->Update( m_DeltaTime );
	m_pExplosion->SetExplosionPos( m_Tranceform.Position );

	// 攻撃を行ったら待機状態に遷移する.
	if ( m_pExplosion->GetIsExplosion() == true ) return;
	m_NowMoveState = Enemy::EMoveState::Wait;
}

// プレイヤーとの当たり判定.
void CSpecialEnemy_1::PlayerCollsion( CActor* pActor )
{
	// オブジェクトのタグがプレイヤーじゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// プレイヤーとエネミーの当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];//プレイヤーと雑魚敵の距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );//プレイヤーの刺さっている長さ

	D3DXVECTOR3 vecPushOut;//押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );//Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// プレイヤーと爆発の当たり判定.
void CSpecialEnemy_1::ExplosionCollsion( CActor * pActor )
{
	// オブジェクトのタグがプレイヤーじゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// 攻撃状態じゃなければ終了.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// 死亡していたら終了.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// 怯み状態なら終了.

	// 爆発とプレイヤーの当たり判定.
	if ( coll::IsSphereToSphere( m_pExplosion->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	// プレイヤーを消す.
	pActor->SetDelete( [&]( bool& isDead )
	{
		isDead = true;
	});

	//// 現在の移動ベクトルを設定.
	//pActor->SetVector( m_MoveVector );
	//// プレイヤーの体力を減らす.
	//pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	//{
	//	life -= pPARAMETER->AttackPower;
	//	isAttack = true;
	//});
}

// 雑魚敵と爆発の当たり判定.
void CSpecialEnemy_1::ExplosionEnemyCollsion( CActor * pActor )
{
	// オブジェクトのタグが雑魚敵じゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::Enemy ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// 攻撃状態じゃなければ終了.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// 死亡していたら終了.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// 怯み状態なら終了.

	// 爆発と雑魚敵の当たり判定.
	if ( coll::IsSphereToSphere( m_pExplosion->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	// 雑魚敵を消す.
	pActor->SetDelete( [&]( bool& isDead )
	{
		isDead = true;
	});
}

// 特殊な敵2と爆発の当たり判定.
void CSpecialEnemy_1::ExplosionSEnemy2Collsion( CActor * pActor )
{
	// オブジェクトのタグが特殊な敵2じゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_2 ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// 攻撃状態じゃなければ終了.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// 死亡していたら終了.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// 怯み状態なら終了.

	// 爆発と特殊な敵2の当たり判定.
	if ( coll::IsSphereToSphere( m_pExplosion->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	// 特殊な敵2を消す.
	pActor->SetDelete( [&]( bool& isDead )
	{
		isDead = true;
	});
}

// デバック更新関数.
void CSpecialEnemy_1::DebugUpdate()
{
#ifdef _DEBUG
	CDebugText::PushText("SpecialEnemy_1", "---------------------------------------------");
	CDebugText::PushText("SpecialEnemy_1", "--------------- SpecialEnemy_1 --------------");
	CDebugText::PushText("SpecialEnemy_1", "---------------------------------------------");
	CDebugText::PushText("SpecialEnemy_1", "Pos : ", m_Tranceform.Position.x, ", ", m_Tranceform.Position.y, ", ", m_Tranceform.Position.z);
	CDebugText::PushText("SpecialEnemy_1", "Rot : ", m_Tranceform.Rotation.x, ", ", m_Tranceform.Rotation.y, ", ", m_Tranceform.Rotation.z);
	CDebugText::PushText("SpecialEnemy_1", "'T' IsDelete = false, 'G' IsDelete = true");
	CDebugText::PushText("SpecialEnemy_1", "IsDelete : ", m_IsDelete == true ? "true" : "false");
	CDebugText::PushText("SpecialEnemy_1", "m_IsExplosion : ", m_pExplosion->GetIsExplosion() == true ? "true" : "false");
#endif
}
