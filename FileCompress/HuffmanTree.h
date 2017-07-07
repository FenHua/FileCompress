#include"Heap.h"
template<class T>struct HuffmanTreeNode
{
	T _weight;
	HuffmanTreeNode<T>*_left;
	HuffmanTreeNode<T>*_right;
	HuffmanTreeNode<T>*_parent;
	HuffmanTreeNode(const T&w = T()) :_weight(w), _left(NULL), _right(NULL), _parent(NULL)
	{}
};
//利用堆解决优先队列的问题
template<class T, class Compare = Large<T>>class PriorityQueue
{
protected:
	Heap<T, Compare>_pq;
public:
	PriorityQueue(int*a, int size) :_pd(a, size)
	{}
	void Push(const T&x)
	{
		_pq.Push(x);
	}
	void Pop()
	{
		_pq.Pop();
	}
	const T&Top()const
	{
		return _pq.Top();
	}
	void Display()
	{
		_pq.Display();
	}
};
template<class T>class HuffmanTree
{
	typedef HuffmanTreeNode<T>Node;
protected:
	Node*_root;
public:
	HuffmanTree() :_root(NULL)
	{};
	HuffmanTree(const T*a, size_t size)
	{
		_root = _CreateHuffmanTree(a, size, invalid);
	}
	//将未出现的字符过滤出来，不构造堆
	HuffmanTree(const T*a, size_t size, const T&invalid)
	{
		_root = _CreateHuffmanTree(a, size, invalid);
	}
	Node*GetRoot()
	{
		return _root;
	}
	~HuffmanTree()
	{
		_Destroy(_root);
	}
protected:
	Node*_CreateHuffmanTree(const T*a, size_t size)
	{
		struct NodeLess
		{
			bool operator()(Node*l, Node*r)const
			{
				return l->_weight < r->_weight;
			}
		};
		Heap<Node*, NodeLess>minHeap;
		//建立结点，放入堆中
		for (size_t i = 0; i < size; i++)
		{
			Node*tmp = new Node(a[i]);
			minHeap.Push(tmp);
		}
		//取出较小的两个结点作为左右孩子并且构建父节点
		while (minHeap.Size()>1)
		{
			Node*left = minHeap.Top();
			minHeap.Pop();
			Node*right = minHeap.Top();
			minHeap.Pop();
			Node*parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;
			minHeap.Push(parent);
		}
		return minHeap.Top();
	}
	//带过滤功能
	Node*_CreateHuffmanTree(const T*a, size_t size, const T&invalid)
	{
		struct NodeLess
		{
			bool operator()(Node*l, Node*r)const
			{
				return l->_weight < r->_weight;
			}
		};
		Heap<Node*, NodeLess>minHeap;
		//建立节点，并放入堆中
		for (size_t i = 0; i < size; i++)
		{
			if (a[i] != invalid)
			{
				Node*tmp = new Node(a[i]);
				minHeap.Push(tmp);
			}
		}
		//取出较小的两个节点作为左右孩子节点并建父节点
		while (minHeap.Size()>1)
		{
			Node*left = minHeap.Top();
			minHeap.Pop();
			Node*right = minHeap.Top();
			minHeap.Pop();
			Node*parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;
			minHeap.Push(parent);
		}
		return minHeap.Top();
	}
	void _Destroy(Node*&root)
	{
		if (root == NULL)
			return;
		Node*cur = root;
		if (cur)
		{
			_Destroy(cur->_left);
			_Destroy(cur->_right);
			delete cur;
			cur = NULL;
			return;
		}
	}
};