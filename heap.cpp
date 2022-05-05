#include <iostream>
#include <cmath>
#include <cstdlib>

#include "./BTreeArray.h"
#include "DynamicHeap.h"
#include "StaticHeap.h"
#include "Sort.h"


bool toHeap(MyLib::BTreeArray<int> &arr, bool isMax = true)
{
    bool ret = true;

    /*因为一棵顺序结构二叉树不一定是完全二叉树,所以下面操作不一定都是合法,因此需要try catch*/
    try
    {

        int size = arr.count();

        //std::cout << "size = " << size << std::endl;

        /*自底向上调整为一个堆,首先从完全二叉树最右边的节点开始,这个节点的编号恰好就是size,而迭代将从它的父亲开始*/
        /*从最右边叶节点的父亲开始,从右向左,自底向上遍历所有节点*/
        for (int i = size >> 1; i >= 1; --i)
        {
            const int e = arr[i];
            int ci = i << 1;    /*ci ----> child of i, 顶点i的左孩子的编号,记作ci*/

            //std::cout << "i = " << i << ", ci = " << ci << std::endl;

            /*自顶点i开始,自顶向下通过逐层比较,将i为根节点的子树调整为堆*/
            while (ci <= size)
            {
                /*在当前顶点的左孩子和右孩子中挑选更大者,ci指向左孩子*/
                //std::cout << "in while :  ci = " << ci << std::endl;
                if (ci < size && (isMax ? (arr[ci] < arr[ci + 1]) : (arr[ci] > arr[ci + 1])))
                    ci++;

                /*如果当前顶点值比左右孩子的都大,那么while迭代可以结束了*/
                if (isMax ? (arr[ci] < e) : (arr[ci] > e))
                    break;

                /*否则的话,将数值更大的孩子的值交换至父节点处*/
                arr[(ci >> 1)] = arr[ci];
                /*父节点的下一层(也就是ci)已经处理完,意思是在顶点i的子树中,i,2i,2i+1这三个顶点构成的树已经能组成一个堆,转向2i*2和 2i*2 + 1也就是下一层*/
                ci <<= 1; //ci = ci * 2
            }

            /*进此处说明顶点i确实移动过,那么ci的父节点就是最终存放原来顶点i的值的位置*/
            if ((ci >> 1) != i)
                arr[(ci >> 1)] = e;
        }
    }
    catch (const MyLib::Exception &)
    {
        ret = false;
    }

    return ret;
}

bool compare(int lv, int rv, bool isMax = true)
{
    return (isMax ? (lv >= rv) : (lv <= rv));
}

bool isHeap(MyLib::BTreeArray<int> &bt, int i = 1, bool isMax = true)
{
    bool ret = true;
    
    if (!bt.isNull(i))
    {
        int lc = bt.left(i);
        int rc = bt.right(i);
        
        /*分4种情况讨论*/
        if (!bt.isNull(lc) && !bt.isNull(rc))
        {
            /*左孩子右孩子都存在的情况,值比左右孩子都大而且左子树右子树也是堆*/
            //ret = ((bt[i] >= bt[lc]) && (bt[i] >= bt[rc]) && (isHeap(bt, lc)) && (isHeap(bt, rc)));
            ret = ((compare(bt[i], bt[lc], isMax)) && (compare(bt[i], bt[rc], isMax)) && (isHeap(bt, lc, isMax)) && (isHeap(bt, rc, isMax)));
        }
        else if (!bt.isNull(lc) && bt.isNull(rc))
        {
            /*左子树存在而右子树不存在,则值比左子树大且左子树也为堆是成立*/
            ret = ((compare(bt[i], bt[lc], isMax)) && (isHeap(bt, lc, isMax)));
        }
        else if (bt.isNull(lc) && !bt.isNull(rc))
        {
            /*仅有右子树,此时该树必然不是完全二叉树,而堆是一种特殊的完全二叉树,因此不成立*/
            ret = false;
        }
        else
        {
            /*左子树右子树都不存在,必然是堆*/
            ret = true;
        }
    }

    return ret;
}

namespace TestHeap
{
    enum { TEST_ARRAY_LEN = 10000 };
    void check(void)
    {
        MyLib::DynamicHeap<int> heap(10);

        int a[] = {2, 4, 1, 3, 5};

        for (unsigned int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
            heap.add(a[i]);

        while (heap.length())
        {
            std::cout << heap.front() << std::endl;
            heap.remove();
        }


        srand(time(NULL));
        int len = ::rand() % TEST_ARRAY_LEN;
        if (!len)
            len = TEST_ARRAY_LEN;

        int *array = new int[len];
        for (int i = 0; i < len; i++)
            array[i] = rand();

        MyLib::Sort::Heap<int>(array, len, true);

        for (int i = 0; i < len; i++)
            std::cout << i << std::endl;

        delete [] array, array = nullptr;
    }
}

namespace TestToHeap
{

    void check(void)
    {
        int a[] = {1, 2, 6, 4, 5, 3};
        MyLib::BTreeArray<int> arr(std::log2(sizeof(a) / sizeof(*a)) + 1);

        for (uint32_t i = 1, j = 0; j < sizeof(a) / sizeof(*a); j++, i++)
            arr.set(i, a[j]);

        if (toHeap(arr, false))
        {
            if (isHeap(arr, 1, false))
                for (int i = 1; i <= arr.count(); i++)
                    std::cout << arr[i] << std::endl;
            else
                std::cout << "not a heap ..." << std::endl;
        }
        else
        {
            std::cout << "to heap failed ..." << std::endl;
        }
    }
}

void test_heap(void)
{
    TestToHeap::check();

    //TestHeap::check();
}
