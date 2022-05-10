#pragma once

#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include "LinkList.h"
#include "Array.h"

namespace MyLib
{

template < typename T >
class WorkList : public LinkList<T>
{
public:
    using Node = typename LinkList<T>::Node;

    WorkList();
    WorkList(const Array<T> &array)
    {
        WorkList(array.array(), array.length());
    }

    WorkList(T *arr, int len)
    {
        for (int i = 0; i < len; i++)
            this->insert(arr[i]);
    }
private:
};

}

#endif
