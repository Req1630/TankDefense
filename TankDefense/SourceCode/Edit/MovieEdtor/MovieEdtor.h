#ifndef MOVIE_EDITOR_H
#define MOVIE_EDITOR_H

#include "..\EditorBase.h"
#include "..\..\Object\CameraBase\MovieCamera\MovieCamera.h"

class CMovie;			// ムービー(演出)クラス.
class CCameraEditor;	// カメラエディタ.
class CStageRender;		// ステージ描画クラス.

/******************************************
*	演出エディタクラス.
**/
class CMovieEditor : public CEditorBase
{
public:
	CMovieEditor();
	virtual ~CMovieEditor();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update() override;
	// ImGui描画.
	virtual bool ImGuiRender() override;
	// モデル描画.
	virtual void ModelRender() override;
	// エフェクト描画.
	virtual void EffectRneder() override;

private:
	// 再生の表示.
	void PlayDraw();
	// パラメータの書き込み.
	virtual void ParameterWriting( const char* filePath ) override;
	// パラメータの読み込み.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::unique_ptr<CMovie>			m_pMovie;			// ムービー.
	std::unique_ptr<CCameraEditor>	m_pCameraEdit;		// カメラエディタ.
	std::unique_ptr<CStageRender>	m_pStageRender;		// ステージの描画.
	float							m_MovieEndSecTime;	// 動画終了時間.

	bool							m_IsMoviePlaying;	// 再生中かどうか.

};

#endif	// #ifndef MOVIE_EDITOR_H.