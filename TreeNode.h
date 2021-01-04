#ifndef _TREENODE_H_
#define _TREENODE_H_


#include "Object.h"


namespace JMLib
{


template < typename T >

class TreeNode : public Object
{
protected:
    bool m_flag;

    /* 两个与拷贝相关的函数：拷贝构造函数和赋值操作符重载函数 */
    TreeNode(const TreeNode<T>&);
    TreeNode<T>& operator = (const TreeNode<T>&);

    void* operator new(unsigned int size) throw()
    {
        return Object::operator new(size);
    }


public:
    T value;
    TreeNode<T>* parent;    // 加上指向父节点的指针在工程上非常有必要

    TreeNode()
    {
        m_flag = false;
        parent = NULL;
    }

    bool flag()
    {
        return m_flag;
    }

    virtual ~TreeNode() = 0;

};


template < typename T >
TreeNode<T>::~TreeNode()
{

}


}



#endif // _TREENODE_H_
