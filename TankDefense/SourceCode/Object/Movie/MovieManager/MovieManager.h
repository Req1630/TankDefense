#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include <memory>
#include <unordered_map>

class CMovie;			// ムービークラス.
class CMovieDataLoader;	// ムービーデータ読み込みクラス.

// ムービーリスト.
enum class enMovieList
{
	None,

	EnemySpawn,	// 敵の出現.
	BossSpawn,	// ボスの出現.
	BossDefeat,	// ボスの撃破.

	Max,

} typedef EMovieList;

/********************************
*	ムービー(演出)マネージャー.
**/
class CMovieManager
{
	using movie_map = std::unordered_map<EMovieList, std::unique_ptr<CMovie>>;
public:
	CMovieManager();
	~CMovieManager();

	// 初期化.
	bool Init();
	// 更新.
	void Update();
	// モデルの描画.
	void ModelRender();
	// 画像の描画.
	void WidgetRender();

	// ムービー(演出)の切り替え.
	void ChangeMovie( const EMovieList& key );

	// ムービー(演出)の再生.
	void Play();

private:
	std::unique_ptr<CMovieDataLoader>	m_pMovieDataLoader;
	movie_map	m_pMovieList;
	CMovie*		m_pActiveMovie;
};

#endif	// #ifndef MOVIE_MANAGER_H.