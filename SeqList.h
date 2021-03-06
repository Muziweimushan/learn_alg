#ifndef __SEQLIST_H__
#define __SEQLIST_H__

#include "List.h"
#include "Exception.h"

namespace MyLib
{

template < typename T >
class SeqList : public List<T>
{
protected:
	T *m_array;//顺序存储空间的指针
	int m_length;//当前线性表的长度
public:
	bool insert(const T &e)
	{
		return insert(m_length, e);
	}


	bool insert(int i, const T &e)
	{
		bool ret = ((0 <= i) && (i <= m_length));
		
		ret = ret && (m_length < capacity());

		if (ret)
		{
			for (int p = m_length - 1; p >= i; p--)
			{
				m_array[p + 1] = m_array[p];
			}
		}
		m_array[i] = e;
		m_length++;
		return ret;
	}

	bool remove(int i)
	{
		bool ret = ((0 <= i) && (i < m_length));

		for (int p = i; p < m_length - 1; p++)
		{
			m_array[p] = m_array[p + 1];
		}
		m_length--;
		return ret;
	}
	
	bool set(int i, const T &e)
	{
		bool ret = ((0 <= i) && (i < m_length));

		if (ret)
		{
			m_array[i] = e;
		}

		return ret;
	}

	bool get(int i, T & e) const
	{
		bool ret = ((0 <= i) && (i < m_length));

		if (ret)
		{
			e = m_array[i];
		}
		return ret;
	}

	int find(const T &e) const		//O(n)
	{
		int ret = -1;

		for (int i = 0; i < m_length; i++)
		{
			if (m_array[i] == e)
			{
				ret = i;
				break;
			}
		}

		return ret;
	}

	int length() const
	{
		return m_length;
	}

	void clear()
	{
		m_length = 0;
	}

	//顺序存储线性表的数组访问方式----重载数组访问符
	T &operator[] (int i)
	{
		if ((0 <= i) && (i < m_length))
		{
			return m_array[i];
		}
		else
		{
			THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalied...");
		}
	}

	T operator[] (int i) const
	{
		return (const_cast<SeqList<T> &>(*this)).operator[](i);	
	}

	//顺序存储空间的最大容量
	virtual int capacity() const = 0;

};



}
















#endif
