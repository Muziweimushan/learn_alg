#ifndef __LINKSTACK_H__
#define __LINKSTACK_H__


#include "Stack.h"
#include "LinkList.h"
#include "Exception.h"

namespace MyLib
{

template < typename T >
class LinkStack : public Stack<T>
{
protected:
	LinkList<T> m_list;

public:

	//入栈直接插入到链表的头部即第0个元素处即可
	void push(const T &e)		//O(1)
	{
		m_list.insert(0, e);
	}

	//我们实现的栈这种数据结构出栈仅仅是将栈顶元素删除即可
	//而获取栈顶元素值则统一通过top函数来实现
	//因此这里直接删除掉(LinkList的remove函数)第0个元素就好
	void pop()					//O(1)
	{
		if (m_list.length() > 0)
		{
			m_list.remove(0);
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in LinkStack currently...");
		}
	}

	//获取栈顶元素
	//直接通过LinkList的get函数获取即可
	T top() const				//O(1)
	{
		if (m_list.length() > 0)
		{
			return m_list.get(0);
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in LinkStack currently...");
		}
	}

	//同样的调用LinkList的清空函数就可以
	void clear()				//O(n)
	{
		m_list.clear();
	}

	//获取栈中元素个数等价于获取链表中的元素
	int size() const			//O(1)
	{
		return m_list.length();
	}


};

}


#endif
