#pragma once

#ifndef __BTREENODE_H__
#define __BTREENODE_H__

#include "TreeNode.h"

namespace MyLib
{

enum BTNodePos
{
	ANY,
	LEFT,
	RIGHT
};

template < typename T >
class BTreeNode : public TreeNode<T>
{
public:
	BTreeNode<T> *left;//左子树
	BTreeNode<T> *right;//右子树


	BTreeNode()
	{
		left = NULL;
		right = NULL;
	}

	static BTreeNode<T> *NewNode(void)
	{
		BTreeNode<T> *ret = NULL;

		ret = new BTreeNode<T>();

		if (NULL != ret)
		{
			ret->m_flag = true;
		}

		return ret;
	}

};


}


#endif
