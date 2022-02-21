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
        : m_value(1, (1 << height) - 1),
        m_mark(m_value.lower(), m_value.upper()),
        m_height(height)
    {
        this->m_array = NULL;
        for (int i = m_mark.lower(); i <= m_mark.upper(); i++) 
           m_mark[i] = false; 
    }

    BTreeArray(const BTreeArray<T> &obj)
        : m_value(obj.m_value),
        m_mark(obj.m_mark),
        m_height(obj.height)
    {
        this->m_array = nullptr;
    }

    BTreeArray<T> &operator= (const BTreeArray<T> &obj)
    {
        if (this != &obj)
        {
            this->m_value = obj.m_value;
            this->m_mark = obj.m_mark;
            this->m_height = obj.m_height;
        }

        return *this;
    }

    /*将元素e存入二叉树位置i处,若其父节点不存在,则插入失败*/
    /*注意此接口不抛出异常*/
    bool set(int i, const T &e)
    {
        /*由于此函数不抛出异常,以此在确定下标i正确之前不能使用mark/value的operator [], 而只能使用set/get接口*/
        bool tmp;
        bool ret = m_mark.get(i, tmp) && (1 == i || m_mark[(i >> 1)]);  /*通过短路规则,当get接口返回true时i的合法性得到满足,同时i非法时后续代码不会执行*/

        if (ret)
        {
            m_mark[i] = true;
            m_value[i] = e;
        }

        return ret;
    }

    /*获取二叉树位置i的元素值*/
    bool get(int i, T &e) const
    {
        bool tmp;
        bool ret = m_mark.get(i, tmp) && tmp;

        if (ret)
            e = m_value[i];

        return ret;
    }

    /*读取二叉树位置i处的元素值,如果不存在抛出异常,如果存在还允许修改对应元素的值(与set接口不一样的地方在于set接口可以新插入元素,而这个接口不能)*/
    T &operator[] (int i)
    {
        if (1 <= i && i < length() && m_mark[i])
            return m_value[i];
        else
            THROW_EXCEPTION(InvalidOperationException, "Position i is invalid ...");
    }

    T operator[] (int i) const
    {
        return const_cast<BTreeArray<T> &>(*this)[i];
    }
 
    /*删除以位置i的顶点为根节点的子树*/
    void remove(int i)
    {
        if (m_mark.lower() <= i && i <= m_mark.upper())
        {
            m_mark[i] = 0;

            /*递归执行即可*/
            remove(left(i));
            remove(right(i));
        }
    }

    /*重置树的高度*/
    void resize(int nh)
    {
        if (nh < 1)
            return;

        int len = (1 << nh) - 1;
        RangeArray<T> value(1, len);
        RangeArray<bool> mark(1, len);

        for (int i = 1; i <= mark.length(); i++)
            mark[i] = false;

        len = (len < length()) ? len : length();
        std::cout << "new length = " << len << std::endl;

        for (int i = 1; i <= len; i++)
        {
            mark[i] = m_mark[i];
            if (mark[i])
            {
                std::cout << "..." << std::endl; 
                value[i] = m_value[i];
            }
        }

        m_value = value;
        m_mark = mark;
        m_height = nh;
    }

    /*删除所有节点*/
    void clear(void)
    {
        for (int i = m_mark.lower(); i <= m_mark.upper(); i++)
            m_mark[i] = false;
    }

    /*返回树的高度*/
    int height(void) const
    {
        return height(1);
    }

    /*返回树的节点个数*/
    int count(void) const
    {
        int ret = 0;
        for (int i = m_mark.lower(); i <= m_mark.upper(); i++)
            ret += m_mark[i];

        return ret;
    }

    /*返回树的容量(最多可插入多少个节点)*/
    int length(void) const
    {
        return m_mark.length();
    }

    /*返回编号i的节点的左孩子的编号*/
    int left(int i) const
    {
        return (i << 1);
    }

    /*返回编号i的节点的右孩子的编号*/
    int right(int i) const
    {
        return ((i << 1) + 1);
    }

    /*返回编号i的节点的父节点编号*/
    int parent(int i) const
    {
        return (i >> 1);
    }

    /*当前节点是否为空*/
    bool isNull(int i) const
    {
        return !(1 <= i && i <= length() && m_mark[i]);
    }

    /*查找节点,约定找不到返回0*/
    int find(const T &e) const
    {
        int ret = 0;

        for (int i = m_mark.lower(); i <= m_mark.upper(); i++)
        {
            if (m_mark[i] && e == m_value[i])
            {
                ret = i;
                break;
            }
        }

        return ret;
    }

protected:
    /*功能函数:获取以i为根节点的子树的高度*/
    int height(int i) const
    {
        int ret = 0;

        if (!isNull(i))
        {
            int lh = height(left(i));
            int rh = height(right(i));

            ret = ((lh < rh) ? rh : lh) + 1;
        }

        return ret;
    }

private:
    RangeArray<T> m_value;
    RangeArray<bool> m_mark;
    int m_height;   /*顺序二叉树的高度*/
};

}



#endif
