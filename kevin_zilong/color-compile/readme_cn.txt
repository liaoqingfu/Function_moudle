Makefile 高亮显示Error、Warning等错误以及警告信息



原文出处：https://my.oschina.net/bingwu/blog/788347        


    经常性用 Makefile 编译程序，但是出错后清一色的编译输出信息让人查找错误点时一阵头大！还好GitHub上有 color_cpmpile 项目，真是造福啊！感谢此项目的原作者！
    GitHub上的项目地址  https://github.com/chinaran/color-compile
    或者在这里下载源代码：http://pan.baidu.com/s/1kUTFtQ3
    1、编译。很简单，进入项目目录里

$ \make /*使用的是系统的 make 来编译,而非 alias 定义的别名*/
     2、安装。编译完成后便是安装了（install 的工作是 copy 主程序到 /usr/local/bin 中，并向 shell 的配置文件（此处一般是 ~/.bashrc）末尾添加别名）

$ sudo \make install /*安装*/
    此时查看 ~/.bashrc 文件，在文件最末尾应能看到新加入的别名
alias gcc="color_compile gcc"
alias g++="color_compile g++"
alias make="color_compile make"
    3、配置交叉编译器。这个也是很简单，只要在 ~/.bashrc 文件末尾加入一下内容即可（这是海思编译器），也可根据格式添加自己的编译器。
 # 编译时颜色输出 2016年11月15日 星期二 14:36:00 CST
alias arm-hisiv100nptl-linux-gcc="color_compile arm-hisiv100nptl-linux-gcc"
alias arm-hisiv100nptl-linux-g++="color_compile arm-hisiv100nptl-linux-g++"
alias arm-hismall-linux-gcc="color_compile arm-hismall-linux-gcc"
alias arm-hismall-linux-g++="color_compile arm-hismall-linux-g++"
    4、生效。需要 source 一下让配置文件生效（也可以重新打开终端）

$ source ~/.bashrc
    此时再去 make 代码工程，就会发现有很多醒目的输出信息了。可以使用自带的例子查看效果

$ make TEST /* ==gcc -Wall test.c */
    输出如下

    
    5、卸载或重装
    a) 删除 ~/.bashrc 后面的所有添加的 alias 别名，以免造成其他未知影响。
    b) 删除 /usr/local/bin/color_compile 可执行程序

$ sudo rm /usr/local/bin/color_compile 其他
    1、gcc 版本为 4.9.0 以上时，可以使用 gcc 内置的高亮显示，加上参数 -fdiagnostics-color=auto 即可
    2、需要修改其他颜色输出，只要修改 out_color_info.c 源码即可，里面有部分颜色的宏定义。




在 ~/目录下面，进入 .bashrc文件，修改启动文件里面的内容：
在文件最后面增加：
alias gcc="color_compile gcc"        //gcc编译出错颜色显示
alias g++="color_compile g++"    //g++编译出错颜色显示
//arm出错编译颜色显示
alias arm-hisiv100nptl-linux-gcc="color_compile arm-hisiv100nptl-linux-gcc"
alias arm-hisiv100nptl-linux-g++="color_compile arm-hisiv100nptl-linux-g++"
alias arm-hismall-linux-gcc="color_compile arm-hismall-linux-gcc"

alias arm-hismall-linux-g++="color_compile arm-hismall-linux-g++"
//mips出错编译颜色显示
alias mipsel-linux-gcc="color_compile mipsel-linux-gcc"

alias mipsel-linux-g++="color_compile mipsel-linux-g++"

alias make="color_compile make"    //make出错颜色显示

