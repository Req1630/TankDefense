#include "Enemy.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\Bullet\EnemyBullet\EnemyBullet.h"

CEnemy::CEnemy()
	: m_pEnemyBase		()
	, m_pEnemyBullet	()
	, m_HasAimPlayer	( false )
	, m_OldHasAimPlayer	( false )

{
	m_pEnemyBullet = std::make_shared<CEnemyBullet>();
	m_ObjectTag = EObjectTag::Enemy;
	Init();
}

CEnemy::~CEnemy()
{
}

// 初期化関数.
bool CEnemy::Init()
{
	if ( m_pEnemyBullet->Init() == false ) return false;	// 雑魚敵の弾の初期化.

	m_pSkinMesh = CMeshResorce::GetSkin( "b_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	
	Spawn( D3DXVECTOR3( 0.0f, -2.5f ,5.0f ) );
	m_IsDelete = false;

	InitCollision();	// 当たり判定の初期化.
	
	return true;
}

// 更新関数.
void CEnemy::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;

	CurrentStateUpdate();	// 現在の状態更新.
	// スポーン.
	if ( CKeyInput::IsMomentPress( 'R' ) == true ) m_IsDelete = false;
	// 死亡.
	if ( CKeyInput::IsMomentPress( 'F' ) == true ) m_IsDelete = true;
	// 怯み.
	if ( CKeyInput::IsMomentPress( 'T' ) == true ) m_IsFirght = true;

	//CDebugText::PushText( "Enemy", "---------------------------------------------" );
	//CDebugText::PushText( "Enemy", "-------------------- Enemy ------------------" );
	//CDebugText::PushText( "Enemy", "---------------------------------------------" );
	//CDebugText::PushText( "Enemy", "Pos : ", m_Tranceform.Position.x, ", ", m_Tranceform.Position.y, ", ", m_Tranceform.Position.z );
	//CDebugText::PushText( "Enemy", "Rot : ", m_Tranceform.Rotation.x, ", ", m_Tranceform.Rotation.y, ", ", m_Tranceform.Rotation.z );
	//CDebugText::PushText( "Enemy", "'R' IsDelete = false, 'F' IsDelete = true" );
	//CDebugText::PushText( "Enemy", "IsDelete : ", m_IsDelete == true ? "true" : "false" );
	//CDebugText::PushText( "Enemy", "m_HasAimPlayer : ", m_HasAimPlayer == true ? "true" : "false" );
	//CDebugText::PushText( "Enemy", "m_ShotFlg : ", m_pEnemyBullet->GetShotflg() == true ? "true" : "false" );

	UpdateCollision();	// 当たり判定の更新.
}

// 描画関数.
void CEnemy::Render()
{
	// 画面の外なら終了.
	if ( IsDisplayOut() == true ) return;

	m_pSkinMesh->SetPosition( m_Tranceform.Position );
	m_pSkinMesh->SetRotation( m_Tranceform.Rotation );
	m_pSkinMesh->SetScale( m_Tranceform.Scale );
	if ( m_IsDelete == false ) m_pSkinMesh->Render();
	m_pEnemyBullet->Render();
}

// 当たり判定関数.
void CEnemy::Collision( CActor* pActor )
{
	if ( pActor == nullptr ) return;
	if ( m_pCollisions == nullptr ) return;
	if ( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;
	if ( m_pEnemyBullet->GetCollision<CSphere>() == nullptr ) return;

	PlayerCollsion( pActor );
	SpecialEnemy_1Collsion( pActor );
	SpecialEnemy_2Collsion( pActor );
	//EnemyBulletCollsion( pActor );
}

// 当たり判定の初期化.
void CEnemy::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// 当たり判定の座標や、半径などの更新.
//	Update関数の最後に呼ぶ.
void CEnemy::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( D3DXVECTOR3( m_Tranceform.Position.x, m_Tranceform.Position.y + 4.0f, m_Tranceform.Position.z ) );
}

// スポーン.
bool CEnemy::Spawn( const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if ( m_NowState != Enemy::EEnemyState::None ) return true;
	m_Tranceform.Position = spawnPos;			// スポーン座標の設定.
	//m_LifePoint = pPARAMETER->LifeMax;		// 体力の設定.
	m_NowState = Enemy::EEnemyState::Spawn;		// 現在の状態をスポーンに変更.
	return true;
}

// 相手座標の設定.
void CEnemy::SetTargetPos( CActor& actor )
{
	// プレイヤーの座標を取得.
	CEnemyBase::SetTargetPos( actor );
}

// エネミー座標の設定.
void CEnemy::SetEnemyPos( CActor & actor )
{
	// エネミーじゃなければ終了.
	if ( actor.GetObjectTag() != EObjectTag::Enemy ) return;
	m_pEnemyBullet->SetTargetPos( actor );		// エネミーの座標を取得.
}

// スポーン中.
void CEnemy::Spawning()
{
	CEnemyBase::Spawning();
}

// 移動.
void CEnemy::Move()
{
	SearchRange();	// プレイヤーを狙う索敵判定.
	// プレイヤーを追跡.
	if ( m_HasAimPlayer == true ) {
		TargetRotation( m_MoveSpeed );							// 回転.
		CEnemyBase::VectorMove( m_MoveSpeed, ATTACK_LENGTH );	// 移動.
		Attack();												// 攻撃.
		CEnemyBase::WaitMove();									// 待機.
	}
	// プレイヤーを見失った.
	else {
		if ( m_IsRand == false ) {
			Rand_Pos();		// ランダムに座標を決める.
		}
		else {
			RandTargetRotation( m_MoveSpeed );	// 回転.
			RandVectorMove( m_MoveSpeed );		// 移動.
			CEnemyBase::WaitMove();				// 待機.
		}
	}
}

// 怯み.
void CEnemy::Fright()
{
	CEnemyBase::Fright();
}

// 死亡.
void CEnemy::Death()
{
	CEnemyBase::Death();
}

// 攻撃関数.
void CEnemy::Attack()
{
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;

	// 回転の補正をかける為に必要.
	SetMoveVector( m_TargetPosition );
	// 弾の大きさの初期化.
	if ( m_pEnemyBullet->GetShotflg() == false ) m_pEnemyBullet->SetBulletScale();

	// 弾の大きさの拡大.
	m_pEnemyBullet->Update( m_DeltaTime );
	// 弾が大きくなっている間は、回転の補正をかける.
	if ( m_pEnemyBullet->GetScale().x < 0.8f ) {
		m_pEnemyBullet->SetBulletPos( m_Tranceform.Position, m_MoveVec3 );
		if( CActor::TargetRotation( m_MoveVec3, 0.1f, TOLERANCE_RADIAN, 0.0f ) == false ) return;
	}

	// 弾が一定距離飛ぶと消える処理.
	if (D3DXVec3Length( &D3DXVECTOR3( m_pEnemyBullet->GetPosition() - m_Tranceform.Position ) ) >= 10.0f ) {
		m_pEnemyBullet->Setshotflg( false );
	}

	// 攻撃を行ったら待機状態に遷移する.
	if ( m_pEnemyBullet->GetShotflg() == true ) return;
	m_NowMoveState = Enemy::EMoveState::Wait;
}

// プレイヤーとの当たり判定.
void CEnemy::PlayerCollsion( CActor* pActor )
{
	// オブジェクトのタグがプレイヤーじゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// 雑魚敵とプレイヤーの当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// 雑魚敵とプレイヤーの距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// 雑魚敵の刺さっている長さ

	D3DXVECTOR3 vecPushOut;// 押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );// Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0] -= vecPushOut * inDistance / 12;
	Pos[1] -= -vecPushOut * inDistance / 12;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// 特殊な敵1との当たり判定.
void CEnemy::SpecialEnemy_1Collsion( CActor * pActor )
{
	// オブジェクトのタグが特殊な敵1じゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_1 ) return;

	// 雑魚敵と特殊な敵1の当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	// 半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// 雑魚敵と特殊な敵1の距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// 雑魚敵の刺さっている長さ

	D3DXVECTOR3 vecPushOut;// 押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );// Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0] -= vecPushOut * inDistance / 12;
	Pos[1] -= -vecPushOut * inDistance / 12;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// 特殊な敵2との当たり判定.
void CEnemy::SpecialEnemy_2Collsion( CActor * pActor )
{
	// オブジェクトのタグが特殊な敵2じゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_2 ) return;

	// 雑魚敵と特殊な敵2の当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	// 半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// 雑魚敵と特殊な敵2の距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// 雑魚敵の刺さっている長さ

	D3DXVECTOR3 vecPushOut;// 押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );// Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0] -= vecPushOut * inDistance / 12;
	Pos[1] -= -vecPushOut * inDistance / 12;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// プレイヤーと雑魚敵の弾の当たり判定.
void CEnemy::EnemyBulletCollsion( CActor * pActor )
{
	// オブジェクトのタグがプレイヤーじゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if ( m_NowMoveState != Enemy::EMoveState::Attack ) return;	// 攻撃状態じゃなければ終了.
	if ( m_NowState == Enemy::EEnemyState::Death ) return;	// 死亡していたら終了.
	if ( m_NowState == Enemy::EEnemyState::Fright ) return;	// 怯み状態なら終了.

	// 雑魚敵の弾とプレイヤーの当たり判定.
	if ( coll::IsSphereToSphere( m_pEnemyBullet->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

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

// 索敵範囲.
void CEnemy::SearchRange()
{
	if ( m_NowMoveState == Enemy::EMoveState::Attack ) return;

	// プレイヤーとの距離を計算.
	float PlayerLength = D3DXVec3Length( &D3DXVECTOR3( m_TargetPosition - m_Tranceform.Position ) );

	m_OldHasAimPlayer = m_HasAimPlayer;
	// プレイヤーの狙う範囲を比較.
	if ( PlayerLength <= 30.0f ) {
		// 索敵範囲内にプレイヤーが居る場合.
		m_HasAimPlayer = true;
		m_IsRand = false;
		SetMoveVector( m_TargetPosition );
	}
	else {
		// 索敵範囲外の場合.
		m_HasAimPlayer = false;
		SetRandMoveVector( m_RandPos );
	}
	if ( m_OldHasAimPlayer == m_HasAimPlayer ) return;
	m_NowMoveState = Enemy::EMoveState::Wait;
}
