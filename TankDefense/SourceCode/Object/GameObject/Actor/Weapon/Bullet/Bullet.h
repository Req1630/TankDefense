/**
* @file		Bullet.h.
* @brief	弾クラス.
* @author	崎田友輝.
*/
#ifndef BULLET_H
#define BULLET_H

#include "..\..\Actor.h"
#include "..\..\Weapon\ShotStatus.h"

class CDX9StaticMesh;

namespace{
	const float DISP_TIME = 10.0f;	// 弾の生存時間.
}

/************************************************
*	弾クラス.
**/
class CBullet
	: public CActor
{
public:
	// 弾の種類.
	enum enType
	{
		Normal,			// 特定の方向に飛ぶ弾.
		Homing,			// 追尾する弾.
		Penetrating,	// 貫通する弾.
	} typedef EType;

public:
	CBullet();
	~CBullet();

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

	// 特定の向きに飛ぶ弾の発射.
	void NormalShot( std::string StaticMeshName, EObjectTag ObjTag, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 MoveVec );

	// 追尾する弾の発射.
	void HomingShot( std::string StaticMeshName, EObjectTag ObjTag, D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, D3DXVECTOR3 Rot );

	// 表示中か取得.
	bool GetDisp() const { return m_Disp; }

	// ステータスを設定.
	void SetStatus( const SStatus& status ) { m_Status = status; }

protected:
	// 作成関数.
	void Create();

	// ベジェ曲線関数.
	D3DXVECTOR3	Evaluate( std::vector<D3DXVECTOR3> ControlPoints, float t );

	// バーンスタイン基底関数.
	float Bernstein( int n, int i, float t );

	// 二項係数を計算する.
	float Binomial( int n, int k );

	// 階乗を計算する.
	float Factorial( int a );

protected:
	CDX9StaticMesh*				m_pStaticMesh;		// 弾のモデル.

	SStatus						m_Status;			// ステータス.
	EType						m_Type;				// 弾のタイプ.
	D3DXVECTOR3					m_MoveVec;			// 移動ベクトル.

	std::vector<D3DXVECTOR3>	m_ControlPoints;	// ベジェ曲線の座標.
	float						m_Distance;			// ベジェ曲線で使用する距離(t).

	float						m_DispTimeCnt;		// 表示時間カウント.
	bool						m_Disp;				// 表示状態.

};

#endif // #ifndef BULLET_H.