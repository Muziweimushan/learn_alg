#ifndef __TREENODE_H__
#define __TREENODE_H__

#include "Object.h"

namespace MyLib
{

template < typename T >
class TreeNode : public Object
{

protected:
	bool m_flag;

	TreeNode(const TreeNode<T> &);
	TreeNode<T> &operator = (const TreeNode<T> &);

	void *operator new(unsigned long size) throw()
	{
		return Object::operator new(size);
	}
public:
	T value;//数据元素
	TreeNode<T> *parent;	//指向父结点

	TreeNode()
	{
		m_flag = false;
		parent = NULL;
	}

	bool flag()
	{
		return m_flag;
	}

	virtual ~TreeNode() = 0;

};

//C++规定析构函数为纯虚函数也一样要提供实现
template < typename T >
TreeNode<T>::~TreeNode()
{
}

}


#endif
