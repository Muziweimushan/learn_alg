#ifndef __DUALCIRCLELIST_H__
#define __DUALCIRCLELIST_H__

#include "LinuxList.h"
#include "DualLinkList.h"

namespace MyLib
{

template < typename T >
class DualCircleList : public DualLinkList<T>
{

protected:
	struct Node : public Object
	{
		list_head head;
		T value;
	};
	list_head m_header;

	list_head *m_current;

	list_head *position(int i) const
	{
		list_head *ret = const_cast<list_head *>(&m_header);

		for (int p = 0; p < i; p++)
		{
			ret = ret->next;
		}

		return ret;
	}

	int mod(int i) const
	{
		return (0 == this->m_length) ? 0 : (i % this->m_length);
	}

public:
	DualCircleList()
	{
		this->m_step = 1;
		this->m_length = 0;

		INIT_LIST_HEAD(&m_header);
	}

	bool insert(const T &e)
	{
		return insert(this->m_length, e);
	}

	bool insert(int i, const T &e)
	{
		bool ret = true;

		Node *node = new Node;
		
		i = i % (this->m_length + 1);

		if (NULL != node)
		{
			node->value = e;
			
			//list_add_tail两个入参: 第一个是待插入节点,第二个是待插入节点的前一个节点,即目标i处的节点
			//照例的通过position的next来获取即可
			list_add_tail(&node->head, position(i)->next);
			this->m_length++;
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No enough mem to create new element for DualCircleList...");
		}
		

		return ret;
	}

	bool remove(int i)
	{
		bool ret = true;

		i = mod(i);

		ret = ((0 <= i) && (i < this->m_length));

		if (ret)
		{
			list_head *toDel = position(i)->next;

			//特殊处理一下游标
			if (toDel == m_current)
			{
				m_current = m_current->next;
			}
			
			//从链表中删去这个元素
			list_del(toDel);

			this->m_length--;

			delete list_entry(toDel, Node, head);
		}

		return ret;
	}


	T get(int i) const		//O(n)
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
		bool ret = true;

		i = mod(i);

		ret = ((0 <= i) && (i < this->m_length));

		if (ret)
		{
			e = list_entry(position(i)->next, Node, head)->value;
		}

		return ret;
	}

	bool set(int i, const T &e)		//O(n)
	{
		bool ret = true;

		i = mod(i);

		ret = ((0 <= i) && (i < this->m_length));

		if (ret)
		{
			list_entry(position(i)->next, Node, head)->value = e;
		}

		return ret;
	}

	int find (const T &e) const
	{
		int ret = -1;
	
		int i = 0;

		list_head *slider = NULL;

		list_for_each(slider, &m_header)
		{
			if (e == list_entry(slider, Node, head)->value)
			{
				ret = i;
				break;
			}
			else
			{
				i++;
			}
		}

		return ret;
	}


	int length() const		//O(1)
	{
		return this->m_length;
	}

	void clear()			//O(n)
	{
		while (this->m_length > 0)
		{
			remove(0);
		}
	}

	virtual bool move(int i, int step = 1)
	{
		bool ret = true;

		i = mod(i);

		ret = (0 <= i) && (i < this->m_length - 1) && (step > 0);

		if (ret)
		{
			m_current = position(i)->next;
			this->m_step = step;
		}

		return ret;
	}

	virtual bool end(void)
	{
		return (NULL == m_current) && (0 == this->m_length);
	}

	virtual T current(void)
	{
		return list_entry(m_current, Node, head)->value;
	}

	//遍历时需要跳过头结点
	bool next(void)
	{
		int i = 0;

		while ((i < this->m_step) && (!end()))
		{
			//判断一下游标当前是否指向头结点,如果是那就继续移动至下一个
			if (&m_header == m_current)
			{
				m_current = m_current->next;
			}
			else
			{

				m_current = m_current->next;
				i++;
			}
		}
		
		//移动完需要再一次判断,因为可能刚好落在头结点的位置上
		if (&m_header == m_current)
		{
			
			m_current = m_current->next;
		}
			
		return (this->m_step == i);
	}

	bool pre(void)
	{
		int i = 0;

		while ((i < this->m_step) && (!end()))
		{
			//判断一下游标当前是否指向头结点,如果是那就继续移动至下一个
			if (&m_header == m_current)
			{
				m_current = m_current->prev;
			}
			else
			{

				m_current = m_current->prev;
				i++;
			}
		}
		
		if (&m_header == m_current)
		{
			
			m_current = m_current->prev;
		}

		return (this->m_step == i);
	}

	~DualCircleList()
	{
		clear();
	}

};

}


#endif
