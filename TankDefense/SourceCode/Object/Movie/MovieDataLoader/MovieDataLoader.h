#ifndef MOVIE_DATA_LOADER_H
#define MOVIE_DATA_LOADER_H

#include "..\LoadCameraData\LoadCameraData.h"
#include "..\LoadWidgetData\LoadWidgetData.h"

#include <unordered_map>

// ムービーリスト.
enum class enMovieList
{
	None,

	EnemySpawn = 0,	// 敵の出現.
	BossSpawn,		// ボスの出現.
	BossDefeat,		// ボスの撃破.

	Max,

} typedef EMovieNo;

// ムービーデータ.
struct stMovieData
{
	float						MovieTime;
	std::vector<SMovieCamera>	CameraList;
	std::vector<SMovieWidget>	WidgetList;
} typedef SMovieData;

/************************************
*	ムービーデータ読み込みクラス.
**/
class CMovieDataLoader
{
	// 各読み込み終了フラグ.
	enum enLoadedFlags : unsigned int
	{
		ELoadedFlag_None	= 0,

		ELoadedFlag_Camera	= 1 << 0,	// カメラ.
		ELoadedFlag_Widget	= 1 << 1,	// ウィジェット.

	} typedef ELoadedFlag;

	// データペア(ファイルパス、データ情報).
	using data_pair = std::pair<std::string, SMovieData>;
public:
	CMovieDataLoader();
	~CMovieDataLoader();

	// 初期化.
	bool Init( const bool& isAllData = true );

	// データの読み込み.
	bool DataLoading( const char* filePath, SMovieData* pMovieData );

	// データの書き込み.
	bool DataWriting( const EMovieNo& movieNo, const SMovieData& movieData );

	// ムービーデータの取得.
	SMovieData GetMovieData( const EMovieNo& movieNo );
	// ムービーデータの設定.
	void SetMovieData( const EMovieNo& movieNo, const SMovieData& movieData );

	// ムービー名リストの取得.
	inline std::unordered_map<EMovieNo, std::string> GetMovieNameList(){ return m_MovieNameList; }

private:
	// ムービーデータパスリストの作成.
	bool InitDataPathList();
	// データリストの作成.
	bool InitDataList();

	// 各データを読み込む.
	template<typename LoadCalas, typename State>
	void LoadingEachData(
		const std::string& s,
		std::vector<std::string>& dataList,
		State& stateList,
		const ELoadedFlag& flag,
		const char* startTag, 
		const char* endTag );

	// 各データを書き込む.
	void WritingEachData( 
		std::fstream& fs,
		const std::vector<std::string>& dataList,
		const char* startTag, 
		const char* endTag );

	// 時間を書き込む.
	void WritingTimeData( std::fstream& fs, const float& time );

private:
	std::unordered_map<EMovieNo, std::string>	m_MovieNameList;
	std::unordered_map<std::string, data_pair>	m_DataPathList;
	unsigned int								m_EachLoadEndFlag;
};

#endif	// #ifndef MOVIE_DATA_LOADER_H.
