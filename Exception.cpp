#include "Exception.h"
#include <cstring>
#include <cstdlib>

using namespace std;


namespace JMLib
{

void Exception::init(const char* message, const char* file, int line)
{
    m_message = (message ? strdup(message) :NULL);

    // 1. 对m_message 异常信息赋值
    if (NULL != message)
    {
        m_message = strdup(message);
    }
    else
    {
        m_message = NULL;
    }

    m_message = strdup(message);


    // 2. 对m_location异常位置赋值
    if (NULL != file)
    {
        char s1[16] = {0};

        // 将line int整型数据转换为字符串类型存放在 s1字符数组中
        itoa(line, s1 , 10);

        m_location = static_cast<char *>(malloc(strlen(file) + strlen(s1) + +2));   // ':' 和 '\0'
        if(NULL != m_location)
        {
            m_location = strcpy(m_location, file);  // 字符串复制
            m_location = strcat(m_location, ":");   // 字符串拼接
            m_location = strcat(m_location, s1);    // 字符串拼接
        }
    }
    else
    {
        m_location = NULL;
    }
}


Exception::Exception(const char* message)
{
    init(message, NULL, 0);
}


Exception::Exception(const char* file, int line)
{
    init(NULL, file, line);
}


Exception::Exception(const char* message, const char* file, int line)
{
    init(message, file, line);
}


Exception::Exception(const Exception& e)                // 拷贝构造函数
{
    m_message  = strdup(e.m_message);
    m_location = strdup(e.m_location);
}

Exception& Exception::operator =(const Exception& e)    // 重载操作符
{
    if (this != &e)
    {
        free(m_message);
        free(m_location);
        m_message = strdup(e.m_message);
        m_message = strdup(e.m_location);
    }

    return *this;
}


const char* Exception::message() const
{
    return m_message;
}


const char* Exception::location() const
{
    return m_location;
}


Exception::~Exception()     // 纯虚函数
{
    if ( !m_location )
    {
        delete m_location;
    }
    if ( !m_message )
    {
        delete m_message;
    }
}

}




