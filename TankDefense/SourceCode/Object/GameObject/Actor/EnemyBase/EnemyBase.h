#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"

class CDX9SkinMesh;
class CPlayer;

namespace Enemy		//状態の格納庫.
{
	// 敵の状態.
	enum class enEnemyState : unsigned char
	{
		None,

		Spawn,
		Move,
		Fright,
		Death,

		Max,
	} typedef EEnemyState;

	// 移動状態.
	enum class enMovState : unsigned char
	{
		None,

		Rotation,
		Move,
		Attack,
		Wait,

		Max,
	} typedef EMoveState;
}

class CEnemyBase : public CActor
{
protected:
	inline static const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.

public:
	// エネミーパラメータ.
	struct stParameter
	{
		float	LifeMax;		// HP.
		float	AttackPower;	// 攻撃力.
		float	MoveSpeed;		// 移動速度.
		stParameter()
			: LifeMax		( 1.0f )
			, AttackPower	( 0.5f )
			, MoveSpeed		( 0.5f )
		{}
	} typedef SParameter;

public:
	CEnemyBase();
	~CEnemyBase();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update( const float& deltaTime ) override;
	// 描画関数.
	virtual void Render() override;

	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor );
	// ベクトルの取得.
	virtual void SetCVector( const D3DXVECTOR3& vec );

	// スポーンの設定.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) = 0;

	// 当たり判定関数.
	virtual void Collision( CActor* pActor );

	// 当たり判定の初期化.
	virtual void InitCollision();
	// 当たり判定の座標や、半径などの更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision();

protected:
	// 現在の状態の更新関数.
	void CurrentStateUpdate();
	// プレイヤーの座標を設定.
	void SetPlayerPos( CActor& actor );
	//// 座標設定関数.
	//virtual void SetPosition( const D3DXVECTOR3& vPos ) override;

	// 移動ベクトル設定関数.
	virtual void SetMoveVector( const D3DXVECTOR3& targetPos );
	// 目的の座標へ回転.
	void TargetRotation();
	// 移動関数.
	virtual void VectorMove( const float& moveSpeed );
	// 待機関数.
	virtual void WaitMove();

	// スポーン中.
	virtual void Spawning();
	// 移動.
	virtual void Move();
	// 怯み.
	virtual void Fright();
	// 死亡.
	virtual void Death();

protected:
	CDX9SkinMesh*		m_pSkinMesh;

	std::shared_ptr<CPlayer>	m_pPlayer;
	D3DXVECTOR3			m_TargetPosition;			// 女の子の座標.
	D3DXVECTOR3			m_TargetRotation;			// 目標の回転情報.
	D3DXVECTOR3			m_BeforeMoveingPosition;	// 移動前の座標.
	D3DXVECTOR3			m_pos;
	D3DXVECTOR3			m_Rot;
	D3DXVECTOR3			m_MoveVec3;
	Enemy::EEnemyState	m_NowState;					// 現在の状態.
	Enemy::EMoveState	m_NowMoveState;				// 現在の移動状態.
	float				m_Life;						// HP.
	float				m_MoveSpeed;				// 移動速度.
	float				m_DeathCount;				// 死亡カウント.
	int					m_FrightCount;				// 怯みカウント.
	int					m_HitCount;					// ヒットカウント.
	int					m_WaitCount;				// 待機カウント.
	int					m_InvincibleCount;			// 無敵カウント.
	bool				m_IsFirght;					// 怯み状態か.
	bool				m_IsExplosion;				// 爆発するか.
	bool				m_IsDelete;					// 消去するかどうか.
};
#endif // !ENEMYBASE_H