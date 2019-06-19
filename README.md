# Ouroboros-dataset
- callgraph - 当前项目的callgraph。
- before zyy - 没有经过处理的llvm IR。
- zyy result - 对llvm IR做预处理，将符合某些规则的几条IR语句转换成assign/store/load/alloca类型的statement，**sigvar_info.txt**中是所有singleton变量。
- ssa result - 对预处理后的结果做SSA转换，并且每条statement对应一个用Integer表示的node，node之间连接的边构成了每个函数的intragraph：**inline.txt**，**node2stmt.txt**中记录了每个node与具体statement之间的对应关系。
- inline result - 结合callgraph对intragraph做inline处理，**final**为进行过inline处理的intergraph，**id_stmt_info.txt**中是final里每个node与具体statement的index之间的对应关系，**index_var_info.txt**中是每个index和其代表的variable之间的对应关系，**var_singleton_info.txt**中是进行完inline后singleton类型的变量。

# Variable
varialbe分为全局变量和局部变量，全局变量以 "@" 开头，局部变量以 "%" 开头，局部变量又分为top-level和address-taken。  
全局变量不进行SSA转换，inline后的表示形式还是变量名本身，如： `@globalC`   
top-level类型的变量需要进行SSA处理，多次赋值会产生多个版本，因此top-level类型的变量表示形式为：inlineVersion.functionName.variableName-ssaVersion，如：`0._Z4fun2iPiPS_S_.%ptr.addr-1`  
address-taken类型的变量不需要进行SSA处理，因此address-taken类型的变量表示形式为：inlineVersion.functionName.variableName，如：`0._Z4fun2iPiPS_S_.%n.addr`

# Statement
statement共包含9种类型：assign/load/store/alloca/phi/call/return/ret/block without stmt。每条statement中使用制表符 **\\t** 作为类型与变量间、变量与变量间的分隔符。

## assign
assign statement表示源代码中形如`x = y`的两个指针变量间的赋值操作。assign statement的格式如下：  
`assign\t左操作数\t右操作数`  
例如，源代码testExample.cpp的函数fun2中的语句`ptr = ptr1;`，其对应的assign statement为：
>    assign **\\t** 0.\_Z4fun2iPiPS_S_.%ptr.addr-1 **\\t** 0.\_Z4fun2iPiPS_S_.%ptr1.addr

## store
store statement表示源代码中形如`*x = y`语句。store statement的格式如下：  
`store\t*x\ty\tx`  
例如，源代码testExample.cpp的函数fun2中的语句`*pptr = ptr;`，其对应的store statement为：
>    store **\\t** 0.\_Z4fun2iPiPS_S_.\*%pptr.addr-0 **\\t** 0.\_Z4fun2iPiPS_S_.%ptr.addr-0 **\\t** 0.\_Z4fun2iPiPS_S_.%pptr.addr-0

## load
load statement表示源代码中形如`x = *y`的语句。load statement的格式如下：  
`load\tx\t*y\ty`  
例如，源代码testExample.cpp的函数fun2中的语句`ptr1 = *pptr;`，其对应的load statement为：
>    load **\\t** 0.\_Z4fun2iPiPS_S_.%ptr1.addr **\\t** 0.\_Z4fun2iPiPS_S_.\*%pptr.addr-0 **\\t** 0.\_Z4fun2iPiPS_S_.%pptr.addr-0

## alloca
alloca statement表示内存分配，源代码中以下三种类型的语句会被转换成alloca statement：

    x = &y;
    通过malloc()函数分配内存；
    通过new关键字分配内存；

alloca statement的格式如下：  
`alloca\tx\t&y\ty`  
例如，源代码testExample.cpp的函数fun2中的语句`ptr = &n;`，其对应的alloca statement为：
>    alloca **\\t** 0.\_Z4fun2iPiPS_S_.%ptr.addr-2 **\\t** 0.\_Z4fun2iPiPS_S_.&%n.addr **\\t** 0.\_Z4fun2iPiPS_S_.%n.addr

## phi
如果一个基本块的多个前驱基本块对同一个变量进行了赋值，则当前基本块需要加入phi statement来计算该变量在基本块开始处的版本，并记录该版本是从哪些旧版本中衍生出来的。phi statement的格式如下：  
`phi\t当前基本块中的变量\t前驱基本块1中的变量\t前驱基本块2中的变量\t……`  
例如，源代码testExample.cpp的函数fun2中分别在if.then和if.else分支中进行了`ptr = ptr1;`和`ptr = &n;`操作，产生了**assign**和**alloca**小节示例中的statement，因此需要在merged block中插入phi statement计算ptr新的版本，其对应的phi statement为：
>    phi **\\t** 0.\_Z4fun2iPiPS_S_.%ptr.addr-3 **\\t** 0.\_Z4fun2iPiPS_S_.%ptr.addr-2 **\\t** 0.\_Z4fun2iPiPS_S_.%ptr.addr-1

## call
源代码中的每次函数调用，图中都会产生相应的call statement和return statement。call statement中不存在别名关系。如果函数调用存在参数传递，则会产生添加 "形式参数 = 实际参数" 的calledge node，并在图中添加call node -> calledge node; calledge node -> 被调用函数第一个node的边。call statement中仅包含类型名称，不包含操作数等内容，call statement格式如下：  
`call`

## return
源代码中的每次函数调用，图中都会产生相应的call statement和return statement。call statement中不存在别名关系。如果被调用函数存在返回值，则会产生添加 "实际返回值 = 形式返回值" 的returnedge node，并在图中添加被调用函数的ret node -> returnedge node; returnedge node -> return node的边。return statement中仅包含类型名称，不包含操作数等内容，return statement格式如下：   
`return`

## ret
如果一个函数存在返回值，则图中会有相应的ret statement。ret statement中不存在别名关系，仅用来和returnedge node连接成边。ret statement中仅包含类型名称，不包含操作数等内容，ret statement格式如下：  
`ret`

## block without stmt
如果一个基本块中没有statement，则需要添加一条block without stmt。该statement仅用于与前驱、后继基本块中的node连接成边，保证CFG没有发生变化。block without stmt中仅包含类型名称，不包含操作数等内容，block without stmt statement格式如下：  
`block without stmt`
