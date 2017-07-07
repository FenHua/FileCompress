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
//���öѽ�����ȶ��е�����
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
	//��δ���ֵ��ַ����˳������������
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
		//������㣬�������
		for (size_t i = 0; i < size; i++)
		{
			Node*tmp = new Node(a[i]);
			minHeap.Push(tmp);
		}
		//ȡ����С�����������Ϊ���Һ��Ӳ��ҹ������ڵ�
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
	//�����˹���
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
		//�����ڵ㣬���������
		for (size_t i = 0; i < size; i++)
		{
			if (a[i] != invalid)
			{
				Node*tmp = new Node(a[i]);
				minHeap.Push(tmp);
			}
		}
		//ȡ����С�������ڵ���Ϊ���Һ��ӽڵ㲢�����ڵ�
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