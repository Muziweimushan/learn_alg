#pragma once

#ifndef __HEAP_H__
#define __HEAP_H__


#include "Queue.h"

namespace MyLib
{

template < typename T >
class Heap : public Queue<T>
{
public:
    Heap(bool isMax)
    {
        m_isMax = isMax;
        this->m_length = 0;
    }

    void add(const T &e)
    {
        int i = length() + 1;   /*完全二叉树对树节点编号从1开始*/

        if (i <= capacity())
        {
            while ((i > 1) && (compare(e, parent(i))))
            {
                get(i) = get(parent(i));
                i = parent(i);
            }

            get(i) = e;

            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No space in Heap currently ...");
        }
    }

    /*满足队列的删除操作,也就是从队列头删除*/
    void remove(int i)
    {


    }
    T front(void) const;
    void clear();
    int length() const;


    virtual int capacity(void) const = 0;

protected:
    bool m_isMax;
    int m_length;

    virtual T &get(int i) = 0;

    T &get(int i)
    {
        return const_cast<Heap<T> *>(this)->get(i);
    }

    /*完全二叉树的性质,这些接口设计为protected主要因为堆同时也是一棵二叉树,可以被当作树来使用*/
    int left(int i) const { return (i << 1); }
    int right(int i) const { return ((i << 1) + 1); }
    int parent(int i) const { return (i >> 1); }

    virtual bool compare(const T &lv, const T &rv) const
    {
        return (m_isMax ? (lv > rv) : (lv < rv));
    }

};

}

#endif
