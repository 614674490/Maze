/*
 * @Author: liguiyin
 * @Date: 2019-12-23 17:52:05
 * @LastEditTime : 2019-12-24 20:01:54
 * @Description: README
 * @FilePath: \Maze\source\queue.c
 */
#include "queue.h"
/**
 * @brief   ��ʼ��ѭ������
 * @param[in]   ѭ������ָ��
 * @retval  Status
 **/
Status InitQueue(Queue *Q)
{
    Q->base=(QElemType *)malloc(sizeof(QElemType)*MAXLEN);
    if(!(Q->base))
    {
        printError((char *)"�ڴ����");
        return OVERFLOW;
    }
    Q->size = MAXLEN;
    Q->front = 0;
    Q->rear = 0;
    Q->step = 0;
    return OK;
}
/**
 * @brief   ����г���
 * @param[in]   ѭ������ָ��
 * @retval  ���г���
 **/
int getLength(Queue Q)
{
    return (Q.rear - Q.front + Q.size) % Q.size;;
}
/**
 * @brief   �ж���
 * @param[in]   ѭ������ָ��
 * @retval  ��������TRUE FALSE
 **/
Status  QueueuIsFull(Queue Q)
{
     if(((Q.rear+1)%Q.size) == Q.front)
        return TRUE;
     else
        return FALSE;
}
/**
 * @brief   �жӿ�
 * @param[in]   ѭ������ָ��
 * @retval  �ӿշ���TRUE ���򷵻�FALSE
 **/
Status  QueueuIsEmpty(Queue Q)
{
    if(Q.front==Q.rear)
        return TRUE;
     else
        return FALSE;
}
/**
 * @brief   ���
 * @param[in]   ѭ������ָ�� ������
 * @retval  Status
 **/
Status EnQueue(Queue *Q, QElemType E)
{
    if(QueueuIsFull(*Q))
    {
        printError((char *)"��������");
        Q->size += MAXADD;
        if(!(Q->base=(QElemType *)realloc(Q->base,sizeof(QElemType)*(Q->size))))//���������ڴ�
        {
            printError((char *)"�ڴ����");
            return OVERFLOW;
        }
    }
    Q->rear = (Q->rear+1)%Q->size;
    Q->base[Q->rear] = E;
    Q->step++;
    return OK;
}
/**
 * @brief   ����
 * @param[in]   ѭ������ָ�� ����Ԫ��
 * @retval  Status
 **/
Status DeQueue(Queue *Q, QElemType *E)
{
    if(QueueuIsEmpty(*Q))
    {
        printError((char *)"����Ϊ��");
        return ERROR;
    }
    else
    {
        Q->front = (Q->front + 1) % Q->size;
        *E = Q->base[Q->front];
        Q->step--;
        return OK;
    }
}
