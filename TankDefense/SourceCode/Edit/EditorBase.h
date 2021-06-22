#ifndef EDITOR_BASE_H
#define EDITOR_BASE_H

#include "..\Global.h"
#include "..\Utility\ImGuiManager\ImGuiManager.h"

/**********************************
*	�G�f�B�^���N���X.
**/
class CEditorBase
{
public:
	CEditorBase( )
		: m_MessageText			( "" )
		, m_MessageRenderCount	( 0.0f )
	{}
	virtual ~CEditorBase()
	{}

	// ������.
	virtual bool Init() = 0;
	// �X�V.
	virtual void Update() = 0;
	// ImGui�`��.
	virtual void ImGuiRender() = 0;
	// ���f���`��.
	virtual void ModelRender() = 0;
	// �G�t�F�N�g�`��.
	virtual void EffectRneder() = 0;

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
	void SetParameterWritingMsg( const bool& isSucceeded );
	// �p�����[�^�̓ǂݍ��݃��b�Z�[�W�̐ݒ�.
	void SetParameterLoadingMsg( const bool& isSucceeded );

protected:
	std::string		m_MessageText;			// ���b�Z�[�W�e�L�X�g.
	float			m_MessageRenderCount;	// ���b�Z�[�W�`��J�E���g.

};

#endif	// #ifndef EDITOR_BASE_H.