#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include <memory>
#include <unordered_map>

class CMovie;			// ���[�r�[�N���X.
class CMovieDataLoader;	// ���[�r�[�f�[�^�ǂݍ��݃N���X.
enum class enMovieList typedef EMovieNo;


/********************************
*	���[�r�[(���o)�}�l�[�W���[.
**/
class CMovieManager
{
	using movie_map = std::unordered_map<EMovieNo, std::unique_ptr<CMovie>>;
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
	void ChangeMovie( const EMovieNo& key );

	// ���[�r�[(���o)�̍Đ�.
	void Play();

private:
	std::unique_ptr<CMovieDataLoader>	m_pMovieDataLoader;
	movie_map	m_pMovieList;
	CMovie*		m_pActiveMovie;
};

#endif	// #ifndef MOVIE_MANAGER_H.