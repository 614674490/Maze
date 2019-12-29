/*
 * @Author: liguiyin
 * @Date: 2019-12-28 16:08:36
 * @LastEditTime : 2019-12-28 16:19:42
 * @Description: README
 * @FilePath: \Maze_V2.0\source\generate.c++
 */
#include "maze.h"
#include "time.h"
#include <iostream>
#include<vector>
using namespace std;
//�Թ����� 0->�ϰ� -1->ͨ��
int mazeRandMap[MAX][MAX] = {0};
int MG[MAX][MAX] =
    {
        {0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        {0, -1, -1,  0, -1, -1, -1,  0, -1,  0},
        {0, -1, -1,  0, -1, -1, -1,  0, -1,  0},
        {0, -1, -1, -1, -1,  0,  0, -1, -1,  0},
        {0, -1,  0,  0,  0, -1, -1, -1, -1,  0},
        {0, -1, -1, -1,  0, -1, -1, -1, -1,  0},
        {0, -1,  0, -1, -1, -1,  0, -1, -1,  0},
        {0, -1,  0,  0,  0, -1,  0,  0, -1,  0},
        {0,  0, -1, -1, -1, -1, -1, -1, -1,  0},
        {0,  0,  0,  0,  0,  0,  0,  0,  0,  0}};
//�����Թ���Ϣ
int tempMaze[MAX][MAX] ={0};
/**
 * @brief:        �Թ����ƺ���
 * @param[in]:    ԭ�Թ� Ŀ���Թ�
 * @return:       null
 * @note:         null
 */
void copyMazeMap(int mazemap[MAX][MAX],int tomazemap[MAX][MAX])
{
    int i,j;
    for(i=0;i<MAXROW;i++)
    {
        for(j=0;j<MAXCOL;j++)
        {
            tomazemap[i][j]=mazemap[i][j];
        }
    }
}
/**
 * @brief:        �Թ���ͼ��պ���
 * @param[in]:    map
 * @return:       null
 * @note:         null
 */
void clearMap(int map[MAX][MAX])
{
    for (int i = 0;i<MAX;i++)
        for (int j = 0; j < MAX;j++)
            map[i][j] = WALL;
}

/**
 * @brief:        ����Թ����ɺ���
 * @param[in]:    row col
 * @return:       �Թ���ά����ָ��
 * @note:         �ٷ�֮��ʮ��ǽ
 */
int (*mapRand(int i,int j))[MAX]
{
    int total;//ǽ������
    int m,n;//����ѭ������
    float scale=WALLRATE;//ǽ�ı���,�ٷ�֮��ʮ
    int count=0;//���������ڶ��Թ��������оֲ�������ʱ��ʹ��
    time_t t;//�趨ʱ������
    srand((unsigned)time(&t));

    for(m=0;m<i;m++)
        for(n=0;n<j;n++)
            mazeRandMap[m][n]=rand()%10;
    for(m=0;m<i;m++)//0����ǽ��-1����·
        for(n=0;n<j;n++)
            if(mazeRandMap[m][n]<5)
                mazeRandMap[m][n]=0;
            else
                mazeRandMap[m][n]=-1;
    //����ǽ��·�ı���
    for(n=0;n<j;n++)
    {
        mazeRandMap[0][n]=0;
        mazeRandMap[i-1][n]=0;
    }
    for(m=1;m<i-1;m++)
    {
        mazeRandMap[m][0]=0;
        mazeRandMap[m][j-1]=0;
    }
    //����Χ��Ϊǽ����ֹ����
    for(m=1;m<i-1;m++)
        for(n=1;n<j-1;n++)
            if(mazeRandMap[m][n]==0)
                count++;
    //ͳ��ǽ�ĸ���

    total=(int)(i-2)*(j-2)*scale;
    while(count!=total)
    if(count<total)
    {
        m=rand()%(i-2)+1;
        n=rand()%(j-2)+1;
        if(mazeRandMap[m][n]==-1)
        {
            mazeRandMap[m][n]=0;
            count++;
        }
    }
    else
    {
       m=rand()%(i-2)+1;
       n=rand()%(j-2)+1;
       if(mazeRandMap[m][n]==0)
       {
         mazeRandMap[m][n]=-1;
         count--;
       }
    }
    mazeRandMap[1][1] = -1;     //�����յ�����ͨ
    mazeRandMap[i - 2][j - 2] = -1;
    mazeRandMap[1][2] = -1;
    mazeRandMap[2][1] = -1;
    mazeRandMap[i - 3][j - 2] = -1;
    mazeRandMap[i - 2][j - 3] = -1;
    //���Թ����оֲ����������ʹǽ��·�ı����Ϻ��涨
    return mazeRandMap;
}
/**
 * @brief:        Prime�㷨��������Թ�
 * @param[in]:    ROW COL
 * @return:       ��ά����ָ��
 * @note:         �ܹ����ɾ���Ψһ·�����Թ�������ʱ�临�ӶȵĲ���
 * �����DFS BFS �ݹ�ָ�����ɵ��Թ�Խ��Խ��Ȼ��·��Խ���ӣ���ѡ�ô��㷨
 */

void PrimeCreateMaze(int row,int col,int map[MAX][MAX])
{
	int Maze[MAX+2][MAX+2] = { WALL };
    int ROW = row + 2;
    int COL = col + 2;
    clearMap(map);
    for (int i = 0; i < ROW; i++)
    {
		Maze[i][0] = ACCESS;
		Maze[i][COL - 1] = ACCESS;
	}
    for (int j = 0; j < COL;j++)
    {
        Maze[0][j] = ACCESS;
        Maze[ROW - 1][j] = ACCESS;
    }

    vector<int> X;
	vector<int> Y;

	X.push_back(2);
	Y.push_back(2);
    srand((unsigned int)(time(NULL)));
	while (X.size())
    {
		int r = rand() % X.size();
		int x = X[r];
		int y = Y[r];

		int count = 0;
		for (int i = x - 1; i < x + 2; i++)
        {
			for (int j = y - 1; j < y + 2; j++)
            {
				if (abs(x - i) + abs(y - j) == 1 && Maze[i][j] == ACCESS)
                {
					++count;
				}
			}
		}

		if (count <= 1)
        {
			Maze[x][y] = ACCESS;
            map[x - 1][y - 1] = ACCESS;
			for (int i = x - 1; i < x + 2; i++)
            {
				for (int j = y - 1; j < y + 2; j++)
                {
					if (abs(x - i) + abs(y - j) == 1 && Maze[i][j] == 0)
                    {
						X.push_back(i);
						Y.push_back(j);
					}
				}
			}
		}

		X.erase(X.begin() + r);
		Y.erase(Y.begin() + r);
	}
    Maze[2][1] = ACCESS;
    for (int i = ROW - 3; i >= 2; i--) {
		if (Maze[i][COL - 3] == ACCESS) {
            for (int j = i; j <= ROW - 3;j++)
            {
                Maze[j][COL - 3] = ACCESS;
                map[j - 1][COL - 4] = ACCESS;
            }
            break;
		}
	}
}
