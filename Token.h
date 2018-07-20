#ifndef _TOKEN_H_
#define _TOKEN_H_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include "GetValue.h"
#include "datado.h"

int echo=1,exitflag=0;
//标识变量，在文件读取的时候不显示输入提示符
void doFunction(char instr[]);
//执行功能
//接受参数:一个字符串
int tell_op(char str[]);
//功能:输入判断,判断语句类型
void Calculate(char instr[]);
//计算功能:输入一个完整的带一个等号的语句,例如:a=3+4
int mathCalculation(int vindex,char expression[]);
//进行数学表达式的计算
//接受参数：将要存储的变量的标号，纯计算表达式
void StorgeVarieties(char *pn);
//存储变量，并酌情自动赋初值（零或者空地址（字符串））
char* TellVar(char instr[]);
//用于声明变量的时候，判断将要定义的数据类型
void IOfunction(char instr[]);
//输入与输出，有print和input两个功能
void readFileName(char name[]);
//接收两个参数：之前的输入流，新的输入流
//实现功能：将输入重定向到新的输入流，然后还原到原来的输入流,实现从文件中读取语句
void readFromFile();
//在命令行中显示提示信息并调用readFileName函数
void Forloop(char instr[]);
//功能：for循环
void IfFun(char instr[]);
//if分支功能
//提示:for和if两个不能自身以及相互嵌套，因为函数里用gets来读取语句，因此只能从命令行或文件中（输入重定向）读取命令
void deleteFun(char vname[]);
//变量删除功能
void deleteFun(char vname[])
{
    int i;
    char *pname=NULL;
    if((pname=strchr(vname,' '))==NULL) {
        return;
    }
    pname++;
    if((i=checkdec(pname))!=-1) {
        free(tokenTable[i].pName);
        free(tokenTable[i].pVal);
        for(; i<nVal-1; i++) {
            tokenTable[i]=tokenTable[i+1];    //覆盖操作
        }
        tokenTable[nVal-1].pName=NULL;
        tokenTable[nVal-1].pVal=NULL;
        --nVal;
    } else {
        printf("%s does not exist.\n",pname);
    }
    return;
}
void IfFun(char inputstr[])
{
    char *left,*right;//存储字符串名
    char instr[30];//暂时存储语句
    char op;//存储操作符
    int trueflag=1;
    int nleft,nright;//存储下标
    if((left=strchr(inputstr,' '))!=NULL&&((right=strchr(inputstr,'>'))!=NULL||(right=strchr(inputstr,'<'))!=NULL||(right=strchr(inputstr,'='))!=NULL)) {
        left++;
        op=*right;
        *right='\0';
        right++;
        if((nleft=checkdec(left))!=-1&&(nright=checkdec(right))!=-1) {
            //不能比较字符串
            if(tokenTable[nleft].type=='s'||tokenTable[nright].type=='s') {
                printf("Cannot compare a string!");
                trueflag=0;
            } else if(tokenTable[nleft].type!=tokenTable[nright].type) {
                printf("Cannot compare different type!");
                trueflag=0;
            }
            if(tokenTable[nleft].type=='i'&&trueflag==1)
                switch(op) {
                case '>':
                    if (*((int*)tokenTable[nleft].pVal)<=*((int*)tokenTable[nright].pVal)) {
                        trueflag=0;
                    }
                    break;
                case '<':
                    if (*((int*)tokenTable[nleft].pVal)>=*((int*)tokenTable[nright].pVal)) {
                        trueflag=0;
                    }
                    break;
                case '=':
                    if (*((int*)tokenTable[nleft].pVal)!=*((int*)tokenTable[nright].pVal)) {
                        trueflag=0;
                    }
                    break;

                }
            else if(tokenTable[nleft].type=='d'&&trueflag==1)
                switch(op) {
                case '>':
                    if (*((double*)tokenTable[nleft].pVal)<=*((int*)tokenTable[nright].pVal)) {
                        trueflag=0;
                    }
                    break;
                case '<':
                    if (*((int*)tokenTable[nleft].pVal)>=*((int*)tokenTable[nright].pVal)) {
                        trueflag=0;
                    }
                    break;
                case '=':
                    if (*((int*)tokenTable[nleft].pVal)!=*((int*)tokenTable[nright].pVal)) {
                        trueflag=0;
                    }
                    break;

                }
        }
    } else {
        trueflag=0;
    }
    if(trueflag==0) {
        gets(instr);
        while(strcmp(instr,"endif")!=0) {
            gets(instr);
        }
        return;
    }

//下面是复制粘贴for循环中的语句，略有改动
    char *if_instructions[50];
    int counti=1,i;
    gets(instr);
    if(strcmp(instr,"endif")==0) {
        return;
    }
    if_instructions[0]=(char*)malloc(strlen(instr)*sizeof(char));
    strcpy(if_instructions[0],instr);
    for(i=1; i<50; i++,counti++) {
        gets(instr);
        if(strcmp(instr,"endif")==0) {
            break;
        }
        if_instructions[i]=(char*)malloc(strlen(instr)*sizeof(char));
        strcpy(if_instructions[i],instr);
    }
    //真正的循环在下面进行
    for(i=0; i<counti; i++) {
        doFunction(if_instructions[i]);
        free(if_instructions[i]);
    }
    return;
}
int mathCalculation(int vindex,char expression[])
{
    if(vindex==-1) {
        printf("No such Variety.\n");
        return -1;
    }
    if(tokenTable[vindex].type=='d') {
        *((double*)tokenTable[vindex].pVal)=getValue(expression);
        return 0;
    } else if(tokenTable[vindex].type=='i') {
        *((int*)tokenTable[vindex].pVal)=(int)getValue(expression);
        return 0;
    }
    return 1;
}
void readFromFile()
{
    char name[100];
    printf("Please input the file name: ");
    gets(name);
    readFileName(name);
    return;
}

void readFileName(char name[])
{
    echo=0;
    char instr[100];
    FILE *fp;
    if((fp=fopen(name,"r"))==NULL) {
        printf("File cannot open!\n");
        echo=1;
        return;
    }
    fclose(fp);
    freopen(name,"r",stdin);
    while(!feof(stdin)) {
        memset(instr,0,20);
        gets(instr);
        doFunction(instr);
        if(instr==NULL) break;
    }
    freopen("CON","r",stdin);//读取结束后，将输入流定向到原来的地方（这里是控制台）
    echo=1;
    return;
}
void doFunction(char instr[])
{
    char *pn;//pn用于暂时存放变量类型
    if (strcmp(instr, "exit") == 0) exitflag=1;
     else if(strcmp(instr, "file")==0) readFromFile();
     else
        switch(tell_op(instr)) {
        case 0:
            Calculate(instr);
            break;
        case 1: {
            char *pdivide;
            pn = TellVar(instr);//pn这个时候是“string”或者“int”/"double"
            pdivide=strchr(pn,',');
            while(pdivide!=NULL) {
                *pdivide='\0';
                tokenTable[nVal+1].type=tokenTable[nVal].type;
                StorgeVarieties(pn);
                pn=pdivide+1;
                pdivide=strchr(pn,',');
            }
            StorgeVarieties(pn);
            break;
        }
        case 2:
            IOfunction(instr);
            break;
        case 3:
            Forloop(instr);
            break;
        case 4:
            IfFun(instr);
            break;
        case 5:
            deleteFun(instr);
            break;
        default:
            if(instr[0]!='\0') {
                printf("Instruction \"%s\" is invalid!\n\n",instr);
            }
            break;
        }
}
void Calculate(char oriinstr[])   //进行运算操作，支持字符串加减运算以及数值的任意运算
{
    char instr[30],*pl,*pr;//pl指向数据的左边界，pr指向数据右边界，主要用于字符串常量的拼接
    strcpy(instr,oriinstr);
    pl = strchr(instr, '=');
    int result;//代表目标数据下标
    *pl = '\0';
    ++pl;//此时pl指向后面的表达式
    if((result=checkdec(instr))!=-1)
        switch(tokenTable[result].type) {
        case 'i':
        case 'd':
            mathCalculation(result,pl);//如果是整型或者浮点型，那么直接调用已有的计算函数
            break;
        case 's': {
            int i;//storge the index of data
            pr = strchr(pl, '+');
            char newstring[100]= {0};
            while(pr!=NULL) {
                *pr='\0';
                pr++;
                if (*pl==34) { //34是双引号的ASCLL码
                    pl++;
                    if((pr=strchr(pl,34))==NULL) {
                        printf("No Match \"\"\"! ");
                        return;
                    }
                    *pr='\0';
                    strcat(newstring,pl);
                    pr++;
                    pr++;//pr移动到加好后面的一个元素
                } else {
                    if((i=checkdec(pl))!=-1) {
                        if(tokenTable[i].type!='s') {
                            printf("Not a string!\n");
                            return;
                        }
                    } else {
                        printf("No such variety!\n");
                        return;
                    }
                    if((char*)tokenTable[i].pVal!=NULL) {
                        strcat(newstring, (char*)tokenTable[i].pVal);
                    }
                }
                pl=pr;
                pr=strchr(pl, '+');
            }//上述过程进行完毕以后，还有最后一项没有被加起来
            if (*pl==34) {
                pl++;
                if((pr=strchr(pl,34))==NULL) {
                    printf("No Match \"!\n");
                    return;
                }
                *pr='\0';
                strcat(newstring,pl);
                pr++;
                pr++;//pr移动到加好后面的一个元素
            } else {
                if((i=checkdec(pl))!=-1) {
                    if(tokenTable[i].type!='s') {
                        printf("Not a string!\n");
                        return;
                    }
                } else {
                    printf("No such variety!\n");
                    return;
                }
                if((char*)tokenTable[i].pVal!=NULL) {
                    strcat(newstring, (char*)tokenTable[i].pVal);
                }
            }
            char *str=(char*)malloc(sizeof(newstring));
            strcpy(str,newstring);
            if(tokenTable[result].pVal!=NULL) {
                free(tokenTable[result].pVal);    //free the original space in Stack used by the variety.
            }
            tokenTable[result].pVal = str;
            tokenTable[result].size = strlen(str);
            break;
        }
        default:
            break;
        }
    return;
}

char* TellVar( char instr[])  //判断输入数据类型
{
    char *pd;
    char *firstworld;//存储第一个单词，判断类型
    pd = strchr(instr, ' ');//求取运算指令
    if (pd != NULL)
    *pd = '\0';
    firstworld = (char*)malloc(pd - instr + 1);
    strcpy(firstworld, instr);
    if (strcmp(firstworld, "int") == 0) {
        tokenTable[nVal].type = 'i';
    } else if (strcmp(firstworld, "string") == 0) {
        tokenTable[nVal].type = 's';
    } else if (strcmp(firstworld, "double") == 0) {
        tokenTable[nVal].type = 'd';
    }
    free(firstworld);
    return pd + 1;//返回下一个字符的地址
}
void StorgeVarieties(char *pn)
{
    //pn指向名称，pd指向值
    char *pd, *s;
    pd = strchr(pn, '=');
    if (pd != NULL) { //检查变量是否已经声明，分为有赋值（等号）和无赋值两种情况
        *pd='\0';
        if(checkdec(pn)!=-1) {
            printf("Already decleared before!\n");
            return;
        } else {
            *pd='=';
        }
    } else if(checkdec(pn)!=-1) {
        printf("Already decleared before!\n");
        return;
    }
    switch (tokenTable[nVal].type) { //在TellVal函数运行结束后已经将类型赋好了
    case 'i':
        tokenTable[nVal].pVal = (int*)malloc(sizeof(int));
        pd = strchr(pn, '=');
        if (pd != NULL) {
            *pd = '\0';//将变量名与值分隔开
            tokenTable[nVal].pName = (char*)malloc(sizeof(pd - pn + 1));
            strcpy(tokenTable[nVal].pName, pn);
            pn = pd + 1;
            mathCalculation(nVal,pn);
        } else {
            tokenTable[nVal].pName = (char*)malloc(sizeof(pd - pn + 1));
            strcpy(tokenTable[nVal].pName, pn);
            *((int*)tokenTable[nVal].pVal) = 0;
        }
        break;
    case 'd':
        tokenTable[nVal].pVal = (double*)malloc(sizeof(double));
        pd = strchr(pn, '=');
        if (pd != NULL) {
            *pd = '\0';//将变量名与值分隔开
            tokenTable[nVal].pName = (char*)malloc(sizeof(pd - pn + 1));
            strcpy(tokenTable[nVal].pName, pn);
            pn = pd + 1;
            mathCalculation(nVal,pn);
        } else {
            tokenTable[nVal].pName = (char*)malloc(sizeof(pd - pn + 1));
            strcpy(tokenTable[nVal].pName, pn);
            *((double*)tokenTable[nVal].pVal) = 0;
        }
        break;
    case 's':
        tokenTable[nVal].pName = (char*)malloc(sizeof(pn));
        pd = strchr(pn, '=');
        if (pd != NULL) {
            *pd = '\0';
            pd++;
            s = (char*)malloc(strlen(pd) + 1);
            strcpy(s, pd);
            tokenTable[nVal].pVal = (char*)s;
            tokenTable[nVal].size = strlen(s);
        } else {
            tokenTable[nVal].pVal = NULL;
            tokenTable[nVal].size = 0;
        }
        strcpy(tokenTable[nVal].pName, pn);//将名字复制到pName指向的内存单元
        break;
    }
    ++nVal;
    return;
}
void IOfunction(char instr[])
{
    char *pw;//存储空格后面的变量名
    int i;
    pw = strchr(instr, ' ');
    *pw = '\0';//操作结束后，instr指向的或者是print，或者是input
    ++pw;
    if (strcmp(instr, "print") == 0) {
        for (i = 0; i<nVal; i++) {
            if (strcmp(pw, tokenTable[i].pName) == 0) {
                switch (tokenTable[i].type) {
                case 'i':
                    printf("%d\n", *(int*)tokenTable[i].pVal);
                    break;
                case 'd':
                    printf("%.6f\n", *(double*)tokenTable[i].pVal);
                    break;
                case 's':
                    puts((char*)tokenTable[i].pVal);
                    break;
                }
                return;
            }
        }
        printf("No variety %s to be print!\n",pw);
        return;
    } else if (strcmp(instr, "input") == 0) {
        for (i = 0; i<nVal; i++) {
            if (strcmp(pw, tokenTable[i].pName) == 0) {
                switch (tokenTable[i].type) {
                case 'i':
                    printf("Please input an integer:\n");
                    scanf("%d", (int*)tokenTable[i].pVal);
                    getchar();
                    return;
                case 'd':
                    printf("Please input an double number:\n");
                    scanf("%lf", (double*)tokenTable[i].pVal);
                    getchar();
                    return;
                case 's': {
                    char str[30], *s;
                    printf("Please enter a string:\n");
                    gets(str);
                    s = (char*)malloc(strlen(str)+1);//需要的内存单元是多一个的
                    strcpy(s, str);
                    free(tokenTable[i].pVal);//这里还是会报错
                    tokenTable[i].pVal = (char*)s;
                    tokenTable[i].size = strlen(s);
                    return;
                }
                }
            }
        }
        printf("No variety %s !\n",pw);

    }
    return;
}
int tell_op(char instr[])
{
    char str[100];
    strcpy(str,instr);
    char stype=-1;
    //0代表计算,1代表声明变量，2代表输入输出，3代表for循环，4代表if语句，5代表删除语句，-1代表语句错误
    char *e,*p = strchr(str, ' ');
    if(p==NULL) { //如果不存在空格但是有等号的话，那么就是赋值语句
        if ((e=strchr(str,'='))!=NULL) {
            *e='\0';
            if (checkdec(str)!=-1) {
                return 0;
            }
        }
    } else {
        *p='\0';//先暂时把这一位设置为空字符，便于字符串比较
        if (strcmp(str, "int") == 0) stype=1;
         else if (strcmp(str, "double") == 0) stype=1;
         else if (strcmp(str, "string") == 0) stype=1;
         else if (strcmp(str, "print") == 0) stype=2;
         else if (strcmp(str, "input") == 0) stype=2;
         else if (strcmp(str, "for") == 0) stype=3;
         else if (strcmp(str, "if") == 0) stype=4;
         else if (strcmp(str, "delete")==0) stype=5;
         else if ((e=strchr(str,'='))!=NULL) { //如果有空格也有等号，那么很可能是字符串的赋值语句
            *e='\0';
            if (checkdec(str)!=-1) {
                stype=0;
            }
        }
    }
    //如果不存在数据类型的值，那么这不是一条变量声明语句
    return stype;
}
void Forloop(char instr[])
{
    //for i=1:100:2
    int indexc;//存储计数变量标号
    int maxv;//存储最大值
    int step=1;//步长
    char *st,*en;//用于分割字符串
    char *instructions[20];//存储for循环体内部的指令
    int counti=1,i;//用于存储指令的数目
    en=strchr(instr,'=');
    *en='\0';
    st=strchr(instr,' ');
    st++;//st指向'i'，st：i
    //check whether the count variety is decleared berfore.
    if((indexc=checkdec(st))==-1) { //if not decleared
        *en='=';
        if((en=strchr(st,':'))!=NULL) {
            *en='\0';
        }
        char *makecount=(char*)malloc(20);//分配临时的空间存储输入的forloop语句
        strcpy(makecount,"int ");//第一步：设置i变量的值
        strcat(makecount,st);//int i=1
        doFunction(makecount);
        free(makecount);
        indexc=nVal-1;//此时index存储i的序号
    } else { //if decleared
        *en='=';
        if((en=strchr(st,':'))!=NULL) {
            *en='\0';    //st:i=1
        }
        doFunction(st);
    }
    en++;//en指向最大值的首地址
    maxv=atoi(en);
    if((st=strchr(en,':'))!=NULL) {
        ++st;
        step=atoi(st);
    }
    gets(instr);
    if(strcmp(instr,"end")==0)return;//因为第一条语句也可能是end，如果是的话，直接退出,不需要执行下面的语句
    instructions[0]=(char*)malloc(strlen(instr)*sizeof(char));
    strcpy(instructions[0],instr);

    for(i=1; i<20; i++,counti++) {
        gets(instr);
        if(strcmp(instr,"end")==0)
            break;
        instructions[i]=(char*)malloc(strlen(instr)*sizeof(char));//存储语句的操作
        strcpy(instructions[i],instr);
    }
    //真正的循环在下面进行
    for(; *((int*)tokenTable[indexc].pVal)<=maxv; (*((int*)tokenTable[indexc].pVal))+=step)
        for(i=0; i<counti; i++)
            doFunction(instructions[i]);
    for(i=0; i<counti; i++)
        free(instructions[i]);

    return;
}
#endif
