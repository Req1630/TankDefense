#include "EnemyBase.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\Player\Player.h"

CEnemyBase::CEnemyBase()
	: m_pSkinMesh				( nullptr )
	, m_pPlayer					( nullptr )
	
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_pos						( 0.0f, 0.0f, 0.0f )
	, m_Rot						( 0.0f, 0.0f, 0.0f )
	, m_MoveVec3				( 0.0f, 0.0f, 0.0f )
	, m_NowState				( Enemy::EEnemyState::None )
	, m_NowMoveState			( Enemy::EMoveState::None )
	, m_Life					( 0.0f )
	, m_MoveSpeed				( 1.0f )
	, m_DeathCount				( 0.0f )
	, m_FrightCount				( 0 )
	, m_HitCount				( 0 )
	, m_WaitCount				( 0 )
	, m_InvincibleCount			( 0 )
	, m_IsFirght				( false )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
{
	m_pPlayer = std::make_shared<CPlayer>();
}

CEnemyBase::~CEnemyBase()
{
}

// 初期化関数.
bool CEnemyBase::Init()
{
	return false;
}

// 更新関数.
void CEnemyBase::Update( const float & deltaTime )
{
	
}

// 描画関数.
void CEnemyBase::Render()
{
	m_pSkinMesh->Render();
}

// 相手座標の設定.
void CEnemyBase::SetTargetPos( CActor & actor )
{
	SetPlayerPos( actor );
}

// ベクトルの取得.
void CEnemyBase::SetCVector( const D3DXVECTOR3 & vec )
{
	if (m_NowState == Enemy::EEnemyState::Spawn) return;
	if (m_NowState == Enemy::EEnemyState::Death) return;
}

// 当たり判定関数.
void CEnemyBase::Collision( CActor * pActor )
{
}

// 当たり判定の初期化.
void CEnemyBase::InitCollision()
{
}

// 当たり判定の座標や、半径などの更新.
//	Update関数の最後に呼ぶ.
void CEnemyBase::UpdateCollision()
{
}

// 現在の状態の更新関数.
void CEnemyBase::CurrentStateUpdate()
{
	switch ( m_NowState )
	{
	case Enemy::EEnemyState::Spawn:
		this->Spawning();
		break;
	case Enemy::EEnemyState::Move:
		this->Move();
		break;
	case Enemy::EEnemyState::Fright:
		this->Fright();
		break;
	case Enemy::EEnemyState::Death:
		this->Death();
		break;
	default:
		break;
	}
}

// プレイヤーの座標を設定.
void CEnemyBase::SetPlayerPos( CActor & actor )
{
	if( m_NowMoveState == Enemy::EMoveState::Move ) return;

	// プレイヤーじゃなければ終了.
	if (actor.GetObjectTag() != EObjectTag::Player) return;
	m_TargetPosition = actor.GetPosition();	// プレイヤーの座標を取得.
	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f(
		m_Tranceform.Position.x - m_Tranceform.Position.x,
		m_Tranceform.Position.z - m_Tranceform.Position.z);

	// プレイヤーと一定の距離置いた座標を設定.
	m_TargetPosition.x += sinf(m_TargetRotation.y);
	m_TargetPosition.z += cosf(m_TargetRotation.y);
}

// 移動ベクトル設定関数.
void CEnemyBase::SetMoveVector( const D3DXVECTOR3 & targetPos )
{
	if (m_NowState == Enemy::EEnemyState::Fright) return;

	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f(
		m_Tranceform.Position.x - targetPos.x,
		m_Tranceform.Position.z - targetPos.z);

	D3DXVECTOR3 m_MoveVector;
	// 移動用ベクトルを取得.
	m_MoveVector.x = sinf(m_TargetRotation.y);
	m_MoveVector.z = cosf(m_TargetRotation.y);
}

// 目的の座標へ回転.
void CEnemyBase::TargetRotation()
{
	if (m_NowMoveState != Enemy::EMoveState::Rotation) return;

	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf(m_Tranceform.Rotation.y);
	myVector.z = cosf(m_Tranceform.Rotation.y);

	// 目的の座標へ向けて回転.
	if (CActor::TargetRotation( m_MoveVec3, 1.0f, TOLERANCE_RADIAN) == false ) return;
	m_Tranceform.Rotation.y = m_TargetRotation.y;					// ターゲットへの回転取得.
	m_BeforeMoveingPosition = m_Tranceform.Position;	// 現在の座標を記憶.
	m_NowMoveState = Enemy::EMoveState::Move;			// 移動状態へ遷移.
}

// 移動関数.
void CEnemyBase::VectorMove( const float & moveSpeed )
{
	if (m_NowMoveState != Enemy::EMoveState::Move) return;

	// ベクトルを使用して移動.
	m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
	m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;

	// 再度目的の座標を探すか比較.
	// 回転時に記憶した座標と現在の座標の距離が一定以上なら.
	if (D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition - m_Tranceform.Position)) >= 2.0f)
		m_NowMoveState = Enemy::EMoveState::Rotation;	// 回転状態へ移動.

	if (D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition - m_Tranceform.Position)) >= 1.0f) return;
	// 現在の座標と目的の座標の距離が一定以上なら.
	m_NowMoveState = Enemy::EMoveState::Wait;	// 待機状態へ遷移.
}

// 待機関数.
void CEnemyBase::WaitMove()
{
	if (m_NowMoveState != Enemy::EMoveState::Wait) return;
	m_WaitCount++;	// 待機カウント加算.
	if (m_WaitCount < 3*FPS) return;
	m_NowMoveState = Enemy::EMoveState::Rotation;	// 移動状態を回転する.
	m_WaitCount = 0;	// 待機カウントの初期化.
}

// スポーン中.
void CEnemyBase::Spawning()
{
	m_NowState = Enemy::EEnemyState::Move;
}

// 移動.
void CEnemyBase::Move()
{
	TargetRotation();			// 回転.
	CEnemyBase::VectorMove(m_MoveSpeed);	// 移動.
	CEnemyBase::WaitMove();			// 待機.

	//if (*m_pIsAlienOtherAbduct == false) return;
	//if (m_NowState == Enemy::EAlienState::Abduct) return;
	//SetAnimation(alien::EAnimNo_Move, m_pAC);
	//m_NowState = Enemy::EAlienState::Escape;
	m_NowMoveState = Enemy::EMoveState::Rotation;	// 移動状態を回転する.
}

// 怯み.
void CEnemyBase::Fright()
{
	if (m_IsFirght == false) return;
	m_FrightCount++;
	const float FRIGHT_SPEED = 0.15f;
	m_Tranceform.Position.x -= sinf(static_cast<float>(D3DX_PI)		* static_cast<float>(m_FrightCount)) * FRIGHT_SPEED;
	m_Tranceform.Position.z -= sinf(static_cast<float>(D3DX_PI)*0.5f	* static_cast<float>(m_FrightCount)) * FRIGHT_SPEED;


	if (m_FrightCount <= (1*0.5f)*FPS) return;

	m_IsFirght = false;
	m_FrightCount = 0;
}

// 死亡.
void CEnemyBase::Death()
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
