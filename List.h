#ifndef __LIST_H__
#define __LIST_H__

#include "Object.h"

namespace MyLib
{

template < typename T >
class List : public Object
{
protected:

	//容器类型的类,禁用拷贝构造和赋值操作
	List(const List &);
	List &operator= (const List &);
public:
	//因为我们定义了拷贝构造函数,因此C++编译器就不会默认提供无参构造函数啦
	List() {}
	//以重载方式添加一个insert函数,往线性表的尾部添加一个元素
	virtual bool insert(const T &e) = 0;
	virtual bool insert(int i, const T &e) = 0;
	virtual bool remove(int i) = 0;
	virtual bool get(int i, T &e) const = 0;
	virtual bool set(int i, const T &e) = 0;
	virtual int find(const T &e) const = 0;
	virtual int length() const = 0;
	virtual void clear() = 0;


};











}


#endif
