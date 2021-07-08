#include "MovieWidget.h"


CMovieWidget::CMovieWidget()
	: m_pRenderState		( nullptr )
	, m_MovieWidgetState	()
	, m_NowTime				( 0.0f )
	, m_IsOncePlay			( false )
	, m_IsPlay				( false )
{
}

CMovieWidget::~CMovieWidget()
{
}

//--------------------------------.
// �������֐�.
//--------------------------------.
bool CMovieWidget::Init()
{
	if( SpriteSetting( m_MovieWidgetState.SpriteName.c_str() ) == false ) return false;
	m_pRenderState = &m_RenderStates.back();

	SettingRenderState();

	return true;
}

//--------------------------------.
// �X�V�֐�.
//--------------------------------.
void CMovieWidget::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;
	m_NowTime += m_DeltaTime;

	if( m_NowTime < m_MovieWidgetState.AciveStartTime ||
		m_IsPlay == false ) return;

	StartFade();	// �t�F�[�h�̊J�n.

	FadeUpdate( m_Alpha );
	FadeWrapUpdate();
}

//--------------------------------.
// �`��֐�.
//--------------------------------.
void CMovieWidget::Render()
{
	if( m_NowTime < m_MovieWidgetState.AciveStartTime ) return;

	SimpleRender();
}

//--------------------------------.
// �P���ɕ`�悷��.
//--------------------------------.
void CMovieWidget::SimpleRender()
{
	if( m_pSprite == nullptr ) return;
	m_pRenderState->Color.w = m_Alpha;
	m_pSprite->Render( m_pRenderState );
}

//--------------------------------.
// �`�����ݒ肷��.
//--------------------------------.
void CMovieWidget::SettingRenderState( const bool& isEdit )
{
	// ���W��ݒ�.
	m_pRenderState->Tranceform.Position.x = m_MovieWidgetState.Position.x;
	m_pRenderState->Tranceform.Position.y = m_MovieWidgetState.Position.y;

	// �A�j���[�V�����̏����擾.
	m_MovieWidgetState.AnimState.SetAnimState( &m_pRenderState->AnimState );

	if( m_MovieWidgetState.FadeState.FadeFlag == SMovieFadeState::None ) return;

	// �t�F�[�h�����擾.
	m_AlphaMax = m_MovieWidgetState.FadeState.AlphaMax;
	m_AlphaMin = m_MovieWidgetState.FadeState.AlphaMin;

	if( m_MovieWidgetState.FadeState.FadeFlag == SMovieFadeState::In ){
		m_Alpha		= m_pRenderState->Color.w = m_AlphaMax;
		m_FadeSpeed	= -m_MovieWidgetState.FadeState.FadeSpeed;
	} else {
		m_Alpha		= m_pRenderState->Color.w = m_AlphaMin;
		m_FadeSpeed	= m_MovieWidgetState.FadeState.FadeSpeed;
	}

	// �G�f�B�b�g���[�h�Ȃ�ő�A���t�@�l��ݒ�.
	if( isEdit == false ) return;
	m_Alpha = m_pRenderState->Color.w = m_AlphaMax;
}

//--------------------------------.
// �Đ�.
//--------------------------------.
void CMovieWidget::Play()
{
	SettingRenderState();
	m_IsOncePlay = false;
	m_IsPlay = true;
}

//--------------------------------.
// ���Z�b�g.
//--------------------------------.
void CMovieWidget::Reset()
{
	m_Alpha = m_pRenderState->Color.w = m_AlphaMax;
	m_IsPlay = false;
}

//--------------------------------.
// �t�F�[�h�̊J�n.
//--------------------------------.
void CMovieWidget::StartFade()
{
	// ���łɊJ�n����Ă���ΏI��.
	if( m_IsOncePlay == true ) return;

	switch( m_MovieWidgetState.FadeState.FadeFlag )
	{
	case SMovieFadeState::In:
		SetFadeIn( m_FadeSpeed );
		break;
	case SMovieFadeState::Out:
		SetFadeOut( m_FadeSpeed );
		break;
	case SMovieFadeState::Wrap:
		SetFadeWrap( m_FadeSpeed );
		break;
	default:
		break;
	}

	m_IsOncePlay = true;
}

//--------------------------------.
// �t�F�[�h�J��Ԃ��̐ݒ�.
//--------------------------------.
void CMovieWidget::SetFadeWrap( const float& speed )
{
	SetFadeOut( speed );
}

//--------------------------------.
// �t�F�[�h�J��Ԃ��̍X�V.
//--------------------------------.
void CMovieWidget::FadeWrapUpdate()
{
	if( m_MovieWidgetState.FadeState.FadeFlag != SMovieFadeState::Wrap ) return;
	// ���݂̃t�F�[�h��Ԃ��I�����ĂȂ��ΏI���.
	if( m_NowFadeState != EFadeState::Finish ) return;

	// �O��̃t�F�[�h��Ԃ��C���Ȃ�A�E�g���J�n����.
	if( m_OldFadeState == EFadeState::In ){
		m_FadeSpeed *= -1.0f;		// �t�F�[�h���x�𔽓].
		SetFadeOut( m_FadeSpeed );

	// �O��̃t�F�[�h��Ԃ��A�E�g�Ȃ�C�����J�n����.
	} else if( m_OldFadeState == EFadeState::Out ){
		m_FadeSpeed *= -1.0f;		// �t�F�[�h���x�𔽓].
		SetFadeIn( m_FadeSpeed );
	}
}