#include<stdio.h>
#include<malloc.h>
int *globalC;
int **globalD;

int *fun1(int n, int *ptr, int **pptr)
{
    globalC = ptr;
    pptr = globalD;
    int * newP;
    newP = (int *)malloc(sizeof(int)*10);
    int *newPP;
    newPP = new int[10];
    return ptr;
    *globalD = ptr;
    pptr = &globalC;
    globalC=*globalD;

    return globalC;
}
int **fun2(int n, int *ptr, int **pptr, int *ptr1)
{
    if(n>0)
    {
        globalC = &n;
        *pptr = ptr;
        ptr1 = *pptr;
        ptr = ptr1;
    		ptr1 = fun1(n, ptr, pptr);
        return pptr;
    }
    else
    {
        globalD = &ptr1;
        ptr = &n;
        *globalD = ptr;
        ptr1 = *pptr;
    		ptr1 = ptr;
        return globalD;
    }
}