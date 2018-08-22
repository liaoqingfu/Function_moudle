#makefile第一个版本，缺点：效率低
#这样的效率太低，需要编译其中某一个文件时，会全部编译，需要改进
#app:head.h main.c add.c sub.c mul.c div.c 
#	gcc head.h main.c add.c sub.c mul.c div.c -o app


#makefile第二个版本，检测目标时间是否是大于依赖时间，不大于就重新生成依赖，缺点：冗余太多
#app:head.h main.o add.o sub.o mul.o div.o 
#	gcc head.h main.o add.o sub.o mul.o div.o -o app

#生成.o文件,规则里面是可以加绝对目录的，如果想生成到特定的目录里面，可加相关的路径
#main.o:main.c
#	gcc main.c -c

#add.o:add.c
#	gcc add.c-c

#sub.o:sub.c
#	gcc sub.c -c

#mul.o:mul.c
#	gcc mul.c

#div.o:div.c
#	gcc div.c
#

#目标：依赖
#	规则(记得规则前面要加一个table)

#makefile第三个版本，缺点可移植性差
#用变量来替代重复使用的依赖
#obj=head.h main.o add.o sub.o mul.o div.o

#app:$(obj)
#	gcc $(obj) -o app
#自动变量
# $@:规则中的目标
# $<:规则中的第一个依赖
# $^:规则中的所有依赖
# 只能在规则的命令中使用
#%.o:%.c
#	gcc -c $< -o $@ 
#模式匹配


#makefile第四个版本,可移植性高,不能清理项目
#查找指定目录指定类型的文件：wildcard

#src=$(wildcard ./*.c) 当前目录，所有.c的文件
#匹配替换
#obj=$(patsubst %.c,%.o,$(src)) 将里面所有.c的文件替换为.o的文件，.c文件是从src里面的

src=$(wildcard ./*.h *.c)
obj=$(patsubst %.c,%.o,$(src))
target=app
$(target):$(obj)
	gcc $^ -o $@
#自动变量
# $@:规则中的目标
# $<:规则中的第一个依赖
# $^:规则中的所有依赖
# 只能在规则的命令中使用
%.o:%.c
	gcc -c $< -o $@

#makefile第五个版本，编写一个清理项目的规则
#clean:
#	rm *.o app

#没有依赖，只有目标，这样就可以通过make hello直接执行目标
hello:
	echo "hello kevin"

#可选择性的执行目标,使用make clean执行下面的规则，没有依赖，只有目标和规则
#clean:
#	rm *.o app
#可以在规则最前面加减号 - ：意思是规则执行失败后，继续执行后面的命令
#声明伪目标：不做更新和检查 ：.PHONY:clean,即使目录文件有文件和命令是同名的也可以执行目标命令
.PHONY:clean
clean:
	rm $(obj) $(target)

