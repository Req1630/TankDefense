#ifndef MOVIE_WIDGET_H
#define MOVIE_WIDGET_H

#include "..\Widget.h"

// ���[�r�[(���o�p)�A�j���[�V�������.
struct stMovieAnimState
{
	D3DXVECTOR2	UVScrollSpeed;		// UV�X�N���[�����x(x : ��,y : �c).
	int			FrameCountSpeed;	// �t���[���J�E���g���x.
	bool		IsAnimation;		// �A�j���[�V�������邩.
	bool		IsUVScrollX;		// ��������UV�X�N���[�������邩.
	bool		IsUVScrollY;		// �c������UV�X�N���[�������邩.

	void SetAnimState( SSpriteAnimState* pState )
	{
		pState->ScrollSpeed		= this->UVScrollSpeed;
		pState->FrameCountSpeed	= this->FrameCountSpeed;
		pState->IsUVScrollX		= this->IsUVScrollX;
		pState->IsUVScrollY		= this->IsUVScrollY;
		pState->IsAnimation		= this->IsAnimation;
	};

} typedef SMovieAnimState;

// ���[�r�[(���o�p)�t�F�[�h���.
struct stMovieFadeState
{
	// �t�F�[�h�t���O.
	enum enFadeFlag : unsigned int
	{
		None = 0,	// �����Ȃ�.

		In,			// �C��.
		Out,		// �A�E�g.
		Wrap,		// �J��Ԃ�.

		Max,
	} typedef EFadeFlag;

	int		FadeFlag;		// �t�F�[�h�t���O.
	float	FadeSpeed;		// �t�F�[�h���x.
	float	AlphaMax;		// �A���t�@�ő�l.
	float	AlphaMin;		// �A���t�@�ŏ��l.

	stMovieFadeState()
		: FadeFlag	( EFadeFlag::None )
		, FadeSpeed	( 0.0f )
		, AlphaMax	( 1.0f )
		, AlphaMin	( 0.0f )
	{}
	stMovieFadeState( 
		const int& flag, 
		const float& speed,
		const float& alphaMax, 
		const float& alphaMin )
		: FadeFlag	( flag )
		, FadeSpeed	( speed )
		, AlphaMax	( alphaMax )
		, AlphaMin	( alphaMin )
	{}

} typedef SMovieFadeState;

// ���[�r�[(���o�p)�摜���.
struct stMovieWidget
{
	SMovieAnimState	AnimState;		// �A�j���[�V�������.
	SMovieFadeState	FadeState;		// �t�F�[�h���.
	D3DXVECTOR2		Position;		// �J�n���W.
	float			AciveStartTime;	// ����J�n����(�b).
	std::string		SpriteName;		// �摜�̖��O.

	stMovieWidget()
		: AnimState			()
		, FadeState			()
		, Position			( 0.0f, 0.0f )
		, AciveStartTime	( 0.0f )
		, SpriteName		()
	{}
	stMovieWidget( 
		const D3DXVECTOR2&	pos,
		const float&		startTime,
		const char*			name )
		: Position			( pos )
		, AciveStartTime	( startTime )
		, SpriteName		( name )
	{}
} typedef SMovieWidget;

/**************************************
*	���[�r�[�p(���o)�摜�N���X.
**/
class CMovieWidget : public CWidget
{
public:
	CMovieWidget();
	virtual ~CMovieWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update( const float& deltaTime ) override;
	// �`��֐�.
	virtual void Render() override;
	// �P���ɕ`�悷��.
	virtual void SimpleRender();

	// ���[�r�[�p(���o)���̐ݒ�.
	inline void SetMovieWidgetState( const SMovieWidget& state ){ m_MovieWidgetState = state; }
	// ���[�r�[�p(���o)���̎擾.
	inline SMovieWidget GetMovieWidgetState() const { return m_MovieWidgetState; }

	// �`�����ݒ肷��.
	void SettingRenderState( const bool& isEdit = false );

	// �Đ�.
	void Play();
	// ���Z�b�g.
	void Reset();

private:
	// �t�F�[�h�̊J�n.
	void StartFade();
	// �t�F�[�h�J��Ԃ��̐ݒ�.
	void SetFadeWrap( const float& speed );
	// �t�F�[�h�J��Ԃ��̍X�V.
	void FadeWrapUpdate();

private:
	SSpriteRenderState*	m_pRenderState;		// �`����.
	SMovieWidget		m_MovieWidgetState;	// ���[�r�[�p���.
	float				m_NowTime;			// ���݂̌o�ߎ���.
	bool				m_IsOncePlay;		// �t�F�[�h���J�n������.
	bool				m_IsPlay;			// �Đ����Ă邩.
};

#endif	// #ifndef MOVIE_WIDGET_H.