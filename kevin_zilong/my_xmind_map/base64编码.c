
��Կ���ɷ�����

	1. ��Դ�㷨�� md5��sha1��sha256 

	2. �Զ����㷨�� r1: 123

			r2: abc		---> a1b2c3   /abc123/321cba/12a3bc

��Կ��Ϣ��

	struct secInfo{ seckey��clientID��serverID��time��seckeyid��state }


�����ڴ������

	�ͻ��ˣ�  struct secInfo	Э����Կ��д��Կ��Ϣ		��ԿУ�飺�������ڴ棬ȡ��
	  N
	  ��
	  1
	��������  struct secInfo[]	Э����Կ��д��Կ��Ϣ		��ԿУ�飺�������ڴ棬ȡ��


�����ӿڷ�װ��		 ���ܺ��� ---> ��		keymng_shmop.c: ��������	keymng_shmop.h: �����������ṹ�嶨��

	�������򿪹����ڴ棺

		int keymng_shmInit(int shmkey, int maxnode, int *shmhdl);

	д��Կ�������ڴ���:

		int keymng_shmWriteSeckey(int shmhdl, struct secInfo *info);	// ����

	�ӹ����ڴ��ȡ��Կ��

		int keymng_shmReadSeckey(int shmhdl, char *clientID, char *serverID, struct secInfo *info��int maxnode);  // ����

---------------------------------------------------------

�ػ����̣�

	�������������ϵͳ��̨�� ������ִ��ĳ�����񣬻�ȴ�ĳһ�¼�������

	���롢��ռ�п����նˣ�ͨ�������û��������������û���ע�����˳�Ӱ�졣

	ͨ�� ������d��β�����֡�	sshd��telnetd��vsftpd��xinetd��mysqld.... 

�������裺

	1. fork�ӽ��̣� �������� �˳���			

	2. �����Ự setsid() ---> ��������ն�	�ӽ���ID == �ỰID == ������ID

	3. �ر�/�ض��� �ļ��������� dup2()  /dev/null --- /dev/zero

	4. �޸�Ĭ�ϴ����ļ�Ȩ��	umask()  022 --> Ŀ¼ 755 --> ��ͨ�ļ� 644		0511 -- 266 Ŀ¼��-w-rw-rw- �ļ���-w-rw-rw-

	5. �޸ĳ�����Ŀ¼ chdir(); 	�ı䵽ϵͳĿ¼�£��򲻿�ж�ص������û�Ŀ¼

	6. while ִ��ҵ��

	7. ��ֹ����shell�ű���

----------------------------

shell�ű���

	��һϵ�� shell������֯��һ���ļ��У�����ִ�С�	

	��һ�У�ָ�������������#��/bin/bash		shell�� bash

	a=10;  ���ر���	

	export a;	��������

	func(){...������...}		��ȡ���� $1/$2/.../$N

	func ��1 ��2 ��3 ������

	"" ��'' ��ʾ�ַ����� ֧��Ƕ�ס���xxx���ϰ��ǡ�xxx����

	$()��``: չ������


win:	\r\n

Linux:	\n		\r --> ^M


���нű���

	chmod 777 stopServer

	chmod u+x stopSrever

	. ./stopServer

	source ./stopServer

	/bin/bash ./stopServer

sigaction()

sa_mask:ֻ���źŲ�׽�����ص��ڼ䣬�����ź����Ρ�

sa_flags:0/SA_RESTART

����ϵͳ���ã�	�п�����ɵ��ý�������������һ��ϵͳ���á�	read��write��sleep��pause��wait��waitpid......

	����ϵͳ���ñ��ź��жϺ� sa_flags = SA_RESTART


=================================================================================================================

���ݿ�������

	1. �л�oracle�û���	su - oracle

	2. sqlplus /nolog

	3. SQL> connect / as sysdba

	4. SQL> startup

	5. lsnrctl start 	���� TNS ����

�����ݵ� curd��

	�飺
		select �������������������� from ���� where �������� order 

	����
		insert into �������� values(��ֵ)

	�ģ�

		update �� set ����=��ֵ where ��������  

	ɾ��
		delete from �� where ��������  

����ĸ��

	4�����ԣ�	ԭ���ԡ�һ���ԡ������ԡ��־���

	���ƣ�	�ύ��
			��ʾ��commit

			��ʽ��DDL ��䡢quit��exit
		�ع���
			��ʾ��rollback

			��ʽ���ϵ硢崻����쳣��

SQL:
	�н������select ��������

	�޽������isnert update delete 


�����ӿڷ�װ��

	int clitPool_init(int num, void *hanlde, char *dbname, char *dbuser, char *passwd);

	int clitPool_getConn(void *hanlde, void **connfd);

�޽������

	int clitPool_execNselSQL(char *sql, void *connfd);

	int clitPool_TransBegin(void *connfd);

	int clitPool_commit(void *connfd);

	int clitPool_rollback(void *connfd);

�н������

	�α꣺
		1. �����α�	---- �α���Ϊĳ�β�ѯ�����ġ�

		2. ���α�	---- �򿪳ɹ���	1��������Ѿ�����������	2���α�ָ�������е����С�

		3. fetch����	---- һ��fetch ֻ����ȡһ�����ݡ�  ͨ����� while ʹ�á�

				����ѭ����100 	ANS1 --- 1403 Oracle

		4. �ر��α�
	
	int clitPool_putConn(void *hanlde, void *connfd, int flg);
	
	int clitPool_destroy(void *hanlde);


---libicdbapi.so ��װ libclntsh.so		-licdbapi -lclntsh


---proc��

	Oracle �ṩ�� C/C++ һ�ַ���Oracle���ݿ�ķ��� Pro*c/c++ �﷨��

	proc�﷨ = C + SQL ���  

	.pc -> proc���빤�� -> .lis -> .c -> gcc���빤�� -> a.out

		libclntsh.so		libicdbapi.so

���ݿ���������

	1. ֱ�ӵ��õĺ����ķ���ֵ��

	2. �鿴 log ��־��

	3. oerr ora ȥ������ �����

�������ͣ�
	0��������ǣ� û�С�;��

	1. Լ������ --- oerr ora

	2. ��֯sql����ʱ���õ��������ó����������ͣ�Ĭ�ϸ��༭����һ�¡�----�����롿

	3. ���ӳ�ʹ�ô�����û��������뷴���������ݿ��û������ڱ������ƣ����ݿ�������û���


��ѯ�����ݣ� �ڴ�ģ�ʹ��죺

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

	��������������

	field[7].cont = &deptno;
	field[7].conLen = 4;	

	row.field = field;
	row.fieldCount = 8;


	openCursor(handle, sql, 8, &cursor);

	while (1) {
		
		ret = fetchByCursor(cursor, &row);	// ����һ������

		if (ret == 100 || 1403)
			break;

	}
	closeCursor();

ģ���װ��--- ҵ���߼�

���ݿ�Ӧ�ã�				seckey_dbop.h	����	seckey_dbop.c  ����

	��ԿЭ�̣�---- ������

��������

	int db_getkeyid(void *handle, int *seckeyid);

		1. �����ݿ��ȡ keysn ���е�ikeysn��ֵ��	--> seckeyid			�����ݿ� -- select

		2. ��ikeysn��ֵ������ д�����ݿ�keysn���е�ikeysn��				д���ݿ� -- update	-12 --�� 13

	int db_insertSeckeyInfo(void *handle, NodeSHMInfo *nodeSHMInfo);

		3. ��д�����ڴ�� ��Կ��Ϣ insert into �����ݿ�� seckeyinfo ���С�		д���ݿ� -- insert
��������

	��ԿУ�飺

		�����ڴ档

	��Կע����

		update ���ݿ�



	Oracle �� varchar2 1024 --> 2048 --> 4096		-- �ɼ��ַ���


	Blob�������ƴ�����

	Clob���ַ�������		--- ����Oracle��ʽ��ת�롣


base64���룺  Ŀ�ģ�Ϊ����������ƥ�䡣

	3 x 8 = 4 x 6

	11101010  10101001  01010101

	00111010  00101010  00100101  00010101  ---> �ɼ��ַ���  A-Z��a-z��0-9��+/  ��=����β��������������


============================ Э����Կ д���ݿ� ===========================

��ԿЭ�̷�������	ҵ���߼�	keymngserver.c

ȫ�����ݣ�

	MngServer_Info srvInfo;	--- ���ᷢ���仯��


���߳� main ()
{
	��ʼ��������������Ϣ

		keymngServer_init(&srvInfo);  // ����

	��ʼ��socket���� socket_init();

	while (1) {

		�ȴ��ͻ��˽�������	socket_accpet(); --> connfd
	
		�������� 	ptherad_create(connfd);

		���̷߳���(����)
	}

	����socket���ӡ�
}

�߳��Ӻ�����
{
	���� �ͻ��� ���͵������� -- Req --- TLV

	���� ������ --> Req { } --> cmdType 

		switch(cmdType) {

			case Э�̣�
				
				keymngServer_Agree(&info, struct Req *pReq, unsigned char **out, int *outlen);
				break;

			case У�飺

				keymngServer_Check(&info, struct Req *pReq, unsigned char **out, int *outlen);
				break;

			case ע����

				keymngServer_Revoke(&info, struct Req *pReq, unsigned char **out, int *outlen);
				break;

			default��

				break;
		}

	����Ӧ����
}


����ģ��	keymngserverop.h	������������Ϣ�ṹ�嶨�� �� ���ܺ���ԭ�͡�

					struct{ serverID��serverIp�� serverPort, shmkey, shmid��maxnode�� dbuser��dbpasswd��dbname }
		keymngserverop.c

int MngServer_init(struct *srvInfo)  	// ����
{
	������������Ϣ��ʼ����������

	�����ڴ洴��

	++++�������ݿ����ӳ�
}		

int MngServer_Agree(struct *srvInfo, struct Req *pReq�� unsigned char **out�� int *outLen)	// ����
{
	ȡ�� req-> r1

	���� Res-> r2

	++++��ȡ���ӳ���һ������

		IC_DBApi_ConnGet(ICDBHandle* handle, int sTimeout, int nsTimeout);

	++++��������

	++++��ȡseckeyid 

		KeyMngsvr_DBOp_GenKeyID(void *dbhdl, int *keyid);

	--������Կ ---> seckeyid (���ݿ�)

	д�����ڴ� 

	++++д���ݿ�

		KeyMngsvr_DBOp_WriteSecKey(void *dbhdl, NodeSHMInfo *pNodeInfo);

	++++�ر����� commit��rollback

	++++�Ż�����

		IC_DBApi_ConnFree(ICDBHandle handle, int validFlag);

		�жϷ���ֵ�� == IC_DB_CONNECT_ERR

				validFlag -> 0  

			     else

				validFlag -> 1 

	��֯Ӧ��ṹ�� Res {clientID��serverID��r2��rv��seckeyid}

	����Ӧ����

	����Ӧ����	
}

============================= ��ԿУ�� ���̷��� =================================

У�飺

	1. ����У�顣	�ǶԳơ��Գƽ�Ϸ�����

	2. Ƭ��У�鷨��	10 - 20 ��50 -70 ��100 - 120			ǰ10�ֽڡ�

�ͻ�������ܣ�

	switch(nSel) {

		case KeyMng_Check :	// ��ԿУ��

			MngClient_Check(&pCltInfo);

			break;		
	}

	��ʾ������û���

����ģ�飺	keymngclientop.c			keymngclientop.h


int MngClient_Check(MngClient_Info *pCltInfo)
{
	1. �������ڴ棬 ȡ����Կǰ10�ֽڡ�	KeyMng_ShmRead();

	2. ��֯������ { cmdType = KeyMng_Check�� r1=ǰ10�ֽ���Կ}

	3. ����������  --- Req TLV

	4. ��ʼ�����������ӡ����������ġ�

	5. ����Ӧ���� --- Res TLV

	6. ����Ӧ����
	
	7. ����rv ��ʾУ������

	return 0;
}


------�������ˣ�

	
���̣߳�

	1. ���������� -- TLV

	2. ���������� -- Req {cmdType}

	3. switch ��cmdType��{

		case KeyMng_Check :	// ��ԿУ��

			MngServer_Check����;
			break;
	}

	4. ����Ӧ���� -- TLV


int MngServer_Check(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	1. ȡ�� Req �е�clientID/serverID
	
	2. ���� clientID/serverID �������ڴ棬 ȡ����Ӧ��Կǰ 10 �ֽڡ�

	3. �ȶ� --> Ӧ���� Res.rv = 0/1

	4. ��֯Ӧ���Ľṹ�� Res		r2��ʹ�ã���ա� seckeyid

	5. ����Ӧ����

	6. ������
}


=================================== ��Կע�� ���̷��� ====================================

	update SECMNG.seckeyinfo set state=1 where keyid = ��seckeyid��


�ͻ�������ܣ�

	switch(nSel) {

		case KeyMng_Revoke :	// ��Կע��

			MngClient_Revoke(&pCltInfo);

			break;		
	}

	��ʾ������û���

����ģ�飺	keymngclientop.c			keymngclientop.h

int MngClient_Revoke(MngClient_Info *pCltInfo�� int seckeyid)
{
	1. �������ڴ棬 ȡ����Կ���	KeyMng_ShmRead();

	2. ��֯������ { cmdType = KeyMng_Revoke�� r1 = �ɵ�seckeyid }

	3. ����������  --- Req TLV

	4. ��ʼ�����������ӡ����������ġ�

	5. ����Ӧ���� --- Res TLV

	6. ����Ӧ����
	
	7. ���� rv ��ʾע�������

	return 0;
}


------�������ˣ�

	
���̣߳�

	1. ���������� -- TLV

	2. ���������� -- Req {cmdType}

	3. switch ��cmdType��{

		case KeyMng_Revoke :	// ��Կע��

			MngServer_Revoke����;
			break;
	}

	4. ����Ӧ���� -- TLV


int MngServer_Revoke(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	1. ȡ�� Req �е� clientID/serverID | ȡ�� r1 ��ȡ������Կ�ı�š�
	
	2. ���� clientID/serverID �������ڴ棬 ȡ�� seckeyid��

	3. server_init() ��ʼ�����ӳأ� ��ȡ���ݿ����ӣ� 

	4. ִ�� update SECMNG.seckeyinfo set state=1 where keyid = ��seckeyid��

		ʵ��һ�������� ���ҵ��������� KeyMngsvr_DBOp_UpdateSecKeyState(void *handle, int seckeyid);


	5. �����ý�� Res.rv = 0/1	ע���ɹ����

	6. ��֯Ӧ���Ľṹ�� Res		r2��ʹ�ã���ա� seckeyid

	7. ����Ӧ����

	8. ������
}

int KeyMngsvr_DBOp_UpdateSecKeyState(void *handle, int seckeyid)
{
	��������

	ִ�� update SECMNG.seckeyinfo set state=1 where keyid = ��seckeyid��

	commit��rollback��
}
















	