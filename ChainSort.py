def chain_sort(array):
    n = len(array)
    check = [False] * n                      #标记某个位置是否在最终交换环中已被处理
    chain = [[-1, i, -1] for i in range(n)]  #一个二维数组，每一行对应一个链表节点，存储了“前驱 / 原下标 / 后继”，后期还会用前驱指针存储“目标下标”
    len_chain = 0
    head = 0        #最小元素对应节点下标
    end = 0         #最大元素对应节点下标
    mid = 0         #中值元素对应节点下标
    #1. 构建有序链表
    for i in range(n):
        if len_chain == 0:
            len_chain += 1
            continue
        #分情况插入：大于尾、 小于头、 介于中间
        if array[i] >= array[chain[end][1]]:
            chain[i][0] = end           #新节点的前驱 = 旧尾
            chain[i][2] = -1            #新节点后继 = -1
            chain[end][2] = i           #旧尾的后继 = 新节点
            end = i                     #更新尾指针
            mid = chain[mid][2]         #mid 往后移动一位
        elif array[i] <= array[chain[head][1]]:
            chain[i][0] = -1
            chain[i][2] = head
            chain[head][0] = i
            head = i
            mid = chain[mid][0]
        else:
            #介于头和尾之间，与 mid 比较大小
            if array[i] >= array[chain[mid][1]]:    #比mid大 从 mid 向后遍历
                pointer = mid
                while pointer != -1 and array[i] >= array[chain[pointer][1]]:
                    pointer = chain[pointer][2]
                #在 pointer 前面插入新节点
                chain[i][0] = chain[pointer][0]
                chain[i][2] = pointer
                chain[chain[pointer][0]][2] = i
                chain[pointer][0] = i
                mid = chain[mid][2]
            else:                                   #比mid小 从 head 向后遍历
                pointer = head
                while pointer != -1 and array[i] >= array[chain[pointer][1]]:
                    pointer = chain[pointer][2]
                #在 pointer 前面插入新节点
                chain[i][0] = chain[pointer][0]
                chain[i][2] = pointer
                chain[chain[pointer][0]][2] = i
                chain[pointer][0] = i
                mid = chain[mid][0]
        len_chain += 1
    #2. 为链表节点分配最终目标位置
    pointer = head
    while len_chain > 0:
        #将元素的前驱指针指向它在原数组中移向的下标
        chain[pointer][0] = len_chain-1 #右值为n-len_chain时，升序排序，右值为len_chain-1时，降序排序
        pointer = chain[pointer][2]
        len_chain -= 1

    for i in range(n):
        chain[chain[i][0]][2] = i         #将元素的后继指针指向将要移动到它位置上元素的下标 
    #4. 利用循环置换完成排序
    for i in range(n):
        #待移动的元素可能会形成多个独立的循环
        #通过判断元素是否已被移动，进入循环,并在循环置换过程中将元素下标标记为已有序
        if not check[i]:
            #记录循环开始位置的元素大小
            temp = array[chain[i][1]]
            #循环置序
            pointer = chain[i][2]
            while pointer!=i:
                check[chain[pointer][0]] = True
                array[chain[pointer][0]] = array[chain[pointer][1]]
                pointer = chain[pointer][2]
            #将首元素放到正确位置
            check[chain[pointer][0]] = True
            array[chain[pointer][0]] = temp
    return array

# 测试示例
if __name__ == "__main__":
    arr = [2, 4, 1, 5, 8, 7, 3, 9, 9, 9, 11]
    arr = print("排序前:", arr)
    chain_sort(arr)
    print("排序后:", arr)
