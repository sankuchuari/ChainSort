#include<iostream>
using namespace std;

void ChainSort(int* array, int* lenArray)
{
    bool* Chack = new bool[*lenArray];             //标记某个位置是否在最终交换环中已被处理
    short(*Chain)[3] = new short[*lenArray][3];   //一个二维数组，每一行对应一个链表节点，存储了“前驱 / 原下标 / 后继”，后期还会用前驱指针存储“目标下标”
    short lenChain = 0; 
    short head = 0;      //最小元素对应节点下标
    short end = 0;        //最大元素对应节点下标
    short mid = 0;        //中值元素对应节点下标
    short pointer = -1; //临时遍历指针
    // 1. 构建有序链表
    for (int i = 0; i < *lenArray; i++)
    {
        Chack[i] = false;
        Chain[i][1] = i;// 第2列记录原数组下标
        if (lenChain == 0)// 链表空时，直接初始化
        {
            Chain[i][0] = -1;  // 前驱
            Chain[i][2] = -1;  // 后继
        }
        else
        {
            // 分情况插入：大于尾、 小于头、 介于中间
            if (array[i] >= array[Chain[end][1]])
            {
                // 插入到尾部
                Chain[i][0] = end;              // 新节点的前驱 = 旧尾
                Chain[i][2] = -1;                // 新节点后继 = -1
                Chain[end][2] = i;              // 旧尾的后继 = 新节点
                end = Chain[end][2];         // 更新尾指针
                mid = Chain[mid][2];         // mid 往后移动一位
            }
            else if (array[i] <= array[Chain[head][1]])
            {
                // 插入到头部
                Chain[i][0] = -1;
                Chain[i][2] = head;
                Chain[head][0] = i;
                head = Chain[head][0];
                mid = Chain[mid][0];          // mid 往前移动
            }
            else if (array[i] > array[Chain[head][1]])
            {
                // 介于头和尾之间，与 mid 比较大小
                if (array[i] >= array[Chain[mid][1]])
                {
                    // 比mid大 从 mid 向后遍历
                    pointer = mid;
                    while (array[i] >= array[Chain[pointer][1]])
                    {
                        pointer = Chain[pointer][2];
                    }
                    // 在 pointer 前面插入新节点
                    Chain[i][0] = Chain[pointer][0];
                    Chain[i][2] = pointer;
                    Chain[Chain[pointer][0]][2] = i;
                    Chain[pointer][0] = i;
                    mid = Chain[mid][2];
                }
                else
                {
                    // 比mid小 从 head 向后遍历
                    pointer = head;
                    while (array[i] >= array[Chain[pointer][1]])
                    {
                        pointer = Chain[pointer][2];
                    }
                    Chain[i][0] = Chain[pointer][0];
                    Chain[i][2] = pointer;
                    Chain[Chain[pointer][0]][2] = i;
                    Chain[pointer][0] = i;
                    mid = Chain[mid][0];
                }
            }
        }
        lenChain += 1;
    }
    // 2. 为链表节点分配最终目标位置
    pointer = head;
    while (lenChain != 0)
    {
        //将元素的前驱指针指向它在原数组中移向的下标
        Chain[pointer][0] = lenChain - 1; //右值为lenArray-lenChain时，升序排序，右值为lenChain-1时，降序排序
        pointer = Chain[pointer][2];
        lenChain -= 1;
    }
    // 3. 构建逆映射
    for (int i = 0; i < *lenArray; i++)
    {
        Chain[Chain[i][0]][2] = i;  //将元素的后继指针指向将要移动到它位置上元素的下标
    }
    // 4. 利用循环置换完成排序
    for (int i = 0; i < *lenArray; i++)
    {
        //待移动的元素可能会形成多个独立的循环
        //通过判断元素是否已被移动，进入循环
        //并在循环置换过程中将元素下标标记为已有序
        if (Chack[i] == false)
        {
            //记录循环开始位置的元素大小
            short TempNum = array[Chain[i][1]];
            //循环置序
            pointer = Chain[i][2];
            while (pointer != i)
            {
                Chack[Chain[pointer][0]] = true;
                array[Chain[pointer][0]] = array[Chain[pointer][1]];
                pointer = Chain[pointer][2];
            }
            //将首元素放到正确位置
            Chack[Chain[pointer][0]] = true;
            array[Chain[pointer][0]] = TempNum;
        }
    }
    // 5. 释放内存
    delete[] Chain;
    delete[] Chack;
}

void ShowArray(int* array, int *lenArray)
{
    for (int i = 0; i < *lenArray; i++)
    {
        cout << array[i] << endl;
    }
}

int main() {
    int arr[] = { 2,4,1,5,8,7,3,9,9,9,11 };
    int len = sizeof(arr) / sizeof(arr[0]);
    cout << "arr中的元素个数：" << len << endl;
    ChainSort(arr, &len);
    ShowArray(arr, &len);
    system("pause");
    return 0;
}
