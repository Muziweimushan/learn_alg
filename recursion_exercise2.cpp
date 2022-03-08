#include <iostream>

struct Node
{
    int value;
    Node *next;
};

/*问题1:在无头单链表中删除指定值x的节点*/
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

namespace check_exer1
{

void print_list(Node *head)
{
    while (head)
    {
        std::cout << head->value << std::endl;
        Node *toDel = head;
        head = head->next;
        delete toDel, toDel = NULL;
    }
}

void check(void)
{

    Node *head = new Node;
    int c[] = {5, 2, 1, 1, 6, 5, 4, 13, 1, 11, 23, 2, 56};
    //int c[] = {1, 4, -1, 22, 33};
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

    rmdup_iter(head, 1);

    print_list(head);
}

}


static int average(int *a, int n, int idx, int &rem)
{
    if (idx >= n)
        return 0;

    int quo = a[idx] / n;
    rem += a[idx] % n;
    quo += rem / n;
    rem = rem % n;

    return quo + average(a, n, idx + 1, rem);
}

/*
 * 问题2:求n个数的平均等价于求 a0 /n + a1 /n + ... an-1 / n
 * 除法会有余数,将所有的余数加起来再除n就能补回来
 */
int average(int *a, int n, int &rem)
{
    return average(a, n, 0, rem);
}


namespace check_exer2
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

    unsigned int length = rand() % 1000;//20000000;  /*数据量太大递归会栈溢出...*/
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

void recursion_exercise2(void)
{
    int a[] = {2, 4, 6, 7};
    int rem = 0;
    std::cout << average(a, (sizeof(a) / sizeof(*a)), rem) << std::endl;
    std::cout << "rem = " << rem << std::endl;

    check_exer2::check();

    check_exer1::check();
}





