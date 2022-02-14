#ifndef __OBJECT_H__
#define __OBJECT_H__

namespace MyLib
{

class Object
{
public:
	void *operator new (unsigned long int size) throw();//throw() 表示不抛出异常
	void operator delete (void *p);

	//同样的不抛出异常仅仅返回空指针
	void *operator new[] (unsigned long int size) throw();
	void operator delete[] (void *p);
	
	bool operator== (const Object &obj);
	bool operator!= (const Object &obj);
	virtual ~Object() = 0;

};

}



#endif
