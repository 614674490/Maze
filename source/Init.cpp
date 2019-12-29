/*
 * @Author: liguiyin
 * @Date: 2019-12-27 09:12:05
 * @LastEditTime : 2019-12-29 00:02:30
 * @Description: README
 * @FilePath: \Maze\source\Init.c
 */
#include <Windows.h>
#include "Init.h"
int GETRANDMAZE;    //0->�涨�Թ� 1->����Թ�
int MAXROW ;        //�Թ���� 80*80
int MAXCOL ;        //�Թ�����
void PrintInit()
{
    printf("\t*************************************************************\n");
    printf("\t*                                                           *\n");
    printf("\t*                   ��ӭ�����Թ����ϵͳ                    *\n");
    printf("\t*                                                           *\n");
    printf("\t*                                ����ߣ����ӡ��������     *\n");
    printf("\t*************************************************************\n");
    printf("\t*                 ����������Թ�   �밴��1                  *\n");
    printf("\t*                 ��ʹ�ù̶��Թ�   �밴��2                  *\n");
    printf("\t*                 ��ʱ�临�ӶȲ��� �밴��3                  *\n");
    printf("\t*                 ���˳�           �밴��4                  *\n");
    printf("\t*************************************************************\n");
}
Status GetInput(void)
{
	int flag;
	char k[20];
	char row[20];
	char col[20];
	printf("�����룺");
	do{
		scanf("%s",k);
		if(strlen(k)>1)
			k[0] = 0;
		flag=0 ;
		switch(k[0])
        {
            case '1':
                Sleep(400);
                system("cls");
                PrintInit();
                GETRANDMAZE = 1;
	    		printf("�밴��ʽ���룺MAXROW MAXCOL(4*4����80*80)\n");
	    		do{
	    			scanf("%s",row);
	    			scanf("%s",col);
	    			MAXROW = atoi(row);
	    			MAXCOL = atoi(col);
	    			if(!(MAXROW >= 4 && MAXCOL >= 4 && MAXROW <= 80 && MAXCOL <= 80))
	    				printf("\n�������ݷǷ�������������:  ");
	    		} while (!(MAXROW >= 4 && MAXCOL >= 4 && MAXROW <= 80 && MAXCOL <= 80));
	    		Sleep(400);
                system("cls");
	    		flag = 1;
				TestMode = 0;
				break;
            case '2':
                flag = 1;
                Sleep(300);
                system("cls");
                GETRANDMAZE = 0;
				MAXROW = 10;
				MAXCOL = 10;
				TestMode = 0;
				break;
			case '3':
				flag = 1;
				GETRANDMAZE = 1;
				TestMode = 1;	//����ʱ�临�ӶȱȽ�ģʽ
				Sleep(300);
				break;
            case '4':
			    return FALSE;
            default:
                printf("\n�������ݷǷ�������������:  ");
	    		break;
	    }
	}while(!flag);
	return TRUE;
}
/**
 * @brief:        ѯ���Ƿ���Ҫ���в���
 * @param[in]:    null
 * @return:       Status
 * @note:         null
 */
Status conOperate(void)
{
	char c;
	printf("���Ƿ����ִ�д�ϵͳ(Y/N)?  ");
	do{
		scanf("%c", &c);
		if(!(c=='Y'||c=='N'||c=='\n'))
        	printf("\n�������ݷǷ�������������:  ");
	}while(!(c=='Y'||c=='N'));
	getchar();
	if(c=='Y')
	{
		Sleep(300);
		system("cls");
		return TRUE;
	}
	else
		return FALSE;
}
