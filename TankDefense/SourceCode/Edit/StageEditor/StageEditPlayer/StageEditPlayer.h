#ifndef STAGE_EDIT_PLAYER_H
#define STAGE_EDIT_PLAYER_H

#include "..\..\..\Object\Object.h"

class CRotLookAtCenter;

/*****************************************
*	ステージエディタ用のプレイヤー.
**/
class CStageEditPlayer : public CObject
{
public:
	CStageEditPlayer();
	virtual ~CStageEditPlayer();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update();
	// 描画関数.
	void Render();

	// 配置情報の取得.
	STranceform GetPutTranceform() { return m_PutTranceform; }

	// 置けるかどうか.
	bool IsPut(){ return m_IsPut; }

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
};

#endif	// #ifndef STAGE_EDIT_PLAYER_H.