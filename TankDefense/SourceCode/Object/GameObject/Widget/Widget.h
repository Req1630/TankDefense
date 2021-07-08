/**
* @file Widget.h.
* @brief �Q�[����UI���N���X.
* @author ���c���.
*/
#ifndef WIDGET_H
#define WIDGET_H

#include "..\GameObject.h"
#include "..\..\..\Common\Sprite\Sprite.h"

/**************************************
*	�Q�[������UI���N���X.
**/
class CWidget : public CGameObject
{
protected:
	// �t�F�[�h�̏��.
	enum class enFadeState
	{
		In,			// �t�F�[�h�C��.
		Out,		// �t�F�[�h�A�E�g.
		Finish,		// �t�F�[�h�I��.
	} typedef EFadeState;

public:
	CWidget();
	virtual ~CWidget();

	// �t�F�[�h�C���̐ݒ�.
	void SetFadeIn( const float& speedSec );
	// �t�F�[�h�A�E�g�̐ݒ�.
	void SetFadeOut( const float& speedSec );

protected:
	// �摜�̐ݒ�.
	virtual bool SpriteSetting( const char* sprite, const int& stateNum = 1 );

	// �t�F�[�h�̍X�V.
	void FadeUpdate( float& alpha );
	// �t�F�[�h�C��.
	void FadeIn( float& alpha );
	// �t�F�[�h�A�E�g.
	void FadeOut( float& alpha );

private:
	// �A���t�@�̌v�Z.
	void AlphaCalc( float& alpha );

protected:
	CSprite*						m_pSprite;		// �摜�N���X.
	std::vector<SSpriteRenderState>	m_RenderStates;	// �摜�`��X�e�[�g���X�g.
	float							m_Alpha;		// �A���t�@�l.
	float							m_AlphaMax;		// �A���t�@�ő�l.
	float							m_AlphaMin;		// �A���t�@�ŏ��l.
	float							m_FadeSpeed;	// �t�F�[�h���x.
	EFadeState						m_NowFadeState;	// ���݂̃t�F�[�h���.
	EFadeState						m_OldFadeState;	// �ߋ��̃t�F�[�h���.
};

#endif	// #ifndef WIDGET_H.
