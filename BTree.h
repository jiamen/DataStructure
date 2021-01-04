#ifndef _BTREE_H_
#define _BTREE_H_


#include "Tree.h"
#include "BTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"
#include "DynamicArray.h"


#include <iostream>
using namespace std;


namespace JMLib
{


enum BTTraversal
{
    PreOrder,
    InOrder,
    PostOrder,
    LevelOrder
};


// GTree为通用树结构，每个结点可以存在多个后继结点
template <typename T>

class BTree : public Tree<T>
{
protected:
    LinkQueue<BTreeNode<T>*> m_queue;


    virtual BTreeNode<T>* find(BTreeNode<T>* node, const T& value) const
    {
        BTreeNode<T>* ret = NULL;

        if ( NULL != node )
        {
            if ( node->value == value )     // 看根结点是不是
            {
                ret = node;
            }
            else
            {
                if ( NULL == ret )          // 查看左子树是不是
                {
                    ret = find(node->left, value);
                }

                if ( NULL == ret )
                {
                    ret = find(node->right, value);
                }
            }
        }

        return ret;
    }

    virtual BTreeNode<T>* find(BTreeNode<T>* node, BTreeNode<T>* obj) const
    {
        BTreeNode<T>* ret = NULL;

        if ( node == obj )          // 直接比较地址，地址相同就是同一个结点
        {
            ret = node;
        }
        else
        {
            if( NULL != node )      // node与obj不符且node不为空的情况下，依次查找左右子树
            {
                if ( NULL == ret )
                {
                    ret = find(node->left, obj);
                }
                if ( NULL == ret )
                {
                    ret = find(node->right, obj);
                }
            }
        }

        return ret;
    }


    virtual bool insert(BTreeNode<T>* n, BTreeNode<T>* np, BTNodePos pos)
    {
        bool ret = true;

        if ( pos == ANY )
        {
            if ( NULL == np->left )
            {
                np->left = n;
            }
            else if ( NULL == np->right )
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }
        }
        else if ( pos == LEFT )
        {
            if ( NULL == np->left )
            {
                np->left = n;
            }
            else
            {
                ret = false;
            }
        }
        else if ( pos == RIGHT )
        {
            if ( NULL == np->right )
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = false;
        }

        return ret;
    }

    virtual void remove(BTreeNode<T>* node, BTree<T>*& ret)
    {
        ret = new BTree<T>();

        if ( NULL == ret )
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory to create new tree ...");
        }
        else
        {
            if ( root() == node )
            {
                this->m_root = NULL;
            }
            else
            {
                BTreeNode<T>* parent = dynamic_cast<BTreeNode<T>*>(node->parent);   // 找到待删除结点的右结点

                if ( parent->left == node )
                {
                    parent->left = NULL;
                }
                else if ( parent->right == node )
                {
                    parent->right = NULL;
                }

                node->parent = NULL;            // 清除node的父子关系
            }

            ret->m_root = node;         // 保留删除子树，删除子树的根结点就是删除结点
        }
    }


    virtual void free(BTreeNode<T>* node)
    {
        if ( NULL != node )
        {
            free(node->left);       // 递归删除左子树
            free(node->right);      //        右子树

            // cout << node->value << endl;

            if ( node->flag() )     // 是堆空间的就直接释放
            {
                delete node;
            }
        }
    }

    int count(BTreeNode<T>* node) const
    {
        /*int ret = 0;

        if ( NULL != node )
        {
            ret = count(node->left) + count(node->right) + 1;
        }

        return ret;*/

        return (NULL != node) ? (count(node->left) + count(node->right) + 1) : 0;
    }

    int height(BTreeNode<T>* node) const
    {
        int ret = 0;

        if ( NULL != node )
        {
            int left_height  = height(node->left);
            int right_height = height(node->right);

            ret = ((left_height > right_height) ? left_height : right_height) + 1;
        }

        return ret;
    }

    int degree(BTreeNode<T>* node) const
    {
        int ret = 0;

        if ( NULL != node )
        {
            BTreeNode<T>* child[] = { node->left, node->right };
            // 二叉树的度最大为2，一旦出现度为2的结点，直接停掉递归
            ret = (!!node->left + !!node->right);

            for ( int i=0; (i<2) && (ret<2); i ++ )
            {
                int d = degree(child[i]);

                if ( ret < d )
                {
                    ret = d;
                }
            }


            /*
            int left_degree  = degree(node->left);
            int right_degree = degree(node->right);

            ret = (!!node->left + !!node->right);       // ret=2
            // 非0值两次取反得到1，0值两次取反得到0

            if ( ret < left_degree )
            {
                ret = left_degree;
            }

            if ( ret < right_degree )
            {
                ret = right_degree;
            }*/


        }

        return ret;
    }


    void PreOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if ( NULL != node )
        {
            queue.add(node);                        // 把根结点加入队列
            PreOrderTraversal(node->left,  queue);   // queue用来保存遍历次序
            PreOrderTraversal(node->right, queue);
        }
    }

    void InOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if ( NULL != node )
        {
            InOrderTraversal(node->left,  queue);
            queue.add(node);                    // 把根结点加入队列
            InOrderTraversal(node->right, queue);
        }
    }

    void PostOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if ( NULL != node )
        {
            PostOrderTraversal(node->left,  queue);
            PostOrderTraversal(node->right, queue);
            queue.add(node);                    // 把根结点加入队列
        }
    }

    void LevelOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if( NULL != node )
        {
            LinkQueue<BTreeNode<T>*> tmp;

            tmp.add(root());

            while( tmp.length() >0 )
            {
                BTreeNode<T>* n = tmp.front();

                if( NULL != n->left )
                {
                    tmp.add(n->left);
                }

                if( NULL != n->right )
                {
                    tmp.add(n->right);
                }

                tmp.remove();       // 删除头结点
                queue.add(n);       // 添加到新队列中
            }
        }
    }


    BTreeNode<T>* clone(BTreeNode<T>* node) const
    {
        BTreeNode<T>* ret = NULL;

        if( NULL != node )
        {
            ret = BTreeNode<T>::NewNode();

            if( NULL != node )
            {
                ret->value = node->value;
                ret->left  = clone(node->left);
                ret->right = clone(node->right);

                // 指定左右子结点的父结点
                if( NULL != ret->left )
                {
                    ret->left->parent = ret;
                }
                if( NULL != ret->right )
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Enough Memory to create new code ...");
            }
        }

        return ret;
    }


    // 判断两个二叉树相不相等
    bool equal(BTreeNode<T>* lh, BTreeNode<T>* rh) const
    {
        if( lh == rh )
        {
            return true;
        }
        else if( (NULL != lh) && (NULL != rh) )
        {
            return (lh->value == rh->value) && (equal(lh->left, lh->left)) && (equal(lh->right, lh->right));
        }
        else
        {
            return false;       // 有一颗子树为0的情况
        }
    }


    BTreeNode<T>* add(BTreeNode<T>* lh, BTreeNode<T>* rh) const
    {   // 需要相加的两棵二叉树的根结点
        BTreeNode<T>* ret = NULL;

        if( (NULL == lh) && (NULL != rh ) )
        {
            ret = clone(rh);
        }
        else if( (NULL != lh) && (NULL == rh ) )
        {
            ret = clone(lh);
        }
        else if( (NULL != lh) && (NULL != rh ) )
        {
            ret = BTreeNode<T>::NewNode();

            if( NULL != ret )
            {
                ret->value = lh->value + rh->value;

                ret->left  = add(lh->left,  rh->left);
                ret->right = add(lh->right, rh->right);


                // 把左右子树与根结点结合，形成父子关系
                if( NULL != ret->left )
                {
                    ret->left->parent = ret;
                }
                if( NULL != ret->right )
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Enough Memory to create BTreeNode<T>* to ADD ...");
            }
        }

        return ret;
    }


    void traversal(BTTraversal order, LinkQueue<BTreeNode<T>*>& queue)
    {
        switch (order)
        {
            case PreOrder:
                PreOrderTraversal(root(), queue);
                break;

            case InOrder:
                InOrderTraversal(root(),  queue);
                break;

            case PostOrder:
                PostOrderTraversal(root(), queue);
                break;

            case LevelOrder:
                LevelOrderTraversal(root(), queue);
                break;

            default:
                THROW_EXCEPTION(InvalidParameterException, "Parameter order is invalue ...");
                break;
        }
    }


    // 将传进来的queue连接成双向链表，并返回双向链表的首结点的地址
    BTreeNode<T>* connect(LinkQueue<BTreeNode<T>*>& queue)
    {
        BTreeNode<T>* ret = NULL;

        if( queue.length() > 0 )
        {
            ret = queue.front();

            BTreeNode<T>* slider = queue.front();

            queue.remove();

            slider->left = NULL;

            while( queue.length() > 0 )
            {
                slider->right = queue.front();      // front返回的是结点地址
                queue.front()->left = slider;
                slider = queue.front();
                queue.remove();
            }

            slider->right = NULL;
        }

        return ret;
    }


public:
    // 基于结点的插入
    bool insert(TreeNode<T>* node)
    {
        return insert(node, ANY);
    }

    // 基于结点的插入虚函数
    virtual bool insert(TreeNode<T>* node, BTNodePos pos)
    {
        bool ret = true;

        if ( NULL != node )
        {
            if ( NULL == this->m_root )     // 根结点
            {
                node->parent = NULL;
                this->m_root = node;
            }
            else
            {
                BTreeNode<T>* np = find(node->parent);      // 在当前树中找到插入结点的 父结点

                if ( NULL != np )
                {
                    ret = insert(dynamic_cast<BTreeNode<T>*>(node), np, pos);
                }
                else
                {
                    THROW_EXCEPTION(InvalidParameterException, "Invalid parent tree node ...");
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter node can not be NULL ...");
        }

        return ret;
    }

    // 基于元素值的插入
    bool insert(const T& value, TreeNode<T>* parent)
    {
        return insert(value, parent, ANY);
    }

    // 基于元素值的插入虚函数
    virtual bool insert(const T& value, TreeNode<T>* parent, BTNodePos pos)
    {
        bool ret = true;
        BTreeNode<T>* node = BTreeNode<T>::NewNode();

        if ( NULL == node )
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new node ...");
        }
        else
        {
            node->value  = value;
            node->parent = parent;

            ret = insert(node, pos);

            if ( !ret )     // 插入不成功的话，直接将申请的结点空间释放
            {
                delete node;
            }
        }

        return ret;
    }


    // 删除结点
    SharedPointer< Tree<T> > remove(const T& value)
    {
        BTree<T>* ret = NULL;
        BTreeNode<T>* node = find(value);

        if ( NULL == node )
        {
            THROW_EXCEPTION(InvalidParameterException, "Can not find the tree node via value ...");
        }
        else
        {
            remove(node, ret);

            m_queue.clear();           // ☆☆☆ 重中之重 △△△
        }

        return ret;
    }

    SharedPointer< Tree<T> > remove(TreeNode<T>* node)
    {
        BTree<T>* ret = NULL;

        node = find(node);

        if ( NULL == node )
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter node is invalid ...");
        }
        else
        {
            remove(dynamic_cast<BTreeNode<T>*>(node), ret);
        }

        return ret;
    }

    // 查找结点
    BTreeNode<T>* find(const T& value) const
    {
        return find(root(), value);
    }

    BTreeNode<T>* find(TreeNode<T>* node) const
    {                   // ↑ 指向父类的指针   ↓  指向子类的指针
        return find(root(), dynamic_cast<BTreeNode<T>*>(node));
    }

    // 根结点访问函数
    BTreeNode<T>* root() const
    {
        return dynamic_cast<BTreeNode<T>*>(this->m_root);
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




    /* 游标思想：for循环遍历三件套 */
    bool begin()
    {
        bool ret = (root() != NULL);

        if ( ret )
        {
            m_queue.clear();                // 保证队列为空
            m_queue.add(root());            // 先把根结点加入队列
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
            BTreeNode<T>* node = m_queue.front();   // 找到队首元素

            m_queue.remove();               // 取出队首元素

            //for ( node->child.move(0); !node->child.end(); node->child.next() )
            //{
            //    m_queue.add(node->child.current());     // LinkList的current，返回的是链表的值
            //}

            if ( NULL != node->left )
            {
                m_queue.add(node->left);
            }
            if ( NULL != node->right )
            {
                m_queue.add(node->right);
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


    // 清空树
    void clear()
    {
        free(root());

        m_queue.clear();

        this->m_root = NULL;
    }


    SharedPointer< Array<T> > traversal(BTTraversal order)
    {
        DynamicArray<T>* ret = NULL;        // 待返回的指定遍历次序的结点中的值
        LinkQueue<BTreeNode<T>*> queue;     // 保存指定遍历次序（先中后）的结点


        traversal(order, queue);


        ret = new DynamicArray<T>(queue.length());

        if ( NULL != ret )
        {
            // 把有遍历次序的结点中的值依次取出来，放到返回数组中
            for( int i=0; i<ret->length(); i ++, queue.remove() )
            {
                ret->set(i, queue.front()->value);
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create DynamicArray<T> ret ...");
        }

        return ret;
    }


    // 二叉树线索化
    BTreeNode<T>* thread(BTTraversal order)
    {
        BTreeNode<T>* ret = NULL;
        LinkQueue<BTreeNode<T>*> queue;

        traversal(order, queue);        // 遍历

        ret = connect(queue);                 // 线索化

        this->m_root = NULL;            // 线索化之后的二叉树成为双向链表，不再拥有一些函数性质，直接清空

        m_queue.clear();

        return ret;
    }


    SharedPointer< BTree<T> >  clone() const
    {
        BTree<T>* ret = new BTree<T>();

        if( NULL != ret )
        {
            ret->m_root = clone(root());
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree ... ");
        }

        return ret;
    }

    bool operator == (const BTree<T>& btree)
    {
        return equal(root(), btree.root());
    }

    bool operator != (const BTree<T>& btree)
    {
        return !(*this == btree);       // 这里调用上面重载的“==”操作符
    }


    SharedPointer< BTree<T> > add(const BTree<T>& btree) const
    {
        BTree<T>* ret = new BTree<T>();

        if( NULL != ret )
        {
            ret->m_root = add(root(), btree.root());        // 传入两棵树的根结点
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory to create new tree ...");
        }

        return ret;
    }


    ~BTree()
    {
        clear();
    }
};

}

#endif // _BTREE_H_
