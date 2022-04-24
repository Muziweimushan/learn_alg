#include <iostream>
#include "BTreeNode.h"
#include "MatrixGraph.h"
#include "StaticHeap.h"
#include "DynamicHeap.h"

using namespace std;
using namespace MyLib;

template < typename T >
BTreeNode<T>* createTree()
{
    static BTreeNode<int> ns[9];

    for(int i=0; i<9; i++)
    {
        ns[i].value = i;
        ns[i].parent = NULL;
        ns[i].left = NULL;
        ns[i].right = NULL;
    }

    ns[0].left = &ns[1];
    ns[0].right = &ns[2];
    ns[1].parent = &ns[0];
    ns[2].parent = &ns[0];

    ns[1].left = &ns[3];
    ns[1].right = NULL;
    ns[3].parent = &ns[1];

    ns[2].left = &ns[4];
    ns[2].right = &ns[5];
    ns[4].parent = &ns[2];
    ns[5].parent = &ns[2];

    ns[3].left = NULL;
    ns[3].right = &ns[6];
    ns[6].parent = &ns[3];

    ns[4].left = &ns[7];
    ns[4].right = NULL;
    ns[7].parent = &ns[4];

    ns[5].left = &ns[8];
    ns[5].right = NULL;
    ns[8].parent = &ns[5];

    return ns;
}

template < typename T >
void printInOrder(BTreeNode<T>* node)
{
    if( node != NULL )
    {
        printInOrder(node->left);

        cout << node->value <<" ";

        printInOrder(node->right);
    }
}

//删除树中度为1的结点,树中结点包含指向父结点的指针
template < typename T >
BTreeNode<T> *delOdd1(BTreeNode<T> *node)
{
	BTreeNode<T> *ret = NULL;

	//node为空也就是空树,直接返回就好
	if (NULL != node)
	{

		//度为1的结点,就是要删除的结点
		//删除的步骤是:
		//1.node的父结点的子结点指针指向node的子结点
		//2.node子结点的父指针指向node的父结点
		//执行完上面两步实际上node结点已经不在树中了,可以删除掉node结点
		//3.对node子结点递归调用delOdd1进行删除

		//对于二叉树来说,度为1也就是一个子树为空另一个子树不为空
		if (((NULL != node->left) && (NULL == node->right)) 
				|| ((NULL == node->left) && (NULL != node->right)))
		{
			//node结点的孩子结点
			BTreeNode<T> *child_node = (NULL == node->left) ? node->right : node->left;
			BTreeNode<T> *parent = dynamic_cast<BTreeNode<T> *>(node->parent);//父结点
			//先来判断一下node结点是否为根结点,根结点的父指针为空
			if (NULL != parent)
			{
				//将父结点的孩子指针指向child_node
				//父结点的孩子指针可以是左子树指针也可能是右子树指针
				//因此需要找到到底是哪一棵子树
				//很显然目标孩子指针实际上就是node,因此比较一下值就可以找到了
				//通过引用就可以直接修改对应的父指针的值
				BTreeNode<T> *&parent_child = (node == parent->left) ? parent->left : parent->right;

				parent_child = child_node;
				//孩子结点父指针指向parent
				child_node->parent = parent;
			}
			else
			{
				child_node->parent = NULL;
			}

			if (node->flag())
			{
				delete node;
			}

			//递归对node的子结点进行删除操作
			//递归一直找到度不为1的结点才返回,因此返回值等于delOdd1的返回值
			ret = delOdd1(child_node);
		}
		else
		{
			delOdd1(node->left);
			delOdd1(node->right);

			//这个分支意味着node不需要被删除,因此返回值就是node
			ret = node;
		}

	}
	
	return ret;
}

//删除树中度为1的结点,树中结点只包含孩子结点指针
template < typename T >
void delOdd2(BTreeNode<T> *&node)
{
	if (NULL != node)
	{
		//判断是否是要删除的结点,也就是一棵子树为空另一棵不为空的结点
		if (((NULL == node->left) && (NULL != node->right)) 
			|| ((NULL != node->left) && (NULL == node->right)))
		{
			//找到node结点的孩子结点
			BTreeNode<T> *child_node = (NULL == node->left) ? node->right : node->left;


			//释放node的内存空间
			if (node->flag())
			{
				delete node;
			}

			//因为node是个指针的引用,所以这里就是修改了对应指针的值,让它指向了node的孩子
			//也就是说,node已经从树中删除掉了,这里BTreeNode<T> *的引用可以看做是二重指针,
			//将node的父结点的孩子指针值修改为了node的孩子指针的值,即使得node的父结点指向了node的孩子结点
			
			node = child_node;
			//经过了上一步之后,node实际上已经指向了node的孩子了
			//还是一样的递归删除
			delOdd2(node);
		}
		else
		{
			//不需要删除的结点
			delOdd2(node->left);
			delOdd2(node->right);
		}
	}
}

template < typename T >
void printDualList(BTreeNode<T>* node)
{
    BTreeNode<T>* g = node;

    cout << "head -> tail: " << endl;

    while( node != NULL )
    {
        cout << node->value << " ";

        g = node;

        node = node->right;
    }

    cout << endl;

    cout << "tail -> head: " << endl;

    while( g != NULL )
    {
        cout << g->value << " ";

        g = g->left;
    }

    cout << endl;
}

#include "StaticArray.h"

#include "RangeArray.h"

static void my_test1(const MyLib::RangeArray<int> &ra)
{
    //ra[2] = 22;
    (void)ra;
}

#include "ListGraph.h"

static void test_graph(void)
{
    cout << "GraphDemo(): " << endl;

    ListGraph<char, float> graph;

    // construct graph
    graph.addVertex('A');  // 0
    graph.addVertex('B');  // 1
    graph.addVertex('C');  // 2 
    graph.addVertex('D');  // 3
    graph.addVertex('E');  // 4

    graph.setEdge(0, 1, 0.2);  // v0->v1, weight: 0.2
    graph.setEdge(0, 4, 1.0);  // v0->v4, weight: 1.0
    graph.setEdge(1, 2, 0.3);  // v1->v2, weight: 0.3
    graph.setEdge(1, 4, 0.7);  // v1->v4, weight: 0.7
    graph.setEdge(2, 0, 0.4);  // v2->v0, weight: 0.4
    graph.setEdge(2, 3, 0.4);  // v2->v3, weight: 0.4
    graph.setEdge(2, 4, 0.3);  // v2->v4, weight: 0.3
    graph.setEdge(3, 4, 0.5);  // v3->v4, weight: 0.5
    graph.setEdge(4, 2, 0.2);  // v4->v2, weight: 0.2

    SharedPointer< Array<int> > r = graph.dijkstra(0, 4, 0xFFFF);
    double d = 0;

    cout << graph.getVertex((*r)[0]);

    for(int i=1; i<r->length(); i++)
    {
        d += graph.getEdge((*r)[i-1], (*r)[i]);
        cout << " -> " << graph.getVertex((*r)[i]);
    }

    cout << endl;
    cout << "distance: " << d << endl;
}

void MatrixGraphDemo()
{
    cout << "MatrixGraphDemo(): " << endl;

    //ListGraph<char, float> graph;
    MatrixGraph<5, char, float> graph;

    // construct graph
    //graph.addVertex('A');  // 0
    //graph.addVertex('B');  // 1
    //graph.addVertex('C');  // 2 
    //graph.addVertex('D');  // 3
    //graph.addVertex('E');  // 4
    //
    graph.setVertex(0, 'A');
    graph.setVertex(1, 'B');
    graph.setVertex(2, 'C');
    graph.setVertex(3, 'D');
    graph.setVertex(4, 'E');

    graph.setEdge(0, 1, 0.2);  // v0->v1, weight: 0.2
    graph.setEdge(0, 4, 1.0);  // v0->v4, weight: 1.0
    graph.setEdge(1, 2, 0.3);  // v1->v2, weight: 0.3
    graph.setEdge(1, 4, 0.7);  // v1->v4, weight: 0.7
    graph.setEdge(2, 0, 0.4);  // v2->v0, weight: 0.4
    graph.setEdge(2, 3, 0.4);  // v2->v3, weight: 0.4
    graph.setEdge(2, 4, 0.3);  // v2->v4, weight: 0.3
    graph.setEdge(3, 4, 0.5);  // v3->v4, weight: 0.5
    graph.setEdge(4, 2, 0.2);  // v4->v2, weight: 0.2

    SharedPointer< Array<int> > r = graph.dijkstra(0, 4, 0xFFFF);
    double d = 0;

    cout << graph.getVertex((*r)[0]);

    for(int i=1; i<r->length(); i++)
    {
        d += graph.getEdge((*r)[i-1], (*r)[i]);
        cout << " -> " << graph.getVertex((*r)[i]);
    }

    cout << endl;
    cout << "distance: " << d << endl;
}

extern void graph_practice(void);
extern void testBTreeArray(void);
extern void test_heap(void);

extern void recursion_exercise1(void);
extern void recursion_exercise2(void);
extern void recursion_exercise3(void);

namespace TestStaticHeap
{
    void check(void)
    {
        MyLib::DynamicHeap<int> heap(10);

        int a[] = {2, 4, 1, 3, 5};

        for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
            heap.add(a[i]);

        while (heap.length())
        {
            std::cout << heap.front() << std::endl;
            heap.remove();
        }

    }
}

int main()
{
    //graph_practice();
    //testBTreeArray();
    //test_heap();

    TestStaticHeap::check(); 
    //
    //recursion_exercise1();
    //recursion_exercise2();
    //recursion_exercise3();
#if 0
    try
    {
        RangeArray<int> ra(-1, 5);

        ra[1] = 2;

        my_test1(ra);

        cout << ra[1] << endl;
    }
    catch (const MyLib::Exception &exception)
    {
        cout << exception.location() << endl;
        cout << exception.message() << endl; 
    }
    catch (...)
    {
        cout << "test failed ..." << endl;
    }

    MatrixGraphDemo();
    //test_graph();
#endif

    return 0;
}

