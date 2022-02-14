#ifndef __STATICLINKLIST_H__
#define __STATICLINKLIST_H__

#include "LinkList.h"

namespace MyLib
{

template < typename T, int N >
class StaticLinkList : public LinkList<T>
{
protected:
	typedef typename LinkList<T>::Node Node;

	struct SNode : public Node
	{
		//重载Node的new操作符,需要一个新的类型继承自Node,然后这个类仅仅重载new操作符就可实现
		void *operator new(unsigned long int size, void *loc)
		{
			return loc;
		}
	};

	unsigned char m_space[N * sizeof(Node)];
	int m_used[N];

public:

	StaticLinkList()
	{
		for (int i = 0; i < N; i++)
		{
			m_used[i] = 0;
		}
	}
	
	//当泛指类型T是类类型时,create函数还需要调用T的构造函数
	//因此需要重载new操作符来实现这个功能
	Node *create()
	{
		SNode *ret = NULL;
		
		for (int i = 0; i < N; i++)
		{
			if (!m_used[i])
			{
				ret = reinterpret_cast<SNode *>(m_space) + i;
				ret = new(ret)SNode;//调用SNode的构造函数,从而实现调用Node--->泛指类型T的构造函数
				m_used[i] = 1;
				break;
			}
		}

		return ret;
	}

	void destroy(Node *pn)
	{
		SNode *space = reinterpret_cast<SNode *>(m_space);
		SNode *psn = dynamic_cast<SNode *>(pn);

		//找到要归还的内存
		for (int i = 0; i < N; i++)
		{
			if (pn == space + i)
			{
				m_used[i] = 0;
				psn->~SNode();//还需要调用Node的析构函数
				break;
			}	
		}
	}

	int capacity() const
	{
		return N;
	}

	~StaticLinkList()
	{
		//析构函数中不会发生多态,因此这里的clear就是LinkList中的clear函数(因为StaticLinkList中没有重写clear)
		//而在clear函数中调用的destroy函数则是先调用StaticLinkList的destroy然后还会调用LinkList的destroy函数
		//因为StaticLinkList是LinkList的子类,析构时子类和父类的析构函数都会调用,先调用的destroy函数是StaticLinkList中的destroy
		//因为先调用的是StaticLinkList的析构函数,而析构函数中是不会发生多态的
		//~StaticLinkList() -> LinkList::clear() -> StaticLinkList::destroy() -> ~SNode() -> ~Node() -> ~T() -> ~LinkList() -> LinkList::clear() -> ~m_header() -> ~List() -> ~Object()
		//以上就是整个StaticLinkList的析构过程
		this->clear();
	}


};



}



#endif
