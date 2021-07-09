#include "DebugConsole.h"

// localtime��unsafe�x���G���[�𖳌���.
#pragma warning(disable:4996)

#include <memory>
#include <chrono>
#include <iomanip>

namespace
{
	const std::string WARNING_MSG = 
		{
			"�R���\�[�����N���b�N�����^�C�~���O�ɂ���ẮA\n"
			"�����Ă���A�v�����~�܂����肷�邱�Ƃ�����܂��B\n"
			"���̍ۂ́A�R���\�[�����A�N�e�B�u�ɂ���Enter�L�[�������Ă��������B"
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
// �C���X�^���X�̎擾.
//-------------------------------.
CDebugConsole* CDebugConsole::GetInstance()
{
	// �C���X�^���X�̍쐬.
	static std::unique_ptr<CDebugConsole> pInstance = std::make_unique<CDebugConsole>();
	return pInstance.get();
}

//-------------------------------.
// ������.
//-------------------------------.
void CDebugConsole::Init()
{
	AllocConsole();	// �R���\�[���̍\�z.
	// �n���h���̎擾.
	GetInstance()->m_hOutput = GetStdHandle( STD_OUTPUT_HANDLE );

	// �W���o�͂�������ꍇ.
	FILE* fp;
	freopen_s( &fp, "CONOUT$", "w", stdout );	// �W���o��(stdout)��V�����R���\�[���Ɍ�����.
	freopen_s( &fp, "CONOUT$", "w", stdin );	// �W������(stdin)��V�����R���\�[���Ɍ�����.
	freopen_s( &fp, "CONOUT$", "w", stderr );	// �W���G���[�o��(stderr)��V�����R���\�[���Ɍ�����.
	fclose( fp );
	fp = nullptr;

	// �R���\�[���̐ݒ�.
	HWND hWnd = GetConsoleWindow();
	// ���j���[�n���h���̎擾.
	HMENU hMenu = GetSystemMenu( hWnd, FALSE );
	// X�{�^���폜.
	RemoveMenu( hMenu, SC_CLOSE, MF_BYCOMMAND );

	// �E�B���h�E�̃T�C�Y�ύX�𖳌���.
	const LONG flag = WS_OVERLAPPEDWINDOW& ~WS_THICKFRAME& ~WS_MAXIMIZEBOX;
	SetWindowLong( hWnd, GWL_STYLE, flag );
	ShowWindow( hWnd, SW_SHOW );

	// �R���\�[���^�C�g�����ύX.
	SetConsoleTitle( "DebugConsole" );

	// �R���\�[���̃J�[�\��������.
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
// ���.
//-------------------------------.
void CDebugConsole::Release()
{
#if 0	// �n���h�������ƃG���[��������̂Ŏ��s���Ȃ�.
	// �n���h�������.
	BOOL isEnd = FALSE;
	if( GetInstance()->m_hOutput != INVALID_HANDLE_VALUE ){
		isEnd = CloseHandle( GetInstance()->m_hOutput );
	}
#endif
	FreeConsole();	// �R���\�[���̉��.
}

//-------------------------------.
// �`��.
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
// ���W�̐ݒ�.
//-------------------------------.
void CDebugConsole::SetPosition( const int& x, const int& y )
{
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition( GetInstance()->m_hOutput, coord );
}

//-------------------------------.
// ���Ԃ̏�������.
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