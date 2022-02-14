#ifndef __DYNAMICLIST_H__
#define __DYNAMICLIST_H__


#include "SeqList.h"
#include "Exception.h"

namespace MyLib
{

template < typename T >
class DynamicList : public SeqList<T>
{

protected:
	int m_capacity;

public:
	DynamicList(int capacity)
	{
		this->m_array = new T[capacity];

		if (NULL != this->m_array)
		{
			this->m_length = 0;
			m_capacity = capacity;
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No Enough memory to create DynamicList object...");	
		}
	}

	int capacity() const
	{
		return m_capacity;
	}

	//重新设置顺序存储空间的大小
	void resize(int capacity)
	{
		if (this->m_capacity != capacity)
		{
			//使用array缓存新堆空间的指针是为了能拷贝原空间的数据到新空间中
			T *array = new T[capacity];


			if (NULL != array)
			{
				int length = (this->m_length < capacity)? this->m_length : capacity;
				
				//赋值操作是必须要执行,赋值操作符被重载是不可避免的,那么就有可能产生异常,resize函数不能保证这里不发生异常
				for (int i = 0; i < length; i++)
				{
					array[i] = this->m_array[i];
				}
				
				//同样的,如果这里直接delete[] this->m_array的话也会调用泛指类型T的析构函数,也是可能产生异常的,resize函数在这里就要保证DynamicList类对象经过这次resize操作后依然合法使用
				T *temp = this->m_array;

				this->m_array = array;
				this->m_capacity = capacity;
				this->m_length = length;

				//保证DynamicList对象的成员变量都已经修改为合法值之后再释放堆空间
				delete[] temp;//释放原堆空间
			}
			else
			{
				THROW_EXCEPTION(NoEnoughMemoryException, "No memory to resize DynamicList object...");
			}
		}
	}

	~DynamicList()
	{
		delete[] this->m_array;
	}




};



}





#endif
