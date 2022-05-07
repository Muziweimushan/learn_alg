#include <iostream>
#include <cstring>

#include "BTreeArray.h"
#include "BTreeNode.h"
#include "MyString.h"
#include "LinkList.h"
#include "BTree.h"
#include "SharedPointer.h"
#include "LinkStack.h"

bool isContained(int a[], int pos, int end, const int x)
{
    if (pos == end)
        return false;

    if (x == a[pos])
        return true;

    return isContained(a, pos + 1, end, x);
}

/*问题1: 判断数组a是否包含数值x*/
bool isContained(int a[], int n, int x)
{
    return isContained(a, 0, n, x);
}

/*判断i与它前面一个之间是否有序*/
bool inOrdered(int a[], int i, bool min2max)
{
    if (i < 1)
        return true;    /*从后向前所有元素两两有序,就是有序的*/

    if (min2max && a[i] < a[i - 1])
        return false;
    else if (!min2max && a[i] > a[i - 1])
        return false;
    else
        return inOrdered(a, i - 1, min2max);

}

/*问题2 : 判断数组a是否已经有序*/
bool isOrdered(int a[], int n, bool min2max = true)
{
    /*数组是有序的,0-1是有序的,1-2是有序的...n-2 到n-1是有序的*/
    return inOrdered(a, n - 1, min2max);
}

int MaxInArray(int a[], int i, int n, int max)
{
    if (i >= n)
        return max;

    return MaxInArray(a, i + 1, n, a[i] > max ? a[i] : max);
}

/*问题3 : 查找数组中最大元素*/
int MaxInArray(int a[], int n)
{
   return MaxInArray(a, 1, n, a[0]);
}

/*问题4 : 求子集*/
/*思路:
 * 对于空集子集就是{}
 * 对于集合{a}来说, 子集就是{}和{a},相当于a加上了空集的子集
 * 对于{a, b}, 子集就是{a} {b} {a, b} {}, 相当于b分别加上集合a的子集
 * 对于个数3个的也是类似的
 * 对于个数n个的集合,它的子集就是set[n-1] 依次加上 前n-1个的集合的子集(或者说由第0个依次加上后面n-1个组成的子集)
 * 注意这里的str需要先去重
 * */
void subst(MyLib::String &str, MyLib::LinkList<MyLib::String> &res)
{
    if (0 == str.length())
    {
        res.insert("");
        return;
    }

    /*拿出来第一个*/
    MyLib::String head = str[0];
    subst(str.remove(0, 1), res);   /*求剩下的n-1个集合的所有子集*/

    MyLib::LinkList<MyLib::String> tmp;
    for (res.move(0); !res.end(); res.next())
        tmp.insert(0, head + res.current());

    for (tmp.move(0); !tmp.end(); tmp.next())
        res.insert(0, tmp.current());
}

static void print_subst(MyLib::String str)
{
    std::cout << "{";
    for (int i = 0; i < str.length(); i++)
            std::cout << ((0 != i) ? "," : "") << str[i];

    std::cout << "}" << std::endl;
}

typedef struct ListHead
{
    int value;
    struct ListHead *next;
}ListHead;

void duplicate(ListHead *list)
{
    /*思路: 先删除与头节点相同的节点,接着以下一个元素为头节点递归执行*/
    if (!list->next)
        return;     /*没有下一个,递归结束*/

    ListHead *cur = list;

    ListHead *toCheck = list->next;
    ListHead *prev = list;
    while (toCheck)
    {
        if (cur->value == toCheck->value)
        {
            ListHead *toDel = toCheck;
            /*需要删除*/
            prev->next = toCheck->next;
            toCheck = toCheck->next;
            delete toDel, toDel = NULL;
            //std::cout << "delete ..." << std::endl;
        }
        else
        {
            toCheck = toCheck->next;
            prev = prev->next;
        }
    }
    /*开启下一轮递归*/
    duplicate(cur->next);
}

/*问题5:删除无序链表中的重复节点*/
void list_duplicate(ListHead *list)
{
    duplicate(list);
}

void print_and_delete_list(ListHead *head)
{
    while (head)
    {
        std::cout << head->value << std::endl;
        ListHead *toDel = head;
        head = head->next;
        delete toDel, toDel = NULL;
    }
}


struct Node
{
    int value;
    Node *next;
};

/*问题6:在无头单链表中删除指定值x的节点*/
void rmdup(Node *&head, int v)
{
    if (nullptr == head)
        return;

    if (v == head->value)
    {
        /*需要删除*/
        Node *toDel = head;
        head = head->next;
        delete toDel, toDel = nullptr;
        rmdup(head, v);
    }
    else
    {
        rmdup(head->next, v);
    }
}

void rmdup_iter(Node *&head, int v)
{
    /*搞个假的头节点*/
    Node fake_head;

    fake_head.next = head;

    Node *cur = &fake_head;     /*当前处理节点初始化为假的头节点*/
    Node *next = fake_head.next;    /*检查的其实是next节点,初始化为head*/

    /*以cur的next为循环条件*/
    while (next)
    {
        if (v != next->value)
        {
            /*不相等不需要删除,同时后移*/
            cur = cur->next;
            next = next->next;
            continue;
        }

        /*要删除的节点是next,删除前需要构建cur与删除节点的next指针的关系,即 cur->next = next->next*/
        Node *toDel = next;
        cur->next = next->next;
        next = next->next;

        delete toDel, toDel = nullptr;
    }

    /*最后将head拿回来返回就OK,如果head不是引用的话就需要在这里返回fake_head.next作为删除后的链表首节点*/
    head = fake_head.next;
}

namespace check_exer6
{

void print_list(Node *head, bool need_free = true)
{
    while (head)
    {
        std::cout << head->value << "  ";
        Node *toDel = head;
        head = head->next;
        if (need_free)
            delete toDel, toDel = NULL;
    }

    std::cout << std::endl;
}

void check(void)
{

    Node *head = new Node;
    //int c[] = {5, 2, 1, 1, 6, 5, 4, 13, 1, 11, 23, 2, 56};
    int c[] = {1, 4, -1, 22, 33, -2, 5, 8, 2, 2, 2, 4, 5, 6, 6, 6, 77, 8,-1};
    //int c[] = {1};
    Node* cur = head;
    head->next = NULL;
    head->value = c[0];
    int len = sizeof(c) / sizeof(*c);
    for (int i = 1; i < len; i++)
    {
        Node *item = new Node;
        item->next = NULL;
        item->value = c[i];
        cur->next = item;
        cur = cur->next;
    }

    int todel = 6;
    std::cout << "we are goint to remove elem " << todel << " in list ..." << std::endl;
    std::cout << "origin list :" << std::endl;
    print_list(head, false);

    //rmdup_iter(head, todel);
    rmdup(head, todel);
    std::cout << "after remove : list = " << std::endl;
    print_list(head);
}

}


static int average(int *a, int n, int idx, int &rem)
{
    if (idx >= n)
        return 0;

    int quo = a[idx] / n;   /*quotient,商*/
    rem += a[idx] % n;      /*remainder,余数*/
    quo += rem / n;
    rem = rem % n;

    return quo + average(a, n, idx + 1, rem);
}

/*
 * 问题7:求n个数的平均等价于求 a0 /n + a1 /n + ... an-1 / n
 * 除法会有余数,将所有的余数加起来再除n就能补回来
 *
 * PS:这里的入参rem是不需要的,只是为了测试用而已
 */
int average(int *a, int n, int &rem)
{
    //int rem = 0
    return average(a, n, 0, rem = 0);
}


namespace check_exer7
{
#include <time.h>
#include <cstdlib>

static inline uint64_t get_sum(int *a, unsigned int length)
{
    uint64_t sum = 0;

    for (unsigned int i = 0; i < length; i++)
        sum += a[i];

    return sum;
}

void check(void)
{
    ::srand(::time(NULL));

    unsigned int length = rand() % 100000;//20000000;  /*数据量太大递归会栈溢出...*/
    int *a = new int[length];
    if (nullptr == a)
    {
        std::cout << "no memory ..." << std::endl;
        return;
    }

    for (uint32_t i = 0; i < length; i++)
        a[i] = rand();

    uint64_t sum = get_sum(a, length);

    int rem = 0;
    int avg = average(a, length, rem);
    std::cout << "average = " << avg  << std::endl;

    uint64_t check_sum = (uint64_t)avg * length + rem;

    if (sum != check_sum)
        std::cout << "check failed ..." << std::endl;
    else
        std::cout << "check success ..." << std::endl;

    delete[] a;
}
}

namespace check_exer8
{
/*课堂演示:链表翻转的三指针解法和递归解法*/

class WorkList : public MyLib::LinkList<int>
{
public:
    using Node = MyLib::LinkList<int>::Node;

    /*链表翻转的三指针常规解法*/
    void reverseClassic(void)
    {
        Node *head = m_header.next;
        Node *pre = nullptr;
        Node *next = (nullptr != head) ? head->next : nullptr;

        while (head)
        {
            head->next = pre;
            pre = head;
            head = next;
            next = next ? next->next : nullptr; /*next指针可能为空*/
            //next->next = head;
        }

        m_header.next = pre;

        std::cout << "done!" << std::endl;
    }
    
    /*递归解法*/
    void reverseRecursive(void)
    {
        m_header.next = reverse(m_header.next);
    }

protected:
    /*递归解法的功能函数,实现递归翻转链表,约定返回翻转后的链表头*/
    Node *reverse(Node *head)
    {
        Node *ret = nullptr;

        if (head)
        {
            /*链表翻转的问题分解为将当前头节点取出来,递归地对剩余链表进行翻转后与头节点进行拼接即可*/

            Node *next = head->next; /*记录下剩余链表在翻转前的头节点才能进行拼接,next也就是翻转后链表的末元素*/
            Node *r = reverse(head->next);

            if (r)
            {
                /*常规情况,将翻转后的链表与当前头节点进行拼接*/
                ret = r;
                next->next = head;  /*将头节点插入到翻转后链表的尾部即完成原问题的解*/
            }
            else
            {
                /*此处为平凡情况,即剩余链表个数为空,这时候head本身就应当是函数的返回值*/
                ret = head;
            }
            /*翻转后head就是链表最后一个元素,next指针需要赋值为空*/
            head->next = nullptr;
        }
        /*如果head为空显然就返回空,符合语义*/
        return ret;
    }
};

void check(void)
{
    WorkList list;

    for (int i = 0; i < 5; i++)
        list.insert(i);

    list.reverseClassic();
    list.reverseRecursive();

    for (list.move(0); !list.end(); list.next())
        std::cout << list.current() << std::endl;
}

}

namespace check_exer9
{
/*
 * 问题描述:对于给定数值sum,查找二叉树中是否存在一条从根节点到叶节点的路径,路径之和等于x
 * 思路:只有一个根节点的情况,直接判断,存在左右孩子时,判断左/右子树的路径和是否等于(sum - 根节点)
 * 这里有个技巧:在递归中如果需要记录中间结果可以使用栈数据结构完成
 */

bool BTreeSumCal(MyLib::BTreeNode<int> *root, int sum, MyLib::Stack<int> &stack)
{
    bool ret = false;

    if (nullptr == root)
        return false;

    if (!root->left && !root->right && sum == root->value)
    {
        stack.push(sum);
        ret = true;
    }
    else
    {
        /*递归进入左右子树查找*/
        if ((ret = BTreeSumCal(root->left, (sum - root->value), stack)))
            stack.push(root->value);
        else if ((ret = BTreeSumCal(root->right, (sum - root->value), stack)))
            stack.push(root->value);
    }

    return ret;
}

MyLib::SharedPointer<MyLib::Stack<int>> BTreeSum(MyLib::BTree<int> &tree, int sum)
{
    MyLib::LinkStack<int> *ret = new MyLib::LinkStack<int>;

    if (!BTreeSumCal(tree.root(), sum, *ret))
        delete ret, ret = nullptr;

    return ret;
}

void check(void)
{
    MyLib::BTree<int> tree;
    MyLib::BTreeNode<int> *node = nullptr;

    tree.insert(1, nullptr);
    node = tree.root();

    tree.insert(2, node, MyLib::LEFT);
    tree.insert(3, node, MyLib::RIGHT);

    node = tree.find(2);
    tree.insert(-1, node);

    MyLib::SharedPointer<MyLib::Stack<int>> ret = BTreeSum(tree, 1);
    if (!ret.isNull())
    {
        MyLib::Stack<int> &stack = *ret.get();
        while (stack.size() > 0)
        {
            std::cout << stack.top() << std::endl;
            stack.pop();
        }
    }
    else
    {
        std::cout << "BTree sum not exist ..." << std::endl;
    }
}

}

namespace check_exer10
{
bool isPostString(MyLib::BTreeNode<char> *node, MyLib::String &s, int &pos)
{
    bool ret = true;

    if (node)
    {
        return ((isPostString(node->left, s, pos)) && (isPostString(node->right, s, pos)) && (s[pos++] == node->value));
    }

    return ret;
}

bool isPostString(MyLib::BTree<char> &bt, MyLib::String &s)
{
    int pos = 0;
    return isPostString(bt.root(), s, pos);
}

void check(void)
{
    MyLib::String s = {"abcde"};
    MyLib::BTree<char> tree;
    MyLib::BTreeNode<char> *node = nullptr;

    tree.insert('e', nullptr);
    node = tree.root();

    tree.insert('a', node, MyLib::LEFT);
    tree.insert('d', node, MyLib::RIGHT);

    node = tree.find('d');
    tree.insert('b', node);
    tree.insert('c', node);

    std::cout << isPostString(tree, s) << std::endl;
}
}

void recursion_exercise1(void)
{

    std::cout << "Run exercise 1 ..." << std::endl;

    int a[] = {1, 23, 44, 21, 2, 5};

    std::cout << "is contained : " << isContained(a, (sizeof(a) / sizeof(*a)), 5) << std::endl;

    std::cout << "End exercise 1 ..." << std::endl << std::endl << std::endl << std::endl << std::endl;



    std::cout << "Run exercise 2 ..." << std::endl;

    //int b[] = {33, 29, 10, 8, -1, -13141};
    //int b[] = {1, 29, 33, 102, 1111, 9999};
    int b[] = {5, 4, 3};
    //int b[] = {33, 29, 10, 8, -1, -13141};
    //std::cout << "is ordered : " << isOrdered(b, (sizeof(b) / sizeof(*b))) << std::endl;
    std::cout << "is ordered : " << isOrdered(b, (sizeof(b) / sizeof(*b)), false) << std::endl;

    std::cout << "End exercise 2 ..." << std::endl << std::endl << std::endl << std::endl << std::endl;

    std::cout << "Run exercise 3 ..." << std::endl;

    std::cout << "MaxInArray : " << MaxInArray(b, (sizeof(b) / sizeof(*b))) << std::endl;

    std::cout << "End exercise 3 ..." << std::endl << std::endl << std::endl << std::endl << std::endl;


    std::cout << "Run exercise 4 ..." << std::endl;
    MyLib::LinkList<MyLib::String> res;
    MyLib::String str = "abcehg";
    /*TODO:要对str先做去重*/
    subst(str, res);

    for (res.move(0); !res.end(); res.next())
        print_subst(res.current());

    std::cout << "subst length = " << res.length() << std::endl;


    std::cout << "End exercise 4 ..." << std::endl << std::endl << std::endl << std::endl << std::endl;

    ListHead *head = new ListHead;
    int c[] = {5, 2, 1, 1, 6, 5, 4, 13, 1, 11, 23, 2, 56};
    //int c[] = {1, 4, -1, 22, 33};
    //int c[] = {1};
    ListHead *cur = head;
    head->next = NULL;
    head->value = c[0];
    int len = sizeof(c) / sizeof(*c);
    for (int i = 1; i < len; i++)
    {
        ListHead *item = new ListHead;
        item->next = NULL;
        item->value = c[i];
        cur->next = item;
        cur = cur->next;
    }

    list_duplicate(head);

    print_and_delete_list(head);

    std::cout << "End exercise 5 ..." << std::endl << std::endl << std::endl << std::endl << std::endl;

    std::cout << "**********************Run exercise 6 ************" << std::endl;

    check_exer6::check();
    std::cout << "**********************End exercise 6 ************" << std::endl << std::endl << std::endl << std::endl << std::endl;



    std::cout << "**********************Run exercise 7 ************" << std::endl;

    check_exer7::check();
    std::cout << "**********************End exercise 7 ************" << std::endl << std::endl << std::endl << std::endl << std::endl;


    check_exer8::check();

    check_exer9::check();

    check_exer10::check();
}
