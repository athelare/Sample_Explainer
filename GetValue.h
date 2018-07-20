#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "datado.h"
#define EPS 1e-6
#ifndef _GETVALUE_H_
#define _GETVALUE_H_
//头文件功能：任意表达式转后缀表达式求值
int endflag;
//用于计算的时候判断变量名后面是否是字符串的结束
double abslu(double d);
//计算绝对值
double FindValinStr(char **st);
//返回字符串中变量的数值（double），如果不存在该变量，返回负一，运行结束后将字符指针自动移到变量的最后一个字符
//接受参数：字符指针的地址
int check_advance(char op1,char op2);
//功能：在形成后缀表达式的过程中判断运算符优先级
int check_type(char c);
//返回字符串中字符的类型，数字，运算符或者括号
double FindValinStr(char **st){

    char *ed;
    char temp;
    int i;
    endflag=0;
    //for(;**st!='\0';*st++){
        if(isalpha(**st)){//
            ed=*st;
            while(*ed!='+'&&*ed!='-'&&*ed!='*'&&*ed!='/'&&*ed!='('&&*ed!=')'&&*ed!='\0')ed++;
            if(*ed=='\0')endflag=1;
            if(endflag==0){
                temp=*ed;
                *ed='\0';
            }
            if((i=checkdec(*st))!=-1){
                if(endflag==0){
                    *ed=temp;
                    *st=ed-1;
                }
                switch(tokenTable[i].type){
                    case 's':
                    printf("Invalid operation on string %s\n",*st);
                    return -1;
                    case 'i':
                    return *(int*)tokenTable[i].pVal;
                    case 'd':
                    return *(double*)tokenTable[i].pVal;
                }
            }
            else {
                printf("No variety %s !\nCauculation Fail.\n",*st);
                return -1;
            }
        }
    //}
    return -1;
}
double getValue(char str[]){
    stacklist bottom=initialStack();
	stacklist *top=&bottom;
	double num[20]={0};//存放数字，压入栈的只是数字的下标（负数）
	//char str[50];
	//gets(str);
	char neostr[20];//存放后缀表达式
	int num_index=1,n_char=0;
	char *st,*ed;//指向字符串数字的起止位置
	int prior,num1=0,num2=0;
	for(endflag=0,st=str,ed=str;*st!='\0'&&endflag==0;st++){//开始进入循环的时候end肯定要清零，因为第二次运行时endflag还是1
        //endflag=0;//FindValinStr函数可能会改变endflag的值
        switch(check_type(*st)){
        case 0:
            ed=st+1;
            while(isdigit(*ed)||(*ed)=='.')ed++;//执行完循环此时ed指向非数字或者小数点的第一个元素
            num[num_index]=atof(st);
            neostr[n_char++]=num_index;
            num_index++;
            st=ed-1;//因为最后st还要加一次1
            break;
        case 2://运算符的情况
            if(POP(&prior,top)==0)//空栈的情况
                PUSH(*st,top);
            else if(check_advance(prior,*st)<0)//如果新的符号优先级比栈顶运算符高，直接加到栈顶
            {
                PUSH(prior,top);
                PUSH(*st,top);
            }
            else if(check_advance(prior,*st)>=0){//如果新的符号优先级小于等于栈顶运算符，那么将所有优先级大或者相等的运算符依次出栈，然后压入当前运算符
                    neostr[n_char++]=prior;
                    while(POP(&prior,top)){
                        if (check_type(prior)==2&&check_advance(prior,*st)>=0)//是否是运算符，是是比当前元素优先级高或相等
                        neostr[n_char++]=prior;//如果是运算符，那么都要出来
                        else{
                            PUSH(prior,top);
                            break;
                        }
                    }
                    PUSH(*st,top);
            }
            break;
        case 3://变量名的情况
            if((num[num_index]=FindValinStr(&st))!=-1)
                {
                    neostr[n_char++]=num_index;
                    num_index++;
                }
            else return 0;
            break;
        case -1://左括号的情况
            PUSH(*st,top);
            break;
        case 1://右括号
            if(POP(&prior,top)==0){
                printf("Cannot start with 'C'!\n");
                return -1;
            }
            while(prior!='('&&*top!=NULL){
                  neostr[n_char++]=prior;
                  POP(&prior,top);
            }
            if(*top==NULL){
                printf("')'is more than '('!\n");
                return -1;
            }
            break;
        }
	}
	if(POP(&prior,top)!=0)
	while(*top!=NULL)//检查是否为空栈，如果是，则prior存放的就是“零”
    {
        if(prior=='('){
            printf("'('is more than ')'!\n");
            return -1;
        }
        neostr[n_char++]=prior;
        if(POP(&prior,top)==0)break;
    }
    neostr[n_char++]=0;
    //以上为后缀表达式运算过程，接下来进行计算

    for(st=neostr;*st!='\0';++st){
        if(*st>0&&*st<36){//ascll码小于36的都不是运算符
            PUSH(*st,top);
            continue;
        }
        switch(*st){
            case '+':
            POP(&num2,top);
            POP(&num1,top);
            num[num1]+=num[num2];
            PUSH(num1,top);
            break;
            case '-':
            POP(&num2,top);
            POP(&num1,top);
            num[num1]-=num[num2];
            PUSH(num1,top);
            break;
            case '*':
            POP(&num2,top);
            POP(&num1,top);
            num[num1]*=num[num2];
            PUSH(num1,top);
            break;
            case '/':
            POP(&num2,top);
            POP(&num1,top);
            if(abslu(num[num2])>EPS)
            num[num1]/=num[num2];
        else {
            printf("Divide Zero Error!\n");
            return -1;
        }
            PUSH(num1,top);
            break;
        }
    }
	return num[1];
}
double abslu(double d){
    return d>0?d:-d;
}
int check_type(char c)
{
    if(c=='(')return -1;
    if(c==')')return 1;
    if(c=='+'||c=='-'||c=='*'||c=='/')return 2;
    if(isdigit(c))return 0;
    if(isalpha(c))return 3;
    return 0;
}
int check_advance(char op1,char op2)//op1存放栈顶元素，op2存放当前元素（还未进栈）
{
    if(op1=='(')return -1;
    if(op1=='+'||op1=='-')op1=0;
    if(op1=='*'||op1=='/')op1=1;
    if(op2=='+'||op2=='-')op2=0;
    if(op2=='*'||op2=='/')op2=1;
    return op1-op2;
}

#endif
