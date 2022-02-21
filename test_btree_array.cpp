#include <iostream>
#include "BTreeArray.h"

void inOrderPrint(MyLib::BTreeArray<char> &bt, int i)
{
    if (!bt.isNull(i))
    {
        inOrderPrint(bt, bt.left(i));

        std::cout << bt[i] << std::endl;

        inOrderPrint(bt, bt.right(i));
    }
}

int count2Degree(MyLib::BTreeArray<char> &bt, int i)
{
    int ret = 0;

    if (!bt.isNull(i))
    {
        int rc = bt.right(i);
        int lc = bt.left(i);

        if (!bt.isNull(rc) && !bt.isNull(lc))
            ret = 1;

        ret += count2Degree(bt, rc) + count2Degree(bt, lc);
    }

    return ret;
}

void testBTreeArray(void)
{
    MyLib::BTreeArray<char> bt(3);
    int node = 1;

    bt.set(node, 'A'); 

    std::cout << "length = " << bt.length() << std::endl;
    std::cout << "count = " << bt.count() << std::endl;
    
    node = bt.find('A');
    bt.set(bt.left(node), 'B');
    bt.set(bt.right(node), 'C');

    node = bt.find('C');
    std::cout << node << std::endl;
    bt.set(bt.left(node), 'F');
    bt.set(bt.right(node), 'G');

    std::cout << "length = " << bt.length() << std::endl;
    std::cout << "count = " << bt.count() << std::endl;

    bt.resize(4);

    for (int i = 1; i <= bt.length(); i++)
    {
        try
        {
            std::cout << bt[i] << std::endl;
        }
        catch (MyLib::Exception &obj)
        {
        }
    }

    inOrderPrint(bt, 1);

    std::cout << count2Degree(bt, 1) << std::endl;

    return;
}
