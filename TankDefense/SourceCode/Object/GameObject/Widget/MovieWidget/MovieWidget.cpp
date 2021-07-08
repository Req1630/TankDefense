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
// 初期化関数.
//--------------------------------.
bool CMovieWidget::Init()
{
	if( SpriteSetting( m_MovieWidgetState.SpriteName.c_str() ) == false ) return false;
	m_pRenderState = &m_RenderStates.back();

	SettingRenderState();

	return true;
}

//--------------------------------.
// 更新関数.
//--------------------------------.
void CMovieWidget::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;
	m_NowTime += m_DeltaTime;

	if( m_NowTime < m_MovieWidgetState.AciveStartTime ||
		m_IsPlay == false ) return;

	StartFade();	// フェードの開始.

	FadeUpdate( m_Alpha );
	FadeWrapUpdate();
}

//--------------------------------.
// 描画関数.
//--------------------------------.
void CMovieWidget::Render()
{
	if( m_NowTime < m_MovieWidgetState.AciveStartTime ) return;

	SimpleRender();
}

//--------------------------------.
// 単純に描画する.
//--------------------------------.
void CMovieWidget::SimpleRender()
{
	if( m_pSprite == nullptr ) return;
	m_pRenderState->Color.w = m_Alpha;
	m_pSprite->Render( m_pRenderState );
}

//--------------------------------.
// 描画情報を設定する.
//--------------------------------.
void CMovieWidget::SettingRenderState( const bool& isEdit )
{
	// 座標を設定.
	m_pRenderState->Tranceform.Position.x = m_MovieWidgetState.Position.x;
	m_pRenderState->Tranceform.Position.y = m_MovieWidgetState.Position.y;

	// アニメーションの情報を取得.
	m_MovieWidgetState.AnimState.SetAnimState( &m_pRenderState->AnimState );

	if( m_MovieWidgetState.FadeState.FadeFlag == SMovieFadeState::None ) return;

	// フェード情報を取得.
	m_AlphaMax = m_MovieWidgetState.FadeState.AlphaMax;
	m_AlphaMin = m_MovieWidgetState.FadeState.AlphaMin;

	if( m_MovieWidgetState.FadeState.FadeFlag == SMovieFadeState::In ){
		m_Alpha		= m_pRenderState->Color.w = m_AlphaMax;
		m_FadeSpeed	= -m_MovieWidgetState.FadeState.FadeSpeed;
	} else {
		m_Alpha		= m_pRenderState->Color.w = m_AlphaMin;
		m_FadeSpeed	= m_MovieWidgetState.FadeState.FadeSpeed;
	}

	// エディットモードなら最大アルファ値を設定.
	if( isEdit == false ) return;
	m_Alpha = m_pRenderState->Color.w = m_AlphaMax;
}

//--------------------------------.
// 再生.
//--------------------------------.
void CMovieWidget::Play()
{
	SettingRenderState();
	m_IsOncePlay = false;
	m_IsPlay = true;
}

//--------------------------------.
// リセット.
//--------------------------------.
void CMovieWidget::Reset()
{
	m_Alpha = m_pRenderState->Color.w = m_AlphaMax;
	m_IsPlay = false;
}

//--------------------------------.
// フェードの開始.
//--------------------------------.
void CMovieWidget::StartFade()
{
	// すでに開始されていれば終了.
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
// フェード繰り返しの設定.
//--------------------------------.
void CMovieWidget::SetFadeWrap( const float& speed )
{
	SetFadeOut( speed );
}

//--------------------------------.
// フェード繰り返しの更新.
//--------------------------------.
void CMovieWidget::FadeWrapUpdate()
{
	if( m_MovieWidgetState.FadeState.FadeFlag != SMovieFadeState::Wrap ) return;
	// 現在のフェード状態が終了してなけば終わる.
	if( m_NowFadeState != EFadeState::Finish ) return;

	// 前回のフェード状態がインならアウトを開始する.
	if( m_OldFadeState == EFadeState::In ){
		m_FadeSpeed *= -1.0f;		// フェード速度を反転.
		SetFadeOut( m_FadeSpeed );

	// 前回のフェード状態がアウトならインを開始する.
	} else if( m_OldFadeState == EFadeState::Out ){
		m_FadeSpeed *= -1.0f;		// フェード速度を反転.
		SetFadeIn( m_FadeSpeed );
	}
}