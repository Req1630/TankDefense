#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include <stack>
#include <functional>

/*************************************************
* �A���h�D/���h�D(���ɖ߂�/��蒼��)����N���X.
*	std::vector�ł̑���̂�.
**/
template<typename T>
class CUndoRedo
{
public:
	struct stPushPram
	{
		int		VectorPos;	// ���삷��z��̈ʒu.
		bool	IsInsert;	// �ǉ����邩 "false" �̏ꍇ�A�폜����.
		T		Parameter;	// vector�ɒǉ�����l.

		stPushPram()
			: VectorPos	()
			, IsInsert	()
			, Parameter	()
		{}
		stPushPram( const int& vecPos, const bool& isinsert, const T& param )
			: VectorPos	( vecPos )
			, IsInsert	( isinsert )
			, Parameter	( param )
		{}
	} typedef SPushPram;
public:
	CUndoRedo()
		: m_pParamList	( nullptr )
		, m_UndoQueue	()
		, m_RedoQueue	()
	{}
	CUndoRedo( std::vector<T>* pList )
		: m_pParamList	( pList )
		, m_UndoQueue	()
		, m_RedoQueue	()
	{}
	~CUndoRedo()
	{}

	// �A���h�D(���ɖ߂�).
	void Undo()
	{
		if( IsUndo() == false ) return;
		const SPushPram  pram = m_UndoQueue.top();
		if( pram.IsInsert == true ){
			Insert( pram.VectorPos, pram.Parameter );
		} else {
			Erase( pram.VectorPos );
		}
		m_UndoQueue.pop();
		PushRedo( pram.VectorPos, !pram.IsInsert, pram.Parameter );
	}
	// ���h�D(��蒼��).
	void Redo()
	{
		if( IsRedo() == false ) return;
		const SPushPram  pram = m_RedoQueue.top();
		if( pram.IsInsert == true ){
			Insert( pram.VectorPos, pram.Parameter );
		} else {
			Erase( pram.VectorPos );
		}
		m_RedoQueue.pop();
		PushUndo( pram.VectorPos, !pram.IsInsert, pram.Parameter, false );
	}

	// �A���h�D����̒ǉ�.
	void PushUndo( const int& vecPos, const bool& isInsert, const T& param, const bool& isClear = true )
	{
		m_UndoQueue.push( SPushPram(vecPos, isInsert, param) );
		if( isClear == true ) StackClear<SPushPram>(m_RedoQueue);
	}
	// ���h�D����̒ǉ�.
	void PushRedo( const int& vecPos, const bool& isInsert, const T& param )
	{
		m_RedoQueue.push( SPushPram(vecPos, isInsert, param) );
	}

	// �ǉ�����.
	void Insert( const int& vecPos, const T& param )
	{
		m_pParamList->insert( m_pParamList->begin() + vecPos, param );
	}
	// �폜����.
	void Erase( const int& vecPos )
	{
		m_pParamList->erase( m_pParamList->begin() + vecPos );
	}

	// �A���h�D�ł��邩.
	inline bool IsUndo() const { return !m_UndoQueue.empty(); }
	// ���h�D�ł��邩.
	inline bool IsRedo() const { return !m_RedoQueue.empty(); }

	// �X�^�b�N�̃N���A.
	template<typename S>
	void StackClear( std::stack<S>& stack )
	{
		const size_t stackSize = stack.size();
		for( size_t i = 0; i < stackSize; i++ ){
			stack.pop();
		}
	};
	void StackClear()
	{
		StackClear<SPushPram>(m_UndoQueue);
		StackClear<SPushPram>(m_RedoQueue);
	};

private:
	std::vector<T>*			m_pParamList;
	std::stack<SPushPram>	m_UndoQueue;
	std::stack<SPushPram>	m_RedoQueue;
};

#endif	// #ifndef UNDO_REDO_H.