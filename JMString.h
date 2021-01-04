#ifndef _JMSTRING_H_
#define _JMSTRING_H_


#include "Object.h"

namespace JMLib
{

class String : public Object
{
protected:
    char* m_str;
    int m_length;

    void init(const char* s);
    bool equal(const char* l, const char* r, int len) const;


public:
    String();

    String(char c);
    String(const char* s);
    String(const String& s);

    int length() const;
    const char* str() const;    // 与传统的字符串进行互操作


    char& operator [] (int i);
    char operator [](int i) const;


    // 下面实现逻辑关系运算操作符
    bool operator == (const String& s) const;   // C++版本
    bool operator == (const char* s) const;     // C  版本
    bool operator != (const String& s) const;
    bool operator != (const char* s) const;
    bool operator > (const String& s) const;
    bool operator > (const char* s) const;
    bool operator < (const String& s) const;
    bool operator < (const char* s) const;
    bool operator >= (const String& s) const;
    bool operator >= (const char* s) const;
    bool operator <= (const String& s) const;
    bool operator <= (const char* s) const;

    // 下面是加法 字符拼接
    String operator + (const String& s) const;
    String operator + (const char* s) const;
    String& operator += (const String& s);
    String& operator += (const char* s);


    // 下面是赋值操作符重载函数
    String& operator = (const String& s);
    String& operator = (const char* s);
    String& operator = (char c);


    bool startWith(const char* s) const;
    bool startWith(const String& s) const;
    bool endOf(const char* s) const;
    bool endOf(const String& s) const;


    String& insert(int i, const char* s);
    String& insert(int i, const String& s);

    // 去掉字符串中的空格
    String& trim();


    ~String();
};

}



#endif // _JMSTRING_H_
