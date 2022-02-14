#include <iostream>

#include "MatrixGraph.h"


constexpr int ROW = 4;
constexpr int COL = 5;

int map[ROW][COL] = 
{
    {1, 1, 0, 0, 1},
    {1, 0, 0, 1, 0},
    {1, 1, 0, 1, 0},
    {0, 0, 1, 0, 0}
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

    for (int i = 0; i < ROW * COL; ++i)
    {
        if (map[i / COL][i % COL])
        {
            MyLib::SharedPointer<MyLib::Array<int>> arr = graph.BFS(i);
            /*本次图遍历经过的点在之后的迭代过程中无需再以此作为起始点,进一步减少运算次数*/
            for (int j = 0; j < arr->length(); j++)
            {
                int index = (*arr)[j];
                int row = index / COL, col = index % COL;
                map[row][col] = 0;
            }

            /*与历史值比较,并更新最大值,如果未有历史值,则以当前值为最大*/
            if (result.isNull() || arr->length() > result->length())
                result = arr;

            /*同理,将本次起始点也清掉*/
            map[i / COL][i % COL] = 0;

            std::cout << "this round : " << arr->length() << std::endl;
        }
    }

    std::cout << "result : " << result->length() << std::endl; 
    for (int i = 0; i < result->length(); i++)
    {
        int index = (*result)[i];
        int row = index / COL, col = index % COL;
        std::cout << "(" << row << ", " << col << ")" << std::endl;
    }
}


int main(void)
{
    test1();

    return 0;
}
