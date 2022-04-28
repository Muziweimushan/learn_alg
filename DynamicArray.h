#pragma once

#ifndef __DYNAMICARRAY_H__
#define __DYNAMICARRAY_H__

#include "Array.h"

namespace MyLib
{
template < typename T >
class DynamicArray : public Array<T>
{
public:
	DynamicArray(int length = 0)		//O(1)
	{
		init(new T[length], length);
	}

	DynamicArray(const DynamicArray<T> &obj)		//O(n)
	{
		init(copy(obj.m_array, obj.m_length, obj.m_length), obj.m_length);
	}

	DynamicArray<T> &operator= (const DynamicArray<T> &obj)		//O(n)
	{
		update(copy(obj.m_array, obj.m_length, obj.m_length), obj.m_length);
		
		return *this;
	}

	void resize(int length)		//O(1)
	{
		update(copy(this->m_array, this->m_length, length), length);
	}

	int length() const			//O(1)
	{
		return m_length;
	}

	~DynamicArray()				//O(1)
	{
		delete[] this->m_array;
	}

protected:
	int m_length;
	
	//申请一段新内存,然后拷贝原数组到新空间中
	T *copy(T *array, int oriLen, int newLen)		//O(min(oriLen, newLen)) => O(n)
	{
		T *ret = new T[newLen];

		if (nullptr != ret)
		{
			int copyLen = (oriLen > newLen) ? newLen : oriLen;

			for (int i = 0; i < copyLen; i++)
			{
				ret[i] = array[i];
			}
		}

		return ret;
	}

	//更新新数组空间信息(指针和长度信息)到对象成员中
	void update(T *array, int newLen)		//O(1)
	{
		if (nullptr != array)
		{
			T *temp = this->m_array;

			this->m_array = array;
			this->m_length = newLen;

			delete[] temp;
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to update object...");
		}
	}
	
	//用于在构造函数中设置数组空间信息,与update区别就是不需要释放原空间,因为没有
	void init(T *array, int length)		//O(1)
	{
		if (nullptr != array)
		{
			this->m_array = array;
			this->m_length = length;
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No enougn memory to create DynamicArray object...");
		}
	}

};

}







#endif
