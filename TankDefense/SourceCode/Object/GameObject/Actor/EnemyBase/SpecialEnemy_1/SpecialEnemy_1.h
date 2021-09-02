#ifndef SPECIALENEMY_1_H
#define SPECIALENEMY_1_H

#include "..\EnemyBase.h"

class CExplosion;

class CSpecialEnemy_1 : public CEnemyBase
{

public:
	CSpecialEnemy_1();
	virtual ~CSpecialEnemy_1();

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
	// プレイヤーと爆発の当たり判定.
	void ExplosionCollsion( CActor* pActor );
	// 雑魚敵と爆発の当たり判定.
	void ExplosionEnemyCollsion( CActor* pActor );
	// 特殊な敵2と爆発の当たり判定.
	void ExplosionSEnemy2Collsion( CActor* pActor );

private:
	CEnemyBase*		m_pEnemyBase;		// 敵ベースクラス.
	std::shared_ptr<CExplosion>		m_pExplosion;	// 爆発クラス.

};

#endif // !SPECIALENEMY_1_H
