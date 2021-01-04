#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "Object.h"

namespace JMLib
{


//定义宏THROW_EXCEPTION，抛出异常时直接写异常类型及异常信息即可
#define THROW_EXCEPTION(e, m) (throw e(m, __FILE__, __LINE__))


class Exception : public Object       // 含有纯虚函数的基类称为抽象基类，不能建立对象
{
protected:
    char* m_message;        // 提示异常信息
    char* m_location;       // 提示异常位置

    // 可以使用辅助的init函数来完成初始化的工作
    void init(const char* message, const char* file, int line);


public:
    // 重载构造函数，这几个重载函数的内部逻辑是差不多的
    Exception(const char* message);
    Exception(const char* file, int line);
    Exception(const char* message, const char* file, int line);

    Exception(const Exception& e);
    Exception& operator = (const Exception& e);


    virtual const char* message() const;
    virtual const char* location() const;


    virtual ~Exception() = 0;       // 析构函数  纯虚函数
};


/******************** 1. 计算异常类的实现 ********************/
class ArithmeticException : public Exception   // 计算异常
{
public:
    ArithmeticException() : Exception( 0 ) {  }
    ArithmeticException(const char* message) : Exception(message)  {  }
    ArithmeticException(const char* file, int line) : Exception(file, line)  {  }
    ArithmeticException(const char* message, const char* file, int line) : Exception(message, file, line)  {  }

    ArithmeticException(const ArithmeticException& e) : Exception(e)  {  }

    ArithmeticException& operator = (const ArithmeticException& e)
    {
        Exception::operator = (e);

        return *this;
    }
};


/******************** 2. 空指针异常类的实现 ********************/
class NullPointerException : public Exception   // 计算异常
{
public:
    NullPointerException() : Exception( 0 ) {  }
    NullPointerException(const char* message) : Exception(message)  {  }
    NullPointerException(const char* file, int line) : Exception(file, line)  {  }
    NullPointerException(const char* message, const char* file, int line) : Exception(message, file, line)  {  }

    NullPointerException(const NullPointerException& e) : Exception(e)  {  }

    NullPointerException& operator = (const NullPointerException& e)
    {
        Exception::operator = (e);

        return *this;
    }
};


/******************** 3. 越界异常类的实现 ********************/
class IndexOutOfBoundsException : public Exception   // 计算异常
{
public:
    IndexOutOfBoundsException() : Exception( 0 ) {  }
    IndexOutOfBoundsException(const char* message) : Exception(message)  {  }
    IndexOutOfBoundsException(const char* file, int line) : Exception(file, line)  {  }
    IndexOutOfBoundsException(const char* message, const char* file, int line) : Exception(message, file, line)  {  }

    IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e)  {  }

    IndexOutOfBoundsException& operator = (const IndexOutOfBoundsException& e)
    {
        Exception::operator = (e);

        return *this;
    }
};


/******************** 4. 内存不足类的实现 ********************/
class NoEnoughMemoryException : public Exception   // 计算异常
{
public:
    NoEnoughMemoryException() : Exception( 0 ) {  }
    NoEnoughMemoryException(const char* message) : Exception(message)  {  }
    NoEnoughMemoryException(const char* file, int line) : Exception(file, line)  {  }
    NoEnoughMemoryException(const char* message, const char* file, int line) : Exception(message, file, line)  {  }

    NoEnoughMemoryException(const NoEnoughMemoryException& e) : Exception(e)  {  }

    NoEnoughMemoryException& operator = (const NoEnoughMemoryException& e)
    {
        Exception::operator = (e);

        return *this;
    }
};


/******************** 5. 参数错误类的实现 ********************/
class InvalidParameterException : public Exception   // 计算异常
{
public:
    InvalidParameterException() : Exception( 0 ) {  }
    InvalidParameterException(const char* message) : Exception(message)  {  }
    InvalidParameterException(const char* file, int line) : Exception(file, line)  {  }
    InvalidParameterException(const char* message, const char* file, int line) : Exception(message, file, line)  {  }

    InvalidParameterException(const InvalidParameterException& e) : Exception(e)  {  }

    InvalidParameterException& operator = (const InvalidParameterException& e)
    {
        Exception::operator = (e);

        return *this;
    }
};


/******************** 6. 成员函数调用状态异常类的实现 ********************/
class InvalidOperationException : public Exception   // 计算异常
{
public:
    InvalidOperationException() : Exception( 0 ) {  }
    InvalidOperationException(const char* message) : Exception(message)  {  }
    InvalidOperationException(const char* file, int line) : Exception(file, line)  {  }
    InvalidOperationException(const char* message, const char* file, int line) : Exception(message, file, line)  {  }

    InvalidOperationException(const InvalidOperationException& e) : Exception(e)  {  }

    InvalidOperationException& operator = (const InvalidOperationException& e)
    {
        Exception::operator = (e);

        return *this;
    }
};


}   // end namespace JMLib


#endif // _EXCEPTION_H_
