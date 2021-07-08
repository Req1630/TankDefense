#ifndef MOVIE_WIDGET_H
#define MOVIE_WIDGET_H

#include "..\Widget.h"

// ムービー(演出用)アニメーション情報.
struct stMovieAnimState
{
	D3DXVECTOR2	UVScrollSpeed;		// UVスクロール速度(x : 横,y : 縦).
	int			FrameCountSpeed;	// フレームカウント速度.
	bool		IsAnimation;		// アニメーションするか.
	bool		IsUVScrollX;		// 横向きにUVスクロールをするか.
	bool		IsUVScrollY;		// 縦向きにUVスクロールをするか.

	void SetAnimState( SSpriteAnimState* pState )
	{
		pState->ScrollSpeed		= this->UVScrollSpeed;
		pState->FrameCountSpeed	= this->FrameCountSpeed;
		pState->IsUVScrollX		= this->IsUVScrollX;
		pState->IsUVScrollY		= this->IsUVScrollY;
		pState->IsAnimation		= this->IsAnimation;
	};

} typedef SMovieAnimState;

// ムービー(演出用)フェード情報.
struct stMovieFadeState
{
	// フェードフラグ.
	enum enFadeFlag : unsigned int
	{
		None = 0,	// 何もなし.

		In,			// イン.
		Out,		// アウト.
		Wrap,		// 繰り返し.

		Max,
	} typedef EFadeFlag;

	int		FadeFlag;		// フェードフラグ.
	float	FadeSpeed;		// フェード速度.
	float	AlphaMax;		// アルファ最大値.
	float	AlphaMin;		// アルファ最小値.

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

// ムービー(演出用)画像情報.
struct stMovieWidget
{
	SMovieAnimState	AnimState;		// アニメーション情報.
	SMovieFadeState	FadeState;		// フェード情報.
	D3DXVECTOR2		Position;		// 開始座標.
	float			AciveStartTime;	// 動作開始時間(秒).
	std::string		SpriteName;		// 画像の名前.

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
*	ムービー用(演出)画像クラス.
**/
class CMovieWidget : public CWidget
{
public:
	CMovieWidget();
	virtual ~CMovieWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update( const float& deltaTime ) override;
	// 描画関数.
	virtual void Render() override;
	// 単純に描画する.
	virtual void SimpleRender();

	// ムービー用(演出)情報の設定.
	inline void SetMovieWidgetState( const SMovieWidget& state ){ m_MovieWidgetState = state; }
	// ムービー用(演出)情報の取得.
	inline SMovieWidget GetMovieWidgetState() const { return m_MovieWidgetState; }

	// 描画情報を設定する.
	void SettingRenderState( const bool& isEdit = false );

	// 再生.
	void Play();
	// リセット.
	void Reset();

private:
	// フェードの開始.
	void StartFade();
	// フェード繰り返しの設定.
	void SetFadeWrap( const float& speed );
	// フェード繰り返しの更新.
	void FadeWrapUpdate();

private:
	SSpriteRenderState*	m_pRenderState;		// 描画情報.
	SMovieWidget		m_MovieWidgetState;	// ムービー用情報.
	float				m_NowTime;			// 現在の経過時間.
	bool				m_IsOncePlay;		// フェードを開始したか.
	bool				m_IsPlay;			// 再生してるか.
};

#endif	// #ifndef MOVIE_WIDGET_H.