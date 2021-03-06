#ifndef __ARRAY_H__
#define __ARRAY_H__


#include "Object.h"
#include "Exception.h"


namespace MyLib
{

template < typename T >
class Array : public Object
{
public:
	virtual bool set(int i, const T &e)				//O(1)
	{
		bool ret = ((0 <= i) && (i < length()));

		if (ret)
		{
			m_array[i] = e;
		}

		return ret;
	}

	virtual bool get(int i, T &e) const				//O(1)
	{
		bool ret = ((0 <= i) && (i < length()));

		if (ret)
		{
			e = m_array[i];
		}

		return ret;
	}


	T &operator[] (int i)					//O(1)
	{
		if ((0 <= i) && (i < length()))
		{
			return m_array[i];	
		}
		else
		{
			THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid...");
		}
	}

	T operator[] (int i) const					//O(1)
	{
		return (const_cast<Array<T> &>(*this))[i];
	}

	//新增成员函数返回封装的数组空间
	T *array() const
	{
		return m_array;
	}

	virtual int length() const = 0;

    Array<T> &self()
    {
        return *this;
    }

protected:
	T *m_array;     /*具体的数组空间由子类决定*/
};




}






#endif
