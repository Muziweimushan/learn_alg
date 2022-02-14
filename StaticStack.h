#ifndef __STATICSTACK_H__
#define __STATICSTACK_H__


#include "Stack.h"
#include "Exception.h"

namespace MyLib
{
template < typename T, int N >
class StaticStack : public Stack<T>
{
protected:
	T m_space[N];	//栈的存储空间,使用C++原生数组实现
	int m_top;		//栈顶标识,初始值为-1,表示没有元素在栈中,入栈时需要先移动栈顶标识再存入元素
	int m_size;		//当前栈中元素的个数

public:

	StaticStack()		//O(1)
	{
		m_top = -1;
		m_size = 0;
	}

	void push(const T &e)	//O(1)
	{
		if (m_size < N)
		{
			int pos = m_top + 1;//先移动栈顶指针
			m_space[pos] = e;

			m_top++;
			m_size++;
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No more space in stack...");
		}
	}

	void pop()				//O(1)
	{
		if (m_size > 0)
		{
			m_top--;
			m_size--;
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in statk currently...");
		}
	}

	T top() const				//O(1)
	{

		if (m_size > 0)
		{
			return m_space[m_top];
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in statk currently...");
		}
	}

	void clear()				//O(1)
	{
		m_size = 0;
		m_top = -1;
	}

	//获取栈中数据元素个数
	int size() const			//O(1)
	{
		return m_size;
	}

	//获取栈的最大容量
	int capacity() const		//O(1)
	{
		return N;
	}


	~StaticStack()
	{
	}

};


}


#endif
