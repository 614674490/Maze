/*
 * @Author: liguiyin
 * @Date: 2019-12-20 15:36:18
 * @LastEditTime: 2020-04-09 15:59:13
 * @Description: README
 * @FilePath: \Maze_V3.0\source\maze.cpp
 */
#include "maze.h"
#include "generate.h"
#include "queue.h"
#include "sort.h"
#include "stack.h"
#include "stdlib.h"
#include "time.h"
#include "windows.h"

int TestMode = 0;
int TestCount = 0;
int compare = 0;
/**
 * @brief:        ��ʼ��ͨ����
 * @param[in]:    CBlockTypePtr
 * @return:       Status
 * @note:         null
 */
CBlockType CBlockInit(void)
{
    CBlockType CB;
    CB.di = null;
    CB.ord = 0;
    CB.seat.col = 0;
    CB.seat.row = 0;
    return CB;
}
/**
 * @brief:        �Թ���Ϣ��ʼ��
 * @param[in]:    mazeInfoPtr
 * @return:       Status
 * @note:         default_start=(1,1) default_end=(MAXROW-2,MAXCOL-2)
 */
Status MazeInit(mazeInfoPtr *maze)
{
    int i;
    *maze = (mazeInfoPtr)malloc(sizeof(mazeInfo));
    if (maze)
    {
        for (i = 0; i < (MAXROUTE + 1); i++)
            if (InitStack(&(*maze)->route[i]) != OK)
                return OVERFLOW;
        if (InitStack(&(*maze)->routeshortest) != OK)
            return OVERFLOW;
        START_POS((*maze)->start);
        END_POS((*maze)->end);
        if (TestMode)
            PrimeCreateMaze(MAXROW, MAXCOL, mazeRandMap);
        else if (!GETRANDMAZE)
            copyMazeMap(MG, mazeRandMap);
        else
            getMazeMap();
        (*maze)->mazeMap = mazeRandMap;
        copyMazeMap(mazeRandMap, tempMaze);
        (*maze)->routeNum = 0; //Ĭ��û��ͨ·
        return OK;
    }
    else
    {
        printError((char *)"�ڴ����ʧ��");
        return OVERFLOW;
    }
}
/**
 * @brief:        ����ͨ������Ϣ
 * @param[in]:    ͨ����ṹ��
 * @return:       null
 * @note:         null
 */
void setCBlockInfo(CBlockType *CB, order ord, posType seat, directionType di)
{
    CB->ord = ord;
    CB->seat = seat;
    CB->di = di;
}
/**
 * @brief:        ��ӡ����Թ�(ԭʼ�Թ���ӡ)
 * @param[in]:    mazeInfo
 * @return:       null
 * @note:         "��"��ʾ�ϰ���"��"��ʾ���ϰ���"��"��ʾ��㣬"��"��ʾ�յ㣬
 *                "��"��ʾ�߹����ǲ�ͨ "��"��ʾ��ȷ·��
 */
void MazePrint(mazeInfo maze)
{
    int i, j;
    printf("\n");
    for (i = 0; i < MAXROW; i++)
    {
        for (j = 0; j < MAXCOL; j++)
        {
            if (i == maze.start.row && j == maze.start.col)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                printf("��");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                continue;
            }
            else if (i == maze.end.row && j == maze.end.col)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                printf("��");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                continue;
            }
            else if (maze.mazeMap[i][j] == WALL)
                printf("��");
            else if (maze.mazeMap[i][j] == ACCESS)
                printf("��");
            else if (maze.mazeMap[i][j] == GONE)
                printf("��");
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
                printf("��");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
        }
        printf("\n");
    }
    printf("\n");
}
/**
 * @brief:        ��ӡ����·��
 * @param[in]:    mazeInfo
 * @return:       null
 * @note:         ����Ԫ����ʽ��ӡ���
 */
void PathPrint(RouteInfo r)
{
    int i = 0;
    stackNodePtr p;
    char d[5] = {'X', 'E', 'S', 'W', 'N'}; //��������
    p = r.top;
    while (r.top->next != NULL)
    {
        printf("(%2d,%2d, %c)->", p->elem.seat.row, p->elem.seat.col, d[p->elem.di]);
        r.top = r.top->next;
        p = r.top;
        if ((++i) % 5 == 0)
            printf("\n");
    }
    printf("(%2d,%2d, %c)\nAll %3d Step\n\n", p->elem.seat.row, p->elem.seat.col, d[p->elem.di], r.step);
}
/**
 * @brief:        ����·����ӡ�Թ���ͼ
 * @param[in]:    mazeInfo RouteInfo
 * @return:       Status
 * @note:         ����֤·������ȷ��
 */
Status MazeMapPrint(mazeInfo maze, RouteInfo r)
{
    int i, j;
    int tempMap[MAX][MAX] = {0};
    int end = 5;
    int step = 1;
    char dir[4][5] = {"��\0", "��\0", "��\0", "��\0"};
    if (r.top->elem.seat.row != maze.start.row || r.top->elem.seat.col != maze.start.col) //��㲻��
    {
        printf("��·���޷���ͨ�Թ�\n");
        return ERROR;
    }
    while (r.top->next) //���ö�ά��������ȷ·��
    {
        tempMap[r.top->elem.seat.row][r.top->elem.seat.col] = r.top->elem.di;
        r.top = r.top->next;
    }
    tempMap[r.top->elem.seat.row][r.top->elem.seat.col] = end; //�յ���5��ʾ
    printf("\n");
    for (i = 0; i < MAXROW; i++)
    {
        for (j = 0; j < MAXCOL; j++)
        {
            if (i == maze.start.row && j == maze.start.col)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                printf("��");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                continue;
            }
            else if (i == maze.end.row && j == maze.end.col)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                printf("��");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                if (tempMap[i][j] != end) //�����յ㲻��
                {
                    printf("��·���޷���ͨ�Թ�\n");
                    return ERROR;
                }
                continue;
            }
            else if (maze.mazeMap[i][j] == WALL)
            {
                if (tempMap[i][j] > 0) //���ϰ�������·��->��·���޷���ͨ�Թ�
                {
                    printf("��·���޷���ͨ�Թ�\n");
                    return ERROR;
                }
                printf("��");
            }
            else if (maze.mazeMap[i][j] == ACCESS)
            {
                if (tempMap[i][j] > 0) //���ϰ����ɳ���·��
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
                    printf("%s", &dir[tempMap[i][j] - 1][0]);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                }
                else
                    printf("��");
            }
            else if (maze.mazeMap[i][j] == GONE)
                printf("��");
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
                printf("%s", &dir[tempMap[i][j] - 1][0]);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
            }
        }
        printf("\n");
    }
    printf("\n");
    return OK;
}
/**
 * @brief:        �����㼣
 * @param[in]:    posType curstep mazeMap
 * @return:       null
 * @note:         ��ǰ��������0
 */
void FootPrint(posType pos, int curstep, int mazeMap[MAX][MAX])
{
    mazeMap[pos.row][pos.col] = curstep;
}
/**
 * @brief:        ���Ϊ������
 * @param[in]:    posType mazeMap
 * @return:       null
 * @note:         ���²���ͨ���ı��
 */
void MarkPrint(posType pos, int mazeMap[MAX][MAX])
{
    mazeMap[pos.row][pos.col] = GONE; //���߲�ͨ�Ŀ���ΪGONE
}
/**
 * @brief:        ��һ��λ��
 * @param[in]:    posType directionType
 * @return:       OK INFEASLIBLE
 * @note:         �ı�����ֵ �����ƶ�
 */
posType NextPos(posType pos, directionType di)
{
    switch (di)
    {
    case east: //��
        pos.col++;
        break;
    case south: //����
        pos.row++;
        break;
    case west: //����
        pos.col--;
        break;
    case north: //��
        pos.row--;
        break;
    }
    return pos;
}
/**
 * @brief:        �жϵ�ǰ�Ƿ��ͨ
 * @param[in]:    posType mazeMap
 * @return:       TRUE->��ͨ FALSE->����ͨ
 * @note:         ǰ����δ���߹����ҿ�ͨ��·��
 */
Status Pass(posType pos, int mazeMap[MAX][MAX])
{
    if (mazeMap[pos.row][pos.col] == ACCESS)
        return TRUE;
    else
        return FALSE;
}
/**
 * @brief:        �Թ�����㷨
 * @param[in]:    mazeInfoPtr RouteInfoPtr
 * @return:       Status
 * @note:         ���ö�ջ�������(���ݷ�)�����������Ž� ��ı�ԭ�Թ�����
 */
Status MazePath(mazeInfoPtr maze, RouteInfoPtr r)
{
    linkStackPtr S;             //���·��
    posType curpos;             //��ǰλ��
    SElemType e = CBlockInit(); //ͨ���黺�����
    clearStack(r);              //���ԭ·��
    int curstep;                //��ǰ����
    InitStack(&S);
    curpos = maze->start; //�趨����ǰλ�á�Ϊ�����λ�á�
    curstep = 1;          //̽����һ��
    if (!TestMode)
    {
        printf("\n���:(%d,%d)->", maze->start.row, maze->start.col);
        printf("�յ�:(%d,%d)\n", maze->end.row, maze->end.col);
    }
    do
    {
        if (Pass(curpos, maze->mazeMap))
        {                                              //��ǰλ�ÿ���ͨ��������δ���ߵ�����ͨ����
            FootPrint(curpos, curstep, maze->mazeMap); //�����㼣
            setCBlockInfo(&e, curstep, curpos, east);
            Push(S, e); //����·��
            if (curpos.row == maze->end.row && curpos.col == maze->end.col)
            { //�����յ�
                maze->routeNum = 1;
                S->top->elem.di = null;
                while (!StackIsEmpty(*S)) //��·�����뵽mazeInfo��
                {
                    Pop(S, &e);
                    Push(r, e);
                }
                if (!TestMode)
                {
                    MazeMapPrint(*maze, *r);
                    PathPrint(*r);
                }
                destoryStack(S);
                return TRUE;
            }
            curpos = NextPos(curpos, east); //��һλ���ǵ�ǰλ�õĶ���
            compare++;
            curstep++; //̽����һ��
        }              //if
        else
        { //��ǰλ�ò���ͨ��
            if (!StackIsEmpty(*S))
            {
                Pop(S, &e);
                while (e.di == north && !StackIsEmpty(*S))
                {
                    MarkPrint(e.seat, maze->mazeMap);
                    Pop(S, &e);
                    curstep--;
                } //while
                if (e.di < north)
                {
                    compare++;
                    e.di++;
                    Push(S, e);
                    curpos = NextPos(e.seat, e.di); //�趨��ǰλ���Ǹ��·����ϵ����ڿ�
                }                                   //if
            }                                       //if
        }                                           //else
    } while (!StackIsEmpty(*S));
    maze->routeNum = 0;
    printf("��ͼΪ���֣�\n");
    printf("\n");
    return FALSE;
}
//���±������ڵݹ�
linkStackPtr S;
posType curpos;
order curstep;
SElemType e;
/**
 * @brief:        �ݹ�������/����·��
 * @param[in]:    mazeInfoPtr mode(0-->simple 1-->all)
 * @return:       Status
 * @note:         ���ڻ��ݷ� ����ı�ԭ�Թ�����
 */
Status MazeAllPath(mazeInfoPtr maze)
{
    clearStack(maze->route[0]);
    maze->routeNum = 0; //·����ʼ��
    InitStack(&S);
    curpos = maze->start; //�趨����ǰλ�á�Ϊ�����λ�á�
    curstep = 1;          //̽����һ��

    printf("\n���:(%d,%d)->", maze->start.row, maze->start.col);
    printf("�յ�:(%d,%d)\n", maze->end.row, maze->end.col);
    //�ݹ����·������
    FindPath(maze);
    //��ഢ������MAXROUTE
    if (maze->routeNum)
    {
        printf("\n----------------------------��%d��·��----------------------------\n", maze->routeNum);
        QSort(maze->route, 1, maze->routeNum, ascending); //������·���Ӷ̵�������
        printf("---------------------------- ���·�� ----------------------------\n");
        MazeMapPrint(*maze, *maze->route[1]);
        PathPrint(*maze->route[1]);
        printf("---------------------------- �·�� ----------------------------\n");
        MazeMapPrint(*maze, *maze->route[maze->routeNum]);
        PathPrint(*maze->route[maze->routeNum]);
        return TRUE;
    }
    else
    {
        printf("��Ǹ��δ�ҵ�һ������㵽�յ��·��\n");
        return FALSE;
    }
    printf("\n");
}
/**
 * @brief:        �ݹ�����Ӻ���
 * @param[in]:    mazeInfoPtr
 * @return:       null
 * @note:         �ݹ����
 */
void FindPath(mazeInfoPtr maze)
{
    posType pos;             //��ʱ������̽λ��
    directionType di = east; //Ĭ�ϴӶ���ʼ��̽
    static int i = 0;
    if (curpos.row == maze->end.row && curpos.col == maze->end.col)
    {
        //�����յ�
        if (maze->routeNum < MAXROUTE)
        {
            maze->routeNum++;
            setCBlockInfo(&e, curstep, curpos, null); //���յ���Ϣ�����ͨ����Ϣ
            Push(S, e);                               //���յ���Ϣѹջ
            //����ǰ·���浽maze��
            StackCopy(*S, &(maze->route[maze->routeNum]));
            if (maze->routeNum <= MAXPRINT)
            {
                //���ͼ��·����Ϣ
                printf("----------------------------��%d��·��----------------------------\n", maze->routeNum);
                MazePrint(*maze);
                PathPrint(*maze->route[maze->routeNum]);
            }
            Pop(S, &e); //���յ���Ϣ����
        }
        curpos = getStackTop(*S).seat; //������һ��λ��
        curstep--;
        return;
    }
    //����ǰλ����Ϣ���ѹջ
    //�����㼣
    //�����ݶ�Ϊ��
    setCBlockInfo(&e, curstep, curpos, di);
    Push(S, e);
    FootPrint(curpos, curstep, maze->mazeMap);
    //���ĸ�����
    for (di; di <= north; di++)
    {
        //��̽λ��
        if (maze->routeNum > MAXROUTE)
            return;
        i++;
        if (i % 3000000 == 0)
        {
            i = 0;
            printf(". ");
        }
        pos = NextPos(curpos, di);
        //�ж��Ƿ����
        if (Pass(pos, maze->mazeMap))
        {
            if (di != east)
            {
                Pop(S, &e);
                e.di = di;
                Push(S, e);
            }
            curstep++; //�ǲ���
            //����λ��
            curpos.col = pos.col;
            curpos.row = pos.row;
            //�ݹ����
            FindPath(maze);
        } //if
    }     //for
    //�����߹���·������Ϊ��ͨ��
    //��ջ
    maze->mazeMap[curpos.row][curpos.col] = -1;
    Pop(S, &e);
    curstep--; //��λ�ã�������
    //��ջ��Ϊ�ո���λ��
    if (!StackIsEmpty(*S))
    {
        curpos = getStackTop(*S).seat;
    }
}
/**
 * @brief:        �ݹ�����·��
 * @param[in]:    mazeInfoPtr mode(0-->simple 1-->all)
 * @return:       Status
 * @note:         ���ڻ��ݷ� ����ı�ԭ�Թ�����
 */
Status MazeSimmplePath(mazeInfoPtr maze, RouteInfoPtr r)
{
    clearStack(r);        //���·��ԭ���洢����Ϣ
    InitStack(&S);        //��ʼ�����ڵݹ�Ļ�����ջ
    maze->routeNum = 0;   //·����Ŀ����
    curpos = maze->start; //�趨����ǰλ�á�Ϊ�����λ�á�
    curstep = 1;          //̽����һ��
    if (!TestMode)
    {
        printf("\n���:(%d,%d)->", maze->start.row, maze->start.col);
        printf("�յ�:(%d,%d)\n", maze->end.row, maze->end.col);
    }
    FindSimpleRoute(maze, r);
    if (maze->routeNum)
    {
        if (!TestMode)
        {
            MazeMapPrint(*maze, *r);
            PathPrint(*r);
        }
        if (GETRANDMAZE)
            copyMazeMap(tempMaze, mazeRandMap);
        else
            copyMazeMap(MG, mazeRandMap);
        return TRUE;
    }
    else
    {
        printf("���Թ�Ϊ����\n");
        return FALSE;
    }
}
/**
 * @brief:        �ݹ���ⵥ��·��
 * @param[in]:    mazeInfoPtr ���·��
 * @return:       null
 * @note:         �˴�ʹ�ö�ջ��Ŀ���ǽ�·����Ϣ���浽maze����ջ��
 */
void FindSimpleRoute(mazeInfoPtr maze, RouteInfoPtr r)
{
    posType pos;             //��ʱ������̽λ��
    directionType di = east; //Ĭ�ϴӶ���ʼ��̽
    if (curpos.row == maze->end.row && curpos.col == maze->end.col)
    {
        //�����յ�
        maze->routeNum++;
        setCBlockInfo(&e, curstep, curpos, null); //���յ���Ϣ�����ͨ����Ϣ
        Push(S, e);                               //���յ���Ϣѹջ
        //����ǰ·���浽maze��
        StackCopy(*S, &r);
        Pop(S, &e);                    //���յ���Ϣ����
        curpos = getStackTop(*S).seat; //������һ��λ��
        curstep--;
        return;
    }
    //����ǰλ����Ϣ���ѹջ
    //�����㼣
    //�����ݶ�Ϊ��
    setCBlockInfo(&e, curstep, curpos, di);
    Push(S, e);
    FootPrint(curpos, curstep, maze->mazeMap);
    //���ĸ�����
    for (di; di <= north; di++)
    {
        compare++;
        //��̽λ��
        if (maze->routeNum == 1) //�ҵ�һ��·���򷵻�
            return;
        pos = NextPos(curpos, di);
        //�ж��Ƿ����
        if (Pass(pos, maze->mazeMap))
        {
            FootPrint(pos, curstep, maze->mazeMap);
            if (di != east)
            {
                Pop(S, &e);
                e.di = di;
                Push(S, e);
            }
            curstep++; //�ǲ���
            //����λ��
            curpos.col = pos.col;
            curpos.row = pos.row;
            //�ݹ����
            FindSimpleRoute(maze, r);
        }                          //if
    }                              //for
    MarkPrint(pos, maze->mazeMap); //����߹�δ��ͨ��·��
    Pop(S, &e);                    //��ջ
    curstep--;                     //��λ�ã�������
    //��ջ��Ϊ�ո���λ��
    if (!StackIsEmpty(*S))
    {
        curpos = getStackTop(*S).seat;
    }
}
/**
 * @brief:        �����˺���
 * @param[in]:    directionType
 * @return:       null
 * @note:         ���ݵ�ǰͨ���洢�ķ��������һͨ������һ������
 */
directionType directionGoback(directionType di)
{
    return Transfrom(di - (di > 2 ? 1 : -1) * 2);
}
/**
 * @brief:        ������ȱ�����һ�����·��
 * @param[in]:    mazeInfoPtr RouteInfoPtr
 * @return:       Status
 * @note:         �ö���ʵ�� ����ı�ԭ�Թ�����
 */
Status ShortestPath(mazeInfoPtr maze, RouteInfoPtr r)
{
    QElemType e;
    Queue Q;
    posType curPos;
    int mark[MAX][MAX] = {0}; //��������ͨ����Ϣ
    order step = 1;
    InitQueue(&Q);
    clearStack(r);
    curPos = maze->start;
    setCBlockInfo(&e, step, curPos, null);
    EnQueue(&Q, e); //�����������
    mark[curPos.row][curPos.col] = e.di;
    if (!TestMode)
    {
        printf("\n���:(%d,%d)->�յ�:(%d,%d)\n", maze->start.row, maze->start.col, maze->end.row, maze->end.col);
    }
    while (!QueueuIsEmpty(Q))
    {
        step++;
        DeQueue(&Q, &e);
        curPos = e.seat;                //��ȡ��һλ����Ϣ
        curPos = NextPos(curPos, east); //��
        compare++;
        if (!mark[curPos.row][curPos.col] && Pass(curPos, maze->mazeMap)) //����Ԫ�ؿ�ͨ(��ͨ δ����)
        {
            setCBlockInfo(&e, step, curPos, west); //west��ʱ����Ѱ����һ������
            EnQueue(&Q, e);
            mark[curPos.row][curPos.col] = e.di;
            if (curPos.row == maze->end.row && curPos.col == maze->end.col) //�����յ�
            {
                break;
            }
        }
        curPos = NextPos(curPos, west); //�����ϴ�λ��

        curPos = NextPos(curPos, south); //����
        compare++;
        if (!mark[curPos.row][curPos.col] && Pass(curPos, maze->mazeMap)) //����Ԫ�ؿ�ͨ
        {
            setCBlockInfo(&e, step, curPos, north);
            EnQueue(&Q, e);
            mark[curPos.row][curPos.col] = e.di;
            if (curPos.row == maze->end.row && curPos.col == maze->end.col) //�����յ�
            {
                break;
            }
        }
        curPos = NextPos(curPos, north); //�����ϴ�λ��

        curPos = NextPos(curPos, west); //����
        compare++;
        if (!mark[curPos.row][curPos.col] && Pass(curPos, maze->mazeMap)) //����Ԫ�ؿ�ͨ
        {
            setCBlockInfo(&e, step, curPos, east);
            EnQueue(&Q, e);
            mark[curPos.row][curPos.col] = e.di;
            if (curPos.row == maze->end.row && curPos.col == maze->end.col) //�����յ�
            {
                break;
            }
        }
        curPos = NextPos(curPos, east); //�����ϴ�λ��

        curPos = NextPos(curPos, north); //��
        compare++;
        if (!mark[curPos.row][curPos.col] && Pass(curPos, maze->mazeMap)) //����Ԫ�ؿ�ͨ
        {
            setCBlockInfo(&e, step, curPos, south);
            EnQueue(&Q, e);
            mark[curPos.row][curPos.col] = e.di;
            if (curPos.row == maze->end.row && curPos.col == maze->end.col) //�����յ�
            {
                break;
            }
        }
    }
    if (!QueueuIsEmpty(Q)) //���зǿ�->�ҵ����·�� ��ʱ��βԪ��Ϊ�յ�
    {
        int i = 2;
        directionType tempdi, tempdi_last;
        e = Q.base[Q.rear];
        tempdi_last = e.di;
        tempdi = directionGoback(tempdi_last); //�����һͨ������һ������
        e.di = null;
        e.ord = 1;
        Push(r, e);
        while (!(curPos.row == maze->start.row && curPos.col == maze->start.col))
        {
            curPos = NextPos(curPos, tempdi_last);                 //�˻���һ��
            tempdi_last = Transfrom(mark[curPos.row][curPos.col]); //���浱ǰ������һ������һ������
            mark[curPos.row][curPos.col] = tempdi;                 //�滻��������
            tempdi = directionGoback(tempdi_last);                 //�����һͨ������һ����
            setCBlockInfo(&e, i++, curPos, Transfrom(mark[curPos.row][curPos.col]));
            Push(r, e);
        }
        if (!TestMode)
        {
            MazeMapPrint(*maze, *r);
            PathPrint(*r);
        }
        return OK;
    }
    else
    {
        printf("��Ǹ����ͼΪ���֣�\n");
        return FALSE;
    }
}
/**
 * @brief:        ��ȡ�Թ����ʱ��
 * @param[in]:    mazeMap fun
 * @return:       ���ʱ��
 * @note:         �������ͨ���Թ�
 */
int getSearchTime(mazeInfoPtr maze, RouteInfoPtr r, Status (*SearchRoute)(mazeInfoPtr maze, RouteInfoPtr r))
{
    int t_start = 0;
    int t_end = 0;
    t_start = clock();
    SearchRoute(maze, r);
    t_end = clock();
    return t_end - t_start;
}
/**
 * @brief:        �Ƚϸ����㷨��ʱ�临�Ӷ�
 * @param[in]:    mazeMap fun
 * @return:       ���ʱ��
 * @note:         �������ͨ���Թ�
 */
void TimeTest(void)
{
    int i = 0;
    int time = 0;
    char row[20];
    char col[20];
    char count[20];
    mazeInfoPtr maze;
    printf("�������Թ���ʼ��С������������MAXROW MAXCOL number(4*4����80*80)\n");
    do
    {
        scanf("%s", row);
        scanf("%s", col);
        scanf("%s", count);
        MAXROW = atoi(row);
        MAXCOL = atoi(col);
        TestCount = atoi(count);
        if (!(MAXROW >= 4 && MAXCOL >= 4 && MAXROW <= 80 && MAXCOL <= 80) || TestCount < 0)
            printf("\n�������ݷǷ�������������:  ");
    } while (!(MAXROW >= 4 && MAXCOL >= 4 && MAXROW <= 80 && MAXCOL <= 80) || TestCount < 0);
    MazeInit(&maze);
    for (i = 0; i < TestCount; i++)
    {
        printf("\t%d*%d\t\t̽������ ʱ��\n", MAXROW, MAXCOL);
        copyMazeMap(tempMaze, mazeRandMap);
        //�ǵݹ���ݷ�Ѱ�ҵ���·��(��ջʵ��)
        printf("\t�ǵݹ���ݷ�:\t");
        compare = 0;
        time = getSearchTime(maze, maze->route[0], MazePath);
        printf("%d\t", compare);
        printf("%3dms\n", time);

        copyMazeMap(tempMaze, mazeRandMap);

        //�������Ѱ��һ�����·��(����ʵ��)
        printf("\t������ȷ�:\t");
        compare = 0;
        time = getSearchTime(maze, maze->route[1], ShortestPath);
        printf("%d\t", compare);
        printf("%3dms\n", time);

        //�ݹ���ݷ�Ѱ��һ��·��(��ջ�洢·��)
        printf("\t�ݹ���ݷ�:\t");
        compare = 0;
        time = getSearchTime(maze, maze->route[2], MazeSimmplePath);
        printf("%d\t", compare);
        printf("%3dms\n\n", time);

        MAXROW += 10;
        MAXCOL += 10;
        if (MAXROW > MAX || MAXCOL > MAX)
        {
            printf("\n�Թ���С�ﵽ���� %d*%d\n", MAX, MAX);
            TestMode = 0;
            return;
        }
        free(maze);
        MazeInit(&maze);
    }
    TestMode = 0; //������ɺ� �˳�����ģʽ
}

directionType operator++(directionType &di, int)
{
    switch (di)
    {
    case east:
        di = south;
        break;
    case south:
        di = west;
        break;
    case west:
        di = north;
        break;
    case north:
        di = error;
        break;
    }
    return di;
}
directionType Transfrom(int d)
{
    directionType di = east;
    switch (d)
    {
    case east:
        di = east;
        break;
    case south:
        di = south;
        break;
    case west:
        di = west;
        break;
    case north:
        di = north;
        break;
    }
    return di;
}
