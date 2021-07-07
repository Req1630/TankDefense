/**
* @file SpriteResource.h.
* @brief �摜���\�[�X�N���X.
* @author ���c���.
*/
#ifndef SPRITE_RESOURCE_H
#define SPRITE_RESOURCE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>	// C++17 �K�{.

#include "..\..\Common\Sprite\Sprite.h"

namespace fs = std::filesystem;

class CSpriteResource
{
	const char* FILE_PATH = "Data\\Sprite";	// �X�v���C�g�t�H���_�p�X.
	typedef std::unordered_map<std::string, std::unique_ptr<CSprite>> sprite_map;
public:
	CSpriteResource();
	~CSpriteResource();

	// �C���X�^���X�̎擾.
	static CSpriteResource* GetInstance();

	// �X�v���C�g�̓ǂݍ���(���b�p�[).
	static HRESULT Load( ID3D11DeviceContext* pContext11 );

	// �X�v���C�g�擾�֐�.
	static CSprite* GetSprite( const std::string& spriteName );

	// �X�v���C�g�����X�g�擾�֐�.
	static std::vector<std::string> GetSpriteNameList();

private:
	// �X�v���C�g�ǂݍ���.
	HRESULT SpriteLoad( ID3D11DeviceContext* pContext11 );


private:
	sprite_map m_SpriteList;	// �X�v���C�g���X�g.
	bool m_HasFinishedLoading;	// �Ǎ����I�������.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSpriteResource( const CSpriteResource & )				= delete;
	CSpriteResource& operator = ( const CSpriteResource & )	= delete;
	CSpriteResource( CSpriteResource && )					= delete;
	CSpriteResource& operator = ( CSpriteResource && )		= delete;
};

#endif	// #ifndef SPRITE_RESOURCE_H.