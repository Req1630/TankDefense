#ifndef MOVIE_DATA_LOADER_H
#define MOVIE_DATA_LOADER_H

#include "..\LoadCameraData\LoadCameraData.h"
#include "..\LoadWidgetData\LoadWidgetData.h"

#include <unordered_map>

// ���[�r�[���X�g.
enum class enMovieList
{
	None,

	EnemySpawn = 0,	// �G�̏o��.
	BossSpawn,		// �{�X�̏o��.
	BossDefeat,		// �{�X�̌��j.

	Max,

} typedef EMovieNo;

// ���[�r�[�f�[�^.
struct stMovieData
{
	float						MovieTime;
	std::vector<SMovieCamera>	CameraList;
	std::vector<SMovieWidget>	WidgetList;
} typedef SMovieData;

/************************************
*	���[�r�[�f�[�^�ǂݍ��݃N���X.
**/
class CMovieDataLoader
{
	// �e�ǂݍ��ݏI���t���O.
	enum enLoadedFlags : unsigned int
	{
		ELoadedFlag_None	= 0,

		ELoadedFlag_Camera	= 1 << 0,	// �J����.
		ELoadedFlag_Widget	= 1 << 1,	// �E�B�W�F�b�g.

	} typedef ELoadedFlag;

	// �f�[�^�y�A(�t�@�C���p�X�A�f�[�^���).
	using data_pair = std::pair<std::string, SMovieData>;
public:
	CMovieDataLoader();
	~CMovieDataLoader();

	// ������.
	bool Init( const bool& isAllData = true );

	// �f�[�^�̓ǂݍ���.
	bool DataLoading( const char* filePath, SMovieData* pMovieData );

	// �f�[�^�̏�������.
	bool DataWriting( const EMovieNo& movieNo, const SMovieData& movieData );

	// ���[�r�[�f�[�^�̎擾.
	SMovieData GetMovieData( const EMovieNo& movieNo );
	// ���[�r�[�f�[�^�̐ݒ�.
	void SetMovieData( const EMovieNo& movieNo, const SMovieData& movieData );

	// ���[�r�[�����X�g�̎擾.
	inline std::unordered_map<EMovieNo, std::string> GetMovieNameList(){ return m_MovieNameList; }

private:
	// ���[�r�[�f�[�^�p�X���X�g�̍쐬.
	bool InitDataPathList();
	// �f�[�^���X�g�̍쐬.
	bool InitDataList();

	// �e�f�[�^��ǂݍ���.
	template<typename LoadCalas, typename State>
	void LoadingEachData(
		const std::string& s,
		std::vector<std::string>& dataList,
		State& stateList,
		const ELoadedFlag& flag,
		const char* startTag, 
		const char* endTag );

	// �e�f�[�^����������.
	void WritingEachData( 
		std::fstream& fs,
		const std::vector<std::string>& dataList,
		const char* startTag, 
		const char* endTag );

	// ���Ԃ���������.
	void WritingTimeData( std::fstream& fs, const float& time );

private:
	std::unordered_map<EMovieNo, std::string>	m_MovieNameList;
	std::unordered_map<std::string, data_pair>	m_DataPathList;
	unsigned int								m_EachLoadEndFlag;
};

#endif	// #ifndef MOVIE_DATA_LOADER_H.
