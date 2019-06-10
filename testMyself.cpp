#include<stdio.h>
#include<malloc.h>
int *globalC;
int **globalD;
int *fun4(int n, int *ptr);
int *fun5(int n, int *ptr, int **pptr);
int *fun6(int n, int *ptr, int **pptr);
int *fun7(int n, int *ptr, int **pptr);

int *fun1(int n, int *ptr, int **pptr)
{
		ptr = fun7(n, ptr, pptr);
    globalC = ptr;
    pptr = globalD;
    //fun7();
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
        return pptr;
    }
    else
    {
        globalD = &ptr1;
        ptr = &n;
        *globalD = ptr;
        ptr1 = *pptr;
        return globalD;
    }
    int *p2 = fun5(n, ptr, pptr);
}

int *fun3(int n, int *ptr)
{
    ptr = &n;
    fun4(n, ptr);
    return ptr;
}

int *fun4(int n, int *ptr){
		fun3(n, ptr);
		return ptr;
}

int *fun5(int n, int *ptr, int **pptr){
		ptr = fun6(n, ptr, pptr);
		return ptr;
}

int *fun6(int n, int *ptr, int **pptr){
		ptr = fun1(n, ptr, pptr);
		return ptr;
}

int *fun7(int n, int *ptr, int **pptr){
		ptr = fun5(n, ptr, pptr);
		return ptr;
}

struct PointerCase
{
    int *a,*b;
    int **d,**e;
    int g;

};

int main()
{
    int a,b,c,d;
    int **e,**f,**q;
    int *g,*h,*p;
    
    int ArrayTmp[10];
    int *PointerArray[10];
    struct PointerCase pointerCase;

    a=1;
    b=2;
    c=3;
    d = a+b+c;
    d = a -b;
    c= d*a;
    b = c+1;
    e =f;
    globalC=*e;
    if(b>0)
    {
        *e = g;
        h = *e;
        q = &h;
        *f = p;
    }
    else
    {
        *q = p;
        g = h;
        p = &b;
    }

    while(a!=0)
    {
        h = *q;
        g = p;
    }

    switch (c)
    {
        case 1:
            d = a + c;
            p = &d;
            break;
        case 2:
            a = d + (c - 1)*2;
            p = &a;
            break;
            
        default:
            break;
    }

    if(d>0)
    {
        p = fun1(a,g,e);
    }
    else
    {
        e = fun2(b,h,q,p);
    }
    fun3(c,g);
    fun4(c,g);
    if(c != 1)
    {
        pointerCase.a = g;
        h = pointerCase.b;
        *pointerCase.d = p;
        h = *pointerCase.e;
        if(a > 0)
        {
            p = &ArrayTmp[2];
            for(int i=0;i<10;i++)
            {
                for(int j=0;j<10;j++)
                    fun3(a,p);
            }
        }
        else
        {
            PointerArray[2] = p;
            PointerArray[1] = pointerCase.b;
            pointerCase.d = &PointerArray[5];
            *pointerCase.e = PointerArray[6];
            PointerArray[7] = *pointerCase.e;
            f = &PointerArray[1];
            PointerArray[3] = *f;
            *e = PointerArray[4];
            
        }
        
        
    }
    else
    {
        pointerCase.e = & g;
        f = &pointerCase.a;
        *e = pointerCase.b;
        pointerCase.a = *f;
    }
    
    while(a>0)
    {
        g = PointerArray[0];
        pointerCase.a = PointerArray[1];
        a--;
        if(b>0)
        {
            *e = g;
            h = *e;
            q = &h;
            *f = p;
        }
        else
        {
            *q = p;
            g = h;
            p = &b;
        }
    }

    int * newP;
    newP = (int *)malloc(sizeof(int)*10);
    int *newPP;
    newPP = new int[10];

    return 0;
}