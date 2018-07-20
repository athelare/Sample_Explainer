#ifndef _DATADO_H_
#define _DATADO_H_
//头文件：存储符号表和栈操作
#include<stdlib.h>
#include<string.h>
struct Token {
    char * pName;  //指向某个变量名字符串的保存位置
    void * pVal;   //指向保存某个变量的值的内存地址，使用时需要强制类型转换
    char   type;   //指示某个变量的数据类型：'i'-整数；'s'-字符串
    int    size;   //指示某个变量所占用的内存大小
} tokenTable[100];
typedef struct Stack {
    int data;      //存放整型数据
    struct Stack *down;//栈顶指针
} STACK,*stacklist;
int nVal=0;
//记录变量的总个数
int POP(int *data,stacklist *pointer);
//出栈操作;
//接受参数:要接受变量(整型)的地址,栈顶指针变量的地址(因为会改变指针的指向),在本程序中接受的是top(top是**STACK类型)
//操作:如果栈顶指针指向栈底,返回负一,否则将
int PUSH(int data1,stacklist *pointer);
//操作:进栈操作;
//参数:变量的值和栈顶指针的地址
stacklist initialStack();
//初始化一个栈,返回指向栈底栈顶指针(栈底分配了一块内存空间,数据是0)
int checkdec(char nname[]);
//寻找某个变量，返回下标



int checkdec(char nname[])
{
    //函数功能：检查是否已经存在变量，没有的话返回-1，否则返回元素下标
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
    //栈底的down是NULL，这样确保不会将bottom指针弹出而丢失栈，也能够判断是否到达栈底
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
