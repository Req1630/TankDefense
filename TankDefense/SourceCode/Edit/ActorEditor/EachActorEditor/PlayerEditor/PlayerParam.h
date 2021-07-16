#pragma once

namespace
{
	constexpr char PLAYER_PARAM_FILE_PATH[] = "Data\\Parameter\\Editor\\EditParam\\PlayerParam.csv";
};


enum enPlayerParamList{

	EPlayerParamNo_None = -1,

	EPlayerParamNo_AttackPower,	// 攻撃力.
	EPlayerParamNo_HP,	// 体力.
	EPlayerParamNo_MoveSpeed,	// 移動速度.
	EPlayerParamNo_InitPos_X,	// 初期座標 : X.
	EPlayerParamNo_InitPos_Y,	// 初期座標 : Y.
	EPlayerParamNo_InitPos_Z,	// 初期座標 : Z.

	EPlayerParamNo_Max,
} typedef EPlayerParamNo;

