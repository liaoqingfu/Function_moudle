1. windows连接数据库
 虚拟机登陆 mysql -uroot -p
		  密码：123456
		   use mysql; //使用一个库
		查看库里面的用户：select user,password,host from user;
		在mysql下创建用户： create user '用户名'@'%'; //创建用户：用户名 自定义，密码随机，%：代表host，即连接端口为所有
		给用户赋予权限： create all on *.* to '用户名’@‘%’; 
		//用户名 自己定义，密码随机，%：代表host，即连接端口为所有
		
		然后在navicat工具里面连接数据库
		定义一个连接的名字，输入虚拟机的IP，端口是3306 默认
		输入用户名 和 密码 连接就行
		
2. linux连接数据库：
		http://blog.csdn.net/m0_37897437/article/details/72956110
		
		
		
		Linux下Mysql_odbc的安装配置及OTL的使用
原创 2017年06月09日 11:46:32 标签：linux /OTL /odbc /mysql /数据库 320
数据库是常见的mysql数据库，数据库访问接口我选择了ODBC-OTL进行连接。因为我也是第一次实际使用OTL操作，曾经对于ODBC的配置等等都是运维人员进行安装配置，如今自己配置就碰上了很大的阻碍。求助度娘、谷歌上的文章进行配置，发现大多内容相同而且无效。如今终于参透了配置方法，放在网上帮助新接触ODBC的同学们少走弯路。

ODBC的配置

第一步：下载必要的软件包

这一步是必须的，要安装ODBC要下的安装包有：

unixODBC-2.3.4.tar.gz
mysql-connector-odbc-5.1.12-linux-debian6.0-x86-64bit.tar.gz
1
2
安装包的版本没有太多的要求，只需要这两个就ok，unixODBC是ODBC软件的安装包，mysql-connector-odbc是mysql连接odbc的插件安装包，官网就可以下载。

第二部：解压安装包进行安装

熟悉Linux系统的各位应该都能很熟练的操作了，此处粘贴我的操作步骤吧：

unixODBC的安装：

# tar -zxvf unixODBC-2.3.4.tar.gz
# cd unixODBC-2.3.4
# ./configure --prefix=/usr/local/unixODBC-2.3.4 --includedir=/usr/include --libdir=/usr/lib64 -bindir=/usr/bin --sysconfdir=/etc
# make
# sudo make install

mysql-connector-odbc的安装：

# tar -zxvf mysql-connector-odbc-5.1.12-linux-debian6.0-x86-64bit.tar.gz
# cd mysql-connector-odbc-5.1.12-linux-debian6.0-x86-64bit/lib
# cp ./*.* /usr/lib64

在配置mysql-connector-odbc还有一步非常重要，我在这一步卡了很长时间，我针对我的安装包版本进行简要说明，请大家在使用时根据实际情况进行相对的调整。在mysql-connector-odbc 5.1之后的版本中只需要把libmyodbc.so文件复制过去就可以了，如果连接不成功需要添加一步操作：

./myodbc-installer -d -a -n "MYSQL" -t "DRIVER=/usr/lib64/libmyodbc5.so;SETUP=/usr/lib64/libmyodbc5.so"  
1
执行之后会报错提示缺少文件：libodbc.so.1和libodbcinst.so.1，只需要操作：

cp /usr/lib64/libodbc.so.2 /usr/lib64/libodbc.so.1
cp /usr/lib64/libodbcinst.so.2 /usr/lib64/libodbcinst.so.1
1
2
再次执行上面的指令就会提示：Success: Usage count is 1 ，到此安装就OK了

第三步：配置文件

安装好odbc之后就是写相关配置文件了，配置文件自动生成在/etc/目录下，分别是：odbc.ini 和 odbcinst.ini ；odbc.ini需要写的是将要连接的数据库信息和连接驱动，odbcinst.ini需要写的是odbc驱动所需要链接的库，下文写下我的小例子：

odbc.ini：
[test]
Description = THE Database for test
Trace = On
TraceFile = staderr
Driver = mysql
SERVER = 192.168.1.1
USER = test
PASSWORD = test123456
PORT = 3306
DATABASE = testdata
charset = UTF8
option = 3

odbcinst.ini
[MYSQL]
Driver=/usr/lib64/libmyodbc5.so
SETUP=/usr/lib64/libmyodbc5.so
UsageCount=1

然后执行isql -v test出现如下内容： isql -v test ;test的名字是根据odbc.ini：[test] ，可以自己随便取，我的是mysql ：isql -v mysql

+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
SQL>

好了，有关mysql-odbc的安装配置终于是完成，成功进入了想连接的数据库了！

OTL的使用

配置好ODBC就进入到使用环节了。此部分内容可以自行度娘，网上有很多相关文章，也可以再继续看下去： 
对于OTL我就不过多介绍了，简单来说，就是一个封装好的操作数据库的可操作的库。只需要下载一个otlv4.h的头文件即可，有官方网站可以提供下载。 
操作也比较简单，下面我放上我一小段代码提供参考：

#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#define _ALLOW_RTCc_IN_STL
#endif

#include <iostream>
using namespace std;

#define OTL_ODBC_MSSQL_2008//这个宏定义要在包含otlv4前
#include <otlv4.h>//将otlv4.h复制到/usr/local/include中

int main()
{
    otl_connect m_db;//连接对象
    otl_connect::otl_initialize();//连接初始化
    m_db.rlogon("DSN=test");//我在连接odbc上卡了很久，DSN指定连接那个配置好的odbc.ini

    char test_buf[1024] = { 0 };
    int test_num = 0;
    //查询数据库
    try
    {
        otl_stream select_info(50, "select * from test", m_db);
        while(!select_info.eof())
        {
            select_info >> test_buf >> test_num;//按照查询顺序与类型依次传入
        }
    }   
    catch(otl_exception &p)
    {
        cout << p.msg;//异常情况
    }

    cout << "test_buf : " << test_buf << "test_num" << test_num << endl;

    m_db.logoff();//关闭数据库

    return 0; 
}

好了，使用OTL_ODBC连接mysql数据库进行操作到这里就完成了

