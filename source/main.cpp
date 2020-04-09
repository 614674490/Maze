/*
 * @Author: Ken Kaneki
 * @Date: 2020-04-09 14:53:12
 * @LastEditTime: 2020-04-09 18:05:39
 * @Description: README
 * @FilePath: \Maze_V3.0\source\main.cpp
 */
#include "Init.h"
#include "generate.h"
#include "maze.h"
#include "queue.h"
#include "stack.h"
#include "windows.h"

void gotoxy(int x, int y) //����ƶ�����x,y��
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
    if (!GetInput())
    {
        system("pause");
        return 0;
    }
    do
    {
        getchar();
        if (!TestMode)
        {
            MazeInit(&maze);
            printTip((char *)"����Թ�ͼ");
            MazePrint(*maze);
            printTip((char *)"�����������·��");
            MazePath(maze, maze->route[0]);
            if (GETRANDMAZE)
                copyMazeMap(tempMaze, mazeRandMap);
            else
                copyMazeMap(MG, mazeRandMap);
            printTip((char *)"������������·��");
            ShortestPath(maze, maze->route[1]);
            printTip((char *)"�ݹ鷨�����·��");
            MazeAllPath(maze);
            free(maze);
        }
        else
            TimeTest();
        if (!conOperate())
            break;
        PrintInit();
    } while (GetInput());
    system("pause");
    return 0;
}
