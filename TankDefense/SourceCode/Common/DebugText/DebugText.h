/**
* @file DebugText.h.
* @brief �f�o�b�O�e�L�X�g�`��N���X.
* @author ���c���.
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
*	�f�o�b�O�e�L�X�g�`��N���X.
**/
class CDebugText : public CCommon
{
public:
	static constexpr char SEPARATOR[] = "----------------------------------------------------------";

private:
	static constexpr size_t	MAX_TEXT_LENGH	= 70;	// �ő啶����.
	static constexpr char	LOG_NAME[] = "Log";
	using text_list = std::vector<std::string>;
public:
	CDebugText();	// �R���X�g���N�^
	virtual ~CDebugText();	// �f�X�g���N�^.

	// �C���X�^���X�̎擾.
	static CDebugText* GetInstance();

	// �������֐�.
	static HRESULT Init( ID3D11DeviceContext* pContext11 );

	// �`��֐�(�ψ���).
	template<class... T1>
	static void PushText( const char* tag, const T1&... t1 );
	// �`��֐�(�ψ���).
	template<class... T1>
	static void PushTextF( const char* tag, const char* fmt, T1... t1 );

	// �`��֐�(�ψ���).
	template<class... T1>
	static void PushLog( const T1&... t1 );
	// �`��֐�.
	static void Render();

private:
	// �X�V�֐�.
	void Update();
	// �e�N�X�`���̓ǂݍ���.
	void TextureLoad();

	// �y�[�W�̐؂�ւ�.
	void ChangePage();
	// �F�̕ύX.
	void ChangeColor();

	// �w�i�摜�̕`��.
	void BackSpriteRender();
	// �t�H���g�̕`��.
	void FontRender();
	// �y�[�W�̕`��.
	void PageRender( D3DXVECTOR3& pos );

	// ���Ԃ̎擾.
	std::string GetTime();

	template <class T>
	std::string to_std_string(const T& x);
	template<class T1, class... T2>
	std::string initstring( const T1& t1, const T2&... t2 );
	template<class T1>
	std::string initstring( const T1& t1 );

private:
	std::unordered_map<std::string, text_list>		m_TextList;			// �e�L�X�g�L���[.
	std::unordered_map<std::string, int>			m_TextCountList;	// �e�L�X�g�L���[.
	std::unique_ptr<CFont>			m_pFont;			// �t�H���g.
	std::unique_ptr<CSpriteRender>	m_pSpriteRender;	// �摜�`��N���X.
	std::vector<std::string>		m_TagList;			// �^�O���X�g.
	CSprite*						m_pBackSprite;		// �w�i�摜.
	std::string						m_NowTagName;		// ���݂̃^�O�̖��O.
	std::mutex						m_Mutex;
	D3DXVECTOR4						m_BackColor;		// �w�i�F.
	D3DXVECTOR4						m_FontColor;		// �����F.
	D3DXVECTOR2						m_BackTextureSize;	// �w�i�̉摜�T�C�Y.
	int								m_TextIndex;		// �e�L�X�g�C���f�b�N�X.
	int								m_NowTagIndex;		// ���݂̃^�O�C���f�b�N�X.
	int								m_NowColorChangeNo;
	bool							m_IsRender;			// �`��ł��邩.
	bool							m_IsMoveing;		// �ړ��ł��邩.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDebugText( const CDebugText & )				= delete;
	CDebugText& operator = ( const CDebugText & )	= delete;
	CDebugText( CDebugText && )						= delete;
	CDebugText& operator = ( CDebugText && )		= delete;
};

// �`��֐�(�ψ���).
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

// �`��֐�(�ψ���).
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

// �`��֐�(�ψ���).
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