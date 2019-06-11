# Ouroboros-dataset
- callgraph - 当前项目的callgraph
- before zyy - 没有经过处理的llvm IR
- zyy result - 对llvm IR做预处理，将符合某些规则的几条IR语句转换成assign/store/load/alloca类型的statement
- ssa result - 对预处理后的结果做SSA转换，并且每条statement对应一个用Integer表示的node，node之间连接的边构成了每个函数的intragraph：**inline.txt**，**node2stmt.txt**中记录了每个node与具体statement之间的对应关系。
- inline result - 结合callgraph对intragraph做inline处理，**final**为进行过inline处理的intergraph，**id_stmt_info.txt**中是final里每个node与具体statement之间的对应关系。

# Statement
statement共包含9种类型：assign/load/store/alloca/phi/call/return/ret/block without stmt。每条statement中使用制表符 **\\t** 作为类型与变量间、变量与变量间的分隔符。

## assign
assign statement表示源代码中形如`x = y`的两个指针变量间的赋值操作。assign statement的格式如下：  
`assign\t左操作数\t右操作数`  
例如，源代码testExample.cpp的函数fun2中的语句`ptr = ptr1;`，其对应的assign statement为：
>    assign **\\t** \_Z4fun2iPiPS_S_.%ptr.addr.1 **\\t** \_Z4fun2iPiPS_S_.%ptr1.addr

## store
store statement表示源代码中形如`*x = y`语句。store statement的格式如下：  
`store\t左操作数\t右操作数`  
例如，源代码testExample.cpp的函数fun2中的语句`*pptr = ptr;`，其对应的store statement为：
>    store **\\t** \_Z4fun2iPiPS_S_.%pptr.addr.0 **\\t** \_Z4fun2iPiPS_S_.%ptr.addr.0

## load
load statement表示源代码中形如`x = *y`的语句。load statement的格式如下：  
`load\t左操作数\t右操作数`  
例如，源代码testExample.cpp的函数fun2中的语句`ptr1 = *pptr;`，其对应的load statement为：
>    load **\\t** \_Z4fun2iPiPS_S_.%ptr1.addr **\\t** \_Z4fun2iPiPS_S_.%pptr.addr.0

## alloca
alloca statement表示内存分配，源代码中以下三种类型的语句会被转换成alloca statement：

    x = &y;
    通过malloc()函数分配内存；
    通过new关键字分配内存；

alloca statement的格式如下：  
`alloca\t左操作数\t右操作数`  
例如，源代码testExample.cpp的函数fun2中的语句`ptr = &n;`，其对应的alloca statement为：
>    alloca **\\t** \_Z4fun2iPiPS_S_.%ptr.addr.2 **\\t** \_Z4fun2iPiPS_S_.%n.addr

## phi
如果一个基本块的多个前驱基本块对同一个变量进行了赋值，则当前基本块需要加入phi statement来计算该变量在基本块开始处的版本，并记录该版本是从哪些旧版本中衍生出来的。phi statement的格式如下：  
`phi\t当前基本块中的变量\t前驱基本块1中的变量\t前驱基本块2中的变量\t……`  
例如，源代码testExample.cpp的函数fun2中分别在if.then和if.else分支中进行了`ptr = ptr1;`和`ptr = &n;`操作，产生了**assign**和**alloca**小节示例中的statement，因此需要在merged block中插入phi statement计算ptr新的版本，其对应的phi statement为：
>    phi **\\t** \_Z4fun2iPiPS_S_.%ptr.addr.3 **\\t** \_Z4fun2iPiPS_S_.%ptr.addr.2 **\\t** \_Z4fun2iPiPS_S_.%ptr.addr.1

## call
源代码中的每次函数调用，图中都会产生相应的call statement和return statement。call statement中不存在别名关系。如果函数调用存在参数传递，则会产生添加statement为assign\t形式参数\t实际参数的calledge node，并在图中添加call node -> calledge node; calledge node -> 被调用函数第一个node的边。call statement的格式如下：  
`call\t函数调用左操作数\t被调用函数`  
例如，源代码testExample.cpp的函数fun2中调用了函数fun1`ptr1 = fun1(n, ptr, pptr);`，其对应的call statement为：
>    call **\\t** \_Z4fun2iPiPS_S_.%call **\\t** i32* @\_Z4fun1iPiPS_(\_Z4fun2iPiPS_S_.%n.addr \_Z4fun2iPiPS_S_.%ptr.addr.1 \_Z4fun2iPiPS_S_.%pptr.addr.0 )

## return
源代码中的每次函数调用，图中都会产生相应的call statement和return statement。call statement中不存在别名关系。如果被调用函数存在返回值，则会产生添加statement为assign\t实际返回值\t形式返回值的returnedge node，并在图中添加被调用函数的ret node -> returnedge node; returnedge node -> return node的边。return statement的格式如下：  
`return\t函数调用左操作数\t被调用函数`  
例如，源代码testExample.cpp的函数fun2中调用了函数fun1`ptr1 = fun1(n, ptr, pptr);`，其对应的return statement为：
>    return **\\t** \_Z4fun2iPiPS_S_.%call **\\t** i32* @\_Z4fun1iPiPS_(\_Z4fun2iPiPS_S_.%n.addr \_Z4fun2iPiPS_S_.%ptr.addr.1 \_Z4fun2iPiPS_S_.%pptr.addr.0 )

## ret
如果一个函数存在返回值，则图中会有相应的ret statement。ret statement中不存在别名关系，仅用来和returnedge node连接成边。ret statement的格式如下：  
`ret\t函数返回值`  
例如，源代码testExample.cpp的函数fun1存在函数返回值`return ptr;`，其对应的ret statement为：
>    ret **\\t** \_Z4fun1iPiPS_.%ptr.addr.0

## block without stmt
如果一个基本块中没有statement，则需要添加一条block without stmt。该statement仅用于与前驱、后继基本块中的node连接成边，保证CFG没有发生变化。
