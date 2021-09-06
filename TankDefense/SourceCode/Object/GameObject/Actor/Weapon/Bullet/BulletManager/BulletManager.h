#pragma once
#include "..\..\..\Actor.h"
#include "..\..\..\Weapon\ShotStatus.h"

class CBullet;

/************************************************
*	弾マネージャー.
**/
class CBulletManager final
	: public CActor
{
public:
	CBulletManager();
	~CBulletManager();

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

	// 特定の向きに飛ぶ弾の発射関数.
	void NormalShot( std::string StaticMeshName, EObjectTag ObjTag, SStatus& Status, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 MoveVec );

	// 追尾する弾の発射関数.
	void HomingShot( std::string StaticMeshName, EObjectTag ObjTag, SStatus& Status, D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, D3DXVECTOR3 Rot );

private:
	// デバックの更新関数.
	void DebugUpdate();

private:
	std::vector<std::unique_ptr<CBullet>>	m_pBullet;			// 弾.

};
