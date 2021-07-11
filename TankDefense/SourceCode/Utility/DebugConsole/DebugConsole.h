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
*	�f�o�b�O�R���\�[���N���X.
**/
class CDebugConsole
{
	using text_list = std::vector<std::string>;
	static constexpr int LOG_TAG_INDEX = -1;
public:
	CDebugConsole();
	~CDebugConsole();

	// �C���X�^���X�̎擾.
	static CDebugConsole* GetInstance();

	// ������.
	static void Init();
	// ���.
	static void Release();

	// �`��.
	static void Render();

	// �R���\�[���ɏ�������.
	template<class... T>
	static void PushText( const char* tag, const int& pos, const T&... t );

	// ���O�̒ǉ�.
	template<class... T>
	static void PushLog( const T&... t );

	// ���W�̐ݒ�.
	static void SetPosition( const int& x, const int& y );

private:
	template <class T>
	std::string to_std_string( const T& t );
	template<class T1, class... T2>
	std::string initstring( const T1& t1, const T2&... t2 );
	template<class T>
	std::string initstring( const T& t );

	// ���Ԃ̏�������.
	std::string PrintTime();

	// �^�O���X�g�̍X�V.
	void UpdateTagList();

	// �y�[�W�̕\��.
	void PageDraw();
	// ���O�̕\��.
	void LogDraw();
	// �e�L�X�g�̕\��.
	void TextDraw();

	// �\���̐؂�ւ�.
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
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDebugConsole( const CDebugConsole & )				= delete;
	CDebugConsole& operator = ( const CDebugConsole & )	= delete;
	CDebugConsole( CDebugConsole && )					= delete;
	CDebugConsole& operator = ( CDebugConsole && )		= delete;
};

//--------------------------------------.
// �R���\�[���ɏ�������.
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
// ���O�̒ǉ�.
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
// �ψ����̒l�𕶎���ɍċA�I�ɕϊ�.
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