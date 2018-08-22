静态库的制作：
	1. 源代码  .c 或者 .cpp
	2. 将源文件生成 .o的文件  gcc *.c -c 生成了 .o的文件 注意如果头文件不在里面的话，需要指定头文件路径  gcc *.c -c -I ../include/    //指定头文件为上级目录的include的文件内
	3. 将 .o的文件打包  ar rcs 静态库的名字 .o 源文件
	 例如： ar rcs libmytest.a  *.o;  //这样就会生成一个.a的静态库文件
	 //静态库的命名规则： libxxxx.a  前面必须是lib开头，后面必须是 .a结束，中间可以写自己的任意名字
使用 nm命令查看静态库内的文件信息 nm libmytest.a
	 
静态库的使用：
	1. main.c文件必须包含头文件 
	2. gcc main.c -I ./include/ -L ./lib/ -l mytest -o app;
	//编译main.c 指定头文件路径 -I （大 I）在当前目录的include文件夹里面， 指定库文件的路径 -L（大L） 在当前目录的lib文件夹里面， 指定库文件名字 用-l(小 l 字母)，但是名字要去掉规定的部分即前面去掉 lib  后面去掉 .a 然后-o（小 o) 生成可执行文件，名字随意取；
	
vi里面同时分屏显示多个文件命令： vi -O(大写表示竖屏显示） *.c
 小写 -o 是横屏显示多个文件
	
	
	
动态库的制作：
	1. 命名规则： libxxx.so  //前面必须是lib开头，后面必须是 .so结束
	2. 将源文件生成 .o 文件 gcc *.c -c -fpic 或者  gcc *.c -c fPIC 两个方法都可以 
	注意如果头文件不在里面的话，需要指定头文件路径  gcc *.c -c -fpic -I ../include/    //指定头文件为上级目录的include的文件内
	3. 将.o 的文件打包成动态库 
		gcc -shared  *.o -o libxxx.so  
	 //这样就会生成一个.so的动态库文件
	 
使用 nm命令查看静态库内的文件信息 nm libxxx.so

动态库的使用：
	1. main.c文件必须包含头文件 
	2. gcc main.c -I ./include/ -L ./lib/ -l mytestm -o app;
	//编译main.c 指定头文件路径 -I （大 I）在当前目录的include文件夹里面， 指定库文件的路径 -L（大L） 在当前目录的lib文件夹里面， 指定库文件名字 用-l(小 l 字母)，但是名字要去掉规定的部分即前面去掉 lib  后面去掉 .a 然后-o（小 o) 生成可执行文件，名字随意取；
	放法和静态库是一样的 ，但是会报错（加载动态库失败问题）：
	./app: error while loading shared libraries: libmycalcm.so: cannot open shared object file: No such file or directory
	
	3. 需要将动态库拷贝到根目录下的lib文件里面，然后在执行程序就OK了，但是最好不要这样弄，会增加内存占用。
	
	4. 使用环境变量： 
		（1）.临时设置： 命令 export LD_LIBRARY_PATH=动态库的路径（只要路径，不要后面的库文件，最好是绝对路径）；
		例如： export LD_LIBRARY_PATH=/home/zilong/test/lib:$LD_LIBRARY_PATH  //临时设置
		（2）.永久设置：  登陆shell时系统会调用 ~/.bashrc 文件，所以将设置放到这个文件内，就一打开终端就会自动配置环境变量了，在用户目录的根目录下
		在最后一行增加配置：export LD_LIBRARY_PATH=/home/zilong/test/lib/
		
	