#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <mutex>
#include <queue>

/****************************
*	デバッグコンソールクラス.
**/
class CDebugConsole
{
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
	static void PushText( const T&... t );

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

private:
	HANDLE					m_hOutput;
	std::mutex				m_Mutex;
	std::vector<std::string>m_LogList;
	std::queue<std::string>	m_TextQueue;	// テキストキュー.
	std::string				m_ClearChar;
	bool					m_IsLogRender;
	
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
void CDebugConsole::PushText( const T&... t )
{
	std::unique_lock<std::mutex> lock( GetInstance()->m_Mutex );
	if( GetInstance()->m_IsLogRender == true ) return;

	const std::string ss = GetInstance()->initstring( t... );
	
	GetInstance()->m_TextQueue.push( ss );
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