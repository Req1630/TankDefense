#include "Enemy.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Player\Player.h"

CEnemy::CEnemy()
	:m_pEnemyBase		()
	, m_vPlayerPos		( 0.0f, 0.0f, 0.0f )
	, m_HasAimPlayer	( false )
	, m_OldHasAimPlayer	( false )

{
	Init();
}

CEnemy::~CEnemy()
{
}

// 初期化関数.
bool CEnemy::Init()
{
	m_pSkinMesh = CMeshResorce::GetSkin( "b_s" );
	m_pSkinMesh->SetAnimSpeed( GetDeltaTime<double>() );
	
	Spawn( D3DXVECTOR3( 0.0f, 0.0f ,0.0f ) );
	m_IsDelete = false;

	return true;
}

// 更新関数.
void CEnemy::Update( const float& deltaTime )
{
	SetTargetPos( *m_pPlayer.get() );
	CurrentStateUpdate();
	// スポーン.
	if (GetAsyncKeyState('R') & 0x0001)	m_IsDelete = false;

	// 死亡.
	if (GetAsyncKeyState('D') & 0x0001)	m_IsDelete = true;
}

// 描画関数.
void CEnemy::Render()
{
	// 画面の外なら終了.
	if( IsDisplayOut() == true ) return;

	if( m_IsDelete == false ) m_pSkinMesh->Render();
}

// 当たり判定関数.
void CEnemy::Collision( CActor * pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollisions == nullptr ) return;
	if( m_pCollisions->GetCollision<CSphere>() == nullptr ) return;
}

// スポーン.
bool CEnemy::Spawn( const D3DXVECTOR3 & spawnPos )
{
	// 既にスポーン済みなら終了.
	if (m_NowState != Enemy::EEnemyState::None) return true;
	//m_vScale = { SCALE_MIN, SCALE_MIN, SCALE_MIN };
	m_Tranceform.Position = spawnPos;			// スポーン座標の設定.
	//m_LifePoint = pPARAMETER->LifeMax;		// 体力の設定.
	m_NowState = Enemy::EEnemyState::Spawn;		// 現在の状態をスポーンに変更.
	//m_AnimSpeed = 0.0;						// アニメーション速度を止める.
	//m_pEffects[alien::EEffectNo_Spawn]->Play(m_vPosition);
	//m_pEffects[alien::EEffectNo_Spawn]->SetSpeed(0.5f);
	return true;
}

// 相手座標の設定.
void CEnemy::SetTargetPos( CActor& actor )
{
	// プレイヤーの座標を取得.
	CEnemyBase::SetTargetPos( actor );
	// プレイヤーの座標を取得.
	CEnemy::SetPlayerPos( actor );
}

// プレイヤー座標の設定.
void CEnemy::SetPlayerPos( CActor& actor )
{
	// プレイヤーじゃなければ終了.
	if (actor.GetObjectTag() != EObjectTag::Player) return;
	m_vPlayerPos = actor.GetPosition();	// プレイヤーの座標を取得.
}

// スポーン中.
void CEnemy::Spawning()
{
	CEnemyBase::Spawning();
}

// 移動.
void CEnemy::Move()
{
	AimPlayerDecision();	// プレイヤーを狙うか判定.
	TargetRotation();		// 回転.
	// プレイヤーを狙っているか.
	if (m_HasAimPlayer == true) {
		CEnemy::VectorMove(m_MoveSpeed);	// 移動.
		Attack();					// 攻撃.
		CEnemyBase::WaitMove();		// 待機.
	}
	else {
		CEnemyBase::VectorMove(m_MoveSpeed);		// 移動.
		CEnemyBase::WaitMove();		// 待機.
	}
	if (m_NowMoveState == Enemy::EMoveState::Move) {
		//SetAnimation(alien::EAnimNo_Move, m_pAC);
	}

	//if (*m_pIsAlienOtherAbduct == false) return;
	//if (m_NowState == alien::EAlienState::Abduct) return;
	//m_pEffects[alien::EEffectNo_Attack]->Stop();
	//SetAnimation(alien::EAnimNo_Move, m_pAC);
	//m_NowState = alien::EAlienState::Escape;	// 逃げる状態へ遷移.
	m_NowMoveState = Enemy::EMoveState::Rotation;	// 移動状態を回転へ遷移する.
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

// 移動関数.
void CEnemy::VectorMove( const float & moveSpeed )
{
	if (m_NowMoveState != Enemy::EMoveState::Move) return;

	// ベクトルを使用して移動.
	m_Tranceform.Position.x -= m_MoveVec3.x * moveSpeed;
	m_Tranceform.Position.z -= m_MoveVec3.z * moveSpeed;

	// プレイヤーの座標と宇宙人の座標を比較.
	if (D3DXVec3Length(&D3DXVECTOR3(m_vPlayerPos - m_Tranceform.Position)) >= 1.0f) return;
	//if (m_IsBarrierHit == true) return;
	m_NowMoveState = Enemy::EMoveState::Attack;
	//m_RotAccValue = pPARAMETER->AttackRotInitPower;
	//m_pEffects[alien::EEffectNo_Attack]->Play(m_vPosition);
}

// 攻撃関数.
void CEnemy::Attack()
{
}

// プレイヤーとの当たり判定.
void CEnemy::PlayerCollison( CActor * pActor )
{
}

// プレイヤーを狙うか判定.
void CEnemy::AimPlayerDecision()
{
	if (m_NowMoveState == Enemy::EMoveState::Attack) return;

	// プレイヤーとの距離を計算.
	float playerLenght = D3DXVec3Length(&D3DXVECTOR3(m_vPlayerPos - m_Tranceform.Position));

	m_OldHasAimPlayer = m_HasAimPlayer;
	// プレイヤーの狙う範囲を比較.
	if (playerLenght <= 3.0f) {
		// プレイヤーのほうが近いのでプレイヤーを狙う.
		m_HasAimPlayer = true;
		SetMoveVector(m_vPlayerPos);
	}
	//else {
	//	// 女の子のほうを狙う.
	//	m_HasAimPlayer = false;
	//	SetMoveVector(m_TargetPosition);
	//}
	if (m_OldHasAimPlayer == m_HasAimPlayer) return;
	m_NowMoveState = Enemy::EMoveState::Rotation;
}
