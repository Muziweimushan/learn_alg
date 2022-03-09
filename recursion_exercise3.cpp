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
    int k = 0;      /*k*/
    int k1 = 0;     /*k-1*/
    int ret = INT_MIN;

    findIdxNum(a, 0, n, (n >> 1), k, k1, false, false);

    if (n & 1)
        /*n为奇数*/
        ret = k;
    else
        ret = ((k + k1) >> 1);

    std::cout << "median = " << ret << std::endl;

    return ret;
}



class MaxPathSum
{
public:
    MaxPathSum()
        : sum(INT_MIN)
    {
    }

    
    int solve(MyLib::BTree<int> &tree);
private:
    int sum;

};

MyLib::LinkList<int> solution;
static int g_max_sum = INT_MIN;

static bool update(int curMax, MyLib::LinkList<int> &queue)
{
    bool ret = false;

    if (curMax > g_max_sum)
    {
        ret = true;
        g_max_sum = curMax;


        solution.clear();
    
        for (queue.move(0); !queue.end(); queue.next())
            solution.insert(queue.current());
    }

    return ret;
}

/*问题4:查找并打印二叉树中的最大路径(路径上结点和最大)*/
/*
 * 思路:最大路径一共有几种情况
 *  a.只有一个根结点(它的左右孩子为根子树的最大路径和都是负的而根结点是正)
 *  b.位于根结点的左子树中,不包括根结点
 *  c.位于...右子树
 *  d.左子树->根结点
 *  e.右子树->根结点
 *  f.左子树->根结点->右子树
 *
 *  对于递归来说(也就是当前结点的结果可以通过拿它的左或者右子树的结果计算得到),情况b c f都是不能作为递归的返回结果的,因为这些结果不能被它的母问题使用
 *  对于这三种情况单独比较,如果满足条件就更新到全局的答案里边
 *
 */
int maxPathSum(MyLib::BTreeNode<int> *root, MyLib::LinkList<int> &queue)
{
    if (nullptr == root)
        return INT_MIN;

    MyLib::LinkList<int> leftQueue;
    MyLib::LinkList<int> rightQueue;
    int maxLeft;
    int maxRight;
    int ret = INT_MIN;

    if (root->left)
        maxLeft = maxPathSum(root->left, leftQueue); 
    else
        maxLeft = INT_MIN;

    if (root->right)
        maxRight = maxPathSum(root->right, rightQueue); 
    else
        maxRight = INT_MIN;

    int curSum = root->value;
    /*接下来开始处理各种情况*/

    /*TODO:我们是否能分成两大类来处理,就是能作为子问题的解和不能作为子问题的解来处理*/
    std::cout << "max left = " << maxLeft << std::endl;
    std::cout << "max right = " << maxRight << std::endl;
    std::cout << "cur sum = " << curSum << std::endl;

    /*左右子树的结果加上当前根节点的值变小了,不要左右子树,包含了左右子树是空的情况*/
    if (curSum > (curSum + maxLeft) && curSum > (curSum + maxRight))
    {
        std::cout << "use root only as sub-result ..." << std::endl;
        /*它是可以作为递归的返回结果的*/
        queue.insert(root->value);
        //sum += root->value;

        ret = root->value;
    }
    
    if (maxLeft <= 0 && maxRight <= 0)
    {
        /*如果子树的结果是小于0的,那么它肯定不作为子问题的解了,因为root+maxLeft必然小于root,所以肯定从root开始而不会从左子树开始*/
        /*但是当前结果可以作为最后的解*/
        if (maxLeft > maxRight)    /*统一用左边的*/
        {
            std::cout << "use left sub-tree as one solve ..." << std::endl;
            update(maxLeft, leftQueue);
        }
        else if (maxRight > INT_MIN)
        {
            std::cout << "use left sub-tree as one solve ..." << std::endl;
            update(maxRight, rightQueue);
        }
        //else ;   /*两个都是INT_MIN*/
    }
    
    if (curSum + maxLeft > curSum || curSum + maxRight > curSum)
    {
        std::cout << "here" << std::endl;
        /*这种是可以作为子问题解的情况*/
        if (maxLeft > maxRight)
        {
            std::cout << "use left tree ..." << std::endl;
            /*局部解为 左子树->root*/
            for (leftQueue.move(0); !leftQueue.end(); leftQueue.next())
                queue.insert(leftQueue.current());

            queue.insert(root->value);

            //sum += (maxLeft + curSum);
            ret = maxLeft + curSum;
        }
        else
        {
            std::cout << "use right tree ..." << std::endl;
            /*局部解为 左子树->root*/
            for (rightQueue.move(0); !rightQueue.end(); rightQueue.next())
                queue.insert(rightQueue.current());

            queue.insert(root->value);

            //sum += (maxRight + curSum);
            ret = maxRight + curSum;
        }
    }

    if (maxLeft > 0 && maxRight > 0)
    {
        std::cout << "find one solve ..." << std::endl;
        /*可以作为问题的一个最终解*/
        /*左子树->root->右子树*/
        MyLib::LinkList<int> ans;

        for (leftQueue.move(0); !leftQueue.end(); leftQueue.next())
            ans.insert(leftQueue.current());

        ans.insert(root->value);

        for (rightQueue.move(0); !rightQueue.end(); rightQueue.next())
            queue.insert(rightQueue.current());

        update(curSum + maxLeft + maxRight, ans);
    }

    return ret;
}

int maxPathSum(MyLib::BTreeNode<int> *root)
{
    MyLib::LinkList<int> queue;

    int sum = maxPathSum(root, queue);
    std::cout << "queue length = " << queue.length() << std::endl;
    if (sum > g_max_sum)
    {
        for (queue.move(0); !queue.end(); queue.next())
            std::cout << queue.current() << std::endl;
    }
    else
    {
        for (solution.move(0); !solution.end(); solution.next())
            std::cout << solution.current() << std::endl;
    }

    std::cout << "sum = " << sum << std::endl;

    return 0;
}

void set_tree(MyLib::BTree<int> &tree)
{
    tree.insert(1, NULL);

    MyLib::BTreeNode<int> *root = tree.root();

    tree.insert(3, root, MyLib::LEFT);

    MyLib::BTreeNode<int> *l1 = root->left;

    tree.insert(-1, l1, MyLib::RIGHT);
    tree.insert(9, l1, MyLib::LEFT);
}

static void test_maxpathsum(void)
{
    MyLib::BTree<int> tree;

    set_tree(tree);

    maxPathSum(tree.root());
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


    int a[] = {1, 4, 3, 4};
    FindMid(a, sizeof(a) / sizeof(*a));


    test_maxpathsum();
}
