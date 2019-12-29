/*
 * @Author: liguiyin
 * @Date: 2019-11-22 10:01:27
 * @LastEditTime : 2019-12-28 22:27:37
 * @Description: README
 * @FilePath: \Schtb\source\main.c
 */
#include "Init.h"
#include "maze.h"
#include "stack.h"
#include "queue.h"
#include "generate.h"
#include "windows.h"
//��ʱ����
void gotoxy(int x, int y)      //����ƶ�����x,y��
{
    COORD coord;
    coord.X = y;
    coord.Y = x;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
int main()
{
    mazeInfoPtr maze;
    PrintInit();
    if(!GetInput())
    {
        system("pause");
        return 0;
    }
    do{
        getchar();
        if(!TestMode)
        {
            MazeInit(&maze);
            printTip((char *)"����Թ�ͼ");
            MazePrint(*maze);
            printTip((char *)"�����������·��");
            MazePath(maze,maze->route[0]);
            if(GETRANDMAZE)
                copyMazeMap(tempMaze,mazeRandMap);
            else
                copyMazeMap(MG,mazeRandMap);
            printTip((char *)"������������·��");
            ShortestPath(maze,maze->route[1]);
            printTip((char *)"�ݹ鷨�����·��");
            MazeAllPath(maze);
            free(maze);
        }
        else
            TimeTest();
        if(!conOperate())
            break;
        PrintInit();
    }while(GetInput());
    system("pause");
    return 0;
}
