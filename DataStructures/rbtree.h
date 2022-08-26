#pragma once

#include "utility.h"

#pragma pack (push, 1)

enum class rbcolor
{
	RED,
	BLACK
};

template <class _Ty>
struct rbtree_iterator;

template <class _Ty>
struct rbnode;

template <class _Ty>
class rbtree
{
public:
	using node = rbnode<_Ty>;
	using iterator = rbtree_iterator<_Ty>;
	using const_iterator = rbtree_iterator<const _Ty>;
	using value_type = _Ty;

	constexpr rbtree();
	constexpr rbtree(const rbtree& _Other);
	constexpr rbtree(rbtree&& _Other);
	constexpr rbtree& operator=(const rbtree& _Other);
	constexpr rbtree& operator=(rbtree&& _Other);

	constexpr iterator begin();
	constexpr const_iterator begin() const;
	constexpr iterator end();
	constexpr const_iterator end() const;

	constexpr size_t size() const;

	constexpr void insert(const _Ty& _Val);
	constexpr void insert(_Ty&& _Val);
	template <class ..._Values>
	constexpr void emplace(_Values&&... _Vals);

	constexpr void remove(const _Ty& _Val);

	constexpr iterator find(const _Ty& _Val);
	constexpr const_iterator find(const _Ty& _Val) const;
	int depth() const;
	int _depth(node* next) const;
protected:
	template <class ..._Values>
	constexpr void _Emplace(_Values&&... _Vals);
	constexpr node*& _Get_root() const;
	constexpr void _Copy(const rbtree& _Other);
	constexpr void _Move(rbtree&& _Other);
	constexpr void _Free_nodes();

private:
	constexpr void _Insert(node* _Node);
	constexpr bool _Bst_insert(node* const& _New, node*& _Curr, node* const _Prev);
	constexpr void _Insert_fixup(node* _Node);
	constexpr void _Remove_fixup(node* _Node, node* _Prev);
	constexpr node* _Find(node* _Begin, const _Ty& _Val) const;
	constexpr void _Rotate_left(node* _Node);
	constexpr void _Rotate_right(node* _Node);
	constexpr rbcolor _Get_color(node* const& _Node) const;
	constexpr void _Set_color(node*& _Node, rbcolor _Color);
	constexpr void _Copy_helper(node* const& _Prev, node*& _Next, const node* const& _Other);
	constexpr void _Free_nodes_helper(node*& _Next);

	node* _Super_root;
	size_t _Size;
};

template<class _Ty>
inline constexpr rbtree<_Ty>::rbtree()
{
	_Super_root = new node();
	_Super_root->color = rbcolor::RED;
	_Size = 0;
}

template<class _Ty>
inline constexpr rbtree<_Ty>::rbtree(const rbtree& _Other)
{
	_Copy(_Other);
}

template<class _Ty>
inline constexpr rbtree<_Ty>::rbtree(rbtree&& _Other)
{
	_Move(move(_Other));
}

template<class _Ty>
inline constexpr rbtree<_Ty>& rbtree<_Ty>::operator=(const rbtree& _Other)
{
	if (this == &_Other) return *this;
	_Free_nodes();
	_Copy(_Other);
	return *this;
}

template<class _Ty>
inline constexpr rbtree<_Ty>& rbtree<_Ty>::operator=(rbtree&& _Other)
{
	if (this == &_Other) return *this;
	_Free_nodes();
	_Move(_Other);
	return *this;
}

template<class _Ty>
inline constexpr rbtree_iterator<_Ty> rbtree<_Ty>::begin()
{
	iterator it(_Super_root);
	it._Find_min();
	return it;
}


template<class _Ty>
inline constexpr rbtree_iterator<const _Ty> rbtree<_Ty>::begin() const
{
	const_iterator it(_Super_root);
	it._Find_min();
	return it;
}

template<class _Ty>
inline constexpr rbtree_iterator<_Ty> rbtree<_Ty>::end()
{
	return iterator(_Super_root);
}

template<class _Ty>
inline constexpr rbtree_iterator<const _Ty> rbtree<_Ty>::end() const
{
	return const_iterator(_Super_root);
}

template<class _Ty>
inline constexpr size_t rbtree<_Ty>::size() const
{
	return _Size;
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::insert(const _Ty& _Val)
{
	_Emplace(_Val);
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::insert(_Ty&& _Val)
{
	_Emplace(move(_Val));
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::remove(const _Ty& _Val)
{
	//red black tree remove _Val
	node* _Curr = _Get_root();
	node* _Prev = nullptr;
	while (_Curr != nullptr)
	{
		if (_Curr->value == _Val)
		{
			break;
		}
		_Prev = _Curr;
		_Curr = _Curr->value < _Val ? _Curr->right : _Curr->left;
	}
	if (_Curr == nullptr) return;
	if (_Curr->left == nullptr)
	{
		if (_Curr->right != nullptr)
		{
			_Curr->right->parent = _Prev;
		}
		if (_Prev == nullptr)
		{
			_Get_root() = _Curr->right;
		}
		else if (_Prev->left == _Curr)
		{
			_Prev->left = _Curr->right;
		}
		else
		{
			_Prev->right = _Curr->right;
		}
		delete _Curr;
	}
	else if (_Curr->right == nullptr)
	{
		if (_Curr->left != nullptr)
		{
			_Curr->left->parent = _Prev;
		}
		if (_Prev == nullptr)
		{
			_Get_root() = _Curr->left;
		}
		else if (_Prev->left == _Curr)
		{
			_Prev->left = _Curr->left;
		}
		else
		{
			_Prev->right = _Curr->left;
		}
		delete _Curr;
	}
	else
	{
		node* _Next = _Curr->right;
		while (_Next->left != nullptr)
		{
			_Next = _Next->left;
		}
		_Curr->value = _Next->value;
		_Remove_fixup(_Next, _Prev);
		delete _Next;
	}

}

template<class _Ty>
inline constexpr rbtree_iterator<_Ty> rbtree<_Ty>::find(const _Ty& _Val)
{
	node* _Result = _Find(_Get_root(), _Val);
	return iterator(_Result);
}

template<class _Ty>
inline constexpr rbtree_iterator<const _Ty> rbtree<_Ty>::find(const _Ty& _Val) const
{
	node* _Result = _Find(_Get_root(), _Val);
	return const_iterator(_Result);
}

template<class _Ty>
inline int rbtree<_Ty>::depth() const
{
	return _depth(_Get_root());
}

template<class _Ty>
inline int rbtree<_Ty>::_depth(node* next) const
{
	if (!next) return 1;
	return max(_depth(next->left), _depth(next->right)) + 1;
}

template<class _Ty>
template<class ..._Values>
inline constexpr void rbtree<_Ty>::emplace(_Values&&... _Vals)
{
	_Emplace(forward<_Values>(_Vals)...);
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Insert(node* _Node)
{
	node*& root = _Get_root();

	if (!_Bst_insert(_Node, root, _Super_root)) return;
	++_Size;

	if (_Node == root) {
		_Node->color = rbcolor::BLACK;
		return;
	}
	_Insert_fixup(_Node);
}

template<class _Ty>
inline constexpr rbnode<_Ty>*& rbtree<_Ty>::_Get_root() const
{
	return _Super_root->left;
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Copy(const rbtree& _Other)
{
	_Size = _Other._Size;
	_Super_root = new node();
	_Copy_helper(_Super_root, _Get_root(), _Other._Get_root());
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Move(rbtree&& _Other)
{
	_Super_root = _Other._Super_root;
	_Size = _Other._Size;
	_Other._Super_root = nullptr;
	_Other._Size = 0;
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Free_nodes()
{
	_Free_nodes_helper(_Super_root);
}

template<class _Ty>
inline constexpr bool rbtree<_Ty>::_Bst_insert(node* const& _New, node*& _Curr, node* const _Prev)
{
	if (_Curr == nullptr) {
		_Curr = _New;
		_Curr->parent = _Prev;
		return true;
	}

	if (_New->value > _Curr->value) return _Bst_insert(_New, _Curr->right, _Curr);
	if (_New->value == _Curr->value) return false;
	return _Bst_insert(_New, _Curr->left, _Curr);
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Insert_fixup(node* _Node)
{
	_Get_root()->parent = nullptr;

	while (_Node != _Get_root() && _Get_color(_Node) == rbcolor::RED && _Get_color(_Node->parent) == rbcolor::RED) {
		node* parent = _Node->parent;
		node* grandparent = _Node->parent->parent;
		if (parent == grandparent->left) {
			node* uncle = grandparent->right;
			if (_Get_color(uncle) == rbcolor::RED) {
				_Set_color(grandparent, rbcolor::RED);
				_Set_color(parent, rbcolor::BLACK);
				_Set_color(uncle, rbcolor::BLACK);
				_Node = grandparent;
			}
			else {
				if (_Node == parent->right) {
					_Rotate_left(parent);
					_Node = parent;
					parent = _Node->parent;
				}
				_Rotate_right(grandparent);
				swap(parent->color, grandparent->color);
				_Node = parent;
			}
		}
		else {
			node* uncle = grandparent->left;
			if (_Get_color(uncle) == rbcolor::RED) {
				_Set_color(grandparent, rbcolor::RED);
				_Set_color(parent, rbcolor::BLACK);
				_Set_color(uncle, rbcolor::BLACK);
				_Node = grandparent;
			}
			else {
				if (_Node == parent->left) {
					_Rotate_right(parent);
					_Node = parent;
					parent = _Node->parent;
				}
				_Rotate_left(grandparent);
				swap(parent->color, grandparent->color);
				_Node = parent;
			}
		}
	}
	_Get_root()->color = rbcolor::BLACK;
	_Get_root()->parent = _Super_root;
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Remove_fixup(node* _Next, node* _Prev)
{
	while (_Next != _Get_root() && _Get_color(_Next) == rbcolor::BLACK) {
		node* parent = _Next->parent;
		if (parent == _Prev) {
			_Next = _Next->right;
			_Prev = _Next->parent;
		} else {
			_Next = _Next->left;
			_Prev = _Next->parent;
		}
		node* grandparent = _Next->parent->parent;
		if (grandparent->left == parent) {
			node* uncle = grandparent->right;
			if (_Get_color(uncle) == rbcolor::RED) {
				_Set_color(uncle, rbcolor::BLACK);
				_Set_color(parent, rbcolor::RED);
				_Rotate_left(parent);
				uncle = grandparent->right;
			}
			if (_Get_color(uncle->left) == rbcolor::BLACK && _Get_color(uncle->right) == rbcolor::BLACK) {
				_Set_color(uncle, rbcolor::RED);
				_Next = grandparent;
				_Prev = grandparent->parent;
			}
			else {
				if (_Get_color(uncle->right) == rbcolor::BLACK) {
					_Set_color(uncle->left, rbcolor::BLACK);
					_Set_color(uncle, rbcolor::RED);
					_Rotate_right(uncle);
					uncle = grandparent->right;
				}
				_Set_color(uncle, grandparent->color);
				_Set_color(grandparent, rbcolor::BLACK);
				_Rotate_left(grandparent);
				_Next = _Get_root();
				_Prev = _Get_root();
			}
		}
		else {
			node* uncle = grandparent->left;
			if (_Get_color(uncle) == rbcolor::RED) {
				_Set_color(uncle, rbcolor::BLACK);
				_Set_color(parent, rbcolor::RED);
				_Rotate_right(parent);
				uncle = grandparent->left;
			}
			if (_Get_color(uncle->right) == rbcolor::BLACK && _Get_color(uncle->left) == rbcolor::BLACK) {
				_Set_color(uncle, rbcolor::RED);
				_Next = grandparent;
				_Prev = grandparent->parent;
			}
			else {
				if (_Get_color(uncle->left) == rbcolor::BLACK) {
					_Set_color(uncle->right, rbcolor::BLACK);
					_Set_color(uncle, rbcolor::RED);
					_Rotate_left(uncle);
					uncle = grandparent->left;
				}
				_Set_color(uncle, grandparent->color);
				_Set_color(grandparent, rbcolor::BLACK);
				_Rotate_right(grandparent);
				_Next = _Get_root();
				_Prev = _Get_root();
			}
		}
	}
	_Set_color(_Next, rbcolor::BLACK);
}

template<class _Ty>
inline constexpr rbnode<_Ty>* rbtree<_Ty>::_Find(node* _Begin, const _Ty& _Val) const
{
	if (!_Begin) return _Super_root;

	if (_Val > _Begin->value) return _Find(_Begin->right, _Val);
	if (_Val == _Begin->value) return _Begin;
	return _Find(_Begin->left, _Val);
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Rotate_left(node* _Node)
{
	node* tmp = _Node->right;
	node* parent = _Node->parent;

	if (!tmp) return;

	_Node->right = tmp->left;
	if (_Node->right) _Node->right->parent = _Node;

	tmp->left = _Node;
	tmp->parent = parent;
	_Node->parent = tmp;

	if (parent) {
		if (parent->left == _Node) parent->left = tmp;
		else parent->right = tmp;
	}
	else {
		_Get_root() = tmp;
	}
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Rotate_right(node* _Node)
{
	node* tmp = _Node->left;
	node* parent = _Node->parent;

	if (!tmp) return;
	_Node->left = tmp->right;
	if (_Node->left) _Node->left->parent = _Node;

	tmp->right = _Node;
	tmp->parent = parent;
	_Node->parent = tmp;

	if (parent) {
		if (parent->left == _Node) parent->left = tmp;
		else parent->right = tmp;
	}
	else {
		_Get_root() = tmp;
	}
}

template<class _Ty>
inline constexpr rbcolor rbtree<_Ty>::_Get_color(node* const& _Node) const
{
	return _Node ? _Node->color : rbcolor::BLACK;
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Set_color(node*& _Node, rbcolor _Color)
{
	if (!_Node) return;
	_Node->color = _Color;
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Copy_helper(node* const& _Prev, node*& _Next, const node* const& _Other)
{
	if (_Other == nullptr) return;

	_Next = new node(_Other->value, _Other->color);
	_Next->parent = _Prev;
	_Copy_helper(_Next, _Next->left, _Other->left);
	_Copy_helper(_Next, _Next->right, _Other->right);
}

template<class _Ty>
inline constexpr void rbtree<_Ty>::_Free_nodes_helper(node*& _Next)
{
	if (_Next == nullptr) return;

	_Free_nodes_helper(_Next->left);
	_Free_nodes_helper(_Next->right);
	delete _Next;
	_Next = nullptr;
}

template<class _Ty>
template<class ..._Values>
inline constexpr void rbtree<_Ty>::_Emplace(_Values&&... _Vals)
{
	node* _New = new node(forward<_Values>(_Vals)...);
	_Insert(_New);
}



/// 
/// NODE
/// 

template <class _Ty>
struct rbnode
{
public:
	friend class rbtree<_Ty>;
	friend class rbtest;
	friend struct rbtree_iterator<_Ty>;
	friend struct rbtree_iterator<const _Ty>;

	constexpr rbnode()
		: value()
	{ }
	constexpr rbnode(const _Ty& _Value)
		: color(rbcolor::RED), value(_Value)
	{ }
	constexpr rbnode(const _Ty& _Value, const rbcolor& _Color)
		: color(_Color), value(_Value)
	{ }
	constexpr rbnode(_Ty&& _Value)
		: color(rbcolor::RED), value(move(_Value))
	{ }
	template <class... _Values>
	constexpr rbnode(_Values&&... _Vals)
		: value(forward<_Values>(_Vals)...)
		, color(rbcolor::RED)
	{ }
	constexpr rbnode(const rbnode& _Other)
		: color(_Other.color), value(_Other.value)
		, left(_Other.left), right(_Other.right), parent(_Other.parent)
	{ }
	constexpr rbnode(rbnode&& _Other)
		: color(_Other.color), value(move(_Other.value))
		, left(_Other.left), right(_Other.right), parent(_Other.parent)
	{ }
	constexpr void swap_color() {
		color = !color;
	}
	constexpr bool is_black() {
		return color == rbcolor::BLACK;
	}
	constexpr bool is_red() {
		return color == rbcolor::RED;
	}
	constexpr rbnode* sibling() {
		return parent->left == this ? parent->right : parent->left;
	}
private:
	rbcolor color;
	rbnode* left = nullptr;
	rbnode* right = nullptr;
	rbnode* parent = nullptr;
	_Ty value;
};



///
/// red black tree iterator
/// 

template <class _Ty>
struct rbtree_iterator
{
public:
	using category = forward_iterator;

	using node = rbnode<remove_const_t<_Ty>>;
	using rbtree = rbtree<remove_const_t<_Ty>>;
	using iterator = rbtree_iterator<_Ty>;
	friend class rbtree;

	rbtree_iterator(node* const _Pointer) {
		_Current_node = _Pointer;
	}
	_Ty& operator*() {
		return _Current_node->value;
	}
	_Ty* operator->() {
		return &_Current_node->value;
	}
	iterator& operator++() {
		_Increment();
		return *this;
	}
	iterator operator++(int) {
		iterator tmp = *this;
		_Increment();
		return tmp;
	}
	bool operator==(const iterator& _Other) const {
		return _Current_node == _Other._Current_node;
	}
	bool operator!=(const iterator& _Other) const {
		return _Current_node != _Other._Current_node;
	}
private:
	void _Increment() {
		if (_Current_node->right) {
			_Current_node = _Current_node->right;
			while (_Current_node->left) {
				_Current_node = _Current_node->left;
			}
		} else {
			node* tmp = _Current_node->parent;
			while (_Current_node == tmp->right) {
				_Current_node = tmp;
				tmp = tmp->parent;
			}
			if (_Current_node->right != tmp) {
				_Current_node = tmp;
			}
		}
	}
	void _Find_min() {
		while (_Current_node->left) {
			_Current_node = _Current_node->left;
		}
	}

	node* _Current_node;
};

#pragma pack(pop)