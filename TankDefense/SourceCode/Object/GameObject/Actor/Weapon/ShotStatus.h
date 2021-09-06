/**
* @file		ShotStatus.h.
* @brief	弾のステータス構造体.
* @author	崎田友輝.
*/
#ifndef SHOTSTATUS_H
#define SHOTSTATUS_H

// 弾ステータス.
struct stStatus
{
	int	  BulletsNum;	// 弾数.
	float Attack;		// 攻撃力.
	float Speed;		// 速度.
	float Interval;		// 間隔(秒).

	stStatus()
		: BulletsNum( 0 )
		, Attack( 0.0f )
		, Speed( 0.0f )
		, Interval( 0.0f )
	{}
} typedef SStatus;

#endif // #ifndef SHOTSTATUS_H.