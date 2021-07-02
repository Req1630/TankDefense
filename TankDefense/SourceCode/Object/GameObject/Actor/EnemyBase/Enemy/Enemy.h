#ifndef ENEMY_H
#define ENEMY_H

#include "..\EnemyBase.h"

class CEnemy : public CEnemyBase
{
	const char* MODEL_NAME = "b_s";

public:
	CEnemy();
	virtual ~CEnemy();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update( const float& deltaTime ) override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// スポーン.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) override;
	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor ) override;

private:
	// プレイヤー座標の設定.
	void SetPlayerPos( CActor& actor );
	// スポーン中.
	virtual void Spawning() override;
	// 移動.
	virtual void Move() override;
	// 怯み.
	virtual void Fright() override;
	// 死亡.
	virtual void Death() override;

	// 移動関数.
	virtual void VectorMove( const float& moveSpeed ) override;
	// 攻撃関数.
	void Attack();
	// プレイヤーとの当たり判定.
	void PlayerCollison( CActor* pActor );

	// プレイヤーを狙うか判定.
	void AimPlayerDecision();

	// 当たり判定の設定.
	bool ColliderSetting();

private:
	CEnemyBase*		m_pEnemyBase;
	D3DXVECTOR3 m_vPlayerPos;	// プレイヤーの座標.
	bool	m_HasAimPlayer;		// プレイヤーを狙うかどうか.
	bool	m_OldHasAimPlayer;	// 前回プレイヤーを狙っていたか.

};

#endif // !ENEMY_H
