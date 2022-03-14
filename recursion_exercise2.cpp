#include <iostream>
#include <ctype.h>  /*for isdigit*/
#include <climits>

#include "BTree.h"
#include "LinkList.h"
#include "SharedPointer.h"

using namespace MyLib;

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

static void mySwap(int &a, int &b)
{
    /*用异或来做不知道为啥不行...*/
    int c = a;
    a = b;
    b = c;
}


/*邓公的数据结构书里边的版本C partition*/
static int partition(int a[], int lo, int hi)
{
   //swap (a[lo], a[ lo + rand() % ( hi - lo ) ] ); //任选一个元素与首元素交换
   int pivot = a[lo];
   int mi = lo;

   for (int k = lo + 1; k < hi; k++)
      if (a[k] < pivot)
            mySwap(a[++mi], a[k]);

   mySwap(a[lo], a[mi]);

   return mi;
}

/*此处实现功能函数,从[lo, hi)中查找排序后下标为k的元素的值*/
void findIdxNum(int a[], int lo, int hi, int k, int &target)
{
    /*递归基,没有元素了*/
    if (lo >= hi)
        return;

    /*首先调用partition找轴点*/
    int pivot = partition(a, lo, hi);
    
    //std::cout << "k = " << k << std::endl;

    /*看一下是否为k或者k-1*/
    if (k == pivot)
    {
        target = a[pivot];
        return;
    }

    /*还没找完,那就在pivot的一侧递归找*/
    if (pivot >= k)
        findIdxNum(a, lo, pivot, k, target);    /*在轴点的左边找*/
    else
        findIdxNum(a, pivot + 1, hi, k, target);

}

/*问题3:在不排序的前提下,求数组的中位数*/
/*
 *中位数的计算公式
 *  当N为奇数,m = a[n / 2],比如0 1 2就需要1 == (3 / 2)
 *  当N为偶数,m = (a[n / 2] + a[n / 2 - 1]) / 2, 比如 0 1 2 3,就需要 1跟2
 *
 * 思路:类似快速排序的partition方法,每次都能从partition中找到一个就位的元素,我们只需要判断partition返回的就位元素的下标是否满足要求,如果不满足则继续调用partition就可以了(在逼乎以前看过有类似的题目找前k个元素也是类似这么弄得)
 * 我们需要的下标应当是 idx = n / 2, 这个不管数组长度是奇数还是偶数都需要,此外偶数还需要下标为 (n/2) - 1的
 */
int FindMid(int a[], int n)
{
    int ret = INT_MIN;

    if (n & 1)
    {
        findIdxNum(a, 0, n, (n >> 1), ret);
    }
    else
    {
        int tmp1;
        int tmp2;
        /*偶数分支,找n/2 和 n/2 - 1*/
        findIdxNum(a, 0, n, (n >> 1), tmp1);
        findIdxNum(a, 0, n, (n >> 1) - 1, tmp2);
        
        std::cout << "tmp1 = " << tmp1 << ", tmp2 = " << tmp2 << std::endl; 
        ret = ((tmp1 + tmp2) >> 1);
    }

    std::cout << "median = " << ret << std::endl;

    return ret;
}

LinkList<int> solution;  /*是一个队列,但是queue不能遍历,这里用个链表来模拟队列,从尾部插入从头部拿*/
static int g_max_sum = INT_MIN;

static bool update(int curMax, LinkList<int> &queue)
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

static bool checkUpdate(int cur_sum)
{
    return (cur_sum > g_max_sum);
}

/*问题4:查找并打印二叉树中的最大路径(路径上结点和最大)*/
/*
 * 思路:二叉树......先进左边看一看,然后进右边看一看,然后结果再汇总
 *
 * 在当前层,当前根节点叫r,它的左子树叫L,右子树叫R
 * 最大路径一共有几种情况
 *  a.只有一个根结点(它的左右孩子为根子树的最大路径和都是负的)
 *  b.位于根结点的左子树中,但是不包括根结点
 *  c.位于根节点的右子树中,但是不包括根节点
 *  d.左子树->根结点
 *  e.右子树->根结点
 *  f.左子树->根结点->右子树(反过来一样的没区别)
 *
 *  对于递归来说(也就是当前结点的结果可以通过拿它的左或者右子树的结果计算得到),情况b c f都是不能作为递归的返回结果的,因为这些结果不能被它爹使用(比如f你不能从L走到R之后再回到r的父节点,也就是L+r+R就是一个结果了,虽然它不一定是最终的结果)
 *  对于这三种情况单独比较,如果满足条件就更新到全局的答案里边
 *  其他三种情况其实就是对a d e这三种情况做比较,哪个大就把它作为当前结果返回出去给它爹
 *
 */
int maxPathSum(BTreeNode<int> *root, LinkList<int> &queue)
{
    if (nullptr == root)
        return INT_MIN;

    LinkList<int> leftQueue;
    LinkList<int> rightQueue;
    int maxLeft;
    int maxRight;
    int ret = INT_MIN;

    maxLeft = maxPathSum(root->left, leftQueue);
    maxRight = maxPathSum(root->right, rightQueue); 

    //std::cout << "max left = " << maxLeft << std::endl;
    //std::cout << "max right = " << maxRight << std::endl;
    //std::cout << "cur sum = " << root->value << std::endl;

    /*接下来开始处理各种情况*/
    /*TODO:是否能分成两大类来处理,就是能作为子问题的解和不能作为子问题的解来处理*/
    /*首先是处理局部解的情况,局部解要么是root,要么是L+r要么是R+r*/
    int left_and_r = (INT_MIN == maxLeft) ? INT_MIN : (root->value + maxLeft);  /*不能直接加,如果子树为空直接加值会变大TOT*/
    int right_and_r = (INT_MIN == maxRight) ? INT_MIN : (root->value + maxRight);
    if (root->value >= left_and_r && root->value >= right_and_r)
    {
        /*选根节点作为局部解*/
        ret = root->value;
        queue.insert(root->value);
    }
    else if (left_and_r > root->value && left_and_r >= right_and_r)
    {
        /*选L+r*/
        ret = root->value + maxLeft;
        if (0 == leftQueue.length())
            std::cout << "Bug!!!!!!" << std::endl;

        for (leftQueue.move(0); !leftQueue.end(); leftQueue.next())
            queue.insert(leftQueue.current());

        queue.insert(root->value);
    }
    else
    {
        /*选R+r*/
        ret = root->value + maxRight;
        if (0 == rightQueue.length())
            std::cout << "Bug!" << std::endl;

        for (rightQueue.move(0); !rightQueue.end(); rightQueue.next())
            queue.insert(rightQueue.current());

        queue.insert(root->value);
    }


    /*接着折腾全局解,3种,分别是L R 和 L+r+R*/
    if (leftQueue.length() && rightQueue.length())
    {
        int left_r_right = maxLeft + root->value + maxRight;

        if (left_r_right > maxLeft && left_r_right > maxRight)
        {
            /*用L+r+R作为全局解*/
            if (checkUpdate(left_r_right))
            {
                LinkList<int> tmp;
                for (leftQueue.move(0); !leftQueue.end(); leftQueue.next())
                    tmp.insert(leftQueue.current());

                tmp.insert(root->value);

                for (rightQueue.move(0); !rightQueue.end(); rightQueue.next())
                    tmp.insert(rightQueue.current());

                //update(left_r_right, tmp);
            }
        }
        else if (maxRight > maxLeft)
        {
            /*用右子树*/
            update(maxRight, rightQueue);
        }
        else
        {
            /*用左子树*/
            update(maxLeft, leftQueue);
        }
    }
    else if (leftQueue.length())
    {
        /*有左子树直接用就好了*/
        update(maxLeft, leftQueue);
    }
    else
    {
        update(maxRight, rightQueue);
    }

    return ret;
}

int BTreeMaxPathSum(BTreeNode<int> *root)
{
    LinkList<int> queue;

    int sum = maxPathSum(root, queue);
    int count = 0;

    /*最后还要将局部解与全局解比较一波*/
    update(sum, queue);

    std::cout << "max path sum = " << g_max_sum << std::endl;
    std::cout << "queue length = " << solution.length() << std::endl;
    for (solution.move(0); !solution.end(); solution.next(), count++)
    {
        if (count)
            std::cout << "->";
        std::cout << solution.current();
    }

    std::cout << std::endl;

    /*TODO:封装到一个class里边*/

    return g_max_sum;
}

#define TEST_CASE (2)

void set_tree(BTree<int> &tree)
{

#if (0 == TEST_CASE)

    BTreeNode<int> *node = nullptr;
    tree.insert(1, NULL);

    node = tree.root();
    tree.insert(2, node, LEFT);
    tree.insert(3, node, RIGHT);
#elif (1 == TEST_CASE)
    /*test case 1*/
    BTreeNode<int> *node = nullptr;
    tree.insert(10, NULL);

    node = tree.root();
    tree.insert(2, node, LEFT);
    tree.insert(9, node, RIGHT);

    node = tree.find(2);
    tree.insert(20, node, LEFT);
    tree.insert(1, node, RIGHT);

    node = tree.find(9);
    tree.insert(-25, node, RIGHT);

    node = tree.find(-25);
    tree.insert(3, node, LEFT);
    tree.insert(4, node, RIGHT);
    //tree.insert(9, l1, LEFT);
#elif (2 == TEST_CASE)

    BTreeNode<int> *node = nullptr;
    tree.insert(-10, NULL);

    node = tree.root();
    tree.insert(9, node, LEFT);
    tree.insert(20, node, RIGHT);

    node = tree.find(20);
    tree.insert(15, node, LEFT);
    tree.insert(7, node, RIGHT);
#elif (3 == TEST_CASE)
    BTreeNode<int> *node = nullptr;
    tree.insert(-1, nullptr);

    node = tree.root();
    tree.insert(-2, node, LEFT);
    tree.insert(3, node, RIGHT);

    node = tree.find(-2);
    tree.insert(10, node, RIGHT);

    node = tree.find(10);
    tree.insert(5, node, LEFT);

#elif (4 == TEST_CASE)
    BTreeNode<int> *node = nullptr;
    tree.insert(-1, nullptr);

    node = tree.root();
    tree.insert(1, node, LEFT); /*注释掉这一行只有一个节点的情况结果也OK*/
#elif (5 == TEST_CASE)

    BTreeNode<int> *node = nullptr;
    tree.insert(-1, nullptr);

    node = tree.root();
    tree.insert(5, node, RIGHT);

    node = tree.find(5);
    tree.insert(-2, node, LEFT);
    tree.insert(-3, node, RIGHT);

#else
#error "test case in invalid"
#endif
}

static void test_maxpathsum(void)
{
    BTree<int> tree;

    set_tree(tree);

    BTreeMaxPathSum(tree.root());
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
