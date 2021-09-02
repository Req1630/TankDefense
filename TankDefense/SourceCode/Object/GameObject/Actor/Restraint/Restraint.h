#ifndef RESTRAINT_H
#define RESTRAINT_H

#include "..\Actor.h"
#include "..\..\..\..\Common\Mesh\RayMesh\RayMesh.h"

class CDX9StaticMesh;

class CRestraint : public CActor
{
public:
	CRestraint();
	virtual ~CRestraint();

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

	// 拘束の位置設定.
	void SetRestraintPos( D3DXVECTOR3 pos, D3DXVECTOR3 movevec );

	// 拘束フラグの取得.
	bool GetIsRestraint() { return m_IsRestraint; }
	void SetIsRestraint( bool IsRestraint ) { m_IsRestraint = IsRestraint; }

protected:
	CDX9StaticMesh*		m_pStaticMesh;		// スタティックメッシュ.

	D3DXVECTOR3			m_Vec3;				// ベクトル
	int					m_RestraintCnt;		// 拘束カウント.
	bool				m_IsRestraint;		// 拘束したかどうか.

};
#endif // !RESTRAINT_H