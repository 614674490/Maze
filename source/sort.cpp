/*
 * @Author: liguiyin
 * @Date: 2019-12-23 09:21:02
 * @LastEditTime : 2019-12-23 09:40:06
 * @Description: README
 * @FilePath: \Maze\source\sort->c
 */
#include "sort.h"
/**
 * @brief:        �����������㷨1
 * @param[in]:    ����� low high
 * @return:       ��������λ��
 * @note:         һ�˿�������
 */
int Partition(SeaList S[], int low, int high, sortOrder sortorder)
{
    KeyType pivotkey = S[low]->step;
    S[0] = S[low]; //�ݴ������¼
    while (low < high)     //�ӱ�����˽������м�ɨ��
    {
        while (low < high && (sortorder == ascending ? S[high]->step >= pivotkey : S[high]->step <= pivotkey))
            high--;
        S[low] = S[high]; //���������¼С�ļ�¼�Ƶ��Ͷ�
        while (low < high && (sortorder == ascending ? S[low]->step <= pivotkey : S[low]->step >= pivotkey))
            low++;
        S[high] = S[low]; //���������¼��ļ�¼�Ƶ��߶�
    }
    S[low] = S[0];
    return low;
}
/**
 * @brief:        �����������㷨2
 * @param[in]:    �����
 * @return:       null
 * @note:         ʱ�����ܣ����:O(nlogn) �:O(n^2)  �ռ����ܣ�O(logn)
 *                �ȶ����ܣ����ȶ�
 */
void QSort(SeaList S[], int low, int high, sortOrder sortorder)
{
    int pivotc;
    if (low < high)
    {
        pivotc = Partition(S, low, high, sortorder); //���ݹ��ӱ�ֳ�������
        QSort(S, low, pivotc - 1, sortorder);        //����
        QSort(S, pivotc + 1, high, sortorder);       //����
    }
}
