#ifndef __STATICLIST_H__
#define __STATICLIST_H__

#include "SeqList.h"


namespace MyLib
{

template < typename T ,int N >
class StaticList : public SeqList<T>
{
protected:
	T m_space[N];

public:
	//指定SeqList父类中的存储空间指针,使其指向StaticList类中定义的数组空间,以及其他成员初始化
	StaticList()
	{
		this->m_array = m_space;
		this->m_length = 0;
	}

	int capacity() const
	{
		return N;
	}



};



}





#endif
