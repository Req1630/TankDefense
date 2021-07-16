#pragma once

namespace
{
	constexpr char PLAYER_PARAM_FILE_PATH[] = "Data\\Parameter\\Editor\\EditParam\\PlayerParam.csv";
};


enum enPlayerParamList{

	EPlayerParamNo_None = -1,

	EPlayerParamNo_AttackPower,	// �U����.
	EPlayerParamNo_HP,	// �̗�.
	EPlayerParamNo_MoveSpeed,	// �ړ����x.
	EPlayerParamNo_InitPos_X,	// �������W : X.
	EPlayerParamNo_InitPos_Y,	// �������W : Y.
	EPlayerParamNo_InitPos_Z,	// �������W : Z.

	EPlayerParamNo_Max,
} typedef EPlayerParamNo;

