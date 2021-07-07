#ifndef SPRITE_STRUCT_H
#define SPRITE_STRUCT_H

#include "..\..\Global.h"

// ���_�̍\����.
struct SpriteVertex
{
	D3DXVECTOR3 Pos;	// ���_���W.
	D3DXVECTOR2	Tex;	// �e�N�X�`�����W.
};

// �������\����(float�^).
struct WHDIZE_FLOAT
{
	float w;	// ��.
	float h;	// ����.
};

// ���[�J�����W�̔ԍ�.
enum class enLocalPosition : unsigned char
{
	None,

	LeftUp = 0,	// ����.
	Left,		// ��.
	LeftDown,	// ����.
	Down,		// ��.
	RightDown,	// �E��.
	Right,		// �E.
	RightUp,	// �E��.
	Up,			// ��.
	Center,		// ����.

	Max,
} typedef ELocalPosition;

// �T���v���X�e�[�g.
enum enSamplerState : unsigned char
{
	ESamplerState_None,

	ESamplerState_Wrap = 0,	// �^�C����ɌJ��Ԃ����.
	ESamplerState_Mirror,	// ���]���Ȃ���J��Ԃ����.
	ESamplerState_Clamp,	// 0~1�ɐݒ肳���.
	ESamplerState_Border,	// 0~1�͈̔͊O��hlsl�Ŏw�肵���F�ɂȂ�.
	ESamplerState_MirrorOne,// 0�𒆐S�ɔ��]�����.

	ESamplerState_Max,

} typedef ESamplerState;

enum enSpriteStateNo : unsigned char
{
	None,

	ESpriteStateNo_LocalPosNum = 0,
	ESpriteStateNo_Disp_w,
	ESpriteStateNo_Disp_h,
	ESpriteStateNo_Base_w,
	ESpriteStateNo_Base_h,
	ESpriteStateNo_Stride_w,
	ESpriteStateNo_Stride_h,
	ESpriteStateNo_vPos_x,
	ESpriteStateNo_vPos_y,
	ESpriteStateNo_vPos_z,
	ESpriteStateNo_AnimNum,

	ESpriteStateNo_Max = ESpriteStateNo_AnimNum,
} typedef ESpriteStateNo;

enum enPSShaderNo : unsigned char
{
	EPSShaderNo_None,

	EPSShaderNo_Main = 0,
	EPSShaderNo_TAlphaOut,
	EPSShaderNo_TCutOut,

	EPSShaderNo_Max,
} typedef EPSShaderNo;

// �X�v���C�g�\����.
struct stSpriteState
{
	ELocalPosition	LocalPosNum;// ���[�J�����W�̔ԍ�.
	WHDIZE_FLOAT	Disp;		// �\����,����.
	WHDIZE_FLOAT	Base;		// ���摜�̕�,����.
	WHDIZE_FLOAT	Stride;		// 1�R�}������̕�,����.
	D3DXVECTOR3		vPos;		// ���W.
	int				AnimNum;	// �A�j���[�V������.
} typedef SSpriteState;

// �X�v���C�g�A�j���[�V�������.
struct stSpriteAnimState
{
	POINT		PatternNo;			// �p�^�[���ԍ�.
	POINT		PatternMax;			// �p�^�[���ő吔.
	D3DXVECTOR2	UV;					// �e�N�X�`��UV���W.
	D3DXVECTOR2	ScrollSpeed;		// �e�N�X�`��UV���W.
	int			AnimNumber;			// �A�j���[�V�����ԍ�.
	int			MaxAnimNumber;		// �ő�A�j���[�V�����ԍ�.
	int			FrameCount;			// �t���[���J�E���g.
	int			FrameCountSpeed;	// �t���[���J�E���g���x.
	int			FrameTime;			// �t���[���^�C��.
	bool		IsAnimation;		// �A�j���[�V�������邩.
	bool		IsUVScrollX;		// UV�X�N���[�������邩.
	bool		IsUVScrollY;		// UV�X�N���[�������邩.


	stSpriteAnimState()
		: PatternNo			{ 1, 1 }
		, PatternMax		{ 1, 1 }
		, UV				( 0.0f, 0.0f )
		, ScrollSpeed		( 0.001f, 0.001f )
		, AnimNumber		( 1 )
		, MaxAnimNumber		( 1 )
		, FrameCount		( 1 )
		, FrameCountSpeed	( 1 )
		, FrameTime			( 5 )
		, IsAnimation		( false )
		, IsUVScrollX		( false )
		, IsUVScrollY		( false )
	{}

	// UV���W�ɕϊ�.
	D3DXVECTOR2 ConvertIntoUV()
	{
		return {
			// x���W.
			static_cast<float>(PatternNo.x) / static_cast<float>(PatternMax.x),
			// y���W.
			static_cast<float>(PatternNo.y) / static_cast<float>(PatternMax.y)
		};
	}

	// �A�j���[�V�����̍X�V.
	void AnimUpdate()
	{
		if( IsAnimation == false ) return;

		if( FrameCount % FrameTime == 0 ){
			// �A�j���[�V�����ԍ����Z�o.
			PatternNo.x = AnimNumber % PatternMax.x;
			PatternNo.y = AnimNumber / PatternMax.x;
			AnimNumber++;	// �A�j���[�V�����ԍ������Z.
			if( AnimNumber >= MaxAnimNumber ){
				// �A�j���[�V�����ԍ����ő�A�j���[�V��������葽�����.
				// ����������.
				FrameCount = 0;
				AnimNumber = 0;
			}
		}
		FrameCount += FrameCountSpeed;
		// UV���W�ɕϊ�.
		UV = ConvertIntoUV();
	}

	// UV�X�N���[���̍X�V.
	void UVScrollUpdate()
	{
		if( IsUVScrollX == true ){
			UV.x += ScrollSpeed.x;
			if( UV.x > 1.0f ) UV.x = 0.0f;
			if( UV.x < 0.0f ) UV.x = 1.0f;
		}

		if( IsUVScrollY == true ){
			UV.y += ScrollSpeed.y;
			if( UV.y > 1.0f ) UV.y = 0.0f;
			if( UV.y < 0.0f ) UV.y = 1.0f;
		}
	}

} typedef SSpriteAnimState;

// �`��p�摜���.
struct stSpriteRenderState
{
	SSpriteAnimState	AnimState;	// �A�j���[�V�������.
	STranceform			Tranceform;	// �ʒu���Ȃ�.
	D3DXVECTOR4			Color;		// �F.
	float				FadeValue;	// �t�F�[�h�A�g�����W�V�����̒l.
	EPSShaderNo			ShaderNo;	// �V�F�[�_�[�̔ԍ�.
	ESamplerState		SmaplerNo;	// �T���v���ԍ�.
	bool				IsUIRender;	// UI�ŕ`�悷�邩 false �Ȃ�3D�ŕ`�悷��.

	stSpriteRenderState()
		: AnimState		()
		, Tranceform	()
		, Color			( 1.0f, 1.0f, 1.0f, 1.0f )
		, FadeValue		( 0.0f )
		, ShaderNo		( EPSShaderNo_Main )
		, SmaplerNo		( ESamplerState_Wrap )
		, IsUIRender	( true )
	{}

} typedef SSpriteRenderState;

#endif	// #ifndef SPRITE_STRUCT_H.