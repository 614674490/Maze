/*
 * @Author: liguiyin
 * @Date: 2019-12-20 15:36:18
 * @LastEditTime : 2019-12-28 20:38:42
 * @Description: README
 * @FilePath: \Maze\include\config.h
 */
#ifndef _config_h
#define _config_h

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//�������״̬����
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASLIBLE -1 //�����е�
#define OVERFLOW -2    //���
//Status ��������ֵ����
typedef int Status;

void printTip(char *str);   //��ʾ��Ϣ��ӡ
void printError(char *str); //������Ϣ��ӡ

#endif
