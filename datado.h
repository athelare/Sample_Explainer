#ifndef _DATADO_H_
#define _DATADO_H_
//ͷ�ļ����洢���ű��ջ����
#include<stdlib.h>
#include<string.h>
struct Token {
    char * pName;  //ָ��ĳ���������ַ����ı���λ��
    void * pVal;   //ָ�򱣴�ĳ��������ֵ���ڴ��ַ��ʹ��ʱ��Ҫǿ������ת��
    char   type;   //ָʾĳ���������������ͣ�'i'-������'s'-�ַ���
    int    size;   //ָʾĳ��������ռ�õ��ڴ��С
} tokenTable[100];
typedef struct Stack {
    int data;      //�����������
    struct Stack *down;//ջ��ָ��
} STACK,*stacklist;
int nVal=0;
//��¼�������ܸ���
int POP(int *data,stacklist *pointer);
//��ջ����;
//���ܲ���:Ҫ���ܱ���(����)�ĵ�ַ,ջ��ָ������ĵ�ַ(��Ϊ��ı�ָ���ָ��),�ڱ������н��ܵ���top(top��**STACK����)
//����:���ջ��ָ��ָ��ջ��,���ظ�һ,����
int PUSH(int data1,stacklist *pointer);
//����:��ջ����;
//����:������ֵ��ջ��ָ��ĵ�ַ
stacklist initialStack();
//��ʼ��һ��ջ,����ָ��ջ��ջ��ָ��(ջ�׷�����һ���ڴ�ռ�,������0)
int checkdec(char nname[]);
//Ѱ��ĳ�������������±�



int checkdec(char nname[])
{
    //�������ܣ�����Ƿ��Ѿ����ڱ�����û�еĻ�����-1�����򷵻�Ԫ���±�
    int i;
    for(i=0; i<nVal; i++) {
        if(strcmp(nname,tokenTable[i].pName)==0) {
            return i;
        }
    }
    return -1;
}
stacklist initialStack()
{
    stacklist bottom=(stacklist)malloc(sizeof(struct Stack));
    bottom->down=NULL;
    bottom->data=0;
    return bottom;
}
int POP (int *data,stacklist *pointer)
{
    stacklist cur;
    if((*pointer)->down==NULL) {
        return 0;
    }
    //ջ�׵�down��NULL������ȷ�����Ὣbottomָ�뵯������ʧջ��Ҳ�ܹ��ж��Ƿ񵽴�ջ��
    *data = (*pointer)->data;
    cur=*pointer;
    *pointer=(*pointer)->down;
    free(cur);
    return 1;
}
int PUSH(int data1,stacklist *pointer)
{
    stacklist top=(stacklist)malloc(sizeof(struct Stack));
    top->data=data1;
    top->down=*pointer;
    *pointer=top;
    return 1;
}
#endif
