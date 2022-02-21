#include <iostream>
#include <climits>

#include "MatrixGraph.h"
#include "ListGraph.h"
#include "SharedPointer.h"
#include "StaticArray.h"

namespace Exercise_1
{

/* 问题1: 判断一幅图是否是二分图
 * 二分图定义 : 将图中顶点分为两部分,任意一条边的一个顶点属于集合A,另一个顶点属于集合B
 */

enum { VERTEX_CNT = 4 };
void set_input(MyLib::ListGraph<int, int> &graph)
{
    for (int i = 0; i < VERTEX_CNT; ++i)
        graph.addVertex(i);
    
    graph.setEdge(0, 1, 1);
    graph.setEdge(1, 0, 1);

    graph.setEdge(0, 2, 1);
    graph.setEdge(2, 0, 1);

    graph.setEdge(2, 3, 1);
    graph.setEdge(3, 2, 1);

    graph.setEdge(1, 3, 1);
    graph.setEdge(3, 1, 1);

    //graph.setEdge(0, 3, 1);
    //graph.setEdge(3, 0, 1);

}

bool solution(void)
{
    MyLib::ListGraph<int, int> graph;
    
    MyLib::StaticArray<bool, VERTEX_CNT> setA;
    MyLib::StaticArray<bool, VERTEX_CNT> setB;
    bool OK = true;

    set_input(graph);

    for (int i = 0; i < VERTEX_CNT; ++i)
    {
        setA[i] = false;
        setB[i] = false;
    }

    for (int i = 0; OK && i < graph.vCount(); ++i)
    {
        /*看一下当前顶点是否已经被划分集合,都没被划分的话就扔给集合A*/
        bool isA = false;

        if (!setB[i])   /*简化成只要不在集合B就往A里边扔*/
        {
            setA[i] = true;
            isA = true;
        }

        MyLib::SharedPointer< MyLib::Array<int> > adj = graph.getAdjacent(i);
        /*遍历顶点i的所有邻居*/
        for (int j = 0; j < adj->length(); ++j)
        {
            int neighbour = (*adj)[j];
            /*判断一下,如果当前顶点i在集合A中,那么它的当前处理邻居要么未分配集合要么只能在集合B中,否则就是错的*/
            if (isA)
            {
                if (setA[neighbour])
                {
                    OK = false;
                    break;
                }

                setB[neighbour] = true;
            }
            else
            {
                if (setB[neighbour])
                {
                    OK = false;
                    break;
                }

                setA[neighbour] = true;
            }
        } 
    }

    return OK;
}

}

namespace Exercise_2
{

/*
 * 问题2: 矩阵中的距离
 *  一个矩阵M只由0和1组成,计算一个大小相同的矩阵D
 *  D中元素是M中对应元素离最近的0元素的距离
 *  水平或者垂直方向相邻的两个矩阵元素距离为1
 */
#define ROW_COL_TO_LINEAR(r, c) ((r) * COL + (c))

#define GET_ROW(index) ((index) / COL)
#define GET_COL(index) ((index) % COL)
#define INF (INT_MAX)

enum { ROW = 4 };
enum { COL = 4 };

int map[ROW][COL] = 
{
    {1, 0, 0, 1},
    {1, 1, 1, 1},
    {1, 1, 0, 0},
    {1, 1, 1, 1}
};

/*模仿数据结构课里边八皇后的棋盘访问方式*/
/*0-3分别对应上下左右*/
const int row_step[4] = {-1, 1, 0, 0};
const int col_step[4] = {0, 0, -1, 1};

inline bool pos_invalid(int r, int c)
{
    return (((0 <= r) && (r < ROW)) && ((0 <= c) && (c < COL)));
}

void print_rst(int *dist)
{
    for (int r = 0; r < ROW; r++)
    {
        int *dist_r = &dist[ROW_COL_TO_LINEAR(r, 0)];
        for (int c = 0; c < COL; c++)
            std::cout << dist_r[c] << "\t";

        std::cout << std::endl;
    }
}

void solution(void)
{
    MyLib::LinkQueue<int> queue;
    MyLib::StaticArray<bool, ROW * COL> discovered;

    /*思路:分别从各个值为0的顶点出发,其前后左右4个方向的顶点如果值为0,则距离为1,再分别从这4个当中值不为0的顶点出发,依次类推,迭代到4个方向的顶点值都不为0就可以返回了*/
    /*整个算法类似于BFS,或者说二叉树的层次遍历更形象一点,从距离为0的点出发,一层一层向下走,layer[i] = layer[i - 1]*/

    int dist[ROW * COL];

    for (int r = 0; r < ROW; r++)
    {
        for (int c = 0; c < COL; c++)
        {
            int index = ROW_COL_TO_LINEAR(r, c);
            if (0 == map[r][c])
            {
                queue.add(index);
                dist[index] = 0;
                discovered[index] = true;   /*标记为已发现*/
            }
            else
            {
                /*初始化阶段值不为0的顶点距离初始化为无限大*/
                dist[index] = INF;
                discovered[index] = false;
            }
        }
    }

    while (queue.length())
    {
        int index = queue.front();
        queue.remove();

        int row = GET_ROW(index);
        int col = GET_COL(index);
        /*index为已经处理过的顶点,它的距离为已知值,以index为出发点,访问其4个方向的顶点,如果当中存在未发现的顶点,则这个顶点的距离值应当为dist[index] + 1*/
        for (int i = 0; i < 4; i++)
        {
            int nr = row + row_step[i];
            int nc = col + col_step[i];

            if (!pos_invalid(nr, nc))
                continue;
    
            int next_idx = ROW_COL_TO_LINEAR(nr, nc);
            if (!discovered[next_idx])
            {
                /*发现一个可计算距离的顶点*/
                /*新发现顶点可作为后续某次迭代的顶点,将其入队列*/
                queue.add(next_idx);

                /*标记为已发现*/
                discovered[next_idx] = true;
                /*新发现顶点的距离应当为 dist[index] + 1*/
                dist[next_idx] = dist[index] + 1;
            }

        } 
    }

    print_rst(dist);
}

}


void graph_practice(void)
{
    std::cout << Exercise_1::solution() << std::endl;

    std::cout << std::endl << std::endl;
    //
    Exercise_2::solution();

}
