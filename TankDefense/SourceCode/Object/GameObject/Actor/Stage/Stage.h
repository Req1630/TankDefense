#ifndef STAGE_H
#define STAGE_H

#include "..\Actor.h"
#include "Stage.h"

/************************************
*	ステージクラス.
**/
class CStage : public CActor
{
public:
	CStage();
	virtual ~CStage();

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
};

#endif	// #ifndef STAGE_H.