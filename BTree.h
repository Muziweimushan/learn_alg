#pragma once

#ifndef __BTREE_H__
#define __BTREE_H__



#include "Tree.h"
#include "BTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"
#include "DynamicArray.h"

#include <iostream>

using namespace std;

namespace MyLib
{

//定义二叉树典型遍历的遍历方式
enum BTTraversal
{
    BTTRAVERSAL_PREORDER,	//先序遍历
    BTTRAVERSAL_INORDER,	//中序遍历
    BTTRAVERSAL_POSTORDER,	//后序遍历
    BTTRAVERSAL_LEVELORDER	//层次遍历
};

template < typename T >
class BTree : public Tree<T>
{
protected:

    LinkQueue<BTreeNode<T> *> m_queue;	//用于二叉树的层次遍历时使用

    //功能函数:以node为根节点在二叉树中查找数据元素值为value的结点
    virtual BTreeNode<T> *find(BTreeNode<T> *node, const T &value) const
    {
        BTreeNode<T> *ret = NULL;

        if (NULL != node)
        {
            if (value == node->value)
            {
                ret = node;
            }
            else
            {
                //先在左子树中找一下有没有,递归调用find
                if (NULL == ret)
                {
                    ret = find(node->left, value);
                }
                //接着如果没找到的话就在右子树中找
                if (NULL == ret)
                {
                    ret = find(node->right, value);
                }
            }
        }

        return ret;
    }

    //功能函数:实现在node为根结点的二叉树中查找结点地址值为obj的结点
    virtual BTreeNode<T> *find(BTreeNode<T> *node, BTreeNode<T> *obj) const
    {
        BTreeNode<T> *ret = NULL;

        if (obj == node)
        {
            ret = node;
        }
        else
        {
            if (NULL != node)
            {
                //一样的先找左子树
                if (NULL == ret)
                {
                    ret = find(node->left, obj);
                }
                if (NULL == ret)
                {
                    ret = find(node->right, obj);
                }
            }
        }

        return ret;
    }

    //功能函数,将待插入结点n插入到指定的父结点np的pos位置上
    virtual bool insert(BTreeNode<T> *n, BTreeNode<T> *np, BTNodePos pos)
    {
        bool ret = true;

        //ANY == pos, 检查父结点的left和right有没有空闲位
        if (ANY == pos)
        {
            if (NULL == np->left)
            {
                np->left = n;
            }
            else if (NULL == np->right)
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }
        }
        else if (LEFT == pos)
        {
            if (NULL == np->left)
            {
                np->left = n;
            }
            else
            {
                ret = false;
            }
        }
        else if (RIGHT == pos)
        {
            if (NULL == np->right)
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            //指定位置上已经有子结点了无法插入
            ret = false;
        }

        return ret;
    }

    //功能函数:将结点node从树中删除,并将删除的子树作为一个新的子树存放在ret中返回
    virtual void remove(BTreeNode<T> *node, BTree<T> *&ret)
    {
        ret = new BTree<T>;

        if (NULL != ret)
        {
            if (root() == node)
            {
                //如果node是当前二叉树的根节点,那直接将根结点指针赋值为空即可
                this->m_root = NULL;
            }
            else
            {
                BTreeNode<T> *parent = dynamic_cast<BTreeNode<T> *>(node->parent);

                if (node == parent->left)
                {
                    parent->left = NULL;
                }
                else if (node == parent->right)
                {
                    parent->right = NULL;
                }

                node->parent = NULL;
            }

            ret->m_root = node;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new BTree...");
        }
    }

    //功能函数:清除以node为根结点的二叉树
    virtual void free(BTreeNode<T> *node)
    {
        if (NULL != node)
        {
            free(node->left);
            free(node->right);

            //释放此结点时需要判断一下是否是从堆空间分配内存的
            if (node->flag())
            {
                delete node;
            }
        }
        //node为空就是递归的出口
    }

    //功能函数:计算node为根结点的二叉树的结点个数
    virtual int count(BTreeNode<T> *node) const
    {
        return (NULL != node) ? (count(node->left) + count(node->right) + 1) : 0;
    }

    //功能函数:计算以node为结点的二叉树的高度
    virtual int height(BTreeNode<T> *node) const
    {
        int ret = 0;

        if (NULL != node)
        {
            int lh = height(node->left);
            int rh = height(node->right);

            ret = ((lh > rh) ? lh : rh) + 1;

        }

        return ret;
    }

    //功能函数:计算以node为根结点的二叉树的度
    virtual int degree(BTreeNode<T> *node) const
    {
        int ret = 0;

        if (NULL != node)
        {
            BTreeNode<T> *child[] = {node->left, node->right};

            ret = (!!node->left) + (!!node->right);

            for (int i = 0; (i < 2) && (ret < 2); i++)
            {
                int d = degree(child[i]);

                ret = (ret < d) ? d : ret;
            }
        }

        return ret;
    }

    //先序遍历的实现函数
    void preOrderTraversal(BTreeNode<T> *node, LinkQueue<BTreeNode<T> *> &queue)
    {
        if (NULL != node)
        {
            queue.add(node);
            preOrderTraversal(node->left, queue);
            preOrderTraversal(node->right, queue);
        }
    }

    //中序遍历的实现函数
    void inOrderTraversal(BTreeNode<T> *node, LinkQueue<BTreeNode<T> *> &queue)
    {
        if (NULL != node)
        {
            inOrderTraversal(node->left, queue);
            queue.add(node);
            inOrderTraversal(node->right, queue);
        }
    }

    //后序遍历的实现函数
    void postOrderTraversal(BTreeNode<T> *node, LinkQueue<BTreeNode<T> *> &queue)
    {
        if (NULL != node)
        {
            postOrderTraversal(node->left, queue);
            postOrderTraversal(node->right, queue);
            queue.add(node);
        }
    }

    //功能函数:层次遍历算法实现到一个函数中,即将move next end current合并为一个函数,去除代码冗余
    //被遍历操作函数调用
    //所谓层次遍历,就是从上到下,从左到右的访问树中结点
    void levelOrderTraversal(BTreeNode<T> *node, LinkQueue< BTreeNode<T> *> &queue)
    {
        LinkQueue< BTreeNode<T> *> tmp;

        if (NULL != node)
        {
            tmp.add(node);

            //当tmp队列为空,遍历也就结束了
            while (tmp.length() > 0)
            {
                BTreeNode<T> *n = tmp.front();

                //将队头元素的左右子结点依次压入tmp中
                if (NULL != n->left)
                {
                    tmp.add(n->left);
                }

                if (NULL != n->right)
                {
                    tmp.add(n->right);
                }
                //删除队头元素
                tmp.remove();
                //将队头元素压入输出队列
                queue.add(n);
            }
        }
    }

    //重构遍历操作的实现函数,实现层次/先序/中序/后序遍历功能,结点遍历顺序保存在queue中
    void traversal(BTTraversal order, LinkQueue< BTreeNode<T> *> &queue)
    {
        switch (order)
        {
            case BTTRAVERSAL_PREORDER:
                {
                    preOrderTraversal(root(), queue);
                    break;
                }
            case BTTRAVERSAL_INORDER:
                {
                    inOrderTraversal(root(), queue);
                    break;
                }
            case BTTRAVERSAL_POSTORDER:
                {
                    postOrderTraversal(root(), queue);
                    break;
                }
            case BTTRAVERSAL_LEVELORDER:
                levelOrderTraversal(root(), queue);
                break;
            default:
                {
                    THROW_EXCEPTION(InvalidParameterException, "Parameter order is invalid...");
                    break;
                }
        }
    }

    //功能函数:拷贝以node为根节点的二叉树
    BTreeNode<T> *clone(BTreeNode<T> *node) const
    {
        BTreeNode<T> *ret = NULL;

        if (NULL != node)
        {
            ret = BTreeNode<T>::NewNode();
            if (NULL != ret)
            {
                //结点数据元素值赋值
                ret->value = node->value;

                //递归调用clone函数,分别拷贝结点的左子树和右子树
                ret->left = clone(node->left);
                ret->right = clone(node->right);

                //还需要维护新的二叉树中各结点中指向父结点的指针
                //很明显ret就是左子树left和右子树right的父结点
                if (NULL != ret->left)
                {
                    ret->left->parent = ret;
                }

                if (NULL != ret->right)
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create Btree node...");
            }
        }

        return ret;
    }

    //功能函数,比较根节点分别为lh和rh的两棵二叉树是否相等
    bool equal(BTreeNode<T> *lh, BTreeNode<T> *rh) const
    {
        //如果两个根结点地址是一样的,那就说明两棵树其实是一样的
        if (lh == rh)
        {
            return true;
        }
        else if ((NULL != lh) && (NULL != rh))
        {
            //如果两个根结点都不为0
            //那么必须满足以下三个条件才能认为这两棵二叉树是相等的
            //1.根节点的数据元素相等
            //2.递归判断lh和rh的左子树是相等的
            //3.递归判断lh和rh的右子树是相等的
            return ((lh->value == rh->value) && (equal(lh->left, rh->left)) && equal(lh->right, rh->right));
        }
        else
        {
            //剩下的场景就是一棵二叉树的根结点为空而另外一棵树不是空树,很明显这样的两棵树不可能相等
            return false;
        }
    }

    //功能函数,将lh为根结点的二叉树与根结点为rh的二叉树相加
    BTreeNode<T> *add(BTreeNode<T> *lh, BTreeNode<T> *rh) const
    {
        BTreeNode<T> *ret = NULL;

        if ((NULL == lh) && (NULL != rh))
        {
            //lh为空树,rh不为空树时,按照约定,直接克隆不为空的那棵树即可
            ret = clone(rh);
        }
        else if ((NULL != lh) && (NULL == rh))
        {
            ret = clone(lh);
        }
        else if ((NULL != lh) && (NULL != rh))
        {
            //lh和rh都不是空树时,首先将lh和rh中数据元素进行相加
            //然后分别对lh和rh的左子树和右子树进行相加操作,也就是递归调用这个add函数即可
            ret = BTreeNode<T>::NewNode();

            if (NULL != ret)
            {
                ret->value = lh->value + rh->value;
                ret->left = add(lh->left, rh->left);
                ret->right = add(lh->right, rh->right);

                //同样的维护一下新树中每个结点的指向父结点的指针域
                if (NULL != ret->left)
                {
                    ret->left->parent = ret;
                }

                if (NULL != ret->right)
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create BTree node...");
            }
        }
        else
        {
            //else分支就是rh和lh都为空树,不需要干啥,直接返回空树即可
        }

        return ret;
    }

public:

	//二叉树插入节点重载版本,将一个树节点插入树中,位置由入参pos和node的parent指定
	virtual bool insert(BTreeNode<T> *node, BTNodePos pos)
	{
		bool ret = true;

		if (NULL != this->m_root)
		{
			if (NULL != node)
			{
				//找一下待插入结点的父结点位置
				BTreeNode<T> *np = find(node->parent);
				if (NULL != np)
				{
					ret = insert(node, np, pos);
				}
				else
				{
					THROW_EXCEPTION(InvalidParameterException, "Can not find parent node...");
				}
			}
			else
			{
				THROW_EXCEPTION(InvalidParameterException, "Parameter node can not be NULL...");
			}
		}
		else
		{
			//插入结点是根节点
			this->m_root = node;
			node->parent = NULL;
		}

		return ret;
	}

    /*二叉树插入节点重载版本,插入位置由node的parent指定*/
	bool insert(TreeNode<T> *node)
	{
		return insert(dynamic_cast<BTreeNode<T> *>(node), ANY);;
	}

	//新建树节点并插入二叉树,新插入节点父亲为入参parent,节点元素值value,插入位置pos
	virtual bool insert(const T &value, BTreeNode<T> *parent, BTNodePos pos)
	{
		bool ret = true;
		BTreeNode<T> *node = BTreeNode<T>::NewNode();

		if (NULL != node)
		{
			//填充新结点信息
			node->value = value;
			node->parent = parent;

			ret = insert(node, pos);

			//判断插入是否成功,因为二叉树插入操作不一定成功的,当父结点对应子结点对应位置已经有子结点了插入就失败了
			//插入不成功释放内存
			if (!ret)
			{
				delete node;
				node = NULL;
			}
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create BTreeNode object...");
		}

		return ret;
	}

    /*新建树节点并插入二叉树重载版本,元素值value,节点父亲parent,不指定插入位置(优先左子树)*/
	bool insert(const T &value, TreeNode<T> *parent)
	{
		return insert(value, dynamic_cast<BTreeNode<T> *>(parent), ANY);;
	}

	SharedPointer< Tree<T> > remove(const T &value)
	{
		BTreeNode<T> *node = find(value);
		BTree<T> *ret = NULL;

		if (NULL != node)
		{
			remove(node, ret);

			m_queue.clear();
		}
		else
		{
			THROW_EXCEPTION(InvalidParameterException, "Can not find parent node via value...");
		}

		return ret;
	}

    /*从二叉树中删除以node为根节点的子树,并将删除的子树返回*/
	SharedPointer< Tree<T> > remove(TreeNode<T> *node)
	{
		node = find(node);
		BTree<T> *ret = NULL;

		if (NULL != node)
		{
			remove(dynamic_cast<BTreeNode<T> *>(node), ret);

			m_queue.clear();
		}
		else
		{
			THROW_EXCEPTION(InvalidParameterException, "Parameter node is invalid...");
		}
		return ret;
	}

	BTreeNode<T> *find(const T &value) const
	{
		return find(root(), value);
	}

	BTreeNode<T> *find(TreeNode<T> *node) const
	{
		//感觉这里是不是要判断一下强制类型转换是否成功啊
		return find(root(), dynamic_cast<BTreeNode<T> *>(node));
	}

	//获取树的根结点,通过根结点可以访问树中所有的数据元素
	BTreeNode<T> *root() const
	{
		return dynamic_cast<BTreeNode<T> *>(this->m_root);
	}

	//获取树的度,所有结点的子树个数最大值
	int degree(void) const
	{
		return degree(root());
	}
	//获取树的结点个数
	int count(void) const
	{
		return count(root());
	}
	//获取树的高度(层级)
	int height(void) const
	{
		return height(root());
	}

	//清空操作
	void clear()
	{
		free(root());

        m_queue.clear();

        this->m_root = NULL;
    }


    //下面实现按层次遍历二叉树所有数据元素的操作,除了next函数有些不一样外其他与通用树的是一样的
    //要注意每次remove和clear函数都会清空队列,需要重新调用begin函数

    /*提供按层次遍历方式访问二叉树方法,调用begin接口开始遍历*/
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

    //提供层次遍历访问元素方式,end接口判断是否遍历完成.
    bool end()
    {
        /*当队列为空时,遍历结束*/
        return (0 == m_queue.length());
    }

    /*层次遍历移动到下一个元素*/
    bool next()
    {
        //next函数是遍历算法的核心,通过LinkQueue,将游标指向下一个数据元素时的实现方法为
        //弹出队头元素,将队头元素的孩子们(子树)压入队列中
        bool ret = (m_queue.length() > 0);

        if (ret)
        {
            BTreeNode<T> *node = m_queue.front();

            m_queue.remove();//出队列

            //压入队头的左右子树即可
            if (NULL != node->left)
            {
                m_queue.add(node->left);
            }

            if (NULL != node->right)
            {
                m_queue.add(node->right);
            }
        }

        return ret;
    }

    /*层次遍历访问当前元素*/
    T current()
    {
        //current直接返回队列的队头元素(类型为BTreeNode<T>)中的value值即可
        if (!end())
        {
            return m_queue.front()->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
        }
    }



    /*****************************************************************
     *
     *					以下是二叉树典型遍历的函数定义
     *					典型遍历包括: 先序遍历、中序遍历和后续遍历
     *
     ****************************************************************/
    SharedPointer< Array<T> > traversal(BTTraversal order)
    {
        DynamicArray<T> *ret = NULL;
        LinkQueue<BTreeNode<T> *> queue;//用于按照遍历类型将结点排序,再依次将各个结点的数据元素值存入返回数组中

        traversal(order, queue);

        ret = new DynamicArray<T> (queue.length());

        if (NULL != ret)
        {
            for (int i = 0; i < ret->length(); i++, queue.remove())
            {
                ret->set(i, queue.front()->value);
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create return array...");
        }

        return ret;
    }


    //克隆当前二叉树,通过返回值返回克隆出来的树
    //同时这个克隆的树通过智能指针返回
    SharedPointer< BTree<T> > clone() const
    {
        BTree<T> *ret = NULL;

        //从堆空间中创建一棵树
        ret = new BTree<T> ();

        if (NULL != ret)
        {
            ret->m_root = clone(root());
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No mem to create new tree...");
        }

        return ret;
    }

    //定义二叉树的比较操作
    bool operator == (const BTree<T> &btree) const
    {
        return equal(root(), btree.root());
    }

    bool operator != (const BTree<T> &btree) const
    {
        return !(*this == btree);
    }


    //定义二叉树的加操作
    SharedPointer< BTree<T> > add(const BTree<T> &btree) const
    {
        BTree<T> *ret = NULL;

        //从堆空间中新建一棵树
        ret = new BTree<T> ();

        if (NULL != ret)
        {
            ret->m_root = add(root(), btree.root());
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree...");
        }

        return ret;
    }

    //功能函数:实现线索化功能中的将遍历后的队列连接为双向链表的形式
    BTreeNode<T> *connect(LinkQueue< BTreeNode<T> * > &queue)
    {
        BTreeNode<T> *ret = NULL;
        BTreeNode<T> *slider = NULL;

        if (queue.length() > 0)
        {
            //链表首元素就是队列的第0个元素
            ret = queue.front();

            //slider先指向第0个元素
            slider = queue.front();

            //第0个元素可以从队列中弹出
            queue.remove();

            //首元素的前驱为空
            slider->left = NULL;

            //whilie循环依次连接各个结点就好了,left指针指向前驱,right指针指向后继
            while (queue.length() > 0)
            {
                slider->right = queue.front();//填充当前结点后继结点信息
                queue.front()->left = slider;//下一个结点的前驱就是slider
                //slider指向下一个,并将下一个元素从队列中删除
                slider = queue.front();
                queue.remove();
            }
        }

        return ret;
    }

    //定义二叉树的线索化功能:
    //线索化就是将二叉树中的各个结点进行重新组织,组织为双向链表存储形式
    //这个双向链表的首元素就是thread函数的返回值,通过结点的left和right指针分别访问结点的前驱和后继
    //当然首结点和最后一个结点的前驱和后继就分别都为空
    //另外要注意的就是,线索化会破坏掉树中结点之间的父子关系,因此进行线索化后就会清空这个树了
    BTreeNode<T> *thread(BTTraversal order)
    {
        LinkQueue<BTreeNode<T> *> queue;
        BTreeNode<T> *ret = NULL;

        traversal(order, queue);

        //连接队列中的各个结点使其组织为双向链表
        ret = connect(queue);


        //线索化后这棵树就莫得了,变成空树
        this->m_root = NULL;
        m_queue.clear();

        return ret;
    }

    ~BTree()
    {
        clear();
    }
};

}

#endif
