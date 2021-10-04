#ifndef TRANCEFORM_H
#define TRANCEFORM_H

// �x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)

#include <D3DX11.h>
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

// ���C�u�����ǂݍ���.
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3dx10.lib" )	//�uD3DX�`�v�̒�`�g�p���ɕK�v.

struct stTranceform
{
	D3DXVECTOR3 Position;	// ���W.
	D3DXVECTOR3 Rotation;	// ��].
	D3DXVECTOR3 Scale;		// �g��k��.

	stTranceform()
		: Position	( 0.0f, 0.0f, 0.0f )
		, Rotation	( 0.0f, 0.0f, 0.0f )
		, Scale		( 1.0f, 1.0f, 1.0f )
	{}

	stTranceform( 
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale )
		: Position	( pos )
		, Rotation	( rot )
		, Scale		( scale )
	{}

	//�@���[���h�s��, �X�P�[���s��, ��]�s��, ���s�ړ��s��.
	D3DXMATRIX GetWorldMatrix( const D3DXMATRIX* pRot = nullptr ) const
	{
		D3DXMATRIX mScale, mRot, mTran;

		// �g��k���s��쐬.
		D3DXMatrixScaling( &mScale, Scale.x, Scale.y, Scale.z );
		// ��]�s����쐬.
		if( pRot == nullptr ){
			D3DXMatrixRotationYawPitchRoll( &mRot, Rotation.y, Rotation.x, Rotation.z );
		} else {
			mRot = *pRot;
		}
		// ���s�ړ��s��.
		D3DXMatrixTranslation( &mTran, Position.x, Position.y, Position.z );

		// ���[���h�s��쐬.
		return mScale * mRot * mTran;
	}

} typedef STranceform;

#endif	// #ifndef TRANCEFORM_H.