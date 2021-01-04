#include "JMString.h"
#include <cstring>
#include <cstdlib>
#include "Exception.h"


namespace JMLib
{

void String::init(const char* s)
{
    m_str = strdup(s);
    // strdup()会先用maolloc()配置与参数s 字符串相同的空间大小，
    // 然后将参数s 字符串的内容复制到该内存地址，然后把该地址返回。
    // 该地址最后可以利用free()来释放。

    if( m_str )
    {
        m_length = strlen(m_str);
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create String Object ...");
    }
}


String::String()
{
    init("");
}

String::String(char c)
{
    char s[] = {c, '\0'};

    init(s);
}

String::String(const char* s)
{
    init(s ? s : "");
}

String::String(const String& s)
{
    init(s.m_str);
}

int String::length() const
{
    return m_length;
}

const char* String::str() const
{
    return m_str;
}


char& String::operator [] (int i)
{
    if( (0 <= i) && (i < m_length) )
    {
        return m_str[i];
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "Parameter i is invalid ... ");
    }
}

char String::operator [](int i) const
{
    return (const_cast<String&>(*this))[i];
}


bool String::equal(const char *l, const char *r, int len) const
{
    bool ret = true;

    for(int i=0; i<len && ret; i ++)
    {
        ret = ret && (l[i] == r[i]);
    }

    return ret;
}


bool String::startWith(const char* s) const
{
    bool ret = (NULL != s);

    if( ret )
    {
        int len = strlen(s);

        ret = (len < m_length) && equal(m_str, s, len);
    }

    return ret;
}

bool String::startWith(const String& s) const
{
    return startWith(s.m_str);
}

bool String::endOf(const char* s) const
{
    bool ret = (NULL != s);

    if( ret )
    {
        int len = strlen(s);
        char* str = m_str + (m_length - len);

        ret = (len < m_length) && equal(str, s, len);
    }

    return ret;
}

bool String::endOf(const String& s) const
{
    return endOf(s.m_str);
}


String& String::trim()
{
    int b = 0;
    int e = m_length - 1;

    while( m_str[b] == ' ' ) b ++;
    while( m_str[e] == ' ' ) e --;

    if( 0 == b )
    {
        m_str[e+1]  = '\0';
        m_length = e + 1;
    }
    else
    {
        for(int i=0, j=b; j<=e; i++, j++)
        {
            m_str[i] = m_str[j];
        }

        m_str[e - b + 1] = '\0';
        m_length = e - b + 1;
    }

    return *this;
}


String& String::insert(int i, const char* s)    // i是插入位置, s是字符串
{
    if( (0 <= i) && (i <= m_length) )
    {
        if( (NULL != s) && (s[0] != '\0') )     // 字符串指针不为空，也不是空串
        {
            int len = strlen(s);    // 求出插入字符串的长度
            char* str = reinterpret_cast<char*>(malloc(m_length + len +1));

            if( str != NULL )
            {
                strncpy(str, m_str, i);
                strncpy(str+i, s, len);
                strncpy(str+i+len, m_str+i, m_length-i);

                str[m_length + len] = '\0';     // 字符串最后一个字符是'\0'

                free(m_str);

                m_str = str;
                m_length = m_length +len;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to insert string value ...");
            }
        }
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }

    return *this;
}

String& String::insert(int i, const String& s)
{
    return insert(i, s.m_str);
}


bool String::operator == (const String& s) const
{
    return (strcmp(m_str, s.m_str) == 0);
}

bool String::operator == (const char* s) const
{
    return (strcmp(m_str, s ? s : "") == 0);     // strcmp相等返回0
}

bool String::operator != (const String& s) const
{
    return !(*this == s);       // 利用上面的重载==比较符
}

bool String::operator != (const char* s) const
{
    return !(*this == s);       // 利用上面的重载==比较符
}

bool String::operator > (const String& s) const
{
    return (strcmp(m_str, s.m_str) > 0);
}

bool String::operator > (const char* s) const
{
    return (strcmp(m_str, s ? s : "") > 0);
}

bool String::operator < (const String& s) const
{
    return (strcmp(m_str, s.m_str) < 0);
}

bool String::operator < (const char* s) const
{
    return (strcmp(m_str, s ? s : "") < 0);
}

bool String::operator >= (const String& s) const
{
    return (strcmp(m_str, s.m_str) >= 0);
}

bool String::operator >= (const char* s) const
{
    return (strcmp(m_str, s ? s : "") >= 0);
}

bool String::operator <= (const String& s) const
{
    return (strcmp(m_str, s.m_str) <= 0);
}

bool String::operator <= (const char* s) const
{
    return (strcmp(m_str, s ? s : "") <= 0);
}


/* 加法拼接函数 */
String String::operator + (const String& s) const
{
    return (*this + s.m_str);       // 直接调用下面的函数重载
}

String String::operator + (const char* s) const
{
    String ret;
    int len = m_length + strlen(s ? s : "");
    char* str = reinterpret_cast<char*>(malloc(len + 1));

    if( str )
    {
        strcpy(str, m_str);         // 把m_str字符串拷贝到str空间
        strcat(str, s ? s : "");

        free(ret.m_str);

        ret.m_str = str;
        ret.m_length = len;
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to add String values ...");
    }

    return ret;
}

String& String::operator += (const String& s)
{
    return (*this = *this + s);     // C++ 字符串加法
}

String& String::operator += (const char* s)
{
    return (*this = *this + s);     // C 字符串加法
}


/* 赋值操作符重载函数 */
String& String::operator = (const String& s)
{
    return (*this = s.m_str);   // 调用下面的赋值函数实现
}

String& String::operator = (const char* s)
{
    if( m_str != s )        // 调用重载操作符
    {
        char* str = strdup(s ? s : "");    // 复制一份出来

        if( str )
        {
            free(m_str);    // 释放原地址空间

            m_str = str;
            m_length = strlen(m_str);
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to assign new String value ...");
        }
    }

    return *this;
}

String& String::operator = (char c)
{
    char s[] = {c, '\0'};

    return (*this = s);
}

String::~String()
{
    free(m_str);
}


}
