/*--------------------------------------------------------
文件名：Compiler_5.0.c
版本：5.0.0
新版本特性：
		支持任意表达式赋值（整型，浮点型），表达式可以使用变量名（不支持字符串）
		用输入重定向功能
        支持数学运算表达
        将函数声明等部分放在头文件中，main函数更简洁
        后来通过使用字符指针数组实现了for需要文件重定向造成的麻烦，现在实现了文件中for的功能
作者：李吉宇
日期：14周（做了一个debug）第十周的时候是版本1.0（源文件已经消失）
程序功能：实现对一些简单语句的解释运算。支持的运算种类：
1.声明变量：整型,浮点型和字符串型
    int a(这种情况下a=0)
    int a=45
    int b=44
    double d=34.33
    string str(str为空）
    string str1=hello!
    string str2=like
1.1 新版本支持连续声明，即int a=34,b=112,c=4,d
                        string st1=asde,bea=xaas
                    不足之处是string连续声明只以逗号为分隔符
1.2 支持声明变量的时候进行计算，支持这个时候变量参与计算
1.3 支持删除已经声明的变量(释放变量的空间)
    delete valname
2.变量之间的运算：整型,浮点型：加减乘除；字符串：相加
    c=34+(a*b)-6/3
    str=str1+str2
2.2等号后面有两个元素的支持变量名与数值混合
    a=34+b
2.3支持数学表达式的运算
    d=3*5+(34.4)/1
2.4支持字符串任意相加，字符串常量用双引号括起来
    a=b+"Hello World"
3.输入输出：整型,double,或者字符串类型
    input str
    input a
    print str
    print a
4.循环语句
    for i=1:100
    *
    *语句……
    *
    end
5.if语句
    *
    *……
    *
    endif
6.从文件中读取命令文件：输入“file”命令，随后输入文件路径
    readformfile
7.读取命令行参数：在命令的后面紧跟文件路径，个数不限

我觉得程序存在的不足：
for循环指令只能实现单层循环
数学表达式计算只支持35个数据左右，因为1-35的ascll码值都不是运算符，用于存储double型数组数据下标
输入命令支持最大长度100
字符串支持最大长度100(这个可以提高上限，但觉得没有必要)
不支持数组
-------------------------------------------------------------------------------*/
#include"datado.h"
#include"GetValue.h"
#include"Token.h"
#include<stdio.h>
#include<direct.h>
int main(int argc,char *argv[])
{
    char instr[100];
    printf("Instructions Explainer [Version:5.0.0]\n2017 DHU Computer Science and Technology\nAuthor: Athelare_JY\n\nCurrent Folder:\n%s\n\n",getcwd(NULL,100));
    memset(tokenTable, 0, sizeof(tokenTable));//程序一开始运行就将符号表清零
    if(argc>1)//实现从命令行中读取参数(允许参数为文件名)
    {
        int i;
        for(i=1; i<argc; i++)
            readFileName(argv[i]);//读取文件,执行文件中的命令
    }
    while (!exitflag)//exit是在token中定义的标志变量，代表结束
    {
        if(feof(stdin))//如果读到了EOF,那肯定是读到了文件的末尾,因此要把输入重定向到控制台
        {
            freopen("CON","r",stdin);
            echo=1;//echo是Tohen中定义的标识变量，在读文件的时候会设置为零,只显示输出的结果
        }
        if(echo==1)printf("-->>");
        gets(instr);
        doFunction(instr);//doFuncion是主要的执行命令的程序
    }
    return 0;
}
