#include <iostream>
#include <vector>
#include <array>
using namespace std;

void ChainSort(vector<int>& vec)
{
    int n = vec.size();
    if (n == 0) return;

    // 用 vector<bool> 标记哪些位置已处理
    vector<bool> check(n, false);
    // 用 vector 存储链表节点，每个节点保存 {前驱, 原下标, 后继}，初始均设为 -1
    vector<array<int, 3>> chain(n, { -1, -1, -1 });
    // 记录链表中最小、中值、最大节点的下标
    array<int, 3> subScript = { 0, 0, 0 };
    int pointer = -1;

    // 1. 构建有序链表
    for (int i = 0; i < n; i++)
    {
        chain[i][1] = i;  // 第二列记录原数组下标
        if (i == 0)
        {
            chain[i][0] = -1; // 前驱
            chain[i][2] = -1; // 后继
        }
        else
        {
            // 分情况插入：大于尾、小于头、介于中间
            if (vec[i] >= vec[chain[subScript[2]][1]])
            {
                // 插入到尾部
                chain[i][0] = subScript[2];  // 新节点前驱为旧尾
                chain[i][2] = -1;            // 后继为空
                chain[subScript[2]][2] = i;   // 更新旧尾后继为新节点
                subScript[2] = chain[subScript[2]][2]; // 更新尾指针
                subScript[1] = chain[subScript[1]][2]; // 中值下标下移
            }
            else if (vec[i] <= vec[chain[subScript[0]][1]])
            {
                // 插入到头部
                chain[i][0] = -1;
                chain[i][2] = subScript[0];
                chain[subScript[0]][0] = i;
                subScript[0] = chain[subScript[0]][0]; // 更新头指针
                subScript[1] = chain[subScript[1]][0]; // 中值下标上移
            }
            else if (vec[i] > vec[chain[subScript[0]][1]])
            {
                // 介于头和尾之间，与中值比较
                if (vec[i] >= vec[chain[subScript[1]][1]])
                {
                    // 大于中值，从中值向后遍历
                    pointer = subScript[1];
                    while (vec[i] >= vec[chain[pointer][1]])
                    {
                        pointer = chain[pointer][2];
                    }
                    chain[i][0] = chain[pointer][0];
                    chain[i][2] = pointer;
                    if (chain[pointer][0] != -1)
                        chain[chain[pointer][0]][2] = i;
                    chain[pointer][0] = i;
                    subScript[1] = chain[subScript[1]][2];  // 中值下标下移
                }
                else
                {
                    // 小于中值，从头部向后遍历
                    pointer = subScript[0];
                    while (vec[i] >= vec[chain[pointer][1]])
                    {
                        pointer = chain[pointer][2];
                    }
                    chain[i][0] = chain[pointer][0];
                    chain[i][2] = pointer;
                    if (chain[pointer][0] != -1)
                        chain[chain[pointer][0]][2] = i;
                    chain[pointer][0] = i;
                    subScript[1] = chain[subScript[1]][0];  // 中值下标上移
                }
            }
        }
    }

    // 2. 为链表节点分配最终目标位置
    pointer = subScript[0];
    for (int i = n; i > 0; i--)
    {
        // 将节点的前驱指针存储为该元素在最终数组中的目标下标
        chain[pointer][0] = i - 1; // 升序排序时，目标下标为 n-i
        pointer = chain[pointer][2];
    }

    // 3. 构建逆映射，将每个节点的后继指针指向将要移动到它位置上元素的下标
    for (int i = 0; i < n; i++)
    {
        chain[chain[i][0]][2] = i;
    }

    // 4. 利用循环置换完成排序
    for (int i = 0; i < n; i++)
    {
        if (!check[i])
        {
            int temp = vec[chain[i][1]];  // 保存循环开始位置的元素
            pointer = chain[i][2];
            while (pointer != i)
            {
                check[chain[pointer][0]] = true;
                vec[chain[pointer][0]] = vec[chain[pointer][1]];
                pointer = chain[pointer][2];
            }
            check[chain[pointer][0]] = true;
            vec[chain[pointer][0]] = temp;
        }
    }
}

void ShowArray(const vector<int>& vec)
{
    for (auto x : vec)
    {
        cout << x << endl;
    }
}

int main() {
    vector<int> arr = { 2, 4, 1, 5, 8, 7, 3, 9, 9, 9, 11 };
    ChainSort(arr);
    ShowArray(arr);
    system("pause");
    return 0;
}
