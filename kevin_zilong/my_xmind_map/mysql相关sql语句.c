登陆mysql:  mysql -hlocalhost -uroot -proot
			mysql –u root -p 123456

创建数据库：create database dbname charset utf8

查看数据库：show databases;

选择数据库：use dbname;

删除数据库：drop database dbname;

创建数据表: create table tbname (
		id int(10) primary key auto_increment,
		name char(10) not null defaule '',
		.......
	    )engine myisam/innodb charset utf8/gbk

设定环境编码的命令：set names gbk;
查看数据表：show tables;

查看建表语句：show create table tbname;

查看表结构：desc tbname;

修改表(增加列)：alter table tbname add 列名称 列类型 

修改表(修改列)：alter table tbname change 旧列名 新列名 列类型

修改表(删除列)：alter table tbname drop 列名称

索引

1. 提高查询速度,但是降低了增删改的速度,所以使用索引时,要综合考虑

2. 索引不是越多越好,一般我们在常出现于条件表达式中的列加索引

3. 值越分散的列，索引的效果越好

索引类型

1. primary key 主键索引

2. index 普通索引

3. unique index 唯一性索引

4. fulltext index 全文索引


修改表(增加主键索引)：alter table tbname add primary key(主键所在列名)

修改表(删除主键索引)：alter table tbname drop primary key;

修改表(增加唯一索引)：alter table tbname add unique index 索引名(列名);

修改表(增加全文索引)：alter table tbname add fulltext index 索引名(列名);

修改表(删除普通索引)：alter table tbname drop unique index 索引名;

清空数据表内容：truncate tbname;

删除表：drop table admin;

插入数据: insert into tbname (列1,列2,列...) values(val1,val2,val...) 指定值与列一一对应
	  insert into tbname values(val1,val2,val3...) 全部列都插入值
	  insert into tbname values(val1,val2,val3...),(val1,val2,val3...),(val1,val2,val3...) 插入多行数据

删除数据: delete from tbname where 列名=列值;

修改数据: update tbname set 列名1=新列值1,列名2=新列值2 where 条件;

查询数据: select 列1,列2,列3 from tbname;

数据类型: 整型 tinyint (0-255/-128-127) smallint (0-65536/-32768-32767) mediumint () int bigint 共5种类型

          unsigned 无符号(不能为负) zerofill (0 填充) M 填充后的宽度
		
	  举例: tinyint unsigned; tinyint(6) zerofill;

数值型:   浮点型: float double 

          格式: float(M,D) D 表示小数位数 unsinged zerofill;

字符型:   char(M) 定长 varchar(M) 变长 text 
	  
	  列		实存字符	实占空间
	  
	  char(M)	0<=i<=M		M

	  varchar(M)	0<=i<=M		i+1

		year		YYYY 范围: 1901~2155. 可输入值2位和4位(如98,2012)
		
		date		YYYY-MM-DD 如: 2010-03-04

日期时间类型:	time		HH:MM:SS 如: 19:26:32
		
		datetime	YYYY-MM-DD HH:MM:SS 如: 2010-03-14 19:26:32

		timestamp	YYYY-MM-DD HH:MM:SS 特性:不用赋值,该列会为自己赋当前的具体时间

查询详解: (1) 条件查询	where	a. select 列1,列2,列3.. from tbname where 列x=列x值;

				b. 比较运算符 select 列1,列2,列3.. from tbname where 列x[=|!=|<|>|<=|>=]列x值;

				c. like,not like ('%'匹配任意多个字符,'_'匹配任意单个字符)

				d. in,not in,between and

				e. is null,is not null

	  (2) 分组	group by  配合5个聚合函数使用 max(最大),min(最小),sum(求和),avg(求平均),count(统计)

	  (3) 筛选	having

	  (4) 排序	order by

	  (5) 限制	limit

连接查询:

左连接: 例句 select 列1,列2,列3... from tbnameA left join tbnameB on tbnameA.列1 = tbnameB.列2;

右连接: 例句 select 列1,列2,列3... from tbnameB right join tbnameA on tbnameA.列1 = tbnameB.列2;

左右连接都是以在左边的表的数据结构为准，沿着左表查询。

内连接是以两张表都有的共同部分数据为准，也就是左右连接的数据之交集。

子查询:

where 型子查询:内层sql的返回值在where后作为条件表达式的一部分

例句: select * from tableA where colA = (select colB from tableB where ...);

from 型子查询:内层sql查询结果,作为一张表,供外层的sql语句再次查询

例句:select * from (select * from ...) as tableName where ...

字符集

客服端sql编码 character_set_client

服务器转化后的sql编码 character_set_connection

服务器返回给客户端的结果集编码 character_set_results

快速把以上3个变量设为相同值: set names 字符集

存储引擎 engine:Myisam/Innodb

1. Myisam  速度快 不支持事务 回滚

2. Innodb  速度慢 支持事务 回滚

事务执行顺序:

a. 开启事务 start transaction

b. 运行sql         

c. 提交,同时生效\回滚 commit\rollback

触发器: trigger

监视地点: 表

监视行为: 增 删 改

触发时间:a fter\before

触发事件: 增 删 改

创建触发器语法:

create trigger tgName

after/before insert/delete/update 

on tableName

for each row

sql 触发语句
	
删除触发器: drop trigger tgName;


综合练习题:

1. 连接数据库 (以本地机为例) 

答: mysql -hlocalhost -uroot -proot 

2. 建立一个gbk编码的数据库

答: create database test charset utf8;

3. 建立商品表和栏目表

答: #商品表
create table goods(
goods_id int primary key auto_increment,
goods_name varchar(20) not null default '',
cat_id int not null default 0,
brand_id int not null default 0,
goods_sn char(12) not null default '',
shop_price float(6,2) not null default 0.00,
goods_desc text
)engine MyISAM charset utf8;

# 栏目表
create table category(
cat_id int primary key auto_increment,
cate_name varchar(20) not null default '',
parent_id int not null default 0
)engine myisam charset utf8;

4. 删除 goods 表中的 goods_desc 字段及货号字段,并增加 click_count 字段

答: alter table goods drop goods_desc;
	
    alter table goods drop brand_id;

    alter table goods add click_count int not null default 0;

5. 在 goods_name 列上加唯一性索引

答: alter table goods add unique index goods_name(goods_name);

6. 在 shop_price 列上加普通索引

答: alter table goods add index shop_price(shop_price);

7. 在 click_count 上增加普通索引，然后再删除

答: alter table goods add index click_count(click_count);

    alter table goods drop index click_count;

基础查询练习一(where条件查询)

1. 查询主键为32的商品

答: select goods_id,goods_name,shop_price from goods where goods_id=32;

2. 查询不属于第三个栏目的所有商品

答: select goods_id,goods_name,shop_price from goods where cat_id!=3;

3. 查询本店价格高于3000的商品

答: select goods_name,goods_id,shop_price from goods where shop_price>3000;

4. 查询本店商品价格低于或等于100元的商品

答: select goods_id,goods_name,shop_price from goods where shop_price <=100;

5. 取出第4个栏目或第11个栏目的商品(不允许用 or )

答: select goods_id,goods_name,shop_price from goods where cat_id in (4,11);

6. 取出本店价格大于100小于500的商品(不允许用 and )

答: select goods_id,goods_name,shop_price from goods where shop_price between 100 and 500;

7. 取出名字以”诺基亚“开头的商品

答: select goods_id,goods_name,shop_price from goods where goods_name like '诺基亚%';

8. 取出名字为”诺基亚Nxxx“的商品

答: select goods_id,goods_name,shop_price from goods where goods_name like '诺基亚N___';

9. 取出不在第3个栏目和不在第11个栏目的商品

答: select goods_id,goods_name,shop_price from goods where cat_id!=3 and cat_id!=11;

    select goods_id,goods_name,shop_price from goods where cat_id not in (3,11);

10. 取出名字不以”诺基亚“开头的商品

答: select goods_id,goods_name,shop_price from goods where goods_name not like '诺基亚%';

11. 取出价格大于100且小于300，或者大于4000且小于5000的商品

答：select goods_id,goods_name,shop_price from goods where shop_price>100 and shop_price<300 or shop_price>4000 and shop_price<5000;

12. 取出第3个栏目下面价格在1000到3000之间，并且点击量大于5，以”诺基亚“开头的系列商品

答: select goods_id,goods_name,shop_price from goods where cat_id=3 and shop_price>1000 and shop_price<3000 and click_count>5 and goods_name like '诺基亚%';
    select goods_id,goods_name,shop_price from goods where cat_id=3 and shop_price between 1000 and 3000 and click_count>5 and goods_name like '诺基亚%';

13. 取出第3个栏目下面价格小于1000或价格大于3000，并且点击量大于5的系列商品

答: select goods_id,goods_name,shop_price from goods where cat_id=3 and (shop_price<1000 or shop_price>3000) and click_count>5;

14. 取出第1个栏目下面的商品(注意 1号栏目下面没有商品，但其子栏目2,3,4,5有商品)

答: select goods_id,goods_name,shop_price from goods where cat_id in (2,3,4,5);

基础查询练习二(group by 分组查询和聚合函数的使用)

1. 查出最贵的商品

答: select goods_id,goods_name,shop_price from goods group by shop_price desc limit 1;

    select max(shop_price) from goods;

2. 查出最大(最新)的商品的编号

答: select max(goods_id) from goods;

    select goods_id from goods group by goods_id desc limit 1;

3. 查出最便宜商品的价格

答: select min(shop_price) from goods;

    select goods_id,goods_name,shop_price from goods group by shop_price asc limit 1;

4. 取出最小(最旧)的商品编号

答: select min(goods_id) fomr goods;

    select goods_id,goods_name,shop_price from goods group by goods_id asc limit 1;

5. 查出本店所有商品的库存量

答: select sum(goods_num) from goods;

6. 查出本店所有商品的平均价格

答: select avg(shop_price) from goods;

7. 查出本店共有多少种商品

答: select count(*) from goods;

基础查询练习三(having 与 group by 综合运用)

1. 查出本店商品价格比市场价格的差价

答: select goods_id,goods_name,market_price-shop_price as g from goods;

2. 查出每个商品所积压的货款

答: select goods_id,goods_name,shop_price*goods_num as g from goods;

3. 查询本店积压的总货款

 答: select sum(shop_price*goods_num) from goods;

4. 查询出每个栏目下积压的总货款

答: select cat_id,sum(shop_price*goods_num) as g from goods group by cat_id;

5. 查询比市场价省200元以上的商品及该商品所省的钱 (分别用 where 和 having 实现)

答: select goods_id,goods_name market_price-shop_price as g from goods having g>200;

    select goods_id,goods_name market_price-shop_price from goods where market_price-shop_price>200;

6. 查询积压货款超过2W的栏目，以及该栏目积压的货款

答: select cat_id,sum(shop_price*goods_num) as g from goods group by cat_id having g>20000;

基础查询练习四(order by 与 limit 查询)

1. 按价格由高到低排序

答: select goods_id,goods_name,shop_price from goods order by shop_price desc;

2. 按发布时间由早到晚排序

答: select goods_id,goods_name,shop_price,pub_time from goods order by pub_time asc;

3. 按栏目由低到高排序，栏目内部按价格由高到低排序

答: select goods_id,goods_name,cat_id,shop_price from goods order by cat_id asc,shop_price desc;

4. 取出价格最高的前三名商品

答: select goods_id,goods_name,shop_price from goods order by shop_price desc limit 3;

5. 取出点击量前3到前5名的商品

答: select goods_id,goods_name,shop_price from goods order by click_count desc limit 2,3;

基础查询练习五(连接查询)

1. 取出所有商品的商品名，栏目名，价格

答: select goods_name,cat_name,shop_price from goods left join category on goods.cat_id=category.cat_id;

2. 取出第4个栏目下的商品的商品名，栏目名，价格

答: select goods_name,cat_name,shop_price from goods left join category on goods.cat_id=category.cat_id where cat_id=4;

3. 取出第4个栏目下的商品的商品名，栏目名，品牌名

答: select goods_name,cat_name,brand_name from goods left join category on goods.cat_id=category.cat_id left join brand on goods.brand_id=brand.brand_id where cat_id=4;

基础查询练习六(UNION 查询)

1. 把 goods 表和 category 表里各取出2列,并集成一个表

答: select goods_name,goods_id from goods limit 4 union select cat_id,cat_name from category limit 4;

基础查询练习七(子查询)

1. 查询出最新一行商品

答: select goods_id,goods_name,shop_price from goods where goods_id=(select max(goods_id) from goods);

2. 查询出编号为19的商品的栏目名称(用左连接和子查询分别实现)

答: select cat_name from category where cat_id=(select cat_id where from goods goods_id=19);

    select cat_name from goods left join category on goods.cat_id=category.cat_id where goods_id=19;

3. 用 where 型子查询把 goods 表中的每个栏目下最新的商品取出来

答: select goods_id,goods_name from goods where goods_id in (select max(goods_id) from goods group by cat_id);

4. 用 from 型子查询把 goods 表中的每个栏目下面最新的商品取出来

答: select * from ()
