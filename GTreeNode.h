#ifndef __GTREENODE_H__
#define __GTREENODE_H__

#include "TreeNode.h"
#include "LinkList.h"

namespace MyLib
{

template < typename T >
class GTreeNode : public TreeNode<T>
{
public:
	LinkList<GTreeNode<T> *> child;	//指向后继结点

	static GTreeNode<T> *NewNode(void)
	{
		GTreeNode<T> *ret = NULL;

		ret = new GTreeNode<T>();

		if (NULL != ret)
		{
			ret->m_flag = true;
		}

		return ret;
	}
};

}

#endif
