#ifndef MOVIE_CAMERA_STATE_H
#define MOVIE_CAMERA_STATE_H

#include "..\CameraBase.h"

// 演出移動用カメラ情報.
struct stMovieMoveCamera
{
	float						ActiveTime;			// 動作時間.
	CCameraBase::SCameraState	StartState;			// 開始カメラ情報.
	CCameraBase::SCameraState	EndState;			// 終了カメラ情報.
	float						PosMoveTime;		// 移動時間(秒).
	float						LookPosMoveTime;	// 注視位置移動時間(秒).
} typedef SMovieMoveCamera;

// 演出揺れ用カメラ情報.
struct stMovieShakeCamera
{
	struct stShakeState
	{
		float		Amplitube;		// 振れ幅(値が大きければ大きく揺れる).
		float		Frequency;		// 周波数(一秒間に何回揺らすか).
		float		Time;			// 揺れ時間(秒).
		D3DXVECTOR3	BasePosition;	// 基底の座標.
		bool		IsAttenuation;	// 減衰するか.
		bool		IsPlaying;		// 動作するか.
		stShakeState()
			: Amplitube		( 0.0f )
			, Frequency		( 0.0f )
			, Time			( 0.0f )
			, BasePosition	( 0.0f, 0.0f, 0.0f )
			, IsAttenuation	( false )
			, IsPlaying		( false )

		{}
	} typedef SShakeState;

	enum enShakePosNo : unsigned char
	{
		EShakePosNo_None,

		EShakePosNo_PosVer = 0,
		EShakePosNo_PosHor,
		EShakePosNo_LookPosVer,
		EShakePosNo_LookPosHor,

		EShakePosNo_Max,
	} typedef EShakePosNo;

	SShakeState ShakeState[EShakePosNo_Max];

	stMovieShakeCamera() : ShakeState()
	{}

} typedef SMovieShakeCamera;

// 演出用カメラ情報.
struct stMovieCamera
{
	SMovieMoveCamera	MoveState;
	SMovieShakeCamera	ShakeState;

	stMovieCamera()
		: MoveState		()
		, ShakeState	()
	{}
	stMovieCamera( const stMovieCamera& movieCamera )
		: MoveState		( movieCamera.MoveState )
		, ShakeState	( movieCamera.ShakeState )
	{}
	stMovieCamera( const SMovieMoveCamera& moveCamera, const SMovieShakeCamera& shakeCamera )
		: MoveState		( moveCamera )
		, ShakeState	( shakeCamera )
	{}
	stMovieCamera( const SMovieMoveCamera& moveCamera )
		: MoveState		( moveCamera )
		, ShakeState	()
	{}
	stMovieCamera( const SMovieShakeCamera& shakeCamera )
		: MoveState		()
		, ShakeState	( shakeCamera )
	{}
} typedef SMovieCamera;

#endif	// #ifndef MOVIE_CAMERA_STATE_H.