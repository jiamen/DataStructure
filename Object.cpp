#include "Object.h"
#include <cstdlib>

namespace JMLib
{

void* Object::operator new(unsigned int size) throw()   // 不抛异常, new失败后返回为NULL
{
    return malloc(size);
}

void Object::operator delete(void* p)
{
    return free(p);
}

void* Object::operator new[] (unsigned int size) throw()
{
    return malloc(size);
}

void Object::operator delete[] (void* p)
{
    return free(p);
}


bool Object::operator == (const Object& obj)
{
    return (this == &obj);      // 通过比较地址判断相不相同
}

bool Object::operator != (const Object& obj)
{
    return (this != &obj);
}

Object::~Object()
{

}


}
