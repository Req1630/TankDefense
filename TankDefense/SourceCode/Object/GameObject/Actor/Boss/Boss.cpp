#include "Boss.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Utility\Input\Input.h"

CBoss::CBoss()
	: m_pSkinMesh				( nullptr )
	
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_MoveVec3				( 0.0f, 0.0f, 0.0f )
	, m_RandPos					( 0.0f, 0.0f, 0.0f )
	, m_OldRandPos				( 1.0f, 0.0f, 0.0f )
	, m_RandRot					( 0.0f, 0.0f, 0.0f )
	, m_NowState				( Boss::EBossState::None )
	, m_NowMoveState			( Boss::EMoveState::None )
	, m_Life					( 0.0f )
	, m_MoveSpeed				( 0.05f )
	, m_DeathCount				( 0.0f )
	, m_FrightCount				( 0 )
	, m_HitCount				( 0 )
	, m_WaitCount				( 0 )
	, m_InvincibleCount			( 0 )
	, m_RandAttack				( 0 )
	, m_AttackWave				( 0 )
	, m_AttackCount				( 0 )
	, m_IsFirght				( false )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
	, m_IsRandMove				( false )
	, m_IsRandAttack			( false )
	, m_HasAimPlayer			( false )
	, m_OldHasAimPlayer			( false )

{
	m_ObjectTag		= EObjectTag::Boss;
	Init();
}

CBoss::~CBoss()
{
}

// 初期化関数.
bool CBoss::Init()
{
	m_pSkinMesh = CMeshResorce::GetSkin( "dragon_kari_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	m_pSkinMesh->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	
	Spawn( D3DXVECTOR3( 0.0f, 0.0f ,-20.0f ) );
	m_IsDelete = false;

	InitCollision();	// 当たり判定の初期化.
	
	return true;
}

// 更新関数.
void CBoss::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;

	CurrentStateUpdate();	// 現在の状態更新.
	//// スポーン.
	//if ( CKeyInput::IsMomentPress( 'R' ) == true ) m_IsDelete = false;
	//// 死亡.
	//if ( CKeyInput::IsMomentPress( 'F' ) == true ) m_IsDelete = true;
	//// 怯み.
	//if ( CKeyInput::IsMomentPress( 'T' ) == true ) m_IsFirght = true;

	DebugUpdate();			// デバック更新.

	UpdateCollision();	// 当たり判定の更新.
}

// 描画関数.
void CBoss::Render()
{
	// 画面の外なら終了.
	if ( IsDisplayOut() == true ) return;

	m_pSkinMesh->SetPosition( m_Tranceform.Position );
	m_pSkinMesh->SetRotation( m_Tranceform.Rotation );
	m_pSkinMesh->SetScale( m_Tranceform.Scale );
	if ( m_IsDelete == false ) m_pSkinMesh->Render();
}

// 当たり判定関数.
void CBoss::Collision( CActor* pActor )
{
	if ( pActor == nullptr ) return;
	if ( m_pCollisions == nullptr ) return;
	if ( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;

	PlayerCollsion( pActor );
	EnemyCollsion( pActor );
	SpecialEnemy_1Collsion( pActor );
	SpecialEnemy_2Collsion( pActor );
}

// 当たり判定の初期化.
void CBoss::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollNo::Sphere );
	m_pCollisions->GetCollision<CSphere>()->SetRadius( 1.5f );
}

// 当たり判定の座標や、半径などの更新.
//	Update関数の最後に呼ぶ.
void CBoss::UpdateCollision()
{
	m_pCollisions->GetCollision<CSphere>()->SetPosition( D3DXVECTOR3( m_Tranceform.Position.x, m_Tranceform.Position.y, m_Tranceform.Position.z ) );
}

// スポーン.
bool CBoss::Spawn( const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if ( m_NowState != Boss::EBossState::None ) return true;
	m_Tranceform.Position = spawnPos;			// スポーン座標の設定.
	//m_LifePoint = pPARAMETER->LifeMax;		// 体力の設定.
	m_NowState = Boss::EBossState::Spawn;		// 現在の状態をスポーンに変更.
	return true;
}

// 相手座標の設定.
void CBoss::SetTargetPos( CActor& actor )
{
	SetPlayerPos( actor );
}

// 現在の状態の更新関数.
void CBoss::CurrentStateUpdate()
{
	switch ( m_NowState )
	{
	case Boss::EBossState::Spawn:
		this->Spawning();
		break;
	case Boss::EBossState::Move:
		this->Move();
		break;
	case Boss::EBossState::Attack:
		this->Attack();
		break;
	case Boss::EBossState::Fright:
		this->Fright();
		break;
	case Boss::EBossState::Death:
		this->Death();
		break;
	default:
		break;
	}
	this->Fright();
}

// プレイヤーの座標を設定.
void CBoss::SetPlayerPos( CActor& actor )
{
	//if( m_NowMoveState == Enemy::EMoveState::Move ) return;

	// プレイヤーじゃなければ終了.
	if ( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_TargetPosition = actor.GetPosition();	// プレイヤーの座標を取得.
	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f(
		m_Tranceform.Position.x - m_TargetPosition.x,
		m_Tranceform.Position.z - m_TargetPosition.z );
}

// 移動ベクトル設定関数.
void CBoss::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	if ( m_NowState == Boss::EBossState::Fright ) return;

	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f(
		m_Tranceform.Position.x - targetPos.x,
		m_Tranceform.Position.z - targetPos.z );

	// 移動用ベクトルを取得.
	m_MoveVec3.x = sinf( m_TargetRotation.y );
	m_MoveVec3.z = cosf( m_TargetRotation.y );
}

// 目的の座標へ回転.
void CBoss::TargetRotation( const float& moveSpeed )
{
	if ( m_NowState == Boss::EBossState::Fright ) return;
	if ( m_NowMoveState != Boss::EMoveState::Rotation ) return;

	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_Tranceform.Rotation.y );
	myVector.z = cosf( m_Tranceform.Rotation.y );

	// 目的の座標へ向けて回転.
	if ( CActor::TargetRotation( m_MoveVec3, 0.05f, TOLERANCE_RADIAN, moveSpeed ) == false ) return;
	m_Tranceform.Rotation.y = m_TargetRotation.y;		// ターゲットへの回転取得.
	m_BeforeMoveingPosition = m_Tranceform.Position;	// 現在の座標を記憶.
	m_NowMoveState = Boss::EMoveState::Move;			// 移動状態へ遷移.
}

// ランダム移動ベクトル関数.
void CBoss::SetRandMoveVector( const D3DXVECTOR3 & targetPos )
{
	if ( m_NowState == Boss::EBossState::Fright ) return;

	// 目的の回転軸を取得.
	m_RandRot.y = atan2f(
		m_Tranceform.Position.x - targetPos.x,
		m_Tranceform.Position.z - targetPos.z );

	// 移動用ベクトルを取得.
	m_MoveVec3.x = sinf( m_RandRot.y );
	m_MoveVec3.z = cosf( m_RandRot.y );
}

// ランダムの目的の座標へ回転.
void CBoss::RandTargetRotation( const float& moveSpeed )
{
	if ( m_NowMoveState != Boss::EMoveState::Rotation ) return;

	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_Tranceform.Rotation.y );
	myVector.z = cosf( m_Tranceform.Rotation.y );

	// 目的の座標へ向けて回転.
	if ( CActor::TargetRotation( m_MoveVec3, 0.05f, TOLERANCE_RADIAN, moveSpeed ) == false ) return;
	m_Tranceform.Rotation.y = m_RandRot.y;			// ターゲットへの回転取得.
	m_NowMoveState = Boss::EMoveState::Move;		// 移動状態へ遷移.
}

// 移動関数.
void CBoss::VectorMove( const float& moveSpeed, const float& length )
{
#if 1
	if ( m_NowMoveState != Boss::EMoveState::Move ) return;

	// 目的の座標へ向けて回転.
	if ( CActor::TargetRotation( m_MoveVec3, 0.05f, TOLERANCE_RADIAN, moveSpeed ) == false ) return;
	m_Tranceform.Rotation.y = m_TargetRotation.y;		// ターゲットへの回転取得.

	// プレイヤーの座標と敵の座標を比較.
	// 敵を一定距離から前に進めさせないため.
	if ( D3DXVec3Length( &D3DXVECTOR3( m_TargetPosition - m_Tranceform.Position ) ) >= length ) {
		// ベクトルを使用して移動.
		m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
		m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;
	}

	// プレイヤーの座標と敵の座標を比較.
	if ( D3DXVec3Length( &D3DXVECTOR3( m_TargetPosition - m_Tranceform.Position ) ) >= length ) return;
	m_NowMoveState = Boss::EMoveState::Attack;

#else
	if (m_NowMoveState != Enemy::EMoveState::Move) return;

	// ベクトルを使用して移動.
	m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
	m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;

	// 再度目的の座標を探すか比較.
	// 回転時に記憶した座標と現在の座標の距離が一定以上なら.
	if (D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition - m_Tranceform.Position)) >= 10.0f)
		m_NowMoveState = Enemy::EMoveState::Rotation;	// 回転状態へ移動.

	if (D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition - m_Tranceform.Position)) >= 10.0f) return;
	// 現在の座標と目的の座標の距離が一定以上なら.
	m_NowMoveState = Enemy::EMoveState::Wait;	// 待機状態へ遷移.
#endif
}

// 待機関数.
void CBoss::WaitMove()
{
	if (m_NowMoveState != Boss::EMoveState::Wait) return;
	m_WaitCount++;	// 待機カウント加算.
	if (m_WaitCount < 3.0*FPS) return;
	m_NowMoveState = Boss::EMoveState::Rotation;	// 移動状態を回転する.
	m_WaitCount = 0;	// 待機カウントの初期化.
}

// スポーン中.
void CBoss::Spawning()
{
	m_NowState = Boss::EBossState::Move;
	m_NowMoveState = Boss::EMoveState::Rotation;
}

// 移動.
void CBoss::Move()
{
	SearchRange();	// プレイヤーを狙う索敵判定.
	// プレイヤーを追跡.
	if ( m_HasAimPlayer == true ) {
		TargetRotation( m_MoveSpeed );				// 回転.
		VectorMove( m_MoveSpeed, ATTACK_LENGTH );	// 移動.
		Attack();									// 攻撃.
		WaitMove();									// 待機.
	}
	// プレイヤーを見失った.
	else {
		if ( m_IsRandMove == false ) {
			Rand_Pos();		// ランダムに座標を決める.
		}
		else {
			RandTargetRotation( m_MoveSpeed );	// 回転.
			RandVectorMove( m_MoveSpeed );		// 移動.
			WaitMove();							// 待機.
		}
	}
}

// 攻撃.
void CBoss::Attack()
{
	if ( m_NowMoveState != Boss::EMoveState::Attack ) return;
	if ( m_IsRandAttack != true ) Rand_Attack();
	m_IsRandAttack = 2;
	switch ( m_RandAttack )
	{
	case 1:		// 踏みつけ攻撃.
		if ( m_AttackWave == 0 ) {
			if ( m_Tranceform.Position.y <= 4.0f ) {
				m_Tranceform.Position.y += 0.1f;

				if ( m_TargetPosition != m_Tranceform.Position ) {
					// ベクトルを使用して移動.
					m_Tranceform.Position.x -= m_MoveVec3.x * 0.1f;
					m_Tranceform.Position.z -= m_MoveVec3.z * 0.1f;
				}
			}
			else {
				m_AttackWave++;
			}
		}

		if ( m_AttackWave == 1 ) {
			if ( m_Tranceform.Position.y >= 0.0f ) {
				m_Tranceform.Position.y -= 0.1f;
			}
			else {
				m_AttackWave++;
			}
		}

		if ( m_AttackWave >= 2 ) {
			m_IsRandAttack = false;
		}

		break;
	case 2:		// 尻尾薙ぎ払い攻撃.
		if ( m_AttackWave == 0 ) {
			if ( m_Tranceform.Rotation.y >= -10.0f ) {
				m_Tranceform.Rotation.y -= 0.1f;
			}
			else {
				m_AttackWave++;
			}
		}

		if (m_AttackWave >= 1) {
			m_IsRandAttack = false;
		}

		break;
	case 3:		// ブレス攻撃.
		m_IsRandAttack = false;
		break;
	case 4:		// 火球攻撃.
		m_IsRandAttack = false;
		break;
	default:
		break;
	}

	// 攻撃を行ったら待機状態に遷移する.
	if ( m_IsRandAttack == true ) return;
	m_AttackCount++;	// 攻撃を行ったら一回カウントする.
	m_NowMoveState = Boss::EMoveState::Wait;
}

// 怯み.
void CBoss::Fright()
{
	if ( m_IsFirght == false ) return;
	m_FrightCount++;
	const float FRIGHT_SPEED = 0.15f;
	m_Tranceform.Position.x -= sinf( static_cast<float>( D3DX_PI )		* static_cast<float>( m_FrightCount ) ) * FRIGHT_SPEED;
	m_Tranceform.Position.z -= sinf( static_cast<float>( D3DX_PI )*0.5f	* static_cast<float>( m_FrightCount ) ) * FRIGHT_SPEED;


	if ( m_FrightCount <= ( 1*0.5f )*FPS ) return;

	m_IsFirght = false;
	m_FrightCount = 0;
}

// 死亡.
void CBoss::Death()
{
	//if (m_NowAnimNo != alien::EAnimNo_Dead) {
	//	m_NowAnimNo = alien::EAnimNo_Dead;
	//	m_AnimSpeed = DEFAULT_ANIM_SPEED;
	//}
	//if (m_AnimFrameList[alien::EAnimNo_Dead].IsNowFrameOver() == false) return;

	//m_AnimSpeed = 0.0;
	//m_DeathCount += pPARAMETER->DeadCountAddValue;
	//m_DeathScale -= pPARAMETER->DeadScaleSubValue;
	//// モデルのサイズの計算.
	//const float scale = m_DeathScale * exp(-m_DeathCount)*sinf(DEATH_SCALE_PI*m_DeathCount);
	//m_vScale = { scale, scale, scale };

	//// 大きさが一定値以上なら.
	//if (m_vScale.x > SCALE_MIN) return;
	//m_vScale = { SCALE_MIN, SCALE_MIN, SCALE_MIN };
	//CSoundManager::PlaySE("AlienDead");
	//m_IsDelete = true;	// 死亡フラグを立てる.

}

// 攻撃方法をランダムで決める関数.
void CBoss::Rand_Attack()
{
	//乱数を生成するためのコード.
	std::random_device rnd;
	std::mt19937_64 mt( rnd() );

	//乱数の範囲を決めるコード.
	std::uniform_int_distribution<> rand_Attack( 1, 4 );

	m_RandAttack = rand_Attack( mt );

	m_AttackWave = 0;
	m_IsRandAttack = true;
}

// ランダムに座標を取る関数.
void CBoss::Rand_Pos()
{
	//乱数を生成するためのコード.
	std::random_device rnd_x;
	std::random_device rnd_z;
	std::mt19937_64 mt_x( rnd_x() );
	std::mt19937_64 mt_z( rnd_z() );

	//乱数の範囲を決めるコード.
	std::uniform_int_distribution<> rand_Pos( -30, 30 );

	m_RandPos.x = rand_Pos( mt_x );
	m_RandPos.z = rand_Pos( mt_z );

	m_OldRandPos = m_RandPos;
	m_IsRandMove = true;
}

// ランダム移動関数.
void CBoss::RandVectorMove( const float & moveSpeed )
{
	if ( m_NowMoveState != Boss::EMoveState::Move ) return;

	// ランダム座標との距離を計算.
	float RandPosLength = D3DXVec3Length( &D3DXVECTOR3( m_RandPos - m_Tranceform.Position ) );

	// ベクトルを使用して移動.
	m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
	m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;

	if ( RandPosLength <= 0.1f ) {
		m_IsRandMove = false;
		m_NowMoveState = Boss::EMoveState::Wait;	// 待機状態へ遷移.
	}
}

// プレイヤーとの当たり判定.
void CBoss::PlayerCollsion( CActor* pActor )
{
	// オブジェクトのタグがプレイヤーじゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// ボスとプレイヤーの当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// ボスとプレイヤーの距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// ボスの刺さっている長さ

	D3DXVECTOR3 vecPushOut;// 押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );// Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// 雑魚敵との当たり判定.
void CBoss::EnemyCollsion( CActor * pActor )
{
	// オブジェクトのタグが雑魚敵じゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::Enemy ) return;

	// ボスと雑魚敵の当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	//半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// ボスと雑魚敵の距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// ボスの刺さっている長さ

	D3DXVECTOR3 vecPushOut;// 押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );// Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// 特殊な敵1との当たり判定.
void CBoss::SpecialEnemy_1Collsion( CActor * pActor )
{
	// オブジェクトのタグが特殊な敵1じゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_1 ) return;

	// ボスと特殊な敵1の当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	// 半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// ボスと特殊な敵1の距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// ボスの刺さっている長さ

	D3DXVECTOR3 vecPushOut;// 押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );// Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// 特殊な敵2との当たり判定.
void CBoss::SpecialEnemy_2Collsion( CActor * pActor )
{
	// オブジェクトのタグが特殊な敵2じゃなければ終了.
	if ( pActor->GetObjectTag() != EObjectTag::SpecialEnemy_2 ) return;

	// ボスと特殊な敵2の当たり判定の実験.
	if ( coll::IsSphereToSphere( m_pCollisions->GetCollision<CSphere>(), pActor->GetCollision<CSphere>() ) == false ) return;

	float Radius = pActor->GetCollision<CSphere>()->GetRadius();	// 半径.

	D3DXVECTOR3 Pos[2]{ m_Tranceform.Position, pActor->GetPosition() };

	D3DXVECTOR3 Distance = Pos[0] - Pos[1];// ボスと特殊な敵2の距離（vector）

	float inDistance = ( Radius )-( D3DXVec3Length( &Distance ) );// ボスの刺さっている長さ

	D3DXVECTOR3 vecPushOut;// 押し出すvector
	D3DXVec3Normalize( &vecPushOut, &Distance );// Ｐ0 - Ｐ1の正規化ﾍﾞｸﾄﾙ 

	// ﾍﾞｸﾄﾙに長さをかけて刺さった分お互いを押し戻す
	Pos[0].x -= vecPushOut.x * inDistance / 8;
	Pos[0].z -= vecPushOut.z * inDistance / 8;
	Pos[1].x -= -vecPushOut.x * inDistance / 8;
	Pos[1].z -= -vecPushOut.z * inDistance / 8;

	//m_Tranceform.Position = Pos[0];
	pActor->SetPosition( Pos[1] );
}

// 索敵範囲.
void CBoss::SearchRange()
{
	if ( m_NowMoveState == Boss::EMoveState::Attack ) return;

	// プレイヤーとの距離を計算.
	float PlayerLength = D3DXVec3Length( &D3DXVECTOR3( m_TargetPosition - m_Tranceform.Position ) );

	m_OldHasAimPlayer = m_HasAimPlayer;
	// プレイヤーの狙う範囲を比較.
	if ( PlayerLength <= 30.0f ) {
		// 索敵範囲内にプレイヤーが居る場合.
		m_HasAimPlayer = true;
		m_IsRandMove = false;
		SetMoveVector( m_TargetPosition );
	}
	else {
		// 索敵範囲外の場合.
		m_HasAimPlayer = false;
		SetRandMoveVector( m_RandPos );
	}
	if ( m_OldHasAimPlayer == m_HasAimPlayer ) return;
	m_NowMoveState = Boss::EMoveState::Wait;
}

// デバック更新関数.
void CBoss::DebugUpdate()
{
#ifdef _DEBUG
	CDebugText::PushText("Boss", "---------------------------------------------");
	CDebugText::PushText("Boss", "-------------------- Boss ------------------");
	CDebugText::PushText("Boss", "---------------------------------------------");
	CDebugText::PushText("Boss", "Pos : ", m_Tranceform.Position.x, ", ", m_Tranceform.Position.y, ", ", m_Tranceform.Position.z);
	CDebugText::PushText("Boss", "Rot : ", m_Tranceform.Rotation.x, ", ", m_Tranceform.Rotation.y, ", ", m_Tranceform.Rotation.z);
	CDebugText::PushText("Boss", "'R' IsDelete = false, 'F' IsDelete = true");
	CDebugText::PushText("Boss", "IsDelete : ", m_IsDelete == true ? "true" : "false");
	CDebugText::PushText("Boss", "m_HasAimPlayer : ", m_HasAimPlayer == true ? "true" : "false");
#endif
}


// 実験でボスを消す関数.
void CBoss::SetDelete( const std::function<void( bool& )>& proc )
{
	m_IsDelete = false;
	proc( m_IsDelete );
}
