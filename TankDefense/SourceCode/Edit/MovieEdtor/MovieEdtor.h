#ifndef MOVIE_EDITOR_H
#define MOVIE_EDITOR_H

#include "..\EditorBase.h"
#include "..\..\Object\CameraBase\MovieCamera\MovieCamera.h"
#include "..\..\Object\GameObject\Actor\Stage\StageStruct.h"

class CMovie;			// ムービー(演出)クラス.
class CCameraEditor;	// カメラエディタ.
class CMovieActorEditor;// アクターエディタ.
class CWidgetEditor;	// ウィジェットエディタ.
class CStageRender;		// ステージ描画クラス.
class CMovieDataLoader;	// ムービデータ読み込みクラス.
enum class enMovieList typedef EMovieNo;

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
	// ウィジェット描画.
	virtual void WidgetRender() override;

	// エディタ用プレイヤーの設定.
	virtual void SetEditPlayer( CEditPlayer* pPlayer ) override;

private:
	// 再生の表示.
	void PlayDraw();
	// ムービーの選択表示.
	void SelectMovieDraw();
	// 保存、読込ボタンの選択.
	void SaveLoadDraw();
	// パラメータの書き込み.
	virtual void ParameterWriting( const char* filePath ) override;
	// パラメータの読み込み.
	virtual void ParameterLoading( const char* filePath ) override;

private:
	std::unordered_map<EMovieNo, std::string>	m_MovieNameList;	// ムービー名リスト.
	std::unique_ptr<CMovieDataLoader>			m_pMovieDataLoader;	// ムービー読み込みクラス.
	std::unique_ptr<CMovie>						m_pMovie;			// ムービー.
	std::unique_ptr<CCameraEditor>				m_pCameraEdit;		// カメラエディタ.
	std::unique_ptr<CMovieActorEditor>			m_pActorEdit;		// アクターエディタ.
	std::unique_ptr<CWidgetEditor>				m_pWidgetEditor;	// ウィジェットエディタ.
	std::unique_ptr<CStageRender>				m_pStageRender;		// ステージの描画.
	EStageNo	m_NowStage;			// 現在のステージ.
	EMovieNo	m_NowSelectMovie;	// 現在選択しているムービー.
	float		m_MovieEndSecTime;	// 動画終了時間.
	bool		m_IsMoviePlaying;	// 再生中かどうか.

};

#endif	// #ifndef MOVIE_EDITOR_H.