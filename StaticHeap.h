#pragma once

#ifndef __STATICHEAP_H__
#define __STATICHEAP_H__

#include "Heap.h"
#include "StaticArray.h"

namespace MyLib
{

/*静态堆实现,内部存储空间使用static array实现*/
template < typename T, int N >
class StaticHeap : public Heap<T>
{
public:
    StaticHeap(bool isMax = true)
        : Heap<T>(isMax)
    {}

    int capacity(void) const
    {
        return N;
    }
protected:

    T &get(int i) 
    {
        /*类似BTreeArray*/
        return m_array[i - 1];
    }

    StaticArray<T, N> m_array;
};


}


#endif
