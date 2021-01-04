#ifndef _TREE_H_
#define _TREE_H_


#include "TreeNode.h"
#include "SharedPointer.h"

namespace JMLib
{

template < typename T >

class Tree : public Object
{
protected:
    TreeNode<T>* m_root;    // 根节点

    /* 两个与拷贝相关的函数：拷贝构造函数和赋值操作符重载函数 */
    /* 目的是防止两棵树的复制 */
    Tree(const Tree<T>&);
    Tree<T>& operator = (const Tree<T>&);     // 这里是保护成员函数，在main函数中，GTree对象不能使用“=”复制


public:
    Tree()
    {
        m_root = NULL;
    }

    // 插入结点
    virtual bool insert(TreeNode<T>* node) = 0;
    virtual bool insert(const T& value, TreeNode<T>* parent) = 0;

    // 删除结点
    virtual SharedPointer< Tree<T> > remove(const T& value) = 0;
    virtual SharedPointer< Tree<T> > remove(TreeNode<T>* node) = 0;

    // 查找结点
    virtual TreeNode<T>* find(const T& value) const = 0;
    virtual TreeNode<T>* find(TreeNode<T>* node) const = 0;

    // 根结点访问函数
    virtual TreeNode<T>* root() const = 0;

    // 树的度访问函数
    virtual int degree() const = 0;

    // 树的高度访问函数
    virtual int height() const = 0;


    // 树的结点数目访问函数
    virtual int count() const = 0;


    // 游标思想遍历树，三件套
    virtual bool begin() = 0;
    virtual bool end()  = 0;
    virtual bool next() = 0;
    virtual T current() = 0;


    // 清空树
    virtual void clear() = 0;
};

}

#endif // _TREE_H_
