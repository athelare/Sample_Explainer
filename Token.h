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
//��ʶ���������ļ���ȡ��ʱ����ʾ������ʾ��
void doFunction(char instr[]);
//ִ�й���
//���ܲ���:һ���ַ���
int tell_op(char str[]);
//����:�����ж�,�ж��������
void Calculate(char instr[]);
//���㹦��:����һ�������Ĵ�һ���Ⱥŵ����,����:a=3+4
int mathCalculation(int vindex,char expression[]);
//������ѧ���ʽ�ļ���
//���ܲ�������Ҫ�洢�ı����ı�ţ���������ʽ
void StorgeVarieties(char *pn);
//�洢�������������Զ�����ֵ������߿յ�ַ���ַ�������
char* TellVar(char instr[]);
//��������������ʱ���жϽ�Ҫ�������������
void IOfunction(char instr[]);
//�������������print��input��������
void readFileName(char name[]);
//��������������֮ǰ�����������µ�������
//ʵ�ֹ��ܣ��������ض����µ���������Ȼ��ԭ��ԭ����������,ʵ�ִ��ļ��ж�ȡ���
void readFromFile();
//������������ʾ��ʾ��Ϣ������readFileName����
void Forloop(char instr[]);
//���ܣ�forѭ��
void IfFun(char instr[]);
//if��֧����
//��ʾ:for��if�������������Լ��໥Ƕ�ף���Ϊ��������gets����ȡ��䣬���ֻ�ܴ������л��ļ��У������ض��򣩶�ȡ����
void deleteFun(char vname[]);
//����ɾ������
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
            tokenTable[i]=tokenTable[i+1];    //���ǲ���
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
    char *left,*right;//�洢�ַ�����
    char instr[30];//��ʱ�洢���
    char op;//�洢������
    int trueflag=1;
    int nleft,nright;//�洢�±�
    if((left=strchr(inputstr,' '))!=NULL&&((right=strchr(inputstr,'>'))!=NULL||(right=strchr(inputstr,'<'))!=NULL||(right=strchr(inputstr,'='))!=NULL)) {
        left++;
        op=*right;
        *right='\0';
        right++;
        if((nleft=checkdec(left))!=-1&&(nright=checkdec(right))!=-1) {
            //���ܱȽ��ַ���
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

//�����Ǹ���ճ��forѭ���е���䣬���иĶ�
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
    //������ѭ�����������
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
    freopen("CON","r",stdin);//��ȡ�����󣬽�����������ԭ���ĵط��������ǿ���̨��
    echo=1;
    return;
}
void doFunction(char instr[])
{
    char *pn;//pn������ʱ��ű�������
    if (strcmp(instr, "exit") == 0) exitflag=1;
     else if(strcmp(instr, "file")==0) readFromFile();
     else
        switch(tell_op(instr)) {
        case 0:
            Calculate(instr);
            break;
        case 1: {
            char *pdivide;
            pn = TellVar(instr);//pn���ʱ���ǡ�string�����ߡ�int��/"double"
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
void Calculate(char oriinstr[])   //�������������֧���ַ����Ӽ������Լ���ֵ����������
{
    char instr[30],*pl,*pr;//plָ�����ݵ���߽磬prָ�������ұ߽磬��Ҫ�����ַ���������ƴ��
    strcpy(instr,oriinstr);
    pl = strchr(instr, '=');
    int result;//����Ŀ�������±�
    *pl = '\0';
    ++pl;//��ʱplָ�����ı��ʽ
    if((result=checkdec(instr))!=-1)
        switch(tokenTable[result].type) {
        case 'i':
        case 'd':
            mathCalculation(result,pl);//��������ͻ��߸����ͣ���ôֱ�ӵ������еļ��㺯��
            break;
        case 's': {
            int i;//storge the index of data
            pr = strchr(pl, '+');
            char newstring[100]= {0};
            while(pr!=NULL) {
                *pr='\0';
                pr++;
                if (*pl==34) { //34��˫���ŵ�ASCLL��
                    pl++;
                    if((pr=strchr(pl,34))==NULL) {
                        printf("No Match \"\"\"! ");
                        return;
                    }
                    *pr='\0';
                    strcat(newstring,pl);
                    pr++;
                    pr++;//pr�ƶ����Ӻú����һ��Ԫ��
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
            }//�������̽�������Ժ󣬻������һ��û�б�������
            if (*pl==34) {
                pl++;
                if((pr=strchr(pl,34))==NULL) {
                    printf("No Match \"!\n");
                    return;
                }
                *pr='\0';
                strcat(newstring,pl);
                pr++;
                pr++;//pr�ƶ����Ӻú����һ��Ԫ��
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

char* TellVar( char instr[])  //�ж�������������
{
    char *pd;
    char *firstworld;//�洢��һ�����ʣ��ж�����
    pd = strchr(instr, ' ');//��ȡ����ָ��
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
    return pd + 1;//������һ���ַ��ĵ�ַ
}
void StorgeVarieties(char *pn)
{
    //pnָ�����ƣ�pdָ��ֵ
    char *pd, *s;
    pd = strchr(pn, '=');
    if (pd != NULL) { //�������Ƿ��Ѿ���������Ϊ�и�ֵ���Ⱥţ����޸�ֵ�������
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
    switch (tokenTable[nVal].type) { //��TellVal�������н������Ѿ������͸�����
    case 'i':
        tokenTable[nVal].pVal = (int*)malloc(sizeof(int));
        pd = strchr(pn, '=');
        if (pd != NULL) {
            *pd = '\0';//����������ֵ�ָ���
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
            *pd = '\0';//����������ֵ�ָ���
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
        strcpy(tokenTable[nVal].pName, pn);//�����ָ��Ƶ�pNameָ����ڴ浥Ԫ
        break;
    }
    ++nVal;
    return;
}
void IOfunction(char instr[])
{
    char *pw;//�洢�ո����ı�����
    int i;
    pw = strchr(instr, ' ');
    *pw = '\0';//����������instrָ��Ļ�����print��������input
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
                    s = (char*)malloc(strlen(str)+1);//��Ҫ���ڴ浥Ԫ�Ƕ�һ����
                    strcpy(s, str);
                    free(tokenTable[i].pVal);//���ﻹ�ǻᱨ��
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
    //0�������,1��������������2�������������3����forѭ����4����if��䣬5����ɾ����䣬-1����������
    char *e,*p = strchr(str, ' ');
    if(p==NULL) { //��������ڿո����еȺŵĻ�����ô���Ǹ�ֵ���
        if ((e=strchr(str,'='))!=NULL) {
            *e='\0';
            if (checkdec(str)!=-1) {
                return 0;
            }
        }
    } else {
        *p='\0';//����ʱ����һλ����Ϊ���ַ��������ַ����Ƚ�
        if (strcmp(str, "int") == 0) stype=1;
         else if (strcmp(str, "double") == 0) stype=1;
         else if (strcmp(str, "string") == 0) stype=1;
         else if (strcmp(str, "print") == 0) stype=2;
         else if (strcmp(str, "input") == 0) stype=2;
         else if (strcmp(str, "for") == 0) stype=3;
         else if (strcmp(str, "if") == 0) stype=4;
         else if (strcmp(str, "delete")==0) stype=5;
         else if ((e=strchr(str,'='))!=NULL) { //����пո�Ҳ�еȺţ���ô�ܿ������ַ����ĸ�ֵ���
            *e='\0';
            if (checkdec(str)!=-1) {
                stype=0;
            }
        }
    }
    //����������������͵�ֵ����ô�ⲻ��һ�������������
    return stype;
}
void Forloop(char instr[])
{
    //for i=1:100:2
    int indexc;//�洢�����������
    int maxv;//�洢���ֵ
    int step=1;//����
    char *st,*en;//���ڷָ��ַ���
    char *instructions[20];//�洢forѭ�����ڲ���ָ��
    int counti=1,i;//���ڴ洢ָ�����Ŀ
    en=strchr(instr,'=');
    *en='\0';
    st=strchr(instr,' ');
    st++;//stָ��'i'��st��i
    //check whether the count variety is decleared berfore.
    if((indexc=checkdec(st))==-1) { //if not decleared
        *en='=';
        if((en=strchr(st,':'))!=NULL) {
            *en='\0';
        }
        char *makecount=(char*)malloc(20);//������ʱ�Ŀռ�洢�����forloop���
        strcpy(makecount,"int ");//��һ��������i������ֵ
        strcat(makecount,st);//int i=1
        doFunction(makecount);
        free(makecount);
        indexc=nVal-1;//��ʱindex�洢i�����
    } else { //if decleared
        *en='=';
        if((en=strchr(st,':'))!=NULL) {
            *en='\0';    //st:i=1
        }
        doFunction(st);
    }
    en++;//enָ�����ֵ���׵�ַ
    maxv=atoi(en);
    if((st=strchr(en,':'))!=NULL) {
        ++st;
        step=atoi(st);
    }
    gets(instr);
    if(strcmp(instr,"end")==0)return;//��Ϊ��һ�����Ҳ������end������ǵĻ���ֱ���˳�,����Ҫִ����������
    instructions[0]=(char*)malloc(strlen(instr)*sizeof(char));
    strcpy(instructions[0],instr);

    for(i=1; i<20; i++,counti++) {
        gets(instr);
        if(strcmp(instr,"end")==0)
            break;
        instructions[i]=(char*)malloc(strlen(instr)*sizeof(char));//�洢���Ĳ���
        strcpy(instructions[i],instr);
    }
    //������ѭ�����������
    for(; *((int*)tokenTable[indexc].pVal)<=maxv; (*((int*)tokenTable[indexc].pVal))+=step)
        for(i=0; i<counti; i++)
            doFunction(instructions[i]);
    for(i=0; i<counti; i++)
        free(instructions[i]);

    return;
}
#endif
