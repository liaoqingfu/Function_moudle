
密钥生成方法：

	1. 开源算法： md5、sha1、sha256 

	2. 自定义算法： r1: 123

			r2: abc		---> a1b2c3   /abc123/321cba/12a3bc

密钥信息：

	struct secInfo{ seckey、clientID、serverID、time、seckeyid、state }


共享内存操作：

	客户端：  struct secInfo	协商密钥：写密钥信息		密钥校验：读共享内存，取出
	  N
	  ：
	  1
	服务器：  struct secInfo[]	协商密钥：写密钥信息		密钥校验：读共享内存，取出


函数接口封装：		 功能函数 ---> 库		keymng_shmop.c: 函数定义	keymng_shmop.h: 函数声明、结构体定义

	创建、打开共享内存：

		int keymng_shmInit(int shmkey, int maxnode, int *shmhdl);

	写密钥到共享内存中:

		int keymng_shmWriteSeckey(int shmhdl, struct secInfo *info);	// 传入

	从共享内存读取密钥：

		int keymng_shmReadSeckey(int shmhdl, char *clientID, char *serverID, struct secInfo *info，int maxnode);  // 传出

---------------------------------------------------------

守护进程：

	长期运行与操作系统后台， 周期性执行某个任务，或等待某一事件发生。

	脱离、不占有控制终端（通常不与用户交互）。不受用户的注销、退出影响。

	通常 采用以d结尾的名字。	sshd、telnetd、vsftpd、xinetd、mysqld.... 

创建步骤：

	1. fork子进程， 将父进程 退出。			

	2. 创建会话 setsid() ---> 脱离控制终端	子进程ID == 会话ID == 进程组ID

	3. 关闭/重定向 文件描述符。 dup2()  /dev/null --- /dev/zero

	4. 修改默认创建文件权限	umask()  022 --> 目录 755 --> 普通文件 644		0511 -- 266 目录：-w-rw-rw- 文件：-w-rw-rw-

	5. 修改程序工作目录 chdir(); 	改变到系统目录下，或不可卸载的其他用户目录

	6. while 执行业务

	7. 终止程序（shell脚本）

----------------------------

shell脚本：

	将一系列 shell命令组织到一个文件中，批量执行。	

	第一行：指定命令解析器：#！/bin/bash		shell、 bash

	a=10;  本地变量	

	export a;	环境变量

	func(){...函数体...}		获取参数 $1/$2/.../$N

	func 参1 参2 参3 。。。

	"" 、'' 表示字符串。 支持嵌套。“xxx的老板是‘xxx’”

	$()、``: 展开命令


win:	\r\n

Linux:	\n		\r --> ^M


运行脚本：

	chmod 777 stopServer

	chmod u+x stopSrever

	. ./stopServer

	source ./stopServer

	/bin/bash ./stopServer

sigaction()

sa_mask:只在信号捕捉函数回调期间，设置信号屏蔽。

sa_flags:0/SA_RESTART

慢速系统调用：	有可能造成调用进程永久阻塞的一类系统调用。	read、write、sleep、pause、wait、waitpid......

	慢速系统调用被信号中断后： sa_flags = SA_RESTART


=================================================================================================================

数据库启动：

	1. 切换oracle用户。	su - oracle

	2. sqlplus /nolog

	3. SQL> connect / as sysdba

	4. SQL> startup

	5. lsnrctl start 	启动 TNS 服务

表数据的 curd：

	查：
		select 列名、列名。。。列名 from 表名 where 过滤条件 order 

	增：
		insert into 表（列名） values(列值)

	改：

		update 表 set 列名=列值 where 过滤条件  

	删：
		delete from 表 where 过滤条件  

事务的概念：

	4大特性：	原子性、一致性、隔离性、持久性

	控制：	提交：
			显示：commit

			隐式：DDL 语句、quit、exit
		回滚：
			显示：rollback

			隐式：断电、宕机、异常。

SQL:
	有结果集：select 处理结果集

	无结果集：isnert update delete 


函数接口封装：

	int clitPool_init(int num, void *hanlde, char *dbname, char *dbuser, char *passwd);

	int clitPool_getConn(void *hanlde, void **connfd);

无结果集：

	int clitPool_execNselSQL(char *sql, void *connfd);

	int clitPool_TransBegin(void *connfd);

	int clitPool_commit(void *connfd);

	int clitPool_rollback(void *connfd);

有结果集：

	游标：
		1. 创建游标	---- 游标是为某次查询而生的。

		2. 打开游标	---- 打开成功：	1）结果集已经保存至本地	2）游标指向结果集中的首行。

		3. fetch数据	---- 一次fetch 只能提取一行数据。  通常结合 while 使用。

				结束循环：100 	ANS1 --- 1403 Oracle

		4. 关闭游标
	
	int clitPool_putConn(void *hanlde, void *connfd, int flg);
	
	int clitPool_destroy(void *hanlde);


---libicdbapi.so 封装 libclntsh.so		-licdbapi -lclntsh


---proc：

	Oracle 提供给 C/C++ 一种访问Oracle数据库的方法 Pro*c/c++ 语法。

	proc语法 = C + SQL 结合  

	.pc -> proc编译工具 -> .lis -> .c -> gcc编译工具 -> a.out

		libclntsh.so		libicdbapi.so

数据库错误分析：

	1. 直接调用的函数的返回值。

	2. 查看 log 日志。

	3. oerr ora 去掉负号 错误号

错误类型：
	0：结束标记， 没有“;”

	1. 约束错误 --- oerr ora

	2. 组织sql语句的时候，用到常量，该常量数据类型，默认跟编辑工具一致。----【乱码】

	3. 连接池使用错误的用户名、密码反复访问数据库用户，出于保护机制，数据库会锁定用户。


查询行数据， 内存模型打造：

	int empno = 0;
	char ename[10];
	char job[10];
	int mgr = 0;
	char hirdate[10];
	......
	int deptno = 0;

	ICDBRow	row;	
	ICDBField field[18];

	field[0].cont = &empno;
	field[0].conLen = 4;

	field[1].cont = ename;
	field[1].conLen = 10;	
	
	field[2].cont = job;
	field[2].conLen = 10;	

	。。。。。。。

	field[7].cont = &deptno;
	field[7].conLen = 4;	

	row.field = field;
	row.fieldCount = 8;


	openCursor(handle, sql, 8, &cursor);

	while (1) {
		
		ret = fetchByCursor(cursor, &row);	// 传出一行数据

		if (ret == 100 || 1403)
			break;

	}
	closeCursor();

模块封装。--- 业务逻辑

数据库应用：				seckey_dbop.h	声明	seckey_dbop.c  定义

	密钥协商：---- 服务器

开启事务

	int db_getkeyid(void *handle, int *seckeyid);

		1. 读数据库获取 keysn 表中的ikeysn列值。	--> seckeyid			读数据库 -- select

		2. 将ikeysn列值自增， 写会数据库keysn表中的ikeysn列				写数据库 -- update	-12 --》 13

	int db_insertSeckeyInfo(void *handle, NodeSHMInfo *nodeSHMInfo);

		3. 将写共享内存的 密钥信息 insert into 到数据库的 seckeyinfo 表中。		写数据库 -- insert
结束事务

	密钥校验：

		共享内存。

	密钥注销：

		update 数据库



	Oracle ： varchar2 1024 --> 2048 --> 4096		-- 可见字符。


	Blob：二进制大数据

	Clob：字符大数据		--- 按照Oracle格式，转码。


base64编码：  目的，为了数据类型匹配。

	3 x 8 = 4 x 6

	11101010  10101001  01010101

	00111010  00101010  00100101  00010101  ---> 可见字符。  A-Z、a-z、0-9、+/  “=”结尾，表结束或者填充


============================ 协商密钥 写数据库 ===========================

密钥协商服务器：	业务逻辑	keymngserver.c

全局数据：

	MngServer_Info srvInfo;	--- 不会发生变化。


主线程 main ()
{
	初始化服务器基础信息

		keymngServer_init(&srvInfo);  // 传出

	初始连socket连接 socket_init();

	while (1) {

		等待客户端建立连接	socket_accpet(); --> connfd
	
		启动子线 	ptherad_create(connfd);

		子线程分离(回收)
	}

	销毁socket连接。
}

线程子函数：
{
	接收 客户端 发送的请求报文 -- Req --- TLV

	解码 请求报文 --> Req { } --> cmdType 

		switch(cmdType) {

			case 协商：
				
				keymngServer_Agree(&info, struct Req *pReq, unsigned char **out, int *outlen);
				break;

			case 校验：

				keymngServer_Check(&info, struct Req *pReq, unsigned char **out, int *outlen);
				break;

			case 注销：

				keymngServer_Revoke(&info, struct Req *pReq, unsigned char **out, int *outlen);
				break;

			default：

				break;
		}

	发送应答报文
}


功能模块	keymngserverop.h	服务器基本信息结构体定义 、 功能函数原型。

					struct{ serverID、serverIp、 serverPort, shmkey, shmid，maxnode， dbuser，dbpasswd，dbname }
		keymngserverop.c

int MngServer_init(struct *srvInfo)  	// 传出
{
	服务器基础信息初始化、传出。

	共享内存创建

	++++创建数据库连接池
}		

int MngServer_Agree(struct *srvInfo, struct Req *pReq， unsigned char **out， int *outLen)	// 传入
{
	取出 req-> r1

	生成 Res-> r2

	++++获取连接池中一条连接

		IC_DBApi_ConnGet(ICDBHandle* handle, int sTimeout, int nsTimeout);

	++++开启事务

	++++获取seckeyid 

		KeyMngsvr_DBOp_GenKeyID(void *dbhdl, int *keyid);

	--生成密钥 ---> seckeyid (数据库)

	写共享内存 

	++++写数据库

		KeyMngsvr_DBOp_WriteSecKey(void *dbhdl, NodeSHMInfo *pNodeInfo);

	++++关闭事务 commit、rollback

	++++放回连接

		IC_DBApi_ConnFree(ICDBHandle handle, int validFlag);

		判断返回值： == IC_DB_CONNECT_ERR

				validFlag -> 0  

			     else

				validFlag -> 1 

	组织应答结构体 Res {clientID、serverID、r2、rv、seckeyid}

	编码应答报文

	传出应答报文	
}

============================= 密钥校验 流程分析 =================================

校验：

	1. 加密校验。	非对称、对称结合方法。

	2. 片段校验法：	10 - 20 ，50 -70 ，100 - 120			前10字节。

客户端主框架：

	switch(nSel) {

		case KeyMng_Check :	// 密钥校验

			MngClient_Check(&pCltInfo);

			break;		
	}

	显示结果给用户。

功能模块：	keymngclientop.c			keymngclientop.h


int MngClient_Check(MngClient_Info *pCltInfo)
{
	1. 读共享内存， 取出密钥前10字节。	KeyMng_ShmRead();

	2. 组织请求报文 { cmdType = KeyMng_Check， r1=前10字节密钥}

	3. 编码请求报文  --- Req TLV

	4. 初始化、建立连接、发送请求报文。

	5. 接收应答报文 --- Res TLV

	6. 解码应答报文
	
	7. 根据rv 显示校验结果。

	return 0;
}


------服务器端：

	
子线程：

	1. 接收请求报文 -- TLV

	2. 解码请求报文 -- Req {cmdType}

	3. switch （cmdType）{

		case KeyMng_Check :	// 密钥校验

			MngServer_Check（）;
			break;
	}

	4. 发送应答报文 -- TLV


int MngServer_Check(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	1. 取出 Req 中的clientID/serverID
	
	2. 根据 clientID/serverID 读共享内存， 取出相应密钥前 10 字节。

	3. 比对 --> 应答报文 Res.rv = 0/1

	4. 组织应答报文结构体 Res		r2不使用，清空。 seckeyid

	5. 编码应答报文

	6. 传出。
}


=================================== 密钥注销 流程分析 ====================================

	update SECMNG.seckeyinfo set state=1 where keyid = 【seckeyid】


客户端主框架：

	switch(nSel) {

		case KeyMng_Revoke :	// 密钥注销

			MngClient_Revoke(&pCltInfo);

			break;		
	}

	显示结果给用户。

功能模块：	keymngclientop.c			keymngclientop.h

int MngClient_Revoke(MngClient_Info *pCltInfo， int seckeyid)
{
	1. 读共享内存， 取出密钥编号	KeyMng_ShmRead();

	2. 组织请求报文 { cmdType = KeyMng_Revoke， r1 = 旧的seckeyid }

	3. 编码请求报文  --- Req TLV

	4. 初始化、建立连接、发送请求报文。

	5. 接收应答报文 --- Res TLV

	6. 解码应答报文
	
	7. 根据 rv 显示注销结果。

	return 0;
}


------服务器端：

	
子线程：

	1. 接收请求报文 -- TLV

	2. 解码请求报文 -- Req {cmdType}

	3. switch （cmdType）{

		case KeyMng_Revoke :	// 密钥注销

			MngServer_Revoke（）;
			break;
	}

	4. 发送应答报文 -- TLV


int MngServer_Revoke(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	1. 取出 Req 中的 clientID/serverID | 取出 r1 获取旧有密钥的编号。
	
	2. 根据 clientID/serverID 读共享内存， 取出 seckeyid。

	3. server_init() 初始化连接池， 获取数据库连接， 

	4. 执行 update SECMNG.seckeyinfo set state=1 where keyid = 【seckeyid】

		实现一个函数， 并且调用起来： KeyMngsvr_DBOp_UpdateSecKeyState(void *handle, int seckeyid);


	5. 将调用结果 Res.rv = 0/1	注销成功与否

	6. 组织应答报文结构体 Res		r2不使用，清空。 seckeyid

	7. 编码应答报文

	8. 传出。
}

int KeyMngsvr_DBOp_UpdateSecKeyState(void *handle, int seckeyid)
{
	开启事务

	执行 update SECMNG.seckeyinfo set state=1 where keyid = 【seckeyid】

	commit、rollback。
}
















	