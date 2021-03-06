#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include<vector>
#include<assert.h>
using namespace std;
//利用仿函数的特性实现代码的复用性
template<class T>struct Small
{
	bool operator()(const T&l, const T&r)
	{
		return l < r;
	}
};
template<class T>struct Large
{
	bool operator()(const T&l, const T&r)
	{
		return l>r;
	}
};
template<class T, class Compare = Large<T>>class Heap
{
	//默认是建立最小堆
protected:
	vector<T>_a;
public:
	Heap()
	{}
	Heap(const T*a, int size)
	{
		assert(a);
		_a.reserve(size);
		for (int i = 0; i < size; i++)
		{
			_a.push_back(a[i]);
		}
		for (int j = (size - 2) / 2; j >= 0; j--)
		{
			_AdjustDown(j);
		}
	}
	void Push(const T&x)
	{
		_a.push_back(x);
		_AdjustUp(_a.size() - 1);
	}
	void Pop()
	{
		assert(_a.empty());
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		_AdjustDown(0);
	}
	size_t Size()
	{
		return _a.size();
	}
	bool Empty()
	{
		return _a.empty();
	}
	const T&Top()const
	{
		assert(!_a.empty());
		return _a[0];
	}
	void Display()
	{
		for (size_t i = 0; i < _a.size(); i++)
		{
			cout << _a[i] << " ";
		}
		cout << endl;
	}
protected:
	void _AdjustDown(int root)
	{
		int parent = root;
		size_t child = 2 * root + 1;
		while (child < _a.size())
		{
			Compare com;
			if ((child + 1 < _a.size()) && com( _a[child + 1], _a[child]))
			{
				child++;
			}
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				//初始的child默认指向左孩子
				child = 2*parent + 1;
			}
			else
				break;
		}
	}
	void _AdjustUp(int child)
	{
		while (child > 0)
		{
			int parent = (child - 1) / 2;
			Compare com;
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
			}
			else
				break;//插入的数据比父节点的数据域小
		}
	}
};