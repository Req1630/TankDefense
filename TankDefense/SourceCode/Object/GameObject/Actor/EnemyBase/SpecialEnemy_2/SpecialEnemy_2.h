#ifndef SPECIALENEMY_2_H
#define SPECIALENEMY_2_H

#include "..\EnemyBase.h"
#include <time.h>
#include <random>
#include <vector>

class CRestraint;

class CSpecialEnemy_2 : public CEnemyBase
{

public:
	CSpecialEnemy_2();
	virtual ~CSpecialEnemy_2();

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
	// 特殊な敵1との当たり判定.
	void SpecialEnemy1Collsion( CActor* pActor );
	// 拘束攻撃の当たり判定.
	void RestraintCollsion( CActor* pActor );

private:
	CEnemyBase*		m_pEnemyBase;		// 敵ベースクラス.
	std::shared_ptr<CRestraint>		m_pRestraint;	// 拘束クラス.
	CSprite*		m_pSprite;			// スプライトクラス

	int				m_ConfuCnt;			// 混乱カウント.
	bool			m_IsConfu;			// 混乱状態になっているかどうか.

};

#endif // !SPECIALENEMY_2_H
