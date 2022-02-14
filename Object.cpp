/*************************************************************************
 * 文件名称：Object.cpp
 * 作    者：muziwei
 * 创建时间：Thu 02 May 2019 11:58:36 PM CST
 * 模块名称：
 * 主要函数：
 * 功    能：
 * 版    本：
 * 与其他文件的关系：
 * 修改记录：
 ************************************************************************/
#include <cstdlib> 
#include "Object.h"

using namespace MyLib;


void *Object::operator new (unsigned long int size) throw()
{
	return malloc(size);
}


void Object::operator delete (void *p)
{
	free(p);
}

void *Object::operator new[] (unsigned long int size) throw()
{
	return malloc(size);
}
void Object::operator delete[] (void *p)
{
	free(p);
}

bool Object::operator== (const Object &obj)
{
	return (this == &obj);
}

bool Object::operator!= (const Object &obj)
{
	return (this != &obj);
}

Object::~Object()
{

}



