/*************************************************************************
 * 文件名称：Exception.cpp
 * 作    者：muziwei
 * 创建时间：Fri 03 May 2019 04:31:25 AM CST
 * 模块名称：
 * 主要函数：
 * 功    能：
 * 版    本：
 * 与其他文件的关系：
 * 修改记录：
 ************************************************************************/
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "Exception.h"


using namespace MyLib;
using namespace std;


void Exception::init(const char *message, const char *file, int line)
{
	if (NULL != message)
	{
		m_message = strdup(message);
	}

	if (NULL != file)
	{
		char sl[16] = {0};

		snprintf(sl, sizeof(sl), "%d", line);

		m_location = static_cast<char *>(malloc(strlen(file) + strlen(sl) + 2));
		if (NULL != m_location)
		{
			strcpy(m_location, file);
			strcat(m_location, ":");
			strcat(m_location, sl);
		}
		else
		{
			m_location = NULL;
			//此处不能抛Exception的异常,否则就等于嵌套调用了
		}
	}
	else
	{
		m_location = NULL;
	}

}


Exception::Exception(const char *message)
{
	init(message, NULL, 0);
}

Exception::Exception(const char *file, int line)
{
	init(NULL, file, line);
}

Exception::Exception(const char *message, const char *file, int line)
{
	init(message, file, line);
}


Exception::Exception(const Exception &e)
{
	if (NULL != e.m_message)
	{
		this->m_message = strdup(e.m_message);
	}


	if (NULL != e.m_location)
	{
		this->m_location = strdup(e.m_location);
	}
}

Exception &Exception::operator= (const Exception &e)
{

	if (this != &e)
	{

		free(this->m_message);
		free(this->m_location);


		if (NULL != e.m_message)
		{
			this->m_message = strdup(e.m_message);
		}


		if (NULL != e.m_location)
		{
			this->m_location = strdup(e.m_location);
		}
	}

	return *this;
}


const char *Exception::message() const
{
	return m_message;
}

const char *Exception::location() const
{
	return m_location;
}

Exception::~Exception()
{

}




