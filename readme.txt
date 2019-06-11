final是图文件，id_stmt_info.txt是图中id与statement对应关系。
statement中所有分隔符都是制表符\t
assign/load/store/alloca：type\t左操作数\t右操作数
call/return/ret/block without stmt没有意义，不需要分析
# Ouroboros-dataset
- before zyy - 没有经过任何处理的llvm IR形式，**callgraph**文件夹中是当前示例的callgraph，**zyy result**中是经过预处理

Statement
·assign

·load

·store

·alloca

·phi

·call

·return

·ret

·block without stmt