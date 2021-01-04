#ifndef _SORT_H_
#define _SORT_H_


#include "Object.h"
#include "Array.h"


namespace JMLib
{

class Sort : public Object
{
private:
    Sort();
    Sort(const Sort&);
    Sort& operator= (const Sort&);

    template < typename T >
    static void Swap(T& a, T& b)
    {
        T c(a);
        a = b;
        b = c;
    }


    // 真正的归并操作
    template< typename T >
    static void Merge(T src[], T helper[], int begin, int mid, int end, bool min2max = true)
    {
        int i = begin;
        int j = mid + 1;
        int k = begin;      // 辅助空间的起始位置

        while( (i <= mid) && (j <= end) )
        {
            if( min2max ? (src[i] < src[j]) : (src[i] > src[j]) )
            {
                helper[k++] = src[i++];     // 把原数组中的数据放到辅助空间中
            }
            else
            {
                helper[k++] = src[j++];
            }
        }

        while( i <= mid )   // 左边一路还有剩余元素没有拷贝到辅助空间中
        {
            helper[k++] = src[i++];
        }

        while( j <= end )
        {
            helper[k++] = src[j++];
        }

        for(i=begin; i<=end; i ++)
        {
            src[i] = helper[i];
        }
    }

    template< typename T >
    static void Merge(T src[], T helper[], int begin, int end, bool min2max = true)
    {
        // if( begin == end )      // 只有一个元素就是出口
        //{
        //    return;
        //}
        //else
        //{
        if( begin < end )
        {
            int mid = (begin + end) / 2;    // 中间位置

            Merge(src, helper, begin, mid, min2max);
            Merge(src, helper, mid+1, end, min2max);
            Merge(src, helper, begin, mid, end, min2max);
        }
        //}
    }


    // 快速排序中的基准函数
    template< typename T >
    static int Partition(T array[], int begin, int end, bool min2max=true)
    {
        T pv = array[begin];        // 选出基准

        while( begin < end )
        {
            while( (begin < end) && (min2max ? (array[end] > pv) : (array[end] < pv)) )
            {
                end --;
            }

            Swap(array[begin], array[end]);

            while( (begin < end) && (min2max ? (array[begin] <= pv) : (array[begin] >= pv) ) )
            {
                begin ++;
            }

            Swap(array[begin], array[end]);
        }

        array[begin] = pv;

        return begin;
    }


    template< typename T >
    static void Quick(T array[], int begin, int end, bool min2max=true)
    {
        if( begin < end )
        {
            int pivot = Partition(array, begin, end, min2max);

            Quick(array, begin, pivot-1, min2max);
            Quick(array, pivot+1, end, min2max);
        }
    }


public:
    // 1、选择排序
    template< typename T >
    static void Select(T array[], int len, bool min2max=true)       // O(n²)
    {
        for(int i=0; i<len; i ++)
        {
            int min=i;

            for(int j=i+1; j<len; j ++)
            {
                if( min2max ? (array[min] > array[j]) : (array[min] < array[j]) )
                {
                    min = j;        // 记录最小值下标
                }
            }

            if(min != i)
            {
                Swap(array[i], array[min]);
            }
        }
    }


    // 2、插入排序
    template< typename T >
    static void Insert(T array[], int len, bool min2max=true)       // O(n²)
    {
        for(int i=1; i<len; i ++)
        {
            int k = i;
            T e = array[i];     // 先取出带插入的数据元素

            for(int j=i-1; (j>=0) && (min2max ? (array[j]>e) : (array[j]<e)); j --)
            {
                //if( array[j] > e )
                //{
                array[j+1] = array[j];
                k  = j;     // 记录这个位置，很可能空出来的这个位置就是 待插入元素的 位置
                //}
                //else
                //{
                //    break;
                //}
            }

            if( k != i )        // 比较操作不耗时
            {
                array[k] = e;   // 赋值操作耗时
            }
        }
    }


    // 3、冒泡排序
    template< typename T >
    static void Bubble(T array[], int len, bool min2max=true)
    {
        bool exchange = true;       // 可以提前发生截断, 终止不需要的遍历

        for(int i=0; (i<len) && exchange; i ++)
        {
            exchange = false;

            for(int j=len-1; j>i; j --)
            {
                if( min2max ? (array[j] < array[j-1]) : (array[j] > array[j-1]) )
                {
                    Swap(array[j], array[j-1]);
                    exchange = true;
                }
            }
        }
    }


    // 4、希尔排序
    template< typename T >
    static void Shell(T array[], int len, bool min2max=true)
    {
        int d = len;

        do
        {
            d = d / 3 + 1;      // 实践证明，这样减小d比较好, d --

            for(int i=d; i<len; i += d)
            {
                int k = i;
                T e = array[i];

                for(int j=i-d; (j>=0) && (min2max ? (array[j]>e) : (array[j]<e)); j -= d)
                {
                    array[j+d] = array[j];
                    k = j;
                }

                if( k != i )
                {
                    array[k] = e;
                }
            }

        }while(d > 1);
    }


    // 5、希尔冒泡排序
    template< typename T >
    static void ShellBubble(T array[], int len, bool min2max=true)
    {
        int d = len;
        // bool exchange = true;

        do
        {
            d = d / 3 + 1;      // 实践证明，这样减小d比较好, d --

            for(int i=d; (i<len); i+=d)
            {
                // exchange = false;

                for(int j=len-d; j>=i; j -= d)
                {
                    if( min2max ? (array[j] < array[j-d]) : (array[j] > array[j-d]) )
                    {
                        Swap(array[j], array[j-d]);
                        // exchange = true;
                    }
                }
            }

        }while(d > 1);
    }


    // 6、二路归并排序
    template< typename T >
    static void Merge(T array[], int len, bool min2max=true)
    {
        T* helper = new T[len];

        if( NULL != helper )
        {
            Merge(array, helper, 0, len-1, min2max);
        }

        delete[] helper;
    }


    // 7、快速排序
    template< typename T >
    static void Quick(T array[], int len, bool min2max=true)
    {
        Quick(array, 0, len-1, min2max);
    }



    /* 以下是数组类对象的排序函数定义 */
    template< typename T >
    static void Select(Array<T>& array, bool min2max=true)
    {
        Select(array.array(), array.length(), min2max);
    }

    template< typename T >
    static void Insert(Array<T>& array, bool min2max=true)
    {
        Insert(array.array(), array.length(), min2max);
    }

    template< typename T >
    static void Bubble(Array<T>& array, bool min2max=true)
    {
        Bubble(array.array(), array.length(), min2max);
    }

    template< typename T >
    static void Shell(Array<T>& array, bool min2max=true)
    {
        Shell(array.array(), array.length(), min2max);
    }

    template< typename T >
    static void Merge(Array<T>& array, bool min2max=true)
    {
        Merge(array.array(), array.length(), min2max);
    }

    template< typename T >
    static void Quick(Array<T>& array, bool min2max=true)
    {
        Quick(array.array(), array.length(), min2max);
    }
};

}

#endif // _SORT_H_
