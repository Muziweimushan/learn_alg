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
 *  当N为奇数,m = a[n + 1 / 2]
 *  当N为偶数,m = (a[n / 2] + a[n / 2 + 1]) / 2
 *
 * 从排序的角度看,中位数就是排序后的数组中最中间的数
 * 也就是 对于已经排好序的数组来说,中位数mid必然大于等于它的前驱
 * 同时mid也必然小于等于它的所有后继
 * */
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
