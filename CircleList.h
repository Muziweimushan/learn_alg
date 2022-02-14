#ifndef __CIRCLELIST_H__
#define __CIRCLELIST_H__

#include "LinkList.h"

namespace MyLib
{

template < typename T >
class CircleList : public LinkList<T>
{

protected:
	//重定义一下父类的Node为Node
	typedef typename LinkList<T>::Node Node;
	Node *last()		//O(n)
	{
		return this->position(this->m_length - 1)->next;//最后一个元素的下标是m_length,然后再通过next获取最后一个元素
	}

	void last_to_first()		//O(n)
	{
		last()->next = this->m_header.next;
	}

	int mod(int i) const	//O(1)
	{
		return (0 == this->m_length) ? 0 : (i % this->m_length);
	}

public:
	bool insert(const T &e)					//O(n)
	{
		insert(this->m_length, e);
	}

	bool insert(int i, const T &e)			//O(n) + O(n) + O(1) = O(2n + 1) = O(n)
	{
		bool ret = false;
		
		i = i % (this->m_length + 1);//对于插入操作, 允许的下标应该是0-N即N+1个		//O(1)
		
		ret = LinkList<int>::insert(i, e);		//O(n)

		if ((ret) && (0 == i))
		{
			last_to_first();					//O(n)
		}

		return ret;
	}

	bool remove(int i)							//O(n) 线性阶复杂度
	{
		bool ret = false;

		i = mod(i);

		if (0 == i)
		{
			Node *toDel = this->m_header.next;

			if (NULL != toDel)
			{
				this->m_header.next = toDel->next;
				this->m_length--;

				if (0 == this->m_length)
				{
					//链表中没有元素了,m_header.next赋值为空而不能调用last_to_first,否则等价于没删除
					this->m_header.next = NULL;
					this->m_current = NULL;
				}
				else
				{
					if (this->m_current == toDel)
					{
						this->m_current = toDel->next;
					}

					last_to_first();	//O(n)
				}

				this->destroy(toDel);
			}
			else
			{
				ret = false;
			}
		}
		else
		{
			//下标不为0直接调用父类的remove函数即可
			ret = LinkList<T>::remove(i);//O(n)
		}

		return ret;
	}

	int find(const T &e)					//O(n)
	{
		int ret = -1;
		Node *slider = this->m_header.next;

		for (int i = 0; i < this->m_length; i++)
		{
			if (e == slider->value)
			{
				ret = i;
				break;
			}

			slider = slider->next;
		}

		return ret;
	}

	T get(int i) const						//O(n)
	{
		return LinkList<T>::get(mod(i));
	}

	bool get(int i, T &e) const				//O(n)
	{
		return LinkList<T>::get(mod(i), e);
	}

	bool set(int i, const T &e)				//O(n)
	{
		return LinkList<T>::set(mod(i), e);
	}

	void clear()				//O(n) + O(1) => O(n)
	{
		//while循环的时间复杂度是线性阶的,因为这里的remove函数调用时间每一次循环都是固定的,每次删除下标为1的元素的时间是常数,
		//与问题规模无关,时间复杂度为O(1), 而while循环与问题规模即链表长度有关, 因此整个while循环的时间复杂度为O(n)
		while (this->m_length > 1)
		{
			LinkList<T>::remove(1);
		}

		if (1 == this->m_length)
		{
			Node *toDel = this->m_header.next;

			this->m_header.next = NULL;
			this->m_current = NULL;
			this->m_length = 0;

			this->destroy(toDel);
			
		}
	}

	bool move(int i, int step = 1)		//O(n)
	{
		return LinkList<T>::move(mod(i), step);	
	}

	//除了m_length为0时循环链表不能遍历外其他情况都可以进行一直遍历,end返回为false
	//此外还需要保证游标的合法性,即用户需要通过先move再遍历的操作,因此这里多加了一个判断current指针是否为空
	bool end(void)						//O(1)
	{
		return ((0 == this->m_length) || (NULL == this->m_current));
	}

	~CircleList()		//O(n)
	{
		clear();
	}

};


}




#endif
