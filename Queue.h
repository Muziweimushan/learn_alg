#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "Object.h"

namespace MyLib
{

template < typename T >
class Queue : public Object
{
public:
	virtual void add(const T &e) = 0;	//队列的入队操作,插入到队列的尾部
	virtual void remove(void) = 0;		//队列的出队操作,从队列的头部删除元素
	virtual T front(void) const = 0;	//获取队头元素
	virtual void clear() = 0;			//清空队列
	virtual int length() const = 0;		//获取队列长度(当前队列中元素的个数)

};

}


#endif
