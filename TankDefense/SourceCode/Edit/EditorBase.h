#ifndef EDITOR_BASE_H
#define EDITOR_BASE_H

#include "..\Global.h"
#include "..\Utility\ImGuiManager\ImGuiManager.h"

class CEditPlayer;

/**********************************
*	�G�f�B�^���N���X.
**/
class CEditorBase
{
public:
	CEditorBase( )
		: m_MessageText			( "" )
		, m_MessageRenderCount	( 0.0f )
		, m_IsImGuiGamepad		( true )
	{}
	virtual ~CEditorBase()
	{}

	// ������.
	virtual bool Init() = 0;
	// �X�V.
	virtual void Update() = 0;
	// ImGui�`��.
	virtual bool ImGuiRender() = 0;
	// ���f���`��.
	virtual void ModelRender() = 0;
	// �G�t�F�N�g�`��.
	virtual void EffectRneder() = 0;
	// �E�B�W�F�b�g�`��.
	virtual void WidgetRender() = 0;

	// �G�f�B�^�p�v���C���[�̐ݒ�.
	virtual void SetEditPlayer( CEditPlayer* pPlayer ){}

	// ImGui�ŃQ�[���p�b�h�����L���ɂ���.
	void OnImGuiGamepad();
	// ImGui�ŃQ�[���p�b�h����𖳌��ɂ���.
	void OffImGuiGamepad();

protected:
	// ImGui�^�u�̊J�n.
	bool BeginTab( const char* tabName );
	// ImGui�^�u�̏I��.
	void EndTab();

	// �ۑ��{�^��.
	void SaveButton( const char* filePath );
	// �Ǎ��{�^��.
	void LoadButton( const char* filePath );
	// ���b�Z�[�W�̕`��.
	void MessageRender();

	// �p�����[�^�̏�������.
	virtual void ParameterWriting( const char* filePath ) = 0;
	// �p�����[�^�̓ǂݍ���.
	virtual void ParameterLoading( const char* filePath ) = 0;

	// �p�����[�^�̏������݃��b�Z�[�W�̐ݒ�.
	bool SetParameterWritingMsg( const bool& isSucceeded );
	// �p�����[�^�̓ǂݍ��݃��b�Z�[�W�̐ݒ�.
	bool SetParameterLoadingMsg( const bool& isSucceeded );

protected:
	std::string		m_MessageText;			// ���b�Z�[�W�e�L�X�g.
	float			m_MessageRenderCount;	// ���b�Z�[�W�`��J�E���g.
	bool			m_IsImGuiGamepad;		// �R���g���[���[��

};

#endif	// #ifndef EDITOR_BASE_H.