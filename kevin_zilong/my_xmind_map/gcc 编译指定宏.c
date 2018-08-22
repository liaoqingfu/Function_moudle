gcc 编译指定宏

那在makefile里面 
gcc -DMACRONAME=MACRODEF 
或者 
gcc -DMACRONAME 
这样就定义了预处理宏，编译的时候可选代码就会被编译进去了。 
举例说明： 
-Dmacro=string，等价于在头文件中定义：#define macro string。 
-DTRUE=true，等价于在头文件中定义：#define TRUE true 
-Dmacro，等价于在头文件中定义：#define macro 1，实际上也达到了定义：#define macro的目的。 
-DLINUX，等价于：#define LINUX 1（与#define LINUX作用类似）。 
–define-macro macro=string与-Dmacro=string作用相同。