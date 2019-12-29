/*
 * @Author: liguiyin
 * @Date: 2019-11-22 11:17:37
 * @LastEditTime : 2019-12-28 20:54:40
 * @Description: README
 * @FilePath: \Maze\include\schtb.h
 */
#ifndef _maze_h
#define _maze_h
#include "config.h"

#define MAX 400
#define WALLRATE 0.30f   //����Թ���ǽ��ռ����

extern int GETRANDMAZE; //0->�涨�Թ� 1->����Թ�
extern int MAXROW ;     //�Թ���� 80*80
extern int MAXCOL;      //�Թ�����
extern int TestCount;   //���ڲ���ʱ�临�Ӷȵ���������

#define MAXPRINT 20
#define getMazeMap() mapRand(MAXROW,MAXCOL)
#define MAXROUTE 1000
#define START_POS(start) start.row=1,start.col=1
#define END_POS(end)     end.row=MAXROW-2,end.col=MAXCOL-2
#define ACCESS  -1   //ԭ����ͨ
#define WALL    0    //ǽ��
#define GONE    -2  //�߹����Ҳ�ͨ(ԭ����-1)

#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define BLUE FOREGROUND_BLUE
#define YELLOW FOREGROUND_GREEN|FOREGROUND_RED
#define PURPLE FOREGROUND_RED|FOREGROUND_BLUE
#define CYAN FOREGROUND_GREEN|FOREGROUND_BLUE
#define DEFAULT 7

typedef int order;  //�������
//λ����Ϣ
typedef struct{
    int row;    //������ 0-MAXROW
    int col;    //������ 0-MAXCOL
} posType;

//������Ϣ
typedef enum
{
    null = 0,
    east = 1,   //�� col+1
    south = 2,  //�� row+1
    west = 3,   //�� col-1
    north = 4,   //�� row-1
	error
} directionType;

//ͨ������Ϣ
typedef struct{
    order ord;          //ͨ������·���ϵ����
    posType seat;       //ͨ�������Թ��е�������Ϣ
    directionType di;   //�Ӵ�ͨ����������һͨ����ķ���
} CBlockType, *CBlockTypePtr;

//��ջ��Ϣ
typedef CBlockType SElemType;
//��ջ������ʽ�洢�ṹ
typedef struct StackNode{
    SElemType elem;
    struct StackNode *next;
} stackNode, *stackNodePtr;
typedef struct LinkStack{
    stackNodePtr top; 	//ջ��ָ�� ʼ��ָ��ջ��Ԫ��
    int step;          //��¼�ܲ���
} linkStack,*linkStackPtr;

//������Ϣ
typedef CBlockType QElemType;
typedef struct{
    QElemType *base; //��̬����
    int front, rear;
    int step;
    int size;
} Queue;

//·����Ϣ
typedef linkStackPtr RouteInfoPtr;
typedef linkStack RouteInfo;
typedef Queue RouteShortest;
//�Թ���Ϣ
typedef struct{
    posType start;                  //�������
    posType end;                    //�յ�����
    int (*mazeMap)[MAX];         //�Թ���ͼ����ָ��(����ָ���� ָ���ֵ���ɸı䣬��ָ��ĵ�ַ���Ըı�)
    RouteInfoPtr route[MAXROUTE+1]; //�洢����·��
    RouteInfoPtr routeshortest;    //�洢һ�����·��
    int routeNum;                   //·����Ŀ 0��ʾû��ͨ·
} mazeInfo,*mazeInfoPtr;

extern int TestMode;  //1-->ʱ�临�ӶȱȽ� 0-->��ͨ�Թ����

//��س�ʼ������
Status MazeInit(mazeInfoPtr *maze);
CBlockType CBlockInit(void);
void setCBlockInfo(CBlockType *CB, order ord, posType seat, directionType di);
//��ش�ӡ����
void MazePrint(mazeInfo maze);
void PathPrint(RouteInfo r);
Status MazeMapPrint(mazeInfo maze, RouteInfo r);
//�ǵݹ���ⵥ��·����غ���(��ջʵ��)
void FootPrint(posType pos, int curstep, int mazeMap[MAX][MAX]);
void MarkPrint(posType pos, int mazeMap[MAX][MAX]);
posType NextPos(posType pos, directionType di);
Status Pass(posType pos, int mazeMap[MAX][MAX]);
Status MazePath(mazeInfoPtr maze, RouteInfoPtr r);
//�ݹ��������·����غ���
Status MazeAllPath(mazeInfoPtr maze);
void FindPath(mazeInfoPtr maze);
//�ݹ���ݷ���ⵥ��·����غ���
void FindSimpleRoute(mazeInfoPtr maze,RouteInfoPtr r);
Status MazeSimmplePath(mazeInfoPtr maze, RouteInfoPtr r);
//�������������·��(����ʵ��)
Status ShortestPath(mazeInfoPtr maze, RouteInfoPtr r);
//ʱ�临�ӶȱȽ���غ���
int getSearchTime(mazeInfoPtr maze, RouteInfoPtr r, Status (*SearchRoute)(mazeInfoPtr maze, RouteInfoPtr r));
void TimeTest(void);
//C++directionType�������ݴ�����غ���
directionType operator++(directionType &di,int);    //������������غ���
directionType Transfrom(int d);

#endif
