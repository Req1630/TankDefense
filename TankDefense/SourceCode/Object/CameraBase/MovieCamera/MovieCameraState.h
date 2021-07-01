#ifndef MOVIE_CAMERA_STATE_H
#define MOVIE_CAMERA_STATE_H

#include "..\CameraBase.h"

// ���o�ړ��p�J�������.
struct stMovieMoveCamera
{
	float						ActiveTime;			// ���쎞��.
	CCameraBase::SCameraState	StartState;			// �J�n�J�������.
	CCameraBase::SCameraState	EndState;			// �I���J�������.
	float						PosMoveTime;		// �ړ�����(�b).
	float						LookPosMoveTime;	// �����ʒu�ړ�����(�b).
} typedef SMovieMoveCamera;

// ���o�h��p�J�������.
struct stMovieShakeCamera
{
	struct stShakeState
	{
		float		Amplitube;		// �U�ꕝ(�l���傫����Α傫���h���).
		float		Frequency;		// ���g��(��b�Ԃɉ���h�炷��).
		float		Time;			// �h�ꎞ��(�b).
		D3DXVECTOR3	BasePosition;	// ���̍��W.
		bool		IsAttenuation;	// �������邩.
		bool		IsPlaying;		// ���삷�邩.
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

// ���o�p�J�������.
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