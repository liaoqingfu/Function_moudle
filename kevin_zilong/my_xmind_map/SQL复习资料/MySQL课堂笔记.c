
自连接：

	emp.mgr = boss.empno

	MySQL下的滤空函数 ifnull(a, b) --- oracle 下的 nvl(a, b);

	nullif(a, b):	判断a, b是否相等，相等返回NULL， 否则返回a

	
MySQL的脚本：

	1. 脚本中的注释“-- ” ，一定不能省略空格。

	2. ``反引号引用的库名、表名区分大小写

	3. 编写脚本所使用的编辑工具有默认字符集设定，出现字符串常量时，严格对应数据库的字符集

	4. MySQL下的外键连接，书写方法不同于Oracle。

	5. 执行脚本：在 mysql> 提示符下，执行 source xxxx.sql(绝对路径)


查看编码：

	cat /etc/sysconfig/i18n 系统的字符编码

	echo $LANG 用户的字符编码（粗略） 


中文乱码：

	1. 由客户端所采用的字符集，与服务器、数据库默认设定的字符集不对应。

	2. 由于数据库服务器端字符集发生变化。

	3. 数据库、表设定的字符集合 访问字符集不一致。

	4. 第三方远程连接工具。

=================================================== MySQL API ==================================================

库名：libmysqlclient		.so  .a		-l

库路径：/usr/lib64/mysql/libmysqlclient.a	-L

头文件名：<mysql.h>	

路径：/usr/include/mysql/mysql.h		-L

LD_LIBRARY_PATH

使用mysql库相关 其他库：

	gcc hellomysql.c -o hellomysql -L/usr/lib64/mysql/ -lmysqlclient -I/usr/include/mysql/ -lstdc++ -ldl -lpthread -lrt

	Makefile


SQL语句分类：

	1. 无结果集返回的。insert、delete、update

	2. 有结果集返回的。select、desc、show...


常用基础类API：

	初始化：

		mysql_init();

	错误：
		mysql_errno();/ mysql_error();

	建立连接：

		mysql_real_connect();

	执行SQL语句：

		mysql_query();

	影响行数：

		mysql_affected_row();
	

	获取结果集：

		mysql_stroe_result();    ---- mysql_use_result(); 结果集较大。

	释放内存：

		mysql_free_result();

	获取列数：

		mysql_filed_count(mysql);

		mysql_num_fields(result);

	获取表头：

		mysql_fetch_fields(result);	全部
		
		mysql_fetch_field(result);	单个

	关闭连接:

		mysql_close();
	

预处理类API：

	insert into emp(empno, ename, sal) values(&empno, &ename, &sal);	

	select * from emp

	desc
	
	

日期和时间类：


多查询类：































