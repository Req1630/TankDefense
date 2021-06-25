#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include <stack>
#include <functional>

/*************************************************
* アンドゥ/リドゥ(元に戻す/やり直し)操作クラス.
*	std::vectorでの操作のみ.
**/
template<typename T>
class CUndoRedo
{
public:
	struct stPushPram
	{
		int		VectorPos;	// 操作する配列の位置.
		bool	IsInsert;	// 追加するか "false" の場合、削除する.
		T		Parameter;	// vectorに追加する値.

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

	// アンドゥ(元に戻す).
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
	// リドゥ(やり直す).
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

	// アンドゥ操作の追加.
	void PushUndo( const int& vecPos, const bool& isInsert, const T& param, const bool& isClear = true )
	{
		m_UndoQueue.push( SPushPram(vecPos, isInsert, param) );
		if( isClear == true ) StackClear<SPushPram>(m_RedoQueue);
	}
	// リドゥ操作の追加.
	void PushRedo( const int& vecPos, const bool& isInsert, const T& param )
	{
		m_RedoQueue.push( SPushPram(vecPos, isInsert, param) );
	}

	// 追加操作.
	void Insert( const int& vecPos, const T& param )
	{
		m_pParamList->insert( m_pParamList->begin() + vecPos, param );
	}
	// 削除操作.
	void Erase( const int& vecPos )
	{
		m_pParamList->erase( m_pParamList->begin() + vecPos );
	}

	// アンドゥできるか.
	inline bool IsUndo() const { return !m_UndoQueue.empty(); }
	// リドゥできるか.
	inline bool IsRedo() const { return !m_RedoQueue.empty(); }

	// スタックのクリア.
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