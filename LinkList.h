#ifndef __LINKLIST_H__
#define __LINKLIST_H__

#include "List.h"
#include "Exception.h"


namespace MyLib
{


template < typename T >
class LinkList : public List<T>
{
protected:
	struct Node : public Object
	{
		T value;
		Node *next;
	};

	//在get函数需要对m_header取地址,而get函数是const属性的,取地址意味着可能会改变对象的成员,编译器编译会报错
	//增加mutable解决
	//同时,在这里很巧妙的避开了直接定义一个泛指类型T的对象从而避免调用T类类型的构造函数从而导致问题
	//而且一定要注意的是必须这个结构体的内存布局要与Node完全一致!!!
	//所以这个结构体同样的需要继承自object才行
	//这个结构体没有名字,是一个匿名类型
	mutable struct : public Object
	{
		char reserved[sizeof(T)];
		Node *next;
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
	LinkList()
	{
		m_header.next = NULL;
		m_length = 0;
		m_current = NULL;
		m_step = 1;
	}

	//插入到线性表的尾部
	bool insert(const T &e)
	{
		return insert(m_length, e);
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

				//对current->next指向的元素进行处理
				node->value = e;
				node->next = current->next;
				current->next = node;

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
				
			//更新一下m_current指向的元素,否则再次使用current函数时会出现段错误
			if (toDel == m_current)
			{
				m_current = m_current->next;
			}

			current->next = toDel->next;
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

		while (node)
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
		while (m_header.next)
		{
			Node *toDel = m_header.next;

			m_header.next = toDel->next;
			m_length--;
			destroy(toDel);//delete toDel;
		}

		//m_length = 0;
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

	~LinkList()				//O(n)
	{
		clear();
	}

};

}

#endif
