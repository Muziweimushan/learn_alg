#include <iostream>

#include "MatrixGraph.h"
#include "DualEndQueue.h"
#include "MyString.h"

constexpr int ROW = 4;
constexpr int COL = 5;

int map[ROW][COL] = 
{
    {1, 2, 1, 0, 1},
    {1, 0, 0, 1, 0},
    {1, 1, 0, 1, 0},
    {0, 0, 10, 0, 7}
};

static void checkAdjacent(int row, int col, MyLib::MatrixGraph<ROW * COL, int, int> &graph)
{
    /*检查当前点与上下左右4个点是否可以构成一条联边*/
    /*我们此处假设当前点(i, j)的值不为0*/
    if (col - 1 >= 0 && map[row][col - 1])
        graph.setEdge(row * COL + col, row * COL + col - 1, 1);

    if (col + 1 < COL && map[row][col + 1])
        graph.setEdge(row * COL + col, row * COL + col + 1, 1);

    if (row - 1 >= 0 && map[row - 1][col])
        graph.setEdge(row * COL + col, (row - 1) * COL + col, 1);

    if (row + 1 < ROW && map[row + 1][col])
        graph.setEdge(row * COL + col, (row + 1) * COL + col, 1);
}

void test1(void)
{
    /*地图中每一个点看作是图的一个顶点*/
    MyLib::MatrixGraph<ROW * COL, int, int> graph;
    for (int i = 0; i < ROW; ++i)
        for (int j = 0; j < COL; ++j)
            graph.setVertex(i * COL + j, map[i][j]); 

    for (int i = 0; i < ROW; ++i)
    {
        for (int j = 0; j < COL; ++j)
        {
            if (map[i][j]) 
                checkAdjacent(i, j, graph);   
        }
    }

    std::cout << "edge count = " << graph.eCount() << std::endl;

    MyLib::SharedPointer<MyLib::Array<int>> result;
    int max = 0;
    for (int i = 0; i < ROW * COL; ++i)
    {
        if (map[i / COL][i % COL])
        {
            int sum = 0;
            MyLib::SharedPointer<MyLib::Array<int>> arr = graph.BFS(i);
            /*本次图遍历经过的点在之后的迭代过程中无需再以此作为起始点,进一步减少运算次数*/
            for (int j = 0; j < arr->length(); j++)
            {
                int index = (*arr)[j];
                int row = index / COL, col = index % COL;
                sum += map[row][col];
                map[row][col] = 0;
            }

            /*与历史值比较,并更新最大值,如果未有历史值,则以当前值为最大*/
            //if (result.isNull() || arr->length() > result->length())
            if (result.isNull() || sum > max)
            {
                max = sum;
                result = arr;
            }

            /*同理,将本次起始点也清掉*/
            map[i / COL][i % COL] = 0;

            //std::cout << "this round : " << arr->length() << std::endl;
            std::cout << "this round : " << sum << std::endl;
        }
    }

    //std::cout << "result : " << result->length() << std::endl; 
    std::cout << "result : " << max << std::endl; 
    for (int i = 0; i < result->length(); i++)
    {
        int index = (*result)[i];
        int row = index / COL, col = index % COL;
        std::cout << "(" << row << ", " << col << ")" << std::endl;
    }
}

static void test_deque(void)
{
    std::cout << "start testing deque ..." << std::endl;

    MyLib::DualEndQueue<int> deque;
    MyLib::DualEndQueue<int> deque1;

    for(int i = 0; i < 10; ++i)
    {
        if (i & 1)
        {
            deque.push(i);
            deque1.push(i);
        }
        else
        {
            deque.add(i);
            deque1.add(i);
        }
    }
    
    std::cout << "printing deque value : " << std::endl;
    while (deque.length() > 0)
    {
        std::cout << deque.front() << std::endl;
        deque.remove();
    }

    std::cout << "printing deque1 value : " << std::endl;
    while (deque1.length() > 0)
    {
        std::cout << deque1.back() << std::endl;
        deque1.dismiss();
    } 
}

bool isSym(int n)
{
    MyLib::DualEndQueue<int> deque;

    if (n < 0)
        n = -n;

    while (n > 0)
    {
        deque.add(n % 10);
        n = n / 10;
    }

    while (deque.length() > 1 && deque.front() == deque.back())
    {
        deque.remove();
        deque.dismiss();
    }

    return deque.length() < 2;
}

bool isSym(const MyLib::String &s)
{
    MyLib::DualEndQueue<char> deque;
    int del = 0;
    char front = 0;     /*用于记录被拿走的字符,并且从队列头拿走的*/
    char back = 0;      /*用于记录被拿走的字符,并且从队列尾拿走的*/

    for (int i = 0; i < s.length(); i++)
        deque.add(s[i]);

    while (deque.length() > 1)
    {
        if (deque.front() == deque.back())
        {
            deque.remove();
            deque.dismiss();

            if (front || back) del++;
        }
        else if (!del && 0 == front && 0 == back)
        {
            front = deque.front();
            deque.remove();
            //del = 1;
        }
        else if (!del && front && !back)
        {
            /*进入此分支的情况是上一次迭代判断头尾不相等,然后把队列头元素拿掉了,再次迭代比较发现还是不相等*/
           /*此时应当将拿掉的头元素放回队列,然后拿掉队尾元素再次进行判断*/
            deque.add(front);
            back = deque.back();
            deque.dismiss();
        }
        else
        {
            /*不管去掉队头还是队尾元素后还是发现了不对称的字符,迭代可以结束了*/
            break;
        }
    }

    return deque.length() < 2;
}

static void test2(void)
{
    /*题目:给定一个整数,判断其是否是对称整数,例子 1 4 7 11 22 343都是对称整数*/

    std::cout << isSym(232) << std::endl;
    /*题目2:给定一个字符串,判断在最多允许删除一个字符的前提下,字符串是否是回文串*/
    std::cout << isSym("acda") << std::endl;
}

int main(void)
{
    //test1();
    //test_deque();
    //
    test2();

    return 0;
}
