#ifndef __STATICQUEUE_H__
#define __STATICQUEUE_H__

#include "Queue.h"
#include "Exception.h"

namespace MyLib
{

template < typename T, int N >
class StaticQueue : public Queue<T>
{

protected:
	T m_space[N];		//队列存储空间,使用原生数组实现
	int m_front;		//队头标识
	int m_rear;			//队尾标识
	int m_length;		//队列的长度

public:

	//构造函数,初始化类中成员
	StaticQueue()
	{
		m_front = 0;
		m_rear = 0;
		m_length = 0;
	}

	int capacity() const
	{
		return N;
	}

	//入队操作,插入元素到队列的尾部
	void add(const T &e)
	{
		//先判断队列是否已满
		if (m_length < N)
		{
			m_space[m_rear] = e;
			m_rear = (m_rear + 1) % N;
			m_length++;
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No space in StaticQueue currently...");
		}
	}

	//出队操作,删除队列头部的元素
	void remove(void)
	{
		//判断队列是否为空
		if (m_length > 0)
		{
			m_front = (m_front + 1) % N;
			m_length--;
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in StaticQueue currently...");
		}
	}

	//获取队头元素
	T front(void) const
	{
		//判断队列是否为空
		if (m_length > 0)
		{
			return m_space[m_front];
		}
		else
		{
			THROW_EXCEPTION(InvalidOperationException, "No element in StaticQueue currently...");
		}
	}

	void clear()
	{
		m_front = 0;
		m_rear = 0;
		m_length = 0;
	}

	int length() const
	{
		return m_length;
	}


};


}


#endif
