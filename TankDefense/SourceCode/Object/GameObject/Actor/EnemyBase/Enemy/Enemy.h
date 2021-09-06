#ifndef ENEMY_H
#define ENEMY_H

#include "..\EnemyBase.h"

class CEnemyBullet;

class CEnemy : public CEnemyBase
{
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
	// 当たり判定の初期化.
	virtual void InitCollision() override;
	// 当たり判定の座標や、半径などの更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;
	// スポーン.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) override;
	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor ) override;
	// エネミー座標の設定.
	void SetEnemyPos(CActor& actor);

private:
	// スポーン中.
	virtual void Spawning() override;
	// 移動.
	virtual void Move() override;
	// 怯み.
	virtual void Fright() override;
	// 死亡.
	virtual void Death() override;

	// 攻撃関数.
	void Attack();
	// プレイヤーとの当たり判定.
	void PlayerCollsion( CActor* pActor );
	// 特殊な敵1との当たり判定.
	void SpecialEnemy_1Collsion( CActor* pActor );
	// 特殊な敵2との当たり判定.
	void SpecialEnemy_2Collsion( CActor* pActor );
	// プレイヤーと雑魚敵の弾の当たり判定.
	void EnemyBulletCollsion( CActor* pActor );

	// 索敵範囲.
	void SearchRange();

	// デバック更新関数.
	void DebugUpdate();

private:
	CEnemyBase*		m_pEnemyBase;		// 敵ベースクラス.
	std::shared_ptr<CEnemyBullet>	m_pEnemyBullet;	// 雑魚敵の弾クラス.

	bool			m_HasAimPlayer;		// プレイヤーを狙うかどうか.
	bool			m_OldHasAimPlayer;	// 前回プレイヤーを狙っていたか.

};

#endif // !ENEMY_H
