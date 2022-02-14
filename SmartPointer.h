#ifndef __SMARTPOINTER_H__
#define __SMARTPOINTER_H__

#include "Pointer.h"

namespace MyLib
{

template < typename T >
class SmartPointer : public Pointer<T>
{

public:

	SmartPointer(T *p = NULL) : Pointer<T>(p)
	{
	}

	SmartPointer(const SmartPointer<T> &obj)
	{
		this->m_pointer = obj.m_pointer;
		
		//因为obj是const变量,不能直接对const变量的成员赋值,需要去除obj的const属性,使用const_cast
		const_cast<SmartPointer<T> &>(obj).m_pointer = NULL;
	}

	//返回自身是为了可以连续赋值 a = b = c;
	SmartPointer<T> &operator= (const SmartPointer<T> &obj)
	{
		if (this != &obj)		//自赋值就不需要这么做了
		{
			T *tmp = this->m_pointer;
			//delete m_pointer;
			
			this->m_pointer = obj.m_pointer;
			
			//去除obj的const属性才能对成员变量进行赋值
			const_cast<SmartPointer<T> &>(obj).m_pointer = NULL;

			//最后再释放原来堆空间,保证异常安全
			delete tmp;
		}

		return *this;
	}

	
	~SmartPointer()
	{
		delete this->m_pointer;
	}

};



}








#endif
