#ifndef STAGE_EDIT_PLAYER_H
#define STAGE_EDIT_PLAYER_H

#include "..\..\Object\Object.h"

class CRotLookAtCenter;

/*****************************************
* エディタ用のプレイヤー.
**/
class CEditPlayer : public CObject
{
public:
	CEditPlayer();
	virtual ~CEditPlayer();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update();
	// 描画関数.
	void Render();

	// 配置情報の取得.
	inline STranceform GetPutTranceform() { return m_PutTranceform; }

	// 置けるかどうか.
	inline bool IsPut(){ return m_IsPut; }

	// 回転操作を行うかどうか.
	inline void SetIsRotController( const bool& isRotController ){ m_IsRotController = isRotController; }

private:
	// カメラ操作.
	void CameraController();
	// 移動操作.
	void MoveController();
	// 回転操作.
	void RotationController();
	// 置く操作.
	void PutController();
	// 配置座標の更新.
	void PutPositionUpdate();

private:
	std::unique_ptr<CRotLookAtCenter>	m_pLookCamera;
	CDX9SkinMesh*						m_pSkinMesh;
	D3DXVECTOR3							m_MoveVector;
	STranceform							m_PutTranceform;
	float								m_MoveSpeed;
	float								m_CameraMoveSpeed;
	float								m_CameraLength;
	bool								m_IsPut;
	bool								m_IsRotController;
};

#endif	// #ifndef STAGE_EDIT_PLAYER_H.