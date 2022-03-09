#include <iostream>
#include <ctype.h>  /*for isdigit*/
#include <climits>

#include "BTree.h"
#include "LinkList.h"
#include "SharedPointer.h"

/*思路:检查字符串s首字符与入参c是否一致,如果是则n+1,然后递归执行,否则将c赋值s[0]开启递归,获取n后与入参n比较即可,当然返回值就是出现次数最多的字符*/
char MaxDupChar(const char *s, int &n, char c)
{
    /*递归基很简单,就是遇到字符串结尾标志*/
    if ('\0' == s[0])
        return c;

    if (c == s[0])
    {
        return MaxDupChar(s + 1, ++n, c);
    }
    else
    {
        int tmp = 1;
        char max_c = MaxDupChar(s + 1, tmp, s[0]);
        if (tmp > n)
        {
            n = tmp;
            return max_c;
        }
        else
        {
            return c;
        }
    }
}

/*问题1:查找字符串连续出现次数最多的字符*/
char MaxDupChar(const char *s, int &n)
{
    //assert : !s || !strlen(s)
    char c = s[0];
    s += 1;
    return MaxDupChar(s, n = 1, c);
}

bool isIntStr(const char *s, int idx, int &n)
{
    /*递归基也是遇到字符串结尾*/
    if ('\0' == s[idx])
        return true;

    if (isdigit(s[idx]))
    {
        /*是数字,则 n = 10 * n + s[idx]*/
        n = n * 10 + (s[idx] - 0x30);   /*字符0的ASCII码是0x30*/

        /*递归*/
        return isIntStr(s, idx + 1, n);
    }
    else
    {
        /*不是数字就可以退出了*/
        return false;
    }
}

/*问题2:判断字符串是不是整数字符串,如果是,将整数保存在n中返回*/
bool isIntStr(const char *s, int &n)
{
    //assert : nullptr == s || 0 == strlen(s)
    return isIntStr(s, 0, n = 0);
}

/*问题3:在不排序的前提下,求数组的中位数*/
/*
 *中位数的计算公式:
 *  当N为奇数,m = a[n / 2],比如0 1 2就需要1 == (3 / 2)
 *  当N为偶数,m = (a[n / 2] + a[n / 2 - 1]) / 2, 比如 0 1 2 3,就需要 1跟2
 *
 * 思路:类似快速排序的partition方法,每次都能从partition中找到一个就位的元素,我们只需要判断partition返回的就位元素的下标是否满足要求,如果不满足则继续调用partition就可以了
 * 我们需要的下标应当是 idx = n / 2, 这个不管数组长度是奇数还是偶数都需要,此外偶数还需要下标为 (n/2) - 1的
 * */

/*邓公的数据结构书里边的版本C partition*/
static int partition(int a[], int lo, int hi)
{
   //swap (a[lo], a[ lo + rand() % ( hi - lo ) ] ); //任选一个元素与首元素交换
   int pivot = a[lo];
   int mi = lo;

   for (int k = lo + 1; k < hi; k++)
      if (a[k] < pivot)
         swap (a[++mi], a[k] );

   swap (a[lo], a[mi]);

   return mi;
}

/*此处实现功能函数,从[lo, hi)中查找排序后下标为k和k-1的值分别存入n和 n1*/
void findIdxNum(int a[], int lo, int hi, int k, int &n, int &n1, bool find_k, bool find_k1)
{
    /*递归基,没有元素了*/
    if (lo >= hi)
        return;

    /*首先调用partition找轴点*/
    int pivot = partition(a, lo, hi);

    /*看一下是否为k或者k-1*/
    if (k == pivot)
    {
        n = a[pivot];
        find_k = true;
        /*如果第k-1个数也找到了,就可以返回*/
        if (find_k1)
            return;
    }
    else if (k - 1 == pivot)
    {
        n1 = a[pivot];
        find_k1 = true;
        /*如果第k个数也找到了,就可以返回*/
        if (find_k)
            return;
    }

    /*还没找完2个数,那就在pivot的一侧递归找*/
    if (pivot >= k)
        findIdxNum(a, lo, pivot, k, n, n1, find_k, find_k1);    /*在轴点的左边找*/
    else
        findIdxNum(a, pivot + 1, hi, k, n, n1, find_k, find_k1);

}

int FindMid(int a[], int n)
{
    return 0;
}


/*问题4:查找并打印二叉树中的最大路径(路径上结点和最大)*/
/*
 *
 *
 */

int maxPathSum(MyLib::BTreeNode<int> *root)
{
    if (nullptr == root)
        return INT_MIN;
}


void recursion_exercise3(void)
{
    int n;
    //char s[] = "dafajgkaajakjjjljgapgjaaaakg";
    char s[] = "333333333jgkaajakjjjljgapgjaaaakg";
    char c = MaxDupChar(s, n);

    std::cout << "c = " << c << ", n = " << n << std::endl;

    //char s1[] = "123456";
    char s1[] = "1af";
    //char s1[] = "1";
    int n1;
    if (isIntStr(s1, n1))
        std::cout << "integer string = " << n1 << std::endl; 
    else
        std::cout << "is not a integer string ..." << std::endl;
}