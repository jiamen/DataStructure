#ifndef _GTREE_H_
#define _GTREE_H_

#include "Tree.h"
#include "GTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"


// #include <iostream>
// using namespace std;


namespace JMLib
{

// GTree为通用树结构，每个结点可以存在多个后继结点
template <typename T>

class GTree : public Tree<T>
{
protected:

    LinkQueue<GTreeNode<T>*> m_queue;       // 58课内容，树的层次遍历


    GTreeNode<T>* find(GTreeNode<T>* node, const T& value) const
    {
        GTreeNode<T>* ret = NULL;

        if ( NULL != node  )
        {
            if ( node->value == value )
            {
                return node;
            }
            else
            {
                // 子树的遍历就是链表的遍历
                for(node->child.move(0); (!node->child.end()) && (NULL == ret); node->child.next())
                {
                    // 在子树中查找value值
                    ret = find(node->child.current(), value);   // 找不到这里就返回NULL
                }
            }
        }

        return ret;
    }

    GTreeNode<T>* find(GTreeNode<T>* node, GTreeNode<T>* obj) const
    {
        GTreeNode<T>* ret = NULL;

        if ( node == obj )
        {
            return node;
        }
        else
        {
            if ( NULL != node )
            {
                for(node->child.move(0); (!node->child.end()) && (NULL == ret); node->child.next())
                {
                    ret = find(node->child.current(), obj);
                }
            }
        }

        return ret;
    }


    void free(GTreeNode<T>* node)
    {
        if ( NULL != node )
        {
            for ( node->child.move(0); !node->child.end(); node->child.next() )
            {
                free(node->child.current());
            }

            // delete node;    // 这里不能简单粗暴地删除结点，一旦node在栈上就会有问题,第55课讲解这个问题

            if( node->flag() )
            {
                delete node;
            }
            //else
            //{
            //    cout << node->value << endl;
            //}
        }
    }

                    // 待删除结点      需要返回的子树
    void remove(GTreeNode<T>* node, GTree<T>*& ret)
    {
        ret = new GTree<T>();

        if ( NULL == ret )
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree ...");
        }
        else
        {
            if ( root() == node )
            {
                this->m_root = NULL;
            }
            else
            {
                // 把当前结点从它的父结点中删除掉
                LinkList<GTreeNode<T>*>& child = dynamic_cast<GTreeNode<T>*>(node->parent)->child;

                child.remove(child.find(node));

                node->parent = NULL;    // 没有把待删除结点直接删除, 放到后面等待返回
            }

            ret->m_root = node;
        }
    }


    int count(GTreeNode<T>* node) const
    {
        int ret = 0;

        if ( NULL != node )
        {
            ret = 1;

            for (node->child.move(0); !node->child.end(); node->child.next())
            {
                ret += count(node->child.current());
            }
        }

        return ret;
    }

    int height(GTreeNode<T>* node) const
    {
        int ret = 0;

        if ( NULL != node )
        {
            for (node->child.move(0); !node->child.end(); node->child.next())
            {
                int h = height(node->child.current());

                if ( ret < h )
                {
                    ret = h;
                }
            }
            ret = ret + 1;
        }

        return ret;
    }


    int degree(GTreeNode<T>* node) const
    {
        int ret = 0;

        if ( NULL != node )
        {
            ret = node->child.length();     // 拥有子结点的数目

            for (node->child.move(0); !node->child.end(); node->child.next())
            {
                int d = degree(node->child.current());

                if ( ret < d )
                {
                    ret = d;        // ret是度数中的最大值
                }
            }
        }

        return ret;
    }


public:
    GTree()
    {

    }


    // 插入结点
    bool insert(TreeNode<T>* node)
    {
        bool ret = true;

        if ( NULL != node )
        {
            // 判断当前树是不是空树, 是的话当前结点就是根结点
            if( NULL == this->m_root )
            {
                node->parent = NULL;
                this->m_root = node;
            }
            else
            {
                GTreeNode<T>* np = find(node->parent);  // 插入结点的父结点是不是在当前树中

                if ( NULL != np )   // 父结点在当前树中，正常插入
                {
                    // np->child.insert();      // 不要直接插入
                    // 先判断待插入结点是否已经存在树中
                    GTreeNode<T>* n = dynamic_cast<GTreeNode<T>*>(node);

                    if ( np->child.find(n) < 0 )        // 不再使用上面find函数，查找子结点中有没有当前结点，没有的话再插入
                    {
                        np->child.insert(n);
                    }
                    else
                    {
                        THROW_EXCEPTION(InvalidParameterException, "Current Parameter in current Tree ... ");
                    }
                }
                else                // 父结点不在当前树中，不插入
                {
                    printf("not111!!!!");
                    THROW_EXCEPTION(InvalidOperationException, "Invalid parent tree node ...");
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter node cannot be NULL ... ");
        }

        return ret;
    }

    bool insert(const T& value, TreeNode<T>* parent)
    {
        bool ret = true;
        // GTreeNode<T>* node = new GTreeNode<T>();
        GTreeNode<T>*  node = GTreeNode<T>::NewNode();

        if ( NULL != node )
        {
            node->value  = value;
            node->parent = parent;

            insert(node);
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree node ...");
        }


        return ret;
    }

    // 删除结点1：基于删除数据元素值的删除
    SharedPointer< Tree<T> > remove(const T& value)
    {
        GTree<T>* ret = NULL;
        GTreeNode<T>* node = find(value);   // 根据待删除值找到对应结点


        if ( NULL == node )
        {
            THROW_EXCEPTION(InvalidParameterException, "Can not find the node via parameter value ...");
        }
        else
        {
            remove(node, ret);
            m_queue.clear();        // 只要删除结点，遍历队列就清空
        }


        return ret;
    }

    // 删除结点2：基于结点的删除
    SharedPointer< Tree<T> > remove(TreeNode<T>* node)
    {
        GTree<T>* ret = NULL;

        node = find(node);          // 确认待删除结点是否在当前树中

        if ( NULL == node )
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter node is invalid ...");
        }
        else
        {
            // ☆☆☆ 要将指向父类的指针 转换成 指向子类的指针 △△△
            remove(dynamic_cast<GTreeNode<T>*>(node), ret);

            m_queue.clear();        // 只要删除结点，遍历队列就清空
        }

        return ret;
    }


    // 查找结点1：基于数据元素值的查找
    GTreeNode<T>* find(const T& value) const    // 赋值兼容性原则
    {
        return find(root(), value);
    }

    // 查找结点2：基于结点的查找
    GTreeNode<T>* find(TreeNode<T>* node) const
    {
        return find(root(), dynamic_cast<GTreeNode<T>*>(node));
    }

    // 根结点访问函数
    GTreeNode<T>* root() const
    {
        // 这里进行强制类型转换, 将TreeNode类型的m_root转换为GTreeNode
        return dynamic_cast<GTreeNode<T>*>(this->m_root);   // m_root是TreeNode类型
    }

    // 树的度访问函数
    int degree() const
    {
        return degree(root());
    }

    // 树的高度访问函数
    int height() const
    {
        return height(root());
    }

    // 树的结点数目访问函数
    int count() const
    {
        return count(root());
    }


    // 清空树
    void clear()
    {
        free(root());

        this->m_root = NULL;

        m_queue.clear();        // 树的结点遍历队列也清除掉
    }


    /* 游标思想：for循环遍历三件套 */
    bool begin()
    {
        bool ret = (root() != NULL);

        if ( ret )
        {
            m_queue.clear();                // 保证队列为空
            m_queue.add(root());
        }

        return ret;
    }

    bool end()
    {
        return (m_queue.length() == 0);     // 队列长度为0，代表遍历结束
    }

    bool next()
    {
        bool ret = (m_queue.length() > 0);

        if ( ret )
        {
            GTreeNode<T>* node = m_queue.front();   // 找到队首元素

            m_queue.remove();               // 取出队首元素

            for ( node->child.move(0); !node->child.end(); node->child.next() )
            {
                m_queue.add(node->child.current());     // LinkList的current，返回的是链表的值
            }
        }

        return ret;
    }


    T current()     // 返回树结点当前的值
    {
        if ( !end() )
        {
            return m_queue.front()->value;      // m_queue为LinkQueue队列，元素为<GTreeNode<T>*>
        }
        else
        {   // 遍历没开始或者已经结束的情形
            THROW_EXCEPTION(InvalidOperationException, "No value at current position ...");
        }
    }


    ~GTree()
    {
        clear();
    }

};


}


#endif // _GTREE_H_
