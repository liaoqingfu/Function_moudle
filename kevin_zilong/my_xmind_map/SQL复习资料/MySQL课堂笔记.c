
�����ӣ�

	emp.mgr = boss.empno

	MySQL�µ��˿պ��� ifnull(a, b) --- oracle �µ� nvl(a, b);

	nullif(a, b):	�ж�a, b�Ƿ���ȣ���ȷ���NULL�� ���򷵻�a

	
MySQL�Ľű���

	1. �ű��е�ע�͡�-- �� ��һ������ʡ�Կո�

	2. ``���������õĿ������������ִ�Сд

	3. ��д�ű���ʹ�õı༭������Ĭ���ַ����趨�������ַ�������ʱ���ϸ��Ӧ���ݿ���ַ���

	4. MySQL�µ�������ӣ���д������ͬ��Oracle��

	5. ִ�нű����� mysql> ��ʾ���£�ִ�� source xxxx.sql(����·��)


�鿴���룺

	cat /etc/sysconfig/i18n ϵͳ���ַ�����

	echo $LANG �û����ַ����루���ԣ� 


�������룺

	1. �ɿͻ��������õ��ַ�����������������ݿ�Ĭ���趨���ַ�������Ӧ��

	2. �������ݿ���������ַ��������仯��

	3. ���ݿ⡢���趨���ַ����� �����ַ�����һ�¡�

	4. ������Զ�����ӹ��ߡ�

=================================================== MySQL API ==================================================

������libmysqlclient		.so  .a		-l

��·����/usr/lib64/mysql/libmysqlclient.a	-L

ͷ�ļ�����<mysql.h>	

·����/usr/include/mysql/mysql.h		-L

LD_LIBRARY_PATH

ʹ��mysql����� �����⣺

	gcc hellomysql.c -o hellomysql -L/usr/lib64/mysql/ -lmysqlclient -I/usr/include/mysql/ -lstdc++ -ldl -lpthread -lrt

	Makefile


SQL�����ࣺ

	1. �޽�������صġ�insert��delete��update

	2. �н�������صġ�select��desc��show...


���û�����API��

	��ʼ����

		mysql_init();

	����
		mysql_errno();/ mysql_error();

	�������ӣ�

		mysql_real_connect();

	ִ��SQL��䣺

		mysql_query();

	Ӱ��������

		mysql_affected_row();
	

	��ȡ�������

		mysql_stroe_result();    ---- mysql_use_result(); ������ϴ�

	�ͷ��ڴ棺

		mysql_free_result();

	��ȡ������

		mysql_filed_count(mysql);

		mysql_num_fields(result);

	��ȡ��ͷ��

		mysql_fetch_fields(result);	ȫ��
		
		mysql_fetch_field(result);	����

	�ر�����:

		mysql_close();
	

Ԥ������API��

	insert into emp(empno, ename, sal) values(&empno, &ename, &sal);	

	select * from emp

	desc
	
	

���ں�ʱ���ࣺ


���ѯ�ࣺ































