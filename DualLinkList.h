#ifndef __DUALLINKLIST_H__
#define __DUALLINKLIST_H__

#include "List.h"
#include "Exception.h"

namespace MyLib
{

template < typename T >
class DualLinkList : public List<T>
{
protected:
	struct Node : public Object
	{
		T value;
		Node *next;
		Node *pre;
	};

	mutable struct : public Object
	{
		char reserved[sizeof(T)];
		Node *next;
		Node *pre;
	}m_header;

	int m_length;
	Node *m_current;
	int m_step;

	//定位至目标位置i,返回对应的元素指针
	Node *position(int i) const				//O(n)
	{
		Node *ret = reinterpret_cast<Node *>(&m_header);

		for (int p = 0; p < i; p++)
		{
			ret = ret->next;
		}

		return ret;
	}

	virtual Node *create()
	{
		return new Node();
	}

	virtual void destroy(Node *pn)
	{
		delete pn;
	}

public:
	DualLinkList()
	{
		m_header.next = NULL;
		m_header.pre = NULL;
		m_length = 0;
		m_current = NULL;
		m_step = 1;
	}

	//插入到线性表的尾部
	bool insert(const T &e)
	{
		insert(m_length, e);
	}

	bool insert(int i, const T &e)		//O(n)
	{
		bool ret = ((0 <= i) && (i <= m_length));

		if (ret)
		{
			Node *node = create();//new Node()

			if (NULL != node)
			{
				Node *current = position(i);
				Node *next = current->next;//待插入节点的后继节点
				//对current->next指向的元素进行处理
				node->value = e;

				
				node->next = next;
				current->next = node;

				//根据约定,第0个元素的pre指针不指向m_header而是为空,这里判断一下插入节点的前驱是不是头结点
				if (current != reinterpret_cast<Node *>(&m_header))
				{
					node->pre = current;
				}
				else
				{
					node->pre = NULL;
				}

				//如果待插入节点的后继节点为空,即插入位置是链表的最末尾,则不能访问next指针
				if (NULL != next)
				{
					next->pre = node;
				}

				m_length++;
			}
			else
			{
				THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to insert new element...");
			}
		}

		return ret;
	}

	bool remove(int i)		//O(n)
	{
		bool ret = ((0 <= i) && (i < m_length));

		if (ret)
		{
			Node *current = position(i);
			Node *toDel = current->next;
			Node *next = toDel->next;
				
			//更新一下m_current指向的元素,否则再次使用current函数时会出现段错误
			if (toDel == m_current)
			{
				m_current = m_current->next;
			}
			
			//前驱的next指向后继
			current->next = next;

			//判断一下后继是否为空,即待删除元素是最后一个元素的情况,这时候next指针是不能访问的
			if (NULL != next)
			{
				next->pre = current;
			}
			m_length--;

			//异常安全,所有操作都完成后再销毁数据元素避免异常产生导致链表信息不对
			destroy(toDel);//delete toDel;
		}
	
		return ret;
	}

	virtual T get(int i) const		//O(n)
	{
		T ret;

		if (get(i, ret))
		{
			return ret;
		}
		else
		{
			THROW_EXCEPTION(IndexOutOfBoundsException, "Invalid parameter i to get element...");
		}
	}

	bool get(int i, T &e) const		//O(n)
	{
		bool ret = ((0 <= i) && (i < m_length));

		if (ret)
		{
			e = position(i)->next->value;
		}

		return ret;
	}

	bool set(int i, const T &e)		//O(n)
	{
		bool ret = ((0 <= i) && (i < m_length));

		if (ret)
		{
			position(i)->next->value = e;
		}

		return ret;
	}

	int find(const T &e) const		//O(n)
	{
		int ret = -1;
		int i = 0;

		Node *node = m_header.next;

		while (node->next)
		{
			if (node->value == e)
			{
				ret = i;
				break;
			}
			i++;
			node = node->next;
		}
		return ret;
	}


	int length() const		//O(1)
	{
		return m_length;
	}

	void clear()			//O(n)
	{
		while (m_length > 0)
		{
			remove(0);
		}
	}

	virtual bool move(int i, int step = 1)
	{
		bool ret = ((0 <= i) && (i < m_length)) && (step > 0);
		
		if (ret)
		{
			m_current = position(i)->next;
			m_step = step;
		}

		return ret;
	}

	virtual bool end(void)
	{
		return (NULL == m_current);
	}

	virtual T current(void)
	{
		return m_current->value;
	}

	virtual bool next(void)
	{
		int i = 0;

		while ((i < m_step) && (!end()))
		{
			m_current = m_current->next;
			i++;
		}

		return (m_step == i);
	}

	virtual bool pre(void)
	{
		int i = 0;

		while ((i < m_step) && (!end()))
		{
			m_current = m_current->pre;
			i++;
		}

		return (m_step == i);
	}

	~DualLinkList()				//O(n)
	{
		clear();
	}

};

}


#endif
