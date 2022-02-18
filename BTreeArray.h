#pragma once

#ifndef __BTREEARRAY_H__
#define __BTREEARRAY_H__

#include "RangeArray.h"

namespace MyLib
{

template < typename T >
class BTreeArray : public Array<T>
{
public:
    BTreeArray(int height)
        : m_value(1, (height << 1) - 1),
        m_mark(m_value.lower(), m_value.upper()),
        m_height(height)
    {
        this->m_array = NULL;
        for (int i = m_mark.lower(); i <= m_mark.upper(); i++)
           m_mark[i] = 0; 
    }

    BTreeArray(const BTreeArray<T> &obj)
        : m_value(obj.m_value),
        m_mark(obj.m_mark),
        m_height(obj.height)
    {}


private:
    RangeArray<T> m_value;
    RangeArray<int> m_mark;
    int m_height;   /*顺序二叉树的高度*/
};

}



#endif
