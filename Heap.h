#pragma once

#ifndef __HEAP_H__
#define __HEAP_H__

#include "Exception.h"
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
        m_length = 0;
    }

    void add(const T &e)
    {
        int i = length() + 1;   /*完全二叉树对树节点编号从1开始*/

        //std::cout << "adding " << e << " ..." << std::endl;

        if (i <= capacity())
        {
            while ((i > 1) && (compare(e, get(parent(i)))))
            {
                //std::cout << "e = " << e << ", parent = " << parent(i) << " , parent value = " << get(parent(i)) <<  std::endl;
                get(i) = get(parent(i));
                i = parent(i);
            }

            //std::cout << "i = " << i << std::endl;
            get(i) = e;

            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No space in Heap currently ...");
        }
    }

    /*满足队列的删除操作,也就是从队列头删除*/
    void remove()
    {
        if (length() <= 0)
            THROW_EXCEPTION(InvalidOperationException, "Heap is empty ...");

        /*从堆中取走队列头也就是将位置为1的元素从队列中拿掉,此时需要重建堆,方法是将堆中最后一个元素移到堆的头部,并从这个元素开始自顶向下的做一次toHeap函数一次for循环的重建即可,可见这里的移动是线性的,而且复杂度为O(lnh)*/

        T &e = get(length());   /*拿出堆中最后一个元素*/
        int i = 1;
        int ci = left(i);

        m_length--;
        while (ci <= length())
        {
            /*看看左孩子右孩子中哪个大,哪个大进入哪一侧*/
            if (ci < length() && (compare(get(ci + 1), get(ci))))
                ci++;
            
            /*如果不需要交换,则循环可以退出*/
            if (!compare(get(ci), e))
                break;
            
            /*孩子比父亲大(小),将孩子移到父亲的位置上*/
            get(i) = get(ci);
            i = ci; /*深入到较大的一侧*/
            ci = left(i);
        }

        get(i) = e;
    }

    /*实现获取队头元素,优先级队列返回队列中最大/最小的元素*/
    T front(void) const
    {
        if (length() > 0) 
            return get(1);  /*完全二叉树对树节点编号从1开始*/
        else
            THROW_EXCEPTION(InvalidOperationException, "Heap is empty ...");
    }

    /*不抛出异常版本的获取优先级队列的队首元素*/
    bool front(T &e) const
    {
        bool ret = true;

        if (length() > 0)
            e = get(1);
        else
            ret = false;

        return ret;
    }

    void clear()
    {
        m_length = 0;
    }

    int length() const
    {
        return m_length;
    }


    virtual int capacity(void) const = 0;

protected:
    bool m_isMax;
    int m_length;

    virtual T &get(int i) = 0;

    T &get(int i) const
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
