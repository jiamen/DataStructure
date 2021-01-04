#ifndef _OBJECT_H_
#define _OBJECT_H_

namespace JMLib
{

class Object
{
public:
    void* operator new(unsigned int size) throw();  // 该函数不会抛出异常
    void operator delete(void* p);

    void* operator new[] (unsigned int size) throw();
    void operator delete[] (void* p);
    bool operator == (const Object& obj);
    bool operator != (const Object& obj);

    virtual ~Object() = 0;  // 纯虚函数，使得该类只能被继承
};


}   // end namespace JMLib

#endif // _OBJECT_H_
