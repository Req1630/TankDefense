#include "DebugConsole.h"
#include "..\Input\Input.h"

// localtime��unsafe�x���G���[�𖳌���.
#pragma warning(disable:4996)

#include <memory>
#include <chrono>
#include <iomanip>

namespace
{
	constexpr int PRINT_PAGE_START_POS		= 5;	// �y�[�W�\���̈ʒu.
	constexpr int PRINT_TEXT_START_POS_Y	= 8;	// �o�͊J�n�ʒu.
	constexpr int PRINT_TEXT_START_POS_X	= 2;	// �o�͊J�n�ʒu.
	const std::string WARNING_MSG = 
		{
			"�R���\�[�����N���b�N�����^�C�~���O�ɂ���ẮA\n"
			"�����Ă���A�v�����~�܂����肷�邱�Ƃ�����܂��B\n"
			"���̍ۂ́A�R���\�[�����A�N�e�B�u�ɂ���Enter�L�[�������Ă��������B"
		};
};

CDebugConsole::CDebugConsole()
	: m_hOutput			( nullptr )
	, m_Mutex			()
	, m_LogList			()
	, m_OldTextList		()
	, m_TextList		()
	, m_TagList			()
	, m_ClearChar		()
	, m_SepacterChar	()
	, m_NowSelectIndex	( LOG_TAG_INDEX )
	, m_IsLogRenderEnd	( false )
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

	for( int i = 0; i < coord.X/2-1; i++ ){
		GetInstance()->m_ClearChar		+= " ";
		GetInstance()->m_SepacterChar	+= "-";
	}
	GetInstance()->m_ClearChar.front()		= 'l';
	GetInstance()->m_ClearChar.back()		= 'l';
	GetInstance()->m_SepacterChar.front()	= 'l';
	GetInstance()->m_SepacterChar.back()	= 'l';

	GetInstance()->PageDraw();
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
	GetInstance()->ChangeDraw();
	GetInstance()->UpdateTagList();

	
	if( GetInstance()->m_NowSelectIndex == LOG_TAG_INDEX ){
		GetInstance()->LogDraw();
	} else {
		GetInstance()->TextDraw();
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

//-------------------------------.
// �^�O���X�g�̍X�V.
//-------------------------------.
void CDebugConsole::UpdateTagList()
{
	const size_t tagSize = static_cast<int>(m_TextList.size());
	if( tagSize != m_TagList.size() ) m_TagList.resize( tagSize );
	int i = 0;
	for( auto& l : m_TextList ){
		m_TagList[i] = l.first;
		i++;
	}
}

//-------------------------------.
// �y�[�W�̕\��.
//-------------------------------.
void CDebugConsole::PageDraw()
{
	std::vector<std::string> tagList;
	if( m_TagList.empty() == false ){
		for( int i = m_NowSelectIndex-1; i <= m_NowSelectIndex+1; i++ ){
			if( i < LOG_TAG_INDEX ){
				tagList.emplace_back( m_TagList.back() );
			} else if( i > m_TagList.size()-1 ){
				tagList.emplace_back( "Log" );
			} else {
				tagList.emplace_back( m_TagList[i] );
			}
		}
	} else {
		for( int i = m_NowSelectIndex-1; i <= m_NowSelectIndex+1; i++ ){
			tagList.emplace_back( "Log" );
		}
	}

	CDebugConsole::SetPosition( 0, PRINT_PAGE_START_POS-1 );
	WriteConsole( m_hOutput, m_SepacterChar.c_str(), m_SepacterChar.size(), nullptr, nullptr );

	CDebugConsole::SetPosition( 0, PRINT_PAGE_START_POS );
	std::string pageNum = " " + std::to_string(m_NowSelectIndex+2) + "/" + std::to_string(m_TagList.size()+1);
	std::string mag = "[" + tagList[0] + "] << Back  [" + tagList[1] + pageNum + "]  Next >> [" + tagList[2] + "]";
	mag += "        \"Left Ctrl\" + \"�� or ��\"";
	std::string ss = m_ClearChar;
	ss.erase( PRINT_PAGE_START_POS, mag.size() ).insert( PRINT_PAGE_START_POS, mag );
	WriteConsole( m_hOutput, ss.c_str(), ss.size(), nullptr, nullptr );

	CDebugConsole::SetPosition( 0, PRINT_PAGE_START_POS+1 );
	WriteConsole( m_hOutput, m_SepacterChar.c_str(), m_SepacterChar.size(), nullptr, nullptr );

	CDebugConsole::SetPosition( 0, PRINT_TEXT_START_POS_Y-1 );
	WriteConsole( GetInstance()->m_hOutput, GetInstance()->m_ClearChar.c_str(), GetInstance()->m_ClearChar.size(), nullptr, nullptr );
	CDebugConsole::SetPosition( 0, PRINT_TEXT_START_POS_Y );
}

//-------------------------------.
// ���O�̕\��.
//-------------------------------.
void CDebugConsole::LogDraw()
{
	if( m_IsLogRenderEnd == true ) return;

	int posY = PRINT_TEXT_START_POS_Y;

	for( auto& l : m_LogList ){
		if( l.size() >= m_ClearChar.size() ) continue;

		CDebugConsole::SetPosition( 0, posY );
		WriteConsole( m_hOutput, "l", 1, nullptr, nullptr );
		CDebugConsole::SetPosition( PRINT_TEXT_START_POS_X, posY );
		WriteConsole( m_hOutput, l.c_str(), l.size(), nullptr, nullptr );
		CDebugConsole::SetPosition( m_SepacterChar.size()-1, posY );
		WriteConsole( m_hOutput, "l", 1, nullptr, nullptr );

		posY++;
	}
	m_IsLogRenderEnd = true;
}

//-------------------------------.
// �e�L�X�g�̕\��.
//-------------------------------.
void CDebugConsole::TextDraw()
{
	const char* tagNme = m_TagList[m_NowSelectIndex].c_str();

	int posY = PRINT_TEXT_START_POS_Y;
	int i = 0;
	for( auto& s : m_TextList[tagNme] ){

		if( i >= m_OldTextList.size() ){
			m_OldTextList.emplace_back( s );
		} else {
			if( m_OldTextList[i] == s ){
				posY++;
				i++;
				s = m_ClearChar;
				continue;
			}
		}

		CDebugConsole::SetPosition( 0, posY );
		WriteConsole( m_hOutput, "l", 1, nullptr, nullptr );
		CDebugConsole::SetPosition( PRINT_TEXT_START_POS_X, posY );
		WriteConsole( m_hOutput, s.c_str(), s.size(), nullptr, nullptr );
		CDebugConsole::SetPosition( m_SepacterChar.size()-1, posY );
		WriteConsole( m_hOutput, "l", 1, nullptr, nullptr );

		posY++;
		i++;
		s = m_ClearChar;
	}
	for( auto& l : m_TextListCount ) l.second = 0;
}

//-------------------------------.
// �\���̐؂�ւ�.
//-------------------------------.
void CDebugConsole::ChangeDraw()
{
	bool isChange = false;
	const int tagSize = static_cast<int>(m_TextList.size());
	if( CKeyInput::IsHold( VK_LCONTROL ) && CKeyInput::IsMomentPress( VK_RIGHT ) ){
		m_NowSelectIndex++;
		isChange = true;
		if( m_NowSelectIndex > tagSize-1 )		m_NowSelectIndex = LOG_TAG_INDEX;
	}
	if( CKeyInput::IsHold( VK_LCONTROL ) && CKeyInput::IsMomentPress( VK_LEFT ) ){
		m_NowSelectIndex--;
		isChange = true;
		if( m_NowSelectIndex < LOG_TAG_INDEX )	m_NowSelectIndex = tagSize-1;
	}

	if( isChange == true ){
		system( "cls" );
		CDebugConsole::SetPosition( 0, 0 );

		SetConsoleTextAttribute( GetInstance()->m_hOutput, 12 + (15 << 4) );
		WriteConsole( GetInstance()->m_hOutput, WARNING_MSG.c_str(), WARNING_MSG.size(), nullptr, nullptr );
		SetConsoleTextAttribute( GetInstance()->m_hOutput, 15 + (0 << 4) );

		PageDraw();

		m_OldTextList.clear();
		m_IsLogRenderEnd = false;
	}
}