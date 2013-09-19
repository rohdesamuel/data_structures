/**
* Author: Sam Rohde
* Free to use and distribute only if you add me in the credits :3
* 
* The "typename _Ptr" is a comparator functor. By default this is a min_heap with the less-than functor
*/


#ifndef HEAP__H
#define HEAP__H

#include <vector>
#include <iostream>

template<typename _Kty, typename _Ty, typename _Ptr = std::less<_Kty> >
class Heap
{
private:
	typedef int Index;
	typedef std::pair<_Kty, _Ty> Data;
	typedef std::vector<Data> DataArray;

	Data _parentObj(Index index) const;
	Data _leftObj(Index index) const;
	Data _rightObj(Index index) const;

	Index _parent(Index index) const;
	Index _left(Index index) const;
	Index _right(Index index) const;

	void _heapifyDown(Index index);
	void _heapifyUp(Index index);
	void _heapify(void);
	void _swap(Index first, Index second);
	bool _comp(Index first, Index second);

	DataArray _dataArray;

public:
	Heap(void);
	Heap(DataArray dataArray);
	~Heap(void);

	void push(Data data);
	void push(_Kty priority, _Ty value);
	_Ty pop(void);

	int size(void);
};

#define HEAP_TEMPLATE typename _Kty, typename _Ty, typename _Ptr

template <HEAP_TEMPLATE>
Heap <HEAP_TEMPLATE>::
	Heap(void)
{
}

template <HEAP_TEMPLATE>
Heap <HEAP_TEMPLATE>::
	Heap(typename Heap<HEAP_TEMPLATE>::DataArray data):_dataArray(data)
{
	_heapify();
}

template <HEAP_TEMPLATE>
Heap <HEAP_TEMPLATE>::
	~Heap(void)
{
}

template <HEAP_TEMPLATE>
typename Heap<HEAP_TEMPLATE>::Index Heap<HEAP_TEMPLATE>::
	_parent(typename Heap<HEAP_TEMPLATE>::Index index) const
{
	return index / 2;
}

template <HEAP_TEMPLATE>
typename Heap<HEAP_TEMPLATE>::Index Heap<HEAP_TEMPLATE>::
	_left(typename Heap<HEAP_TEMPLATE>::Index index) const
{
	if (index * 2 + 1 < _dataArray.size())
		return index * 2 + 1;
	else
		return -1;
}

template <HEAP_TEMPLATE>
typename Heap<HEAP_TEMPLATE>::Index Heap<HEAP_TEMPLATE>::
	_right(typename Heap<HEAP_TEMPLATE>::Index index) const
{
	if (index * 2 + 2 < _dataArray.size())
		return index * 2 + 2;
	else
		return -1;
}

template <HEAP_TEMPLATE>
void Heap<HEAP_TEMPLATE>::
	_heapifyDown(typename Heap<HEAP_TEMPLATE>::Index index)
{
	Index left = _left(index);
	Index right = _right(index);
	Index comp = index;

	if (left >= 0 && !_comp(index,left))
		comp = left;

	if (right >= 0 && !_comp(comp,right))
		comp = right;

	if (comp != index)
	{
		_swap(index, comp);
		_heapifyDown(comp);
	}
}

template <HEAP_TEMPLATE>
void Heap<HEAP_TEMPLATE>::
	_heapifyUp(typename Heap<HEAP_TEMPLATE>::Index index)
{
	Index parent = _parent(index);
	if (!_comp(parent, index) && index != 0)
	{
		_swap(parent, index);
		_heapifyUp(parent);
	}
}

template <HEAP_TEMPLATE>
void Heap<HEAP_TEMPLATE>::
	_heapify(void)
{
	int index = _dataArray.size() / 2;
	while(index > 0)
	{
		_heapifyDown(index);
		--index;
	}
}

template <HEAP_TEMPLATE>
void Heap<HEAP_TEMPLATE>::
	_swap(typename Heap<HEAP_TEMPLATE>::Index first, typename Heap<HEAP_TEMPLATE>::Index second)
{
	Data tmp = _dataArray.at(first);
	_dataArray.at(first) = _dataArray.at(second);
	_dataArray.at(second) = tmp;
}

template <HEAP_TEMPLATE>
bool Heap<HEAP_TEMPLATE>::
	_comp(typename Heap<HEAP_TEMPLATE>::Index first, typename Heap<HEAP_TEMPLATE>::Index second)
{
	_Ptr comp;
	return comp(_dataArray.at(first).first, _dataArray.at(second).first);
}

template <HEAP_TEMPLATE>
void Heap<HEAP_TEMPLATE>::
	push(typename Heap<HEAP_TEMPLATE>::Data data)
{
	_dataArray.push_back(data);
	_heapifyUp(_dataArray.size() - 1);
}

template <HEAP_TEMPLATE>
void Heap<HEAP_TEMPLATE>::
	push(_Kty priority, _Ty value)
{
	_dataArray.push_back(std::pair<_Kty,_Ty>(priority, value));
	_heapifyUp(_dataArray.size() - 1);
}

template <HEAP_TEMPLATE>
_Ty Heap<HEAP_TEMPLATE>::
	pop(void)
{
	_swap(0, _dataArray.size() - 1);

	_Ty ret = _dataArray.back().second;
	_dataArray.pop_back();
	_heapifyDown(0);

	return ret;
}

template <HEAP_TEMPLATE>
int Heap<HEAP_TEMPLATE>::
	size(void)
{
	return _dataArray.size();
}

#endif
