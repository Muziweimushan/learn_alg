#pragma once

#ifndef __DUALENDQUEUE_H__
#define __DUALENDQUEUE_H__

#include "LinkQueue.h"
#include "Exception.h"

namespace MyLib
{

template < typename T >
class DualEndQueue : public LinkQueue<T>
{
    typedef typename LinkQueue<T>::Node Node;
public:
    /*将元素压入队列头部*/
    void push(const T &e)
    {
        Node *node = new Node;
        if (nullptr != node)
        {
            node->value = e;
            /*list_add函数将header的后面,也就是实现了头插法,满足要求*/
            list_add(&node->head, &this->m_header); 
            this->m_length++;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory for new DualEndQueue element ...");
        }
    }

    /*将队尾元素删除*/
    void dismiss(void)
    {
        if (this->m_length > 0)
        {
            list_head *toDel = this->m_header.prev;

            list_del(toDel);
            this->m_length--;

            delete list_entry(toDel, Node, head);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in DualEndQueue ...");
        } 
    }

    /*获取队尾元素*/
    T back(void) const
    {
        if (this->m_length > 0)
        {
            return list_entry(this->m_header.prev, Node, head)->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in DualEndQueue ...");
        } 
    }
};

}

#endif
