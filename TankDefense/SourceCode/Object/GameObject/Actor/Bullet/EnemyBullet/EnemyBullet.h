#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H

#include "..\Bullet.h"

class CEnemyBullet
	: public CBullet
{
public:
	CEnemyBullet();
	~CEnemyBullet();

	// 初期化関数.
	virtual bool Init();
	// 更新関数.
	virtual void Update( const float& deltaTime );
	// 描画関数.
	virtual void Render();

	// 当たり判定関数.
	virtual void Collision( CActor* pActor );

	// 当たり判定の初期化.
	virtual void InitCollision();
	// 当たり判定の座標や、半径などの更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision();

	// 弾の表示設定.
	void SetBulletScale();
	// 弾の位置設定.
	void SetBulletPos( D3DXVECTOR3 Pos, D3DXVECTOR3 MoveVec );
	void SetTargetPos( CActor& actor );
	// 弾のフラグ取得.
	bool GetShotflg() { return m_ShotFlag; }
	void Setshotflg( bool shotflg ) { m_ShotFlag = shotflg; }

private:
	D3DXVECTOR3		m_TargetPosition;	// 攻撃対象の座標.

	bool			m_ShotFlag;			// 弾が発射されたかどうか.
};
#endif	// #ifndef ENEMYBULLET_H.