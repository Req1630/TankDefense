#include "MovieWidget.h"


CMovieWidget::CMovieWidget()
	: m_pRenderState		( nullptr )
	, m_MovieWidgetState	()
	, m_NowTime				( 0.0f )
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

	if( m_NowTime < m_MovieWidgetState.AciveStartTime ) return;

}

//--------------------------------.
// 描画関数.
//--------------------------------.
void CMovieWidget::Render()
{
	if( m_NowTime < m_MovieWidgetState.AciveStartTime ) return;
	if( m_pSprite == nullptr ) return;
	m_pSprite->Render( m_pRenderState );
}

//--------------------------------.
// 単純に描画する.
//--------------------------------.
void CMovieWidget::SimpleRender()
{
	if( m_pSprite == nullptr ) return;
	m_pSprite->Render( m_pRenderState );
}

//--------------------------------.
// 描画情報を設定する.
//--------------------------------.
void CMovieWidget::SettingRenderState()
{
	m_MovieWidgetState.AnimState.SetAnimState( &m_pRenderState->AnimState );
	m_pRenderState->Tranceform.Position.x = m_MovieWidgetState.Position.x;
	m_pRenderState->Tranceform.Position.y = m_MovieWidgetState.Position.y;
}