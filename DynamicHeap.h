#pragma once

#ifndef __DYNAMICHEAP_H__
#define __DYNAMICHEAP_H__

#include "DynamicArray.h"

namespace MyLib
{

template < typename T >
class DynamicHeap : public Heap<T>
{
public:
    DynamicHeap(int capacity, bool isMax = true)
        : Heap<T>(isMax), m_array(capacity)
    {
    }
    
    int capacity(void) const { return m_array.length(); }

    void resize(int newSize)
    {
        m_array.resize(newSize);

        this->m_length = (this->m_length < newSize) ? this->m_length : newSize;
    }

protected:
    DynamicArray<T> m_array;
    
    T &get(int i)
    {
        return m_array[i - 1];
    }
};


}

#endif
