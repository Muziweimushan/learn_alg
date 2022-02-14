#ifndef __LINKQUEUE_H__
#define __LINKQUEUE_H__



//使用Linux内核链表实现的链式队列
#if 1

#include "Queue.h"
#include "LinuxList.h"
#include "Exception.h"

namespace MyLib
{

template < typename T >
class LinkQueue : public Queue<T>
{

protected:
	struct Node : public Object
	{
		list_head head;
		T value;
	};

	list_head m_header;//Linux内核链表的头部
	int m_length;//存放当前队列的长度

public:

	//构造函数,初始化类中成员
	LinkQueue()
	{
		m_length = 0;
		INIT_LIST_HEAD(&m_header);
	}


	//入队操作,插入元素到队列的尾部
	void add(const T &e)
	{
		Node *node = new Node;

		if (NULL != node)
		{
			node->value = e;
			//list_add_tail 的第二个入参指的是待插入节点的后一个元素,在这里就是m_header了
			list_add_tail(&node->head, &m_header);
			m_length++;
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No mem to insert new LinkQueue element...");
		}
	}

	//出队操作,删除队列头部的元素
	void remove(void)
	{
		if (m_length > 0)
		{
			list_head *toDel = m_header.next;

			list_del(toDel);

			m_length--;

			delete list_entry(toDel, Node, head);
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in LinkQueue currently...");
		}
	}

	//获取队头元素
	T front(void) const
	{
		//判断队列是否为空
		if (m_length > 0)
		{
			return list_entry(m_header.next, Node, head)->value;
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in StaticQueue currently...");
		}
	}

	void clear()
	{
		while (m_length > 0)
		{
			remove();
		}
	}

	int length() const
	{
		return m_length;
	}

	~LinkQueue()
	{
		clear();
	}


};


}



#else

//使用LinkList实现的链式队列
#include "Queue.h"
#include "LinkList.h"
#include "Exception.h"


namespace MyLib
{

template < typename T >
class LinkQueue : public Queue<T>
{
protected:
	LinkList<T> m_list;

public:

	//入队操作即插入到队列的尾部,直接通过LinkList的insert函数即可完成
	void add(const T &e)
	{
		m_list.insert(e);
	}

	void remove(void)
	{
		if (m_list.length() > 0)
		{
			m_list.remove(0);
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in LinkQueue currently...");
		}
	}

	T front(void) const
	{
		
		if (m_list.length() > 0)
		{
			return m_list.get(0);
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in LinkQueue currently...");
		}
	}

	void clear(void)
	{
		m_list.clear();
	}

	int length() const
	{
		return m_list.length();
	}

	~LinkQueue()
	{
		clear();
	}


};

}
#endif


#endif
