/*************************************************************************
 * 文件名称：MyString.cpp
 * 作    者：muziwei
 * 创建时间：Sat 01 Jun 2019 10:15:38 AM CST
 * 模块名称：
 * 主要函数：
 * 功    能：
 * 版    本：
 * 与其他文件的关系：
 * 修改记录：
 ************************************************************************/
#include <cstdlib>
#include <cstring>
#include "MyString.h"
#include "Exception.h"

using namespace std;

namespace MyLib
{

int *String::make_pmt(const char *p)
{
	int len = strlen(p);

	int *ret = reinterpret_cast<int *>(malloc(sizeof(int *) * len));

	if (NULL != ret)
	{
		int ll = 0;

		ret[0] = 0;

		for (int i = 1; i < len; i++)
		{
			while((ll > 0) && (p[ll] != p[i]))
			{
				ll = ret[ll - 1];
			}

			if (p[ll] == p[i])
			{
				ll++;
			}

			ret[i] = ll;
		}
	}

	return ret;
}

int String::kmp(const char *s, const char *p)
{
	int ret = -1;
	int sl = strlen(s);
	int pl = strlen(p);
	int *pmt = make_pmt(p);

	//必须满足三个条件
	//1.pmt生成成功
	//2.子串长度不为0
	//3.子串长度小于等于目标串的长度
	if ((NULL != pmt) && (pl > 0) && (pl <= sl))
	{
		for (int i = 0, j = 0; i < sl; i++)
		{
			while ((j > 0) && (s[i] != p[j]))
			{
				j = pmt[j - 1];
			}
			
			if (s[i] == p[j])
			{
				j++;
			}

			//查找到的条件
			if (pl == j)
			{
				ret = i + 1 - pl;
				break;
			}
		}
	}

	free(pmt);
	return ret;
}

void String::init(const char *s)
{
	m_str =	strdup(s); 

	if (NULL != m_str)
	{
		m_length = strlen(m_str);
	}
	else
	{
		THROW_EXCEPTION(NoEnoughMemoryException, "No enough to create String object...");
	}
}


String::String()
{
	init("");//空字符串
}

String::String(const char *s)
{
	init(s ? s : "");
}

String::String(const String &s)
{
	init(s.m_str);
}

String::String(const char c)
{
	char s[] = {c, '\0'};

	init(s);
}


bool String::operator == (const char *s) const
{
	return (0 == strcmp(m_str, s ? s : ""));
}

bool String::operator == (const String &s) const
{
	return (0 == strcmp(m_str, s.m_str));
}

bool String::operator != (const char *s) const
{
	return !(*this == s);
}

bool String::operator != (const String &s) const
{
	return !(*this == s);
}

bool String::operator > (const char *s) const
{
	return (strcmp(m_str, s ? s : "") > 0);
}

bool String::operator > (const String &s) const
{
	return (strcmp(m_str, s.m_str) > 0);
}

bool String::operator < (const char *s) const
{
	return (strcmp(m_str, s ? s : "") < 0);
}
bool String::operator < (const String &s) const
{
	return (strcmp(m_str, s.m_str) < 0);
}


bool String::operator >= (const char *s) const
{
	return (strcmp(m_str, s ? s : "") >= 0);
}

bool String::operator >= (const String &s) const
{
	return (strcmp(m_str, s.m_str));
}

bool String::operator <= (const char *s) const
{
	return (strcmp(m_str, s ? s : "") <= 0);
}
bool String::operator <= (const String &s) const
{
	return (strcmp(m_str, s.m_str));
}

//加法操作符重载
String String::operator + (const String &s) const
{
	return (*this + s.m_str);
}
String String::operator + (const char *s) const
{
	String ret;

	int len = m_length + strlen(s);
	char *str = reinterpret_cast<char *>(malloc(len + 1));

	if (NULL != str)
	{
		strcpy(str, m_str);
		strcat(str, s);
		free(ret.m_str);

		ret.m_str = str;
		ret.m_length = len;
		
	}
	else
	{
		THROW_EXCEPTION(NoEnoughMemoryException, "No memory to add String values...");
	}

	return ret;
}

String& String::operator += (const String &s)
{
	return (*this = *this + s.m_str);
}

String& String::operator += (const char *s)
{
	return (*this = *this + (s ? s : ""));
}

String &String::operator -= (const String &s)
{
	return remove(s);
}

String &String::operator -= (const char *s)
{
	return remove(s);
}

String String::operator - (const String &s) const
{
	return String(*this).remove(s);
}

String String::operator - (const char *s) const
{
	return String(*this).remove(s);
}



String &String::operator = (const char *s)
{
	if (m_str != s)//自赋值情况
	{
		char *str = strdup(s ? s : "");

		if (NULL != str)
		{
			free(m_str);

			m_str = str;
			m_length = strlen(m_str);
		}
		else
		{
			THROW_EXCEPTION(NoEnoughMemoryException, "No memory to assign new String value...");
		}
	}

	return *this;
}

String &String:: operator = (const String &s)
{
	return (*this = s.m_str);
}

String &String:: operator = (char c)
{
	char s[] = {c, '\0'};

	return (*this = s);
}

char &String::operator [] (int i)
{
	if ((0 <= i) && (i < m_length))
	{
		return m_str[i];
	}
	else
	{
		THROW_EXCEPTION(IndexOutOfBoundsException, "Invalid parameter i...");
	}
}

char String::operator[] (int i) const
{
	return (const_cast<String &>(*this))[i];//此处会创建一个临时变量用于存放这个字符元素
}

bool String::equal(const char *l, const char *r, int len) const
{
	bool ret = true;
	
	for (int i = 0; i < len && ret; i++)//只要有一个字符不一样就可以直接退出,返回false
	{
		ret = (l[i] == r[i]);//感觉不需要ret && (l[i] == r[i])
	}

	return ret;
}


bool String::startWith(const char *s) const
{
	bool ret = true;

	ret = (NULL != s);

	if (ret)
	{
		ret = equal(m_str, s, strlen(s));
	}
	return ret;
}

bool String::startWith(const String &s) const
{
	return startWith(s.m_str);
}

//endOf与startWith类似,仅仅是l的下标不一样而已
bool String::endOf(const char *s) const
{
	bool ret = true;
	char *str = NULL;
	int length = 0;

	ret = (NULL != s);

	if (ret)
	{
		length = strlen(s);
		str = m_str + (m_length - length);
		ret = equal(str, s, length);
	}
	return ret;
}

bool String::endOf(const String &s) const
{
	return endOf(s.m_str);
}


String &String::insert(int i, const char *s)
{
	if ((0 <= i) && (i <= m_length))
	{
		if ((NULL != s) && ('\0' != s[0]))
		{
			int len = strlen(s);
			char *str = reinterpret_cast<char *>(malloc(len + m_length + 1));

			if (NULL != str)
			{
				strncpy(str, m_str, i);
				strncpy(str + i, s, len);
				strncpy(str + i + len, m_str + i, m_length - i);

				str[m_length + len] = '\0';

				free(m_str);
				m_str = str;
				m_length = len + m_length;
			}
			else
			{
				THROW_EXCEPTION(NoEnoughMemoryException, "No memory to insert string values...");
			}
		}
	}
	else
	{
		THROW_EXCEPTION(IndexOutOfBoundsException, "Invalid parameter i...");
		
	}

	return *this;
}

String &String::insert(int i, const String &s)
{
	return insert(i, s.m_str);
}

String &String::trim()
{
	int b = 0;
	int e = m_length - 1;

	while (' ' == m_str[b] && b < m_length) b++;
	
	while (' ' == m_str[e] && e >= 0) e--;

	if (0 == b)
	{
		//我觉得还是检查一下字符串尾部有没有空格比较保险
		if (e >= 0)
		{
			m_str[e + 1] = '\0';
			m_length = e + 1;
		}
	}
	else
	{
		//有头无尾(e = -1)或者有尾没有头(b = m_length)的情况都是b比e要大
		if (b <= e)
		{

			for (int i = 0, j = b; j <= e; i++, j++)
			{
				m_str[i] = m_str[j];
			}
			m_length = e - b + 1;
			m_str[m_length] = '\0';
		}
	}

	return *this;
}


int String::indexOf(const char *s) const
{
	return kmp(m_str, s ? s : "");
}
int String::indexOf(const String &s) const
{
	return kmp(m_str, s.m_str);
}

String &String::remove(const char *s)
{
	return remove(indexOf(s), strlen(s ? s : ""));
}

String &String::remove(const String &s)
{
	return remove(s.m_str);
}

String &String::remove(int i, int len)
{
	if ((0 <= i) && (i < m_length))
	{
		int n = i;
		int m = i + len;
		//需要删除的字符位于坐标[n, m)之间
		//实际只需要将[m, m_length)的的数据依次拷贝到n位置起始的位置处即可
		while ((n < m) && (m < m_length))
		{
			m_str[n] = m_str[m];
			n++;
			m++;
		}

		//拷贝完后有效字符的坐标为n-1
		m_str[n] = '\0';
		m_length = n;
	}

	return *this;
}


//将字符串中子串t替换为子串s
String &String::replace(const char *t, const char *s)
{
	int index = indexOf(t);

	if (index >= 0)
	{
		remove(t);
		insert(index, s);
	}

	return *this;
}

String &String::replace(const char *t, const String &s)
{
	return replace(t, s.m_str);
}

String &String::replace(const String &t, const char *s)
{
	return replace(t.m_str, s);
}

String &String::replace(const String &t, const String &s)
{
	return replace(t.m_str, s.m_str);
}

String String::sub(int i, int len) const
{
	String ret;

	if ((0 <= 0) && (i < m_length))
	{
		if (i < 0) i = 0;
		if (i + len > m_length) len = m_length - i;

		char *str = reinterpret_cast<char *>(malloc(len + 1));

		strncpy(str, m_str + i, len);

		ret = str;
	}
	else
	{
		THROW_EXCEPTION(IndexOutOfBoundsException, "parameter i is invalid...");
	}

	return ret;
}

int String::length() const
{
	return m_length;
}

const char *String::str() const
{
	return m_str;
}


String::~String()
{
	free(m_str);
}


}

