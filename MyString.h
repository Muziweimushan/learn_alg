#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include "Object.h"

namespace MyLib
{

class String : public Object
{
protected:
	int m_length;//字符串长度
	char *m_str;//字符串存储空间

	void init(const char *s);//封装的统一初始化函数
	//封装的字符串比较函数
	bool equal(const char *l, const char *r, int len) const;

	static int *make_pmt(const char *p);
	static int kmp(const char *s, const char *p);
public:
	String();
	String(const char *s);
	String(const String &s);
	String(const char c);

	//比较操作符重载
	bool operator == (const char *s) const;
	bool operator == (const String &s) const;

	bool operator != (const char *s) const;
	bool operator != (const String &s) const;

	bool operator > (const char *s) const;
	bool operator > (const String &s) const;

	bool operator < (const char *s) const;
	bool operator < (const String &s) const;


	bool operator >= (const char *s) const;
	bool operator >= (const String &s) const;

	bool operator <= (const char *s) const;
	bool operator <= (const String &s) const;

	String &operator += (const String &s);
	String &operator += (const char *s);
	String operator + (const String &s) const;
	String operator + (const char *s) const;

	String &operator -= (const String &s);
	String &operator -= (const char *s);
	String operator - (const String &s) const;
	String operator - (const char *s) const;
	
	String &operator = (const char *s);
	String &operator = (const String &s);
	String &operator = (char c);

	char &operator [] (int i);
	char operator [] (int i) const;

	bool startWith(const char *s) const;
	bool startWith(const String &s) const;
	bool endOf(const char *s) const;
	bool endOf(const String &s) const;

	String &insert(int i, const char *s);
	String &insert(int i, const String &s);

	String &trim();
	
	int indexOf(const char *s) const;
	int indexOf(const String &s) const;
	String &remove(const char *s);
	String &remove(const String &s);
	String &remove(int i, int len);


	//将字符串中子串t替换为子串s
	String &replace(const char *t, const char *s);
	String &replace(const char *t, const String &s);
	String &replace(const String &t, const char *s);
	String &replace(const String &t, const String &s);

	//从i处提取长度为len的字符串
	String sub(int i, int len) const;

	int length() const;
	const char *str() const;

	~String();

};


}





#endif
