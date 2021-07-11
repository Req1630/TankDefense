#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include <Windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <mutex>
#include <queue>

/****************************
*	デバッグコンソールクラス.
**/
class CDebugConsole
{
	using text_list = std::vector<std::string>;
	static constexpr int LOG_TAG_INDEX = -1;
public:
	CDebugConsole();
	~CDebugConsole();

	// インスタンスの取得.
	static CDebugConsole* GetInstance();

	// 初期化.
	static void Init();
	// 解放.
	static void Release();

	// 描画.
	static void Render();

	// コンソールに書き込む.
	template<class... T>
	static void PushText( const char* tag, const int& pos, const T&... t );

	// ログの追加.
	template<class... T>
	static void PushLog( const T&... t );

	// 座標の設定.
	static void SetPosition( const int& x, const int& y );

private:
	template <class T>
	std::string to_std_string( const T& t );
	template<class T1, class... T2>
	std::string initstring( const T1& t1, const T2&... t2 );
	template<class T>
	std::string initstring( const T& t );

	// 時間の書き込み.
	std::string PrintTime();

	// タグリストの更新.
	void UpdateTagList();

	// ページの表示.
	void PageDraw();
	// ログの表示.
	void LogDraw();
	// テキストの表示.
	void TextDraw();

	// 表示の切り替え.
	void ChangeDraw();

private:
	HANDLE										m_hOutput;
	std::mutex									m_Mutex;
	text_list									m_LogList;
	text_list									m_OldTextList;
	std::unordered_map<std::string, text_list>	m_TextList;
	std::unordered_map<std::string, int>		m_TextListCount;
	std::vector<std::string>					m_TagList;
	std::string									m_ClearChar;
	std::string									m_SepacterChar;
	int											m_NowSelectIndex;
	bool										m_IsLogRenderEnd;
	
private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDebugConsole( const CDebugConsole & )				= delete;
	CDebugConsole& operator = ( const CDebugConsole & )	= delete;
	CDebugConsole( CDebugConsole && )					= delete;
	CDebugConsole& operator = ( CDebugConsole && )		= delete;
};

//--------------------------------------.
// コンソールに書き込む.
//--------------------------------------.
template<class... T>
void CDebugConsole::PushText( const char* tag, const int& pos, const T&... t )
{
	std::unique_lock<std::mutex> lock( GetInstance()->m_Mutex );
	text_list& textList = GetInstance()->m_TextList[tag];

	if( GetInstance()->m_NowSelectIndex == LOG_TAG_INDEX ) return;

	int& count = GetInstance()->m_TextListCount[tag];

	const std::string ss = GetInstance()->initstring( t... );
	if( static_cast<int>(textList.size()) <= count ){
		textList.emplace_back( ss );
	} else {
		textList[count] = ss;
	}
	count++;
}

//--------------------------------------.
// ログの追加.
//--------------------------------------.
template<class... T>
void CDebugConsole::PushLog( const T&... t )
{
	std::unique_lock<std::mutex> lock( GetInstance()->m_Mutex );

	const std::string ss = GetInstance()->PrintTime() + GetInstance()->initstring( t... );

	GetInstance()->m_LogList.emplace_back( ss );
	GetInstance()->m_IsLogRenderEnd = false;
}

//--------------------------------------.
// 可変引数の値を文字列に再帰的に変換.
//--------------------------------------.
template <class T>
std::string CDebugConsole::to_std_string( const T& t )
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

template<class T1, class... T2>
std::string CDebugConsole::initstring( const T1& t1, const T2&... t2 )
{
	std::string s = to_std_string(t1);
	return s + initstring(t2...);
}

template<class T>
std::string CDebugConsole::initstring( const T& t )
{
	std::string s = to_std_string(t);
	return s;
}

#endif	// #ifndef DEBUG_CONSOLE_H.