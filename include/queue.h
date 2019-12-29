/*
 * @Author: liguiyin
 * @Date: 2019-12-23 17:52:40
 * @LastEditTime : 2019-12-28 21:54:20
 * @Description: README
 * @FilePath: \Maze\include\queue.h
 */
#ifndef _queue_h
#define _queue_h
#include "config.h"
#include "maze.h"

#define MAXSIZE 1000
#define MAXLEN 100        //���г�ʼ����
#define MAXADD 10       //���������ڴ�

//������غ���
Status InitQueue(Queue *Q);
int getLength(Queue Q);
Status QueueuIsFull(Queue Q);
Status QueueuIsEmpty(Queue Q);
Status EnQueue(Queue *Q, QElemType E);
Status DeQueue(Queue *Q, QElemType *E);
#endif
