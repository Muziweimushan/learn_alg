#ifndef __SORT_H__
#define __SORT_H__

#include "Object.h"
#include "Array.h"
namespace MyLib
{

class Sort : public Object
{
private:
	//无参构造函数、拷贝构造函数以及赋值操作符定义为private的,使得这个类不能定义对象
	Sort();
	Sort(const Sort &);
	Sort &operator = (const Sort &);

	template < typename T >
	static void Swap(T &a, T &b)
	{
		T c(a);
		a = b;
		b = c;
	}


	//实现2个有序序列的合并
	//while循环的运行次数为end-begin+1次, for循环也一样,所以时间复杂度为 O(n)线性阶的复杂度
	template < typename T >
	static void Merge(T src[], T helper[], int begin, int mid, int end, bool min2max = true)
	{
		int i = begin;
		int j = mid + 1;
		int k = begin;

		
		while ((i <= mid) && (j <= end))
		{
			if ((min2max) ? (src[i] < src[j]) : (src[i] > src[j]))
			{
				helper[k++] = src[i++];
			}
			else
			{
				helper[k++] = src[j++];
			}
		}

		while (i <= mid)
		{
			helper[k++] = src[i++];
		}

		while (j <= end)
		{
			helper[k++] = src[j++];
		}

		//以上3个while循环加起来运行次数必然等于 end - begin + 1次

		for (i = begin; i <= end; i++)
		{
			src[i] = helper[i];
		}
	}

	//将src数组元素进行二路归并排序, 范围从begin到end, helper是辅助空间
	template < typename T >
	static void Merge(T src[], T helper[], int begin, int end, bool min2max = true)
	{
		//只有一个元素的数组,就是递归出口
		if (end == begin)
		{
			return;
		}
		else
		{
			int mid = (end + begin) / 2;

			//分为2路分别进行排序,递归排序...
			Merge(src, helper, begin, mid, min2max);
			Merge(src, helper, mid + 1, end, min2max);

			//经过上述两个调用后,src中以mid为分界线, src[begin]到src[mid]是有序的
			//src[mid + 1] 到 src[end]也是有序的,接下来就是将两个有序的子序列进行合并
			Merge(src, helper, begin, mid, end, min2max);
		}
	}

	//用于快速排序中实现找基准和基准就位的功能,返回值就是基准就位后在序列中的下标
	template < typename T >
	static int Partition(T array[], int begin, int end, bool min2max)
	{
		T pv = array[begin];

		while (begin < end)
		{
			//先从end开始,比较end与基准的关键字,如果array[end] < 基准值,就交换begin和end对应的数据元素
			//否则就移动end
			while ((begin < end) & (min2max ? (pv < array[end]) : (pv > array[end])))
			{
				end--;//不停移动end
			}
			//找到一个比基准小的值,将它扔到基准的左侧
			if (begin < end)
				Swap(array[begin], array[end]);

			//这时候从begin开始找
			while ((begin < end) && (min2max ? (pv >= array[begin]) : (pv <= array[begin])))
			{
				begin++;//一直移动begin
			}

			//找到一个比基准大的值,扔到基准右侧
			//判断一哈begin和end是否相等了,如果是的话没必要交换了
			if (begin < end)
				Swap(array[begin], array[end]);

		}

		//我认为这里不需要将pv填到begin指向的位置啊
		//array[begin] = pv;
		//
		return begin;
	}

	//快排实际实现函数
	template < typename T >
	static void Quick(T array[], int begin, int end, bool min2max)
	{
		//大于一个继续,否则直接返回,也就是递归结束条件
		if (begin < end)
		{

			int pivot = Partition(array, begin, end, min2max);

			//对左右两侧子序列递归进行快排即可,因为pivot所指向元素已经就位,所以左侧下标为[begin, pivot)
			//右侧下标为 (pivot, end]
			Quick(array, begin, pivot - 1, min2max);
			Quick(array, pivot + 1, end, min2max);
		}
	}

public:
	//排序函数统一通过static成员函数方式对外提供
	//min2max为true时从小到大排序

	//选择排序
	template < typename T >
	static void Select(T array[], int len, bool min2max = true)
	{
		for (int i = 0; i < len; i++)
		{
			int min = i;
			
			//当前最小值是i,自然的就应该从i + 1位置开始比较
			for (int j = i + 1; j < len; j++)
			{
				//选择排序做法就是从第0个位置开始,每次都找出最小的元素,然后放置在从0开始的位置上
				//即第i轮找到的值放在第i位置上
				if (min2max ? (array[min] > array[j]) : (array[min] < array[j])) //当需要从大到小排序时改为小于号即可
				{
					min = j;
				}
				
			}
			//循环结束后找到了在i位置上的有序值
			//
			//避免无意义的交换,当泛指类型T是复杂类时交换操作效率会很低
			if (i != min)
			{
				Swap(array[i], array[min]);
			}
		}
	}

	//插入排序,稳定的排序
	template < typename T >
	static void Insert(T array[], int len, bool min2max = true)
	{
		//从第1个元素开始,因为第0个元素前面没有有序序列的,可以认为第0个元素在最开始就是有序的
		for (int i = 1; i < len; i++)
		{
			T e = array[i];
			int k = i;
			//在有序序列中从后往前找
			for (int j = i - 1; (j >= 0) && (min2max ? (array[j] > e) : (array[j] < e)); j--)
			{
				//if (array[j] > e)
				{
					//当前下标数据元素待插入元素大,向后挪一格
					array[j + 1] = array[j];
					k = j;
				}
#if 0
				else
				{
					//当前下标处数据元素小于等于i处元素,本次循环退出
					break;
				}
#endif
			}

			if (k != i)
			{
				array[k] = e;
			}
		}

	}

	template < typename T >
	static void Bubble(T *array, int len, bool min2max = true)
	{
		bool exchange = true;//标志位,每一轮如果存在交换操作则为true,当一轮操作完成后该值为false说明所有元素已就位

		for (int i = 0; (i < len) && exchange; i++)
		{
			exchange = false;

			for (int j = len - 1; j > i; j--)
			{
				if (min2max ? (array[j] < array[j - 1]) : (array[j] > array[j - 1]))	//如果后面值比前面值要大,说明是逆序的,就要交换一哈
				{
					exchange = true;

					Swap(array[j], array[j - 1]);
				}
			}

		}

	}

	template < typename T >
	static void Shell(T array[], int len, bool min2max = true)
	{
		int d = len;

		do
		{
			d = d / 3 + 1;

			for (int i = d; i < len; i += d)
			{
				T e = array[i];
				int k = i;
				//在有序序列中从后往前找
				for (int j = i - d; (j >= 0) && (min2max ? (array[j] > e) : (array[j] < e)); j -= d)
				{
					//if (array[j] > e)
					{
						//当前下标数据元素待插入元素大,向后挪一格
						array[j + d] = array[j];
						k = j;
					}
				}

				if (k != i)
				{
					array[k] = e;
				}
			}

		}while (d > 1);
	}


	template < typename T >
	static void Merge(T array[], int len, bool min2max = true)
	{
		T *helper = new T[len];

		if (helper)
		{
			Merge(array, helper, 0, len - 1, min2max);
		}

		delete[] helper;
	}

	template < typename T >
	static void Quick(T array[], int len, bool min2max = true)
	{
		Quick(array, 0, len - 1, min2max);
	}

	template < typename T >
	static void Select(Array<T> &array, bool min2max = true)
	{
		Select(array.array(), array.length(), min2max);
	}

	template < typename T >
	static void Insert(Array<T> &array, bool min2max = true)
	{
		Insert(array.array(), array.length(), min2max);
	}

	template < typename T >
	static void Bubble(Array<T> &array, bool min2max = true)
	{
		Bubble(array.array(), array.length(), min2max);
	}

	template < typename T >
	static void Shell(Array<T> &array, bool min2max = true)
	{
		Shell(array.array(), array.length(), min2max);
	}

	template < typename T >
	static void Merge(Array<T> &array, bool min2max = true)
	{
		Merge(array.array(), array.length(), min2max);
	}

	template < typename T >
	static void Quick(Array<T> &array, bool min2max = true)
	{
		Quick(array.array(), array.length(), min2max);
	}

};

}



#endif
