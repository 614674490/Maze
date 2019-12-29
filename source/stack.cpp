#include "stack.h"
#include "assert.h"
/**
 * @brief   ��ջ��ʼ������
 * @param[in]   linkStackPtr
 * @retval  Status
 **/
Status InitStack(linkStackPtr *S)
{
    if((*S = (linkStack *)malloc(sizeof(linkStack))))
    {
        (*S)->top=NULL;
        (*S)->step = 0;
        return OK;
    }
    else
    {
        printError((char *)"????????");
        return OVERFLOW;
    }
}
/**
 * @brief   ջ�пպ���
 * @param[in]   linkStack
 * @retval  Status
 **/
Status StackIsEmpty(linkStack S)
{
    if(S.top==NULL)
        return TRUE;
    else
        return FALSE;
}
/**
 * @brief   ����ջ
 * @param[in]   linkStack
 * @retval  Status
 **/
Status destoryStack(linkStack* S)
{
    if(NULL == S)
        return FALSE;
    clearStack(S);
    free(S->top);
    free(S);
    return TRUE;
}
/**
 * @brief   ���ջ
 * @param[in]   linkStack
 * @retval  Status
 **/
Status clearStack(linkStack* S)
{
    if(StackIsEmpty(*S))
        return ERROR;
    SElemType temp;
    while(!StackIsEmpty(*S))
        Pop(S, &temp);
    S->step=0;
    return OK;
}
/**
 * @brief   ��ȡջ��Ԫ��
 * @param[in]   linkStack
 * @retval  Status
 **/
SElemType getStackTop(linkStack S)
{
    return S.top->elem;
}
/**
 * @brief   ��ջ
 * @param[in]   linkStack?SElemType
 * @retval  Status
 **/
Status Push(linkStack *S, SElemType CB)
{
    stackNodePtr p;
    if(!(p= (stackNodePtr)malloc(sizeof(stackNode))))
    {
        printError((char *)"????????");
        return OVERFLOW;
    }
    p->elem = CB;
    p->next = S->top;
    S->top = p;
    S->step += 1;
    return OK;
}
/**
 * @brief   ��ջ
 * @param[in]   linkStack?SElemType
 * @retval  Status
 **/
Status Pop(linkStack *S, SElemType *CB)
{
    stackNodePtr p;
    if(StackIsEmpty(*S))
    {
        printError((char *)"????????");
        return ERROR;
    }
    else
    {
        *CB = S->top->elem;
        p = S->top;
        S->top = S->top->next;
        free(p);  //??��??????��??
        S->step -= 1;
        return OK;
    }
}
/**
 * @brief:        ջ����
 * @param[in]:  null
 * @return:     null
 * @note:       null
 */
Status StackCopy(linkStack r,RouteInfoPtr *route)
{
    while(r.top)
    {
        CBlockType CB;
        CB = r.top->elem;
        r.top = r.top->next;
        Push(*route,CB);
    }
    return TRUE;
}
