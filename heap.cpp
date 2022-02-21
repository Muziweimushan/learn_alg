#include <iostream>
#include <cmath>

#include "./BTreeArray.h"


bool toHeap(MyLib::BTreeArray<int> &arr)
{
    bool ret = true;
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
            if (ci < size && (arr[ci] < arr[ci + 1]))
                ci++;

            /*如果当前顶点值比左右孩子的都大,那么while迭代可以结束了*/
            if (arr[ci] < e)
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

    return ret;
}


void test_heap(void)
{
    int a[] = {1, 2, 6, 4, 5, 3};
    MyLib::BTreeArray<int> arr(log2(sizeof(a) / sizeof(*a)) + 1);

    for (uint32_t i = 1, j = 0; j < sizeof(a) / sizeof(*a); j++, i++)
        arr.set(i, a[j]);

    toHeap(arr);

    for (int i = 1; i <= arr.count(); i++)
        std::cout << arr[i] << std::endl;
}
