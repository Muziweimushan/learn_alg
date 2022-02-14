#pragma once

#ifndef __TREE_H__
#define __TREE_H__

#include "TreeNode.h"
#include "SharedPointer.h"

namespace MyLib
{

//类tree是一个抽象类,直接继承自顶层父类Object
//同时内部使用了TreeNode类(组合关系)
template < typename T >
class Tree : public Object
{
protected:
	TreeNode<T> *m_root;	//指向树的根结点

	//禁用拷贝构造和赋值操作符重载
	Tree(const Tree<T>&);
	Tree<T> &operator = (const Tree<T>&);
public:
	Tree()
	{
		m_root = NULL;
	}

	//定义数据结构类型三板斧: 插入、删除、查找
	virtual bool insert(TreeNode<T> *node) = 0;
	virtual bool insert(const T &value, TreeNode<T> *parent) = 0;

	//删除操作返回对应删除的结点,因为此结点可能下面有很多结点,删除后
	//就从原来的树中彻底被移除了
	//这里返回对应删除的结点实际作为一个新的树(这个删除的结点就是树的根)
	//子树中的元素留给用户自己处理
	virtual SharedPointer< Tree<T> > remove(const T &value) = 0;
	virtual SharedPointer< Tree<T> > remove(TreeNode<T> *node) = 0;

	virtual TreeNode<T> *find(const T &value) const = 0;
	virtual TreeNode<T> *find(TreeNode<T> *node) const = 0;

	//获取树的根结点,通过根结点可以访问树中所有的数据元素
	virtual TreeNode<T> *root() const = 0;

	//获取树的度,所有结点的子树个数最大值
	virtual int degree(void) const = 0;
	//获取树的结点个数
	virtual int count(void) const = 0;
	//获取树的高度(层级)
	virtual int height(void) const = 0;
	
	//清空操作
	virtual void clear() = 0;

	//树的层次遍历操作函数
	virtual bool begin() = 0;
	virtual bool end() = 0;
	virtual bool next() = 0;
	virtual T current() = 0;

	//virtual ~Tree() = 0;

};

}

#endif
