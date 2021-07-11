/**
* @file DebugText.h.
* @brief デバッグテキスト描画クラス.
* @author 福田玲也.
*/
#ifndef DEBUG_TEXT_H
#define DEBUG_TEXT_H

#include <queue>
#include <string>
#include <sstream>
#include <unordered_map>
#include <mutex>

#include "..\Common.h"

class CDebugTextShader;
class CFont;
class CSpriteRender;

/*********************************
*	デバッグテキスト描画クラス.
**/
class CDebugText : public CCommon
{
public:
	static constexpr char SEPARATOR[] = "----------------------------------------------------------";

private:
	static constexpr size_t	MAX_TEXT_LENGH	= 70;	// 最大文字数.
	static constexpr char	LOG_NAME[] = "Log";
	using text_list = std::vector<std::string>;
public:
	CDebugText();	// コンストラクタ
	virtual ~CDebugText();	// デストラクタ.

	// インスタンスの取得.
	static CDebugText* GetInstance();

	// 初期化関数.
	static HRESULT Init( ID3D11DeviceContext* pContext11 );

	// 描画関数(可変引数).
	template<class... T1>
	static void PushText( const char* tag, const T1&... t1 );
	// 描画関数(可変引数).
	template<class... T1>
	static void PushTextF( const char* tag, const char* fmt, T1... t1 );

	// 描画関数(可変引数).
	template<class... T1>
	static void PushLog( const T1&... t1 );
	// 描画関数.
	static void Render();

private:
	// 更新関数.
	void Update();
	// テクスチャの読み込み.
	void TextureLoad();

	// ページの切り替え.
	void ChangePage();
	// 色の変更.
	void ChangeColor();

	// 背景画像の描画.
	void BackSpriteRender();
	// フォントの描画.
	void FontRender();
	// ページの描画.
	void PageRender( D3DXVECTOR3& pos );

	// 時間の取得.
	std::string GetTime();

	template <class T>
	std::string to_std_string(const T& x);
	template<class T1, class... T2>
	std::string initstring( const T1& t1, const T2&... t2 );
	template<class T1>
	std::string initstring( const T1& t1 );

private:
	std::unordered_map<std::string, text_list>		m_TextList;			// テキストキュー.
	std::unordered_map<std::string, int>			m_TextCountList;	// テキストキュー.
	std::unique_ptr<CFont>			m_pFont;			// フォント.
	std::unique_ptr<CSpriteRender>	m_pSpriteRender;	// 画像描画クラス.
	std::vector<std::string>		m_TagList;			// タグリスト.
	CSprite*						m_pBackSprite;		// 背景画像.
	std::string						m_NowTagName;		// 現在のタグの名前.
	std::mutex						m_Mutex;
	D3DXVECTOR4						m_BackColor;		// 背景色.
	D3DXVECTOR4						m_FontColor;		// 文字色.
	D3DXVECTOR2						m_BackTextureSize;	// 背景の画像サイズ.
	int								m_TextIndex;		// テキストインデックス.
	int								m_NowTagIndex;		// 現在のタグインデックス.
	int								m_NowColorChangeNo;
	bool							m_IsRender;			// 描画できるか.
	bool							m_IsMoveing;		// 移動できるか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDebugText( const CDebugText & )				= delete;
	CDebugText& operator = ( const CDebugText & )	= delete;
	CDebugText( CDebugText && )						= delete;
	CDebugText& operator = ( CDebugText && )		= delete;
};

// 描画関数(可変引数).
template<class... T1>
void CDebugText::PushText( const char* tag, const T1&... t1 )
{
	std::unique_lock<std::mutex> lock( GetInstance()->m_Mutex );

	if( GetInstance()->m_pSpriteRender		== nullptr )	return;
	if( GetInstance()->m_pBackSprite		== nullptr )	return;
	if( GetInstance()->m_IsRender			== false )		return;

	text_list&	textList	= GetInstance()->m_TextList[tag];
	int&		count		= GetInstance()->m_TextCountList[tag];

	if( GetInstance()->m_NowTagName			== LOG_NAME )	return;

	const std::string text	= GetInstance()->initstring(t1...);
	if( text.length() >= MAX_TEXT_LENGH ) return;
	if( static_cast<int>(textList.size()) <= count ){
		textList.emplace_back( text );
	} else {
		textList[count] = text;
	}
	count++;
}

// 描画関数(可変引数).
template<class... T1>
void CDebugText::PushTextF( const char* tag, const char* fmt, T1... t1 )
{
	std::unique_lock<std::mutex> lock( GetInstance()->m_Mutex );

	if( GetInstance()->m_pSpriteRender		== nullptr )	return;
	if( GetInstance()->m_pBackSprite		== nullptr )	return;
	if( GetInstance()->m_IsRender			== false )		return;

	text_list&	textList	= GetInstance()->m_TextList[tag];
	int&		count		= GetInstance()->m_TextCountList[tag];

	if( GetInstance()->m_NowTagName			== LOG_NAME )	return;

	char color[256];
	const int size = sprintf( color, fmt, t1... );

	const std::string text	= GetInstance()->initstring(t1...);
	if( size >= MAX_TEXT_LENGH ) return;
	if( static_cast<int>(textList.size()) <= count ){
		textList.emplace_back( color );
	} else {
		textList[count] = color;
	}
	count++;
}

// 描画関数(可変引数).
template<class... T1>
void CDebugText::PushLog( const T1&... t1 )
{
	std::unique_lock<std::mutex> lock( GetInstance()->m_Mutex );

	const std::string text	= GetInstance()->GetTime() + GetInstance()->initstring(t1...);
	text_list&	textList	= GetInstance()->m_TextList[LOG_NAME];
	int&		count		= GetInstance()->m_TextCountList[LOG_NAME];

	textList.emplace_back( text );
	count++;
}

template <class T>
std::string CDebugText::to_std_string(const T& x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}
template<class T1, class... T2>
std::string CDebugText::initstring( const T1& t1, const T2&... t2 )
{
	std::string s = to_std_string(t1);
	return s + initstring(t2...);
}
template<class T1>
std::string CDebugText::initstring( const T1& t1 )
{
	std::string s = to_std_string(t1);
	return s;
}

#endif // #ifndef DEBUG_TEXT_H.