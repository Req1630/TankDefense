#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"

class CDX9StaticMesh;

class CExplosion : public CActor
{
public:
	CExplosion();
	virtual ~CExplosion();

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

	// 爆発の位置設定.
	void SetExplosionPos( D3DXVECTOR3 pos );

	// 爆発フラグの取得.
	bool GetIsExplosion() { return m_IsExplosion; }
	void SetIsExplosion( bool IsExplosion ) { m_IsExplosion = IsExplosion; }

protected:
	CDX9StaticMesh*		m_pStaticMesh;		// スタティックメッシュ.

	int					m_ExplosionCnt;		// 爆発カウント.
	bool				m_IsExplosion;		// 爆発したかどうか.

};
#endif // !EXPLOSION_H