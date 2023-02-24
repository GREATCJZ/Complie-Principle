# Complie-Principle
词法分析、递归下降的语法分析、属性文法、四元式中间代码生成

refer to github and csdn

## SysY 语言定义(2022 版)1 

<br>
1. SysY 语言概要 
<br>
SysY 语言是编译系统设计赛要实现的编程语言。由 C 语言的一个子集扩展 而成。每个 SysY 程序的源码存储在一个扩展名为 sy 的文件中。该文件中有且仅有一个名为 main 的主函数定义，还可以包含若干全局变量声明、常量声明和其他函数定义。SysY 语言支持 int/float 类型和元素为 int/float 类型且按行优先存储的多维数组类型，其中 int 型整数为 32 位有符号数；float 为 32 位单精度浮点数；const 修饰符用于声明常量。SysY 支持 int 和 float 之间的隐式类型，但是无显式的强制类型转化支持。 
<br>
<br>
变量/常量声明：可以在一个变量/常量声明语句中声明多个变量或常量，声 
明时可以带初始化表达式。所有变量/常量要求先定义再使用。在函数外声明的 
为全局变量/常量，在函数内声明的为局部变量/常量。 
<br>
<br>
语句：语句包括赋值语句、表达式语句(表达式可以为空)、语句块、if 语句、 
while 语句。语句块中可以包含若干变量声明和语句。 
<br>
<br>
表达式：支持基本的算术运算（+、-、*、/、%）、关系运算（==、!=、<、>、 
<=、>=）和逻辑运算（!、&&、||），非 0 表示真、0 表示假，而关系运算或逻辑运算的结果用 1 表示真、0 表示假。算符的优先级和结合性以及计算规则(含逻辑运算的“短路计算”)与 C 语言一致。 

<br>
<br>
<br>
2.SysY 语言的文法 
<br>
SysY 语言的文法采用扩展的 Backus 范式（EBNF，Extended Backus-Naur 
Form）表示，其中：
<br>
符号[...]表示方括号内包含的为可选项； 
<br>
符号{...}表示花括号内包含的为可重复 0 次或多次的项； 
<br>
终结符或者是单引号括起的串，或者是 Ident、InstConst、floatConst 这样的记号。 
<br>
<br>
SysY 语言的文法表示如下，其中 CompUnit 为开始符号： 
<br>
编译单元 
<br>
CompUnit → [ CompUnit ] ( Decl | FuncDef ) 
<br>
<br>
声明 
<br>
Decl → ConstDecl 
<br>| VarDecl 
<br>
<br>
常量声明 
<br>
ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';' 
<br>
<br>
基本类型 
<br>
BType → 'int' 
<br>| 'float' 
<br>
<br>
常数定义 
<br>
ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal 
<br>
<br>
常量初值 
<br>
ConstInitVal → ConstExp 
<br>| '{' [ ConstInitVal { ',' ConstInitVal } ] '}' 
<br>
<br>
变量声明 
<br>
VarDecl → BType VarDef { ',' VarDef } ';' 
<br>
<br>
变量定义 
<br>
VarDef → Ident { '[' ConstExp ']' } 
<br>| Ident { '[' ConstExp ']' } '=' InitVal 
<br>
<br>
变量初值 
<br>
InitVal → Exp 
<br>| '{' [ InitVal { ',' InitVal } ] '}' 
<br>
<br>
语句块 
<br>
Block → '{' { BlockItem } '}' 
<br>
<br>
语句块项 
<br>
BlockItem → Decl 
<br>| Stmt 
<br>
<br>
语句 
<br>
Stmt → LVal '=' Exp ';' 
<br>
| [Exp] ';' 
<br>| Block 
<br>
| 'if' '( Cond ')' Stmt [ 'else' Stmt ] 
<br>
| 'while' '(' Cond ')' Stmt 
<br>
<br>
表达式 
<br>
Exp → AddExp 
<br>
注：SysY 表达式是 int/float 型表达式 
<br>
<br>
条件表达式 
<br>
Cond → LOrExp 
<br>
<br>
左值表达式 
<br>
LVal → Ident {'[' Exp ']'} 
<br>
<br>
基本表达式 
<br>
PrimaryExp → '(' Exp ')' 
<br>| LVal 
<br>| Number 
<br>
<br>
数值 
<br>
Number → IntConst | floatConst 
<br>
<br>
一元表达式 
<br>
UnaryExp → PrimaryExp 
<br>| Ident '(' [FuncRParams] ')' 
<br>
| UnaryOp UnaryExp 
<br>
<br>
单目运算符 
<br>
UnaryOp → '+' 
<br>| '−' 
<br>| '!' 
<br>注：'!'仅出现在条件表达式中 
<br>
<br>
乘除模表达式 
<br>
MulExp → UnaryExp 
<br>| MulExp ('*' | '/' | '%') UnaryExp 
<br>
<br>
加减表达式 
<br>
AddExp → MulExp 
<br>| AddExp ('+' | '−') MulExp 
<br>
<br>
关系表达式 
<br>
RelExp → AddExp 
<br>| RelExp ('<' | '>' | '<=' | '>=') AddExp 
<br>
<br>
相等性表达式 
<br>
EqExp → RelExp 
<br>| EqExp ('==' | '!=') RelExp 
<br>
<br>
逻辑与表达式 
<br>
LAndExp → EqExp 
<br>| LAndExp '&&' EqExp 
<br>
<br>
逻辑或表达式 
<br>
LOrExp → LAndExp 
<br>| LOrExp '||' LAndExp 
<br>
<br>
常量表达式 
<br>
ConstExp → AddExp 注：使用的 Ident 必须是常量 
<br>
<br>
<br>
SysY 语言的终结符特征 
<br>
1、标识符 Ident 
SysY 语言中标识符 Ident 的规范如下(identifier)： 
标识符 
<br>
identifier → identifier-nondigit 
<br>
| identifier identifier-nondigit 
<br>
| identifier digit 
<br>
其中 identifier-nondigit 为下划线或以下之一 , identifier-digit 为以下之一 
<br>
0 1 2 3 4 5 6 7 8 9 
<br>
注：请参考 ISO/IEC 9899 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf 
第 51 页关于标识符的定义 
<br>
<br>
同名标识符的约定： 
<br>
全局变量和局部变量的作用域可以重叠，重叠部分局部变量优先；同名局部变量的作用域不能重叠； 
<br>
SysY 语言中变量名可以与函数名相同。 
<br>
<br>
<br>
2、注释 
SysY 语言中注释的规范与 C 语言一致，如下： 
<br>
单行注释：以序列 ‘//’ 开始，直到换行符结束，不包括换行符。
<br> 
多行注释：以序列 ‘/*’ 开始，直到第一次出现 ‘*/’ 时结束，包括结束处 ‘*/’。 
<br>
注：请参考 ISO/IEC 9899 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf 
第 66 页关于注释的定义 
<br>
<br>
<br>
3、数值常量 
SysY语言中数值常量可以是整型数IntConst，也可以是浮点型数FloatConst。 
<br>
整型数 IntConst 的规范如下（对应 integer-const）： 
<br>
<br>
整型常量 
<br>
integer-const → decimal-const 
<br>| octal-const
<br> 
| hexadecimal-const 
<br>
decimal-const → nonzero-digit 
<br>| decimal-const digit 
<br>
<br>
octal-const → 0 
<br>| octal-const octal-digit 
<br>
hexadecimal-const hexadecimal-prefix hexadecimal-digit 
<br>
| hexadecimal-const hexadecimal-digit 
<br>
hexadecimal-prefix → ‘0x’ 
<br>| ‘0X’ 
<br>
nonzero-digit 为以下之一 
<br>
1 2 3 4 5 6 7 8 9 
<br>
<br>
octal-digit 为以下之一 
<br>
0 1 2 3 4 5 6 7 
<br>
<br>
hexadecimal-digit 为以下之一 
<br>
0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F 
<br>
<br>
注：请参考 ISO/IEC 9899 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf 
第 54 页关于整型常量的定义，在此基础上忽略所有后缀。 
<br>
浮 点 型 数 FloatConst 的 定 义 参 考 ISO/IEC 9899 http://www.open 
std.org/jtc1/sc22/wg14/www/docs/n1124.pdf 第 57 页关于浮点常量的定义，在此基础 
上忽略所有后缀。
<br>
<br>
<br>
3. SysY 语言的语义约束 
<br>
符合上述文法的程序集合是合法的 SysY 语言程序集合的超集。下面进一步 给出 SysY 语言的语义约束。 
<br>
<br>
CompUnit 
<br>
编译单元 
<br>
CompUnit → [ CompUnit ] ( Decl | FuncDef ) 
<br>
<br>
声明 
<br>
Decl → ConstDecl | VarDecl 
<br>
1. 一个 SysY 程序由单个文件组成，文件内容对应 EBNF 表示中的 CompUnit。 
在该 CompUnit 中，必须存在且仅存在一个标识为 ‘main’ 、无参数、返回类 
型为 int 的 FuncDef(函数定义)。main 函数是程序的入口点，main 函数的返 
回结果需要输出。 
<br>
2. CompUnit 的顶层变量/常量声明语句（对应 Decl）、函数定义（对应 FuncDef） 
都不可以重复定义同名标识符（Ident），即便标识符的类型不同也不允许。 
<br>
3. CompUnit 的变量/常量/函数声明的作用域从该声明处开始到文件结尾。 
<br>
<br>
ConstDef 
<br>
常数定义 
<br>
ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal 
<br>
1. ConstDef 用于定义符号常量。ConstDef 中的 Ident 为常量的标识符，在 Ident 
后、‘=’之前是可选的数组维度和各维长度的定义部分，在 ‘=’ 之后是初始值。 
<br>
2. ConstDef 的数组维度和各维长度的定义部分不存在时，表示定义单个变量。 
此时 ‘=’ 右边必须是单个初始数值。 
<br>
3. ConstDef 的数组维度和各维长度的定义部分存在时，表示定义数组。其语义 
和 C 语言一致，比如[2][8/2][1*3]表示三维数组，第一到第三维长度分别为 
2、4、3，每维的下界从 0 编号。 ConstDef 中表示各维长度的 ConstExp 都 
必须能在编译时求值到非负整数。 
<br>
ISO/IEC 9899 http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf 第 125 页6.7.8 节的第 6 点规定如下： 
注意：SysY 在声明数组时各维长度都需要显式给出，而不允许是未知的。 
<br>
4. 当 ConstDef 定义的是数组时，‘=’ 右边的 ConstInitVal 表示常量初始化器。 
ConstInitVal 中的 ConstExp 是能在编译时求值的 int/float 型表达式，其中可 
以引用已定义的符号常量。 
<br>
5. ConstInitVal 初始化器必须是以下三种情况之一： 
<br>
a) 一对花括号 {}，表示所有元素初始为 0。 
<br>
b) 与多维数组中数组维数和各维长度完全对应的初始值，如{{1,2},{3,4}, 
{5,6}}、{1,2,3,4,5,6}、{1,2,{3,4},5,6}均可作为 a[3][2]的初始值。 
<br>
c) 如果花括号括起来的列表中的初始值少于数组中对应维的元素个数，则 
该维其余部分将被隐式初始化，需要被隐式初始化的整型元素均初始为 
0，如{{1, 2},{3}, {5}}、{1,2,{3},5}、{{},{3,4},5,6}均可作为 a[3][2]的初 
始值，前两个将 a 初始化为{{1, 2},{3,0}, {5,0}}，{{},{3,4},5,6}将 a 初始 
化为{{0,0},{3,4},{5,6}}。 
<br>
例如：下图中变量 a~e 的声明和初始化都是允许的。 
<br>
d) 数组元素初值类型应与数组元素声明类型一致，例如整型数组初值列表 
中不能出现浮点型元素；但是浮点型数组的初始化列表中可以出现整型 
常量或整型常量表达式； 
<br>
e) 数组元素初值大小不能超出对应元素数据类型的表示范围； 
<br>
f) 初始化列表中的元素个数不能超过数组声明时给出的总的元素个数。 
<br>
<br>
VarDef 
<br>
变量定义 
<br>
VarDef → Ident { '[' ConstExp ']' } 
<br>
| Ident { '[' ConstExp ']' } '=' InitVal
<br> 
1. VarDef 用于定义变量。当不含有‘=’和初始值时，其运行时实际初值未定义。 
<br>
2. VarDef 的数组维度和各维长度的定义部分不存在时，表示定义单个变量。存 
在时，和 ConstDef 类似，表示定义多维数组。（参见 ConstDef 的第 2 点） 
<br>
3. 当 VarDef 含有 ‘=’ 和初始值时， 
‘=’ 右边的 InitVal 和 CostInitVal 的结构要求相同，唯一的不同是 ConstInitVal 中的表达式是 ConstExp 常量表达式，而 
InitVal 中的表达式可以是当前上下文合法的任何 Exp。 
<br>
4. VarDef 中表示各维长度的 ConstExp 必须是能求值到非负整数，但 InitVal 中 
的初始值为 Exp，其中可以引用变量，例如下图中的变量 e 的初始化表达式 
d[2][1]。 
<br>
<br>
初值 
<br>
常量初值 
<br>
ConstInitVal → ConstExp 
<br>
| '{' [ ConstInitVal { ',' ConstInitVal } ] '}' 
<br>
<br>
变量初值 
<br>
InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}' 
<br>
1. 全局变量声明中指定的初值表达式必须是常量表达式。 
<br>
2. 常量或变量声明中指定的初值要与该常量或变量的类型一致 
如下形式的 VarDef / ConstDef 不满足 SysY 语义约束： 
<br>
a[4] = 4 
<br>
a[2] = {{1,2}, 3} 
<br>
a = {1,2,3} 
<br>
3. 未显式初始化的局部变量，其值是不确定的；而未显式初始化的全局变量， 
其（元素）值均被初始化为 0 或 0.0。 

<br>
<br>
Block 
<br>
1. Block 表示语句块。语句块会创建作用域，语句块内声明的变量的生存期在 
该语句块内。 
<br>
2. 语句块内可以再次定义与语句块外同名的变量或常量（通过 Decl 语句)，其 
作用域从定义处开始到该语句块尾结束，它隐藏语句块外的同名变量或常量。 
<br>
<br>
Stmt 
1. Stmt 中的 if 类型语句遵循就近匹配。 
<br>
2. 单个 Exp 可以作为 Stmt。Exp 会被求值，所求的值会被丢弃。 
<br>
<br>
LVal 
1. LVal 表示具有左值的表达式，可以为变量或者某个数组元素。 
<br>
2. 当 LVal 表示数组时，方括号个数必须和数组变量的维数相同（即定位到元 
素）。 
<br>
3. 当 LVal 表示单个变量时，不能出现后面的方括号。
<br>
<br>
Exp 与 Cond 
<br>
1. Exp 在 SysY 中代表 int/float 型表达式，故它定义为 AddExp；Cond 代表条件表达式，故它定义为 LOrExp。前者的单目运算符中不出现'!'，后者可以出现。 
此外，当 Exp 作为数组维度时，必须是非负整数。
<br> 
2. LVal 必须是当前作用域内、该 Exp 语句之前有定义的变量或常量；对于赋值号左边的 LVal 必须是变量。 
<br>
3. 函数调用形式是 Ident ‘(’ FuncRParams ‘)’，其中的 FuncRParams 表示实际参 
数。实际参数的类型和个数必须与 Ident 对应的函数定义的形参完全匹配。 
<br>
4. SysY 中算符的优先级与结合性与 C 语言一致，在上面的 SysY 文法中已体现 
出优先级与结合性的定义。 
<br>
<br>
隐式类型转换 
<br>
SysY 语言的 int 和 float 类型的数有以下隐式类型转换： 
<br>
1、当 float 类型的值隐式转换为整型时，例如通过赋值 int i = 4.0; 小数部分将被丢弃；如果整数部分的值不在整型的表示范围，则其行为是未定义的； 
<br>
2、当 int 类型的值转换为 float 型时，例如通过赋值 float j = 3; 则转换后的值保持不变。 
