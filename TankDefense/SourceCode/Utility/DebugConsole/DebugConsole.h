#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <mutex>
#include <queue>

/****************************
*	�f�o�b�O�R���\�[���N���X.
**/
class CDebugConsole
{
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
	static void PushText( const T&... t );

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

private:
	HANDLE					m_hOutput;
	std::mutex				m_Mutex;
	std::vector<std::string>m_LogList;
	std::queue<std::string>	m_TextQueue;	// �e�L�X�g�L���[.
	std::string				m_ClearChar;
	bool					m_IsLogRender;
	
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
void CDebugConsole::PushText( const T&... t )
{
	std::unique_lock<std::mutex> lock( GetInstance()->m_Mutex );
	if( GetInstance()->m_IsLogRender == true ) return;

	const std::string ss = GetInstance()->initstring( t... );
	
	GetInstance()->m_TextQueue.push( ss );
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