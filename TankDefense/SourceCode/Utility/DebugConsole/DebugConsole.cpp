#include "DebugConsole.h"

// localtimeのunsafe警告エラーを無効化.
#pragma warning(disable:4996)

#include <memory>
#include <chrono>
#include <iomanip>

namespace
{
	const std::string WARNING_MSG = 
		{
			"コンソールをクリックしたタイミングによっては、\n"
			"動いているアプリが止まったりすることがあります。\n"
			"その際は、コンソールをアクティブにしてEnterキーを押してください。"
		};
};

CDebugConsole::CDebugConsole()
	: m_hOutput		( nullptr )
	, m_Mutex		()
	, m_LogList		()
	, m_TextQueue	()
	, m_ClearChar	()
	, m_IsLogRender	( true )
{
}

CDebugConsole::~CDebugConsole()
{
}

//-------------------------------.
// インスタンスの取得.
//-------------------------------.
CDebugConsole* CDebugConsole::GetInstance()
{
	// インスタンスの作成.
	static std::unique_ptr<CDebugConsole> pInstance = std::make_unique<CDebugConsole>();
	return pInstance.get();
}

//-------------------------------.
// 初期化.
//-------------------------------.
void CDebugConsole::Init()
{
	AllocConsole();	// コンソールの構築.
	// ハンドルの取得.
	GetInstance()->m_hOutput = GetStdHandle( STD_OUTPUT_HANDLE );

	// 標準出力を向ける場合.
	FILE* fp;
	freopen_s( &fp, "CONOUT$", "w", stdout );	// 標準出力(stdout)を新しいコンソールに向ける.
	freopen_s( &fp, "CONOUT$", "w", stdin );	// 標準入力(stdin)を新しいコンソールに向ける.
	freopen_s( &fp, "CONOUT$", "w", stderr );	// 標準エラー出力(stderr)を新しいコンソールに向ける.
	fclose( fp );
	fp = nullptr;

	// コンソールの設定.
	HWND hWnd = GetConsoleWindow();
	// メニューハンドルの取得.
	HMENU hMenu = GetSystemMenu( hWnd, FALSE );
	// Xボタン削除.
	RemoveMenu( hMenu, SC_CLOSE, MF_BYCOMMAND );

	// ウィンドウのサイズ変更を無効化.
	const LONG flag = WS_OVERLAPPEDWINDOW& ~WS_THICKFRAME& ~WS_MAXIMIZEBOX;
	SetWindowLong( hWnd, GWL_STYLE, flag );
	ShowWindow( hWnd, SW_SHOW );

	// コンソールタイトル名変更.
	SetConsoleTitle( "DebugConsole" );

	// コンソールのカーソルを消す.
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo( GetInstance()->m_hOutput, &cci );
	cci.bVisible = FALSE;
	SetConsoleCursorInfo( GetInstance()->m_hOutput, &cci );

	SetConsoleTextAttribute( GetInstance()->m_hOutput, 12 + (15 << 4) );
	WriteConsole( GetInstance()->m_hOutput, WARNING_MSG.c_str(), WARNING_MSG.size(), nullptr, nullptr );
	SetConsoleTextAttribute( GetInstance()->m_hOutput, 15 + (0 << 4) );

	COORD coord = GetLargestConsoleWindowSize( GetInstance()->m_hOutput );

	for( int i = 0; i < coord.X/2; i++ ) GetInstance()->m_ClearChar += " ";
}

//-------------------------------.
// 解放.
//-------------------------------.
void CDebugConsole::Release()
{
#if 0	// ハンドルを閉じるとエラーがおこるので実行しない.
	// ハンドルを閉じる.
	BOOL isEnd = FALSE;
	if( GetInstance()->m_hOutput != INVALID_HANDLE_VALUE ){
		isEnd = CloseHandle( GetInstance()->m_hOutput );
	}
#endif
	FreeConsole();	// コンソールの解放.
}

//-------------------------------.
// 描画.
//-------------------------------.
void CDebugConsole::Render()
{
	if( GetAsyncKeyState('L') & 0x0001 ){
		GetInstance()->m_IsLogRender = !GetInstance()->m_IsLogRender;
		system( "cls" );
		CDebugConsole::SetPosition( 0, 0 );
		SetConsoleTextAttribute( GetInstance()->m_hOutput, 12 + (15 << 4) );
		WriteConsole( GetInstance()->m_hOutput, WARNING_MSG.c_str(), WARNING_MSG.size(), nullptr, nullptr );
		SetConsoleTextAttribute( GetInstance()->m_hOutput, 15 + (0 << 4) );
	}

	CDebugConsole::SetPosition( 0, 4 );
	if( GetInstance()->m_IsLogRender == false ){
		std::queue<std::string>& textQueue = GetInstance()->m_TextQueue;

		const int queueSize = static_cast<int>(textQueue.size());
		for( int i = 0; i < queueSize; i++ ){
			const std::string msg = textQueue.front();
			textQueue.pop();
			if( msg.size() >= GetInstance()->m_ClearChar.size() ) continue;

			std::string ss = GetInstance()->m_ClearChar;
			ss.erase( msg.size() );
			ss.insert( 0, msg );
			ss += "\n";
			WriteConsole( GetInstance()->m_hOutput, ss.c_str(), ss.size(), nullptr, nullptr );
		}
	} else {
		for( auto& l : GetInstance()->m_LogList ){
			if( l.size() >= GetInstance()->m_ClearChar.size() ) continue;

			std::string ss = GetInstance()->m_ClearChar;
			ss.erase( l.size() );
			ss.insert( 0, l );
			ss += "\n";
			WriteConsole( GetInstance()->m_hOutput, ss.c_str(), ss.size(), nullptr, nullptr );
		}
	}


}

//-------------------------------.
// 座標の設定.
//-------------------------------.
void CDebugConsole::SetPosition( const int& x, const int& y )
{
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition( GetInstance()->m_hOutput, coord );
}

//-------------------------------.
// 時間の書き込み.
//-------------------------------.
std::string CDebugConsole::PrintTime()
{
	auto now	= std::chrono::system_clock::now();
	auto now_c	= std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time( localtime(&now_c), "%T" );
	std::string timeMsg = "[" + ss.str() + "] ";
	return timeMsg;
}