#ifndef __SHAREDPOINTER_H__
#define __SHAREDPOINTER_H__

#include <cstdlib>
#include "Pointer.h"
#include "Exception.h"

namespace MyLib
{


template < typename T >
class SharedPointer : public Pointer<T>
{
protected:
	int *m_ref;	//计数机制成员指针

	//封装一个成员赋值函数
	void assign(const SharedPointer<T> &obj)
	{

		this->m_pointer = obj.m_pointer;
		this->m_ref = obj.m_ref;

		if (NULL != this->m_ref)
		{
			(*this->m_ref)++;
		}
	}

public:
	SharedPointer(T *p = NULL) : m_ref(NULL)
	{
		if (NULL != p)
		{
			m_ref = static_cast<int *>(malloc(sizeof(int)));

			if (NULL != m_ref)
			{
				this->m_pointer = p;
				*m_ref = 1;
			}
			else
			{
				THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create SharedPointer obj...");
			}
		}
	}

	SharedPointer(const SharedPointer<T> &obj) : Pointer<T>(NULL)
	{
		assign(obj);
	}

	SharedPointer<T> &operator= (const SharedPointer<T> &obj)
	{
		if (this != &obj)
		{
			clear();

			assign(obj);
		}

		return *this;
	}

	void clear()
	{
		T *tmp = this->m_pointer;
		int *ref = this->m_ref;

		this->m_pointer = NULL;
		this->m_ref = NULL;

		//检查指向的堆空间是否可以释放了
		if (NULL != ref)
		{
			*(ref) = *(ref) - 1;

			if (0 == *ref)
			{
				free(ref);
				delete tmp;
			}
		}
	}

	~SharedPointer()
	{
		clear();
	}

};

//全局的比较操作符重载,比较操作符应该是要得到智能指针是否指向同一片堆空间
template < typename T >
static bool operator == (const SharedPointer<T> &l, const SharedPointer<T> &r)
{
	return (l.get() == r.get());
}

template < typename T >
static bool operator != (const SharedPointer<T> &l, const SharedPointer<T> &r)
{
	return !(l == r);
}

template < typename T >
static bool operator == (const SharedPointer<T> &l, const T *r)
{
    return (l.get() == r);
}

template < typename T >
static bool operator != (const SharedPointer<T> &l, const T *r)
{
    return !(l.get() == r);
}

template < typename T >
static bool operator == (const T *l, const SharedPointer<T> &r)
{
    return (l == r.get());
}

template < typename T >
static bool operator != (const T *l, const SharedPointer<T> &r)
{
    return !(l == r.get());
}
}




#endif
