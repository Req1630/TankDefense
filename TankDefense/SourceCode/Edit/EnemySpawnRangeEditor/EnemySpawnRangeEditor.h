#ifndef ENEMY_SPAEN_RANGE_EDITOR_H
#define ENEMY_SPAEN_RANGE_EDITOR_H

#include "..\EditorBase.h"
#include "..\UndoRedo\UndoRedo.h"
#include "..\StageEditor\StageEditor.h"

class CAuraMesh;		// �I�[�����b�V���N���X.
class CStageRender;		// �X�e�[�W�`��N���X.

/***************************************************
* �G�X�|�[���͈̓G�f�B�^.
**/
class CEnemySpawnRangeEditor : public CEditorBase
{
	struct stBoxRange
	{
		D3DXVECTOR2 Range;		// �͈�,
		STranceform	Tranceform;	// ���W�Ȃ�.
		stBoxRange()
			: Range			( 10.0f, 10.0f )
			, Tranceform	()
		{}
		stBoxRange( const D3DXVECTOR2& range, const STranceform& t )
			: Range			( range )
			, Tranceform	( t )
		{}
	} typedef SBoxRange;
public:
	CEnemySpawnRangeEditor();
	virtual ~CEnemySpawnRangeEditor();

	// ������.
	virtual bool Init() override;
	// �X�V.
	virtual void Update() override;
	// ImGui�`��.
	virtual bool ImGuiRender() override;
	// ���f���`��.
	virtual void ModelRender() override;
	// �G�t�F�N�g�`��.
	virtual void EffectRneder() override;

	// �G�f�B�^�p�v���C���[�̐ݒ�.
	virtual void SetEditPlayer( CEditPlayer* pPlayer ) override { m_pEditPlayer = pPlayer; }

private:
	// �z�u�����̐؂�ւ�.
	void ChangeArrangement();
	// �A�N�^�[�̍폜����.
	void DeleteActor();
	// �z�u����͈͂̕ύX�\��.
	void ChangeRangeDraw();
	// �폜����A�N�^�[���b�V���I���̕\��.
	void DelteActorMeshSelectDraw();
	// �A���h�D/���h�D�̕\��.
	void UndoRedoDraw();
	// ��������̕\��.
	void ControllerDraw();

	// �p�����[�^�̏�������.
	virtual void ParameterWriting( const char* filePath ) override;
	// �p�����[�^�̓ǂݍ���.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	CEditPlayer*							m_pEditPlayer;			// �G�f�B�^�p�v���C���[.
	std::unique_ptr<CAuraMesh>				m_pAuraMesh;			// �I�[�����b�V��
	std::unique_ptr<CUndoRedo<SBoxRange>>	m_pUndoRedo;			// ���ɖ߂�����N���X.
	std::unique_ptr<CStageRender>			m_pStageRender;			// �X�e�[�W�̕`��.
	std::vector<SBoxRange>					m_BoxRangeList;			// �͈͂̃��X�g.
	SBoxRange								m_NowSelectActor;		// ���ݑI�����Ă���A�N�^�[.
	int										m_DeleteActorNo;		// �폜����A�N�^�[�̔ԍ�.
};

#endif	// #ifndef ENEMY_SPAEN_RANGE_EDITOR_H.