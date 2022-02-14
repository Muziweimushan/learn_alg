#pragma once

#ifndef __GTREE_H__
#define __GTREE_H__

#include "Tree.h"
#include "GTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"

namespace MyLib
{

template < typename T >
class GTree : public Tree<T>
{
protected:

	LinkQueue<GTreeNode<T> *>m_queue;

	//在node为结点的树中查找数据元素值等于value的结点
	GTreeNode<T> *find(GTreeNode<T> *node, const T &value) const
	{
		GTreeNode<T> *ret = NULL;
	
		if (node)
		{
			if (value == node->value)
			{
				ret = node;
			}
			else
			{
				//否则就遍历node的所有后继结点
				//实际是个递归过程,node下方的所有结点都会被处理
				for (node->child.move(0); ((!node->child.end()) && (NULL == ret)); node->child.next())
				{
					ret = find(node->child.current(), value);
				}
			}
		}

		return ret;
	}
	
	//在node为根结点的树中查找结点地址为obj的结点,有的话返回该结点地址
	GTreeNode<T> *find(GTreeNode<T> *node, GTreeNode<T> *obj) const
	{
		GTreeNode<T> *ret = NULL;

		if (obj == node)
		{
			ret = node;
		}
		else
		{
			if (node)
			{
				for (node->child.move(0); ((!node->child.end()) && (ret == NULL)); node->child.next())
				{
					ret = find(node->child.current(), obj);
				}
			}
		}

		return ret;
	}
	
	//功能函数:清除以node为根结点的树
	void free(GTreeNode<T> *node)
	{
		if (NULL == node)
		{
			//递归的出口,当这个结点是空结点
		}
		else
		{
			//依次清空这个结点下的子树
			for (node->child.move(0); !node->child.end(); node->child.next())
			{
				free(node->child.current());
			}

			//最后将这个根结点删除掉
			if (node->flag())
			{
				delete node;
			}
		}
	}

	//功能函数,用于将node作为根节点的子树从原来的树中删除
	//ret是返回值,返回删除掉的子树,它是一个引用,引用类型为GTree<T> *,也就是一个指针的引用(或者叫指针的别名),通过它就可以访问删除掉的子树了
	void remove(GTreeNode<T> *node, GTree<T> *&ret)
	{
		ret = new GTree<T> ();

		if (NULL != ret)
		{
			//当前要删除的结点不是根节点
			//那就将这个结点从这个结点的父结点中删除掉即可
			if (root() != node)
			{
				//结点的子树由链表来管理,删除这个结点下的某个子树就是将这个子树从链表中删除即可
				//这里必须将parent进行类型转换为子类GTreeNode,否则会报TreeNode中没有child成员
				LinkList<GTreeNode<T> *> &child = dynamic_cast<GTreeNode<T> *>(node->parent)->child;
				//LinkList<GTreeNode<T> *> &child = (node->parent)->child;
				child.remove(child.find(node));

				node->parent = NULL;
			}
			else
			{
				this->m_root = NULL;
			}

			ret->m_root = node;
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create GTree Object...");
		}
	}
	
	//功能函数:在node为根结点的树中统计结点数目
	int count(GTreeNode<T> *node) const
	{
		int ret = 0;

		//如果node为空,也就是个空树,结点个数就为0
		if (NULL != node)
		{
			//最起码有node这个结点,所以先将ret赋值为1
			ret = 1;

			LinkList<GTreeNode<T> *> &child = node->child;

			//遍历node下的所有子树,做一个求和操作
			for (child.move(0); !child.end(); child.next())
			{
				//子树中的结点数通过递归调用count函数来获得
				ret += count(child.current());
			}
		}

		return ret;
	}

	//功能函数:获取node为根结点的树的高度
	int height(GTreeNode<T> *node) const
	{
		int ret = 0;

		if (NULL != node)
		{
			LinkList<GTreeNode<T> *> &child = node->child;

			for (child.move(0); !child.end(); child.next())
			{
				int h = height(child.current());

				if (ret < h)
				{
					ret = h;
				}
			}

			ret = ret + 1;

		}

		return ret;
	}

	//功能函数:获取node为根结点的树的度
	int degree(GTreeNode<T> *node) const
	{
		int ret = 0;

		if (NULL != node)
		{
			LinkList<GTreeNode<T> *> &child = node->child;
			ret = child.length();//node结点子树的个数

			//遍历node结点下的所有子结点,求它们各自的子树个数最大值,并与node结点本身子树的个数一起求最大值
			//结果就为node结点的度了
			for (child.move(0); !child.end(); child.next())
			{
				int d = degree(child.current());

				if (ret < d)
				{
					ret = d;
				}
			}
		}

		return ret;
	}

public:
	GTree() {}

	bool insert(TreeNode<T> *node)
	{
		bool ret = true;

		if (NULL != node)
		{
			//是个空树,直接将这个结点作为根结点完事,注意根结点的parent为空
			if (NULL == this->m_root)
			{
				node->parent = NULL;
				this->m_root = node;
			}
			else
			{
				GTreeNode<T> *np = find(node->parent);

				if (NULL != np)
				{
					GTreeNode<T> *n = dynamic_cast<GTreeNode<T> *>(node);

					//在父结点的子结点中有没有node这个结点,避免重复插入...
					if (np->child.find(n) < 0)
					{
						np->child.insert(n);
					}
				}
				else
				{
					THROW_EXCEPTION(InvalidParameterException, "Invalid parent tree node...");
				}
			}
		}
		else
		{
			//插入结点肯定不能为空...
			THROW_EXCEPTION(InvalidParameterException, "Parameter node cannot be NULL...");
		}

		return ret;
	}

	bool insert(const T &value, TreeNode<T> *parent)
	{
		bool ret = true;
		
		//GTreeNode<T> *node = new GTreeNode<T>;
		GTreeNode<T> *node = GTreeNode<T>::NewNode();

		if (NULL != node)
		{
			node->value = value;
			node->parent = parent;

			insert(node);
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree node...");
		}

		return ret;
	}

	SharedPointer< Tree<T> > remove(const T &value)
	{
		GTree<T> * ret = NULL;

		GTreeNode<T> *node = find(value);

		if (NULL != node)
		{
			remove(node, ret);

			m_queue.clear();
		}
		else
		{
			THROW_EXCEPTION(InvalidParameterException, "Can not find the node via parameter value...");
		}
		
		return ret;
	}

	SharedPointer< Tree<T> > remove(TreeNode<T> *node)
	{
		GTree<T> *ret = NULL;
		node = find(node);

		if (NULL != node)
		{
			remove(dynamic_cast<GTreeNode<T> *>(node), ret);

			m_queue.clear();
		}
		else
		{
			THROW_EXCEPTION(InvalidParameterException, "Parameter node is invalid...");
		}

		return ret;
	}

	//查找树中是否有数据元素值等于value的结点,有的话返回这个结点的地址
	GTreeNode<T> *find(const T &value) const
	{
		return find(root(), value);
	}

	//查找树中是否有node这个结点
	GTreeNode<T> *find(TreeNode<T> *node) const
	{
		return find(root(), dynamic_cast<GTreeNode<T> *>(node));
	}

	GTreeNode<T> *root() const
	{
		return dynamic_cast<GTreeNode<T> *>(this->m_root);
	}

	int degree(void) const
	{
		return degree(root());
	}
	int count(void) const
	{
		return count(root());
	}
	int height(void) const
	{
		return height(root());
	}

	void clear()
	{
		free(root());
		this->m_root = NULL;

		m_queue.clear();
	}

	//下面实现按层次遍历树所有数据元素的操作
	//要注意每次remove和clear函数都会清空队列,需要重新调用begin函数
	
	bool begin()
	{
		bool ret = (NULL != root());

		if (ret)
		{
			//清空上一次遍历的状态
			m_queue.clear();

			m_queue.add(root());
		}

		return ret;
	}

	//当队列为空时,遍历结束
	bool end()
	{
		return (0 == m_queue.length());
	}

	//next函数是遍历算法的核心,通过LinkQueue,将游标指向下一个数据元素时的实现方法为
	//弹出队头元素,将队头元素的孩子们(子树)压入队列中
	bool next()
	{
		bool ret = (m_queue.length() > 0);

		if (ret)
		{
			GTreeNode<T> *node = m_queue.front();

			m_queue.remove();//出队列

			LinkList<GTreeNode<T> *> &child = node->child;

			for (child.move(0); !child.end(); child.next())
			{
				m_queue.add(child.current());
			}
		}

		return ret;
	}

	//current直接返回队列的队头元素(类型为GTreeNode<T>)中的value值即可
	T current()
	{
		if (!end())
		{
			return m_queue.front()->value;
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
		}
	}


	~GTree()
	{
		clear();
	}

};


}



#endif
