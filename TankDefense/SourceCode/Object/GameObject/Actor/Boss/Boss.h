#ifndef BOSS_H
#define BOSS_H

#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"
#include <time.h>
#include <random>
#include <vector>

class CDX9SkinMesh;

namespace Boss		//状態の格納庫.
{
	// 敵の状態.
	enum class enBossState : unsigned char
	{
		None,

		Spawn,
		Move,
		Attack,
		Fright,
		Death,

		Max,
	} typedef EBossState;

	// 移動状態.
	enum class enMoveState : unsigned char
	{
		None,

		Rotation,
		Move,
		Attack,
		Wait,

		Max,
	} typedef EMoveState;
}

class CBoss : public CActor
{
protected:
	inline static const float TOLERANCE_RADIAN			= static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.
	inline static const float ATTACK_LENGTH				= 10.0f;	// 攻撃をする長さ.
	inline static const float EXPLOSION_ATTACK_LENGTH	= 5.0f;		// 爆発攻撃をする長さ.
	inline static const float RESTRAINT_ATTACK_LENGTH	= 5.0f;		// 拘束攻撃をする長さ.

public:
	// ボスパラメータ.
	struct stParameter
	{
		float	LifeMax;		// HP.
		float	AttackPower;	// 攻撃力.
		float	MoveSpeed;		// 移動速度.
		stParameter()
			: LifeMax		( 1.0f )
			, AttackPower	( 0.5f )
			, MoveSpeed		( 0.05f )
		{}
	} typedef SParameter;

public:
	CBoss();
	virtual ~CBoss();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update( const float& deltaTime ) override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 当たり判定の初期化.
	virtual void InitCollision() override;
	// 当たり判定の座標や、半径などの更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

	// スポーンの設定.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos );

	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor );

private:
	// 現在の状態の更新関数.
	void CurrentStateUpdate();
	// プレイヤーの座標を設定.
	void SetPlayerPos( CActor& actor );

	// 移動ベクトル設定関数.
	virtual void SetMoveVector( const D3DXVECTOR3& targetPos );
	// 目的の座標へ回転.
	void TargetRotation( const float& moveSpeed );
	// ランダム移動ベクトル関数.
	void SetRandMoveVector( const D3DXVECTOR3& targetPos );
	// ランダムの目的の座標へ回転.
	void RandTargetRotation( const float& moveSpeed );
	// 移動関数.
	virtual void VectorMove( const float& moveSpeed, const float& length );
	// 待機関数.
	virtual void WaitMove();

	// スポーン中.
	virtual void Spawning();
	// 移動.
	virtual void Move();
	// 攻撃.
	virtual void Attack();
	// 怯み.
	virtual void Fright();
	// 死亡.
	virtual void Death();

	// 攻撃方法をランダムで決める関数.
	void Rand_Attack();

	// ランダムに座標を取る関数.
	void Rand_Pos();
	// ランダム移動関数.
	void RandVectorMove( const float& moveSpeed );

	// プレイヤーとの当たり判定.
	void PlayerCollsion( CActor* pActor );
	// 雑魚敵との当たり判定.
	void EnemyCollsion( CActor* pActor );
	// 特殊な敵1との当たり判定.
	void SpecialEnemy_1Collsion( CActor* pActor );
	// 特殊な敵2との当たり判定.
	void SpecialEnemy_2Collsion( CActor* pActor );

	// 索敵範囲.
	void SearchRange();

	// デバック更新関数.
	void DebugUpdate();

	// 実験でボスを消す関数.
	virtual void SetDelete( const std::function<void( bool& )>& ) override;

private:
	CDX9SkinMesh*		m_pSkinMesh;

	D3DXVECTOR3			m_TargetPosition;			// プレイヤーの座標.
	D3DXVECTOR3			m_TargetRotation;			// 目標の回転情報.
	D3DXVECTOR3			m_BeforeMoveingPosition;	// 移動前の座標.
	D3DXVECTOR3			m_MoveVec3;					// 移動ベクトル.
	D3DXVECTOR3			m_RandPos;					// ランダム座標.
	D3DXVECTOR3			m_OldRandPos;				// 前回のランダム座標.
	D3DXVECTOR3			m_RandRot;					// ランダム回転.
	Boss::EBossState	m_NowState;					// 現在の状態.
	Boss::EMoveState	m_NowMoveState;				// 現在の移動状態.
	float				m_Life;						// HP.
	float				m_MoveSpeed;				// 移動速度.
	float				m_DeathCount;				// 死亡カウント.
	int					m_FrightCount;				// 怯みカウント.
	int					m_HitCount;					// ヒットカウント.
	int					m_WaitCount;				// 待機カウント.
	int					m_InvincibleCount;			// 無敵カウント.
	int					m_RandAttack;				// 攻撃方法をランダムで決める.
	int					m_AttackWave;				// 攻撃の流れ.
	int					m_AttackCount;				// 攻撃を何回行ったか.
	bool				m_IsFirght;					// 怯み状態か.
	bool				m_IsExplosion;				// 爆発するか.
	bool				m_IsDelete;					// 消去するかどうか.
	bool				m_IsRandMove;				// ランダム移動しているかどうか.
	bool				m_IsRandAttack;				// 攻撃方法をランダムで決めているかどうか.
	bool				m_HasAimPlayer;				// プレイヤーを狙うかどうか.
	bool				m_OldHasAimPlayer;			// 前回プレイヤーを狙っていたか.

};
#endif // !BOSS_H