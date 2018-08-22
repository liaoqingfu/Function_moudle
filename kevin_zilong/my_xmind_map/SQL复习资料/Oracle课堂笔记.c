
别名的使用：

	在过滤条件中 ： where having 不能使用别名。

	在子查询中定义的别名， 可以在 主查询的过滤条件中使用。where having 都能使用别名	。

表数据的 curd ：


插入：
	insert into 表名 values（列值[, 列值]）

	insert into 表名（列名） values（列值[, 列值]）

	insert into 表名（列名） values（&列名 [, &列名]）

	insert into 表名 select 语句		

更新：
	update 表名 set 列名 = 列值	where  过滤条件

删除：
	delete from  表名 where 过滤条件

	delete / truncate 


事务：
	批量执行 多条 SQL语句， 要么都成功，要么都失败。

	4大特性：原子性、一致性、隔离性、持久性。

	开启事务：

		DML 语句， Oracle 自动开启事务。

	关闭事务：

		显示：  commit、rollback

		隐式：	DDL 语句、 exit/quit （隐式 commit）。 断电、宕机。（隐式rollback）

表 的 curd：

创建表：

	create table 表名（列名 列类型[, 列名 列类型]）

	create table 表名 as select xxxx where 假条件 	创建已知表结构相同的表

	create table 表名 as select xxxx where 真条件 	创建已知表结构相同的表的同时， 携带表数据。

	create table 表名 as where 子查询/多表查询	创建多张表涵盖的数据的表。  携带多张表数据。   注意：对应表达来说，要使用别名。

修改表：

	添加列：	alter table 表名 add 列名 列类型

	修改列：	alter table 表名 modify 列名 列类型	--- 修改列类型。

	删除列：	alter table 表名 drop cloumn 列名

	重命名列：	alter table 表名 rename cloumn 旧列名 to 新列名。


删除表：

	drop table 表名。			show recyclebin		purge recyclebin		

	drop table 表名 purge。		不经过回收站。		-- 普通用户

重名表：
	rename 旧表名 to 新表名。


表约束：

	非空 	not null

	唯一性	unique

	主键约束 	primary  key == not null + unique

	检查性	check 	（Oracle 中）

	外键	Foreign key 			A 表 的列  是 B 表的外键		---- 级联置空、级联删除




索引：

	创建： create index 索引表名 on  表名 （列名）

	删除： drop  index 索引表名


MySQL的版本：

	社区版：
		1. 源码版		

			window: 32/64

			Linux

			mac

		2. 通用版

		3. 标准版

		4. 精简版

	企业版

	集群版

Oracle:	基于用户的数据管理系统

MySQL：	基于数据库的数据管理系统

	1. 选定数据库	2. 使用数据表

数据库的curd：

	创建数据库：

		create database mydb1;

		create database mydb2 character set utf8;  	指定字符集


	查看数据库：

		show databases

		show create database mydb1;

	修改数据库：

		alter database mydb1 character set utf8;	修改字符集


	删除数据库：

		drop database 数据库名。

	
表的 curd：

	创建表：

		create table 表名（列名 列类型[, 列名 列类型]）   带约束	注意数据类型不同

	查看表：

		show tables；

		desc 表名

		show create table 表名

	修改表：
		rename table 旧表名 to 新表名

		alter table 表名 add 列名 列类型

		alter table 表名 modify 列名 列新类型

		alter table 表名 delete column 列名

		alter table 表名 rename column 列名 to 列新名

		alter table employee character set gbk  		修改字符集

	删除表：

		drop table 表名

表数据的 curd：

	insert 数据：

		insert into 表 values(列值);

		insert into 表（列名） values(列值);

	update 数据：

		update 表名 set 列名 = 列新值


多表查询：

	
内连接：		--- 等值连接/不等值连接

	select 列名

	from 表1， 表2					“,” ---》 inner 	join 不能省略。

	where  表1列= 表2列				“where” ---> on

外链接：

	select 列名

	from 表1， 表2					“,” ---》 left [outer] join/ right [outer] join

	where  表1列= 表2列				“where” ---> on	


	选择左外、右外，取决于 from 后 表的书写顺序。与 on 之后判别表达式的书写顺序 无关！！！












