#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include <memory>
#include <unordered_map>

class CMovie;			// ���[�r�[�N���X.
class CMovieDataLoader;	// ���[�r�[�f�[�^�ǂݍ��݃N���X.

// ���[�r�[���X�g.
enum class enMovieList
{
	None,

	EnemySpawn,	// �G�̏o��.
	BossSpawn,	// �{�X�̏o��.
	BossDefeat,	// �{�X�̌��j.

	Max,

} typedef EMovieList;

/********************************
*	���[�r�[(���o)�}�l�[�W���[.
**/
class CMovieManager
{
	using movie_map = std::unordered_map<EMovieList, std::unique_ptr<CMovie>>;
public:
	CMovieManager();
	~CMovieManager();

	// ������.
	bool Init();
	// �X�V.
	void Update();
	// ���f���̕`��.
	void ModelRender();
	// �摜�̕`��.
	void WidgetRender();

	// ���[�r�[(���o)�̐؂�ւ�.
	void ChangeMovie( const EMovieList& key );

	// ���[�r�[(���o)�̍Đ�.
	void Play();

private:
	std::unique_ptr<CMovieDataLoader>	m_pMovieDataLoader;
	movie_map	m_pMovieList;
	CMovie*		m_pActiveMovie;
};

#endif	// #ifndef MOVIE_MANAGER_H.