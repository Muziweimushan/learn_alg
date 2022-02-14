#ifndef __POINTER_H__
#define __POINTER_H__

#include "Object.h"

namespace MyLib
{

template < typename T >
class Pointer : public Object
{
protected:
	T *m_pointer;

public:
	Pointer(T *p = NULL)
	{
		m_pointer = p;
	}

	T *operator ->()
	{
		return m_pointer;
	}

	const T *operator ->() const
	{
		return m_pointer;
	}

	T &operator *()
	{
		return *m_pointer;
	}

	const T &operator *() const
	{
		return *m_pointer;
	}

	bool isNull() const 
	{
		return (NULL == m_pointer);
	}

	T *get() const
	{
		return m_pointer;
	}

	//不需要提供析构函数,因为顶层父类Object是一个纯虚父类,只要这里不实现析构函数,那么这个子类也是一个纯虚类

};


}





#endif
