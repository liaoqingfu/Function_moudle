1. windows�������ݿ�
 �������½ mysql -uroot -p
		  ���룺123456
		   use mysql; //ʹ��һ����
		�鿴��������û���select user,password,host from user;
		��mysql�´����û��� create user '�û���'@'%'; //�����û����û��� �Զ��壬���������%������host�������Ӷ˿�Ϊ����
		���û�����Ȩ�ޣ� create all on *.* to '�û�����@��%��; 
		//�û��� �Լ����壬���������%������host�������Ӷ˿�Ϊ����
		
		Ȼ����navicat���������������ݿ�
		����һ�����ӵ����֣������������IP���˿���3306 Ĭ��
		�����û��� �� ���� ���Ӿ���
		
2. linux�������ݿ⣺
		http://blog.csdn.net/m0_37897437/article/details/72956110
		
		
		
		Linux��Mysql_odbc�İ�װ���ü�OTL��ʹ��
ԭ�� 2017��06��09�� 11:46:32 ��ǩ��linux /OTL /odbc /mysql /���ݿ� 320
���ݿ��ǳ�����mysql���ݿ⣬���ݿ���ʽӿ���ѡ����ODBC-OTL�������ӡ���Ϊ��Ҳ�ǵ�һ��ʵ��ʹ��OTL��������������ODBC�����õȵȶ�����ά��Ա���а�װ���ã�����Լ����þ������˺ܴ���谭����������ȸ��ϵ����½������ã����ִ��������ͬ������Ч��������ڲ�͸�����÷������������ϰ����½Ӵ�ODBC��ͬѧ��������·��

ODBC������

��һ�������ر�Ҫ�������

��һ���Ǳ���ģ�Ҫ��װODBCҪ�µİ�װ���У�

unixODBC-2.3.4.tar.gz
mysql-connector-odbc-5.1.12-linux-debian6.0-x86-64bit.tar.gz
1
2
��װ���İ汾û��̫���Ҫ��ֻ��Ҫ��������ok��unixODBC��ODBC����İ�װ����mysql-connector-odbc��mysql����odbc�Ĳ����װ���������Ϳ������ء�

�ڶ�������ѹ��װ�����а�װ

��ϤLinuxϵͳ�ĸ�λӦ�ö��ܺ������Ĳ����ˣ��˴�ճ���ҵĲ�������ɣ�

unixODBC�İ�װ��

# tar -zxvf unixODBC-2.3.4.tar.gz
# cd unixODBC-2.3.4
# ./configure --prefix=/usr/local/unixODBC-2.3.4 --includedir=/usr/include --libdir=/usr/lib64 -bindir=/usr/bin --sysconfdir=/etc
# make
# sudo make install

mysql-connector-odbc�İ�װ��

# tar -zxvf mysql-connector-odbc-5.1.12-linux-debian6.0-x86-64bit.tar.gz
# cd mysql-connector-odbc-5.1.12-linux-debian6.0-x86-64bit/lib
# cp ./*.* /usr/lib64

������mysql-connector-odbc����һ���ǳ���Ҫ��������һ�����˺ܳ�ʱ�䣬������ҵİ�װ���汾���м�Ҫ˵����������ʹ��ʱ����ʵ�����������Եĵ�������mysql-connector-odbc 5.1֮��İ汾��ֻ��Ҫ��libmyodbc.so�ļ����ƹ�ȥ�Ϳ����ˣ�������Ӳ��ɹ���Ҫ���һ��������

./myodbc-installer -d -a -n "MYSQL" -t "DRIVER=/usr/lib64/libmyodbc5.so;SETUP=/usr/lib64/libmyodbc5.so"  
1
ִ��֮��ᱨ����ʾȱ���ļ���libodbc.so.1��libodbcinst.so.1��ֻ��Ҫ������

cp /usr/lib64/libodbc.so.2 /usr/lib64/libodbc.so.1
cp /usr/lib64/libodbcinst.so.2 /usr/lib64/libodbcinst.so.1
1
2
�ٴ�ִ�������ָ��ͻ���ʾ��Success: Usage count is 1 �����˰�װ��OK��

�������������ļ�

��װ��odbc֮�����д��������ļ��ˣ������ļ��Զ�������/etc/Ŀ¼�£��ֱ��ǣ�odbc.ini �� odbcinst.ini ��odbc.ini��Ҫд���ǽ�Ҫ���ӵ����ݿ���Ϣ������������odbcinst.ini��Ҫд����odbc��������Ҫ���ӵĿ⣬����д���ҵ�С���ӣ�

odbc.ini��
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

Ȼ��ִ��isql -v test�����������ݣ� isql -v test ;test�������Ǹ���odbc.ini��[test] �������Լ����ȡ���ҵ���mysql ��isql -v mysql

+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
SQL>

���ˣ��й�mysql-odbc�İ�װ������������ɣ��ɹ������������ӵ����ݿ��ˣ�

OTL��ʹ��

���ú�ODBC�ͽ��뵽ʹ�û����ˡ��˲������ݿ������ж�������кܶ�������£�Ҳ�����ټ�������ȥ�� 
����OTL�ҾͲ���������ˣ�����˵������һ����װ�õĲ������ݿ�Ŀɲ����Ŀ⡣ֻ��Ҫ����һ��otlv4.h��ͷ�ļ����ɣ��йٷ���վ�����ṩ���ء� 
����Ҳ�Ƚϼ򵥣������ҷ�����һС�δ����ṩ�ο���

#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#define _ALLOW_RTCc_IN_STL
#endif

#include <iostream>
using namespace std;

#define OTL_ODBC_MSSQL_2008//����궨��Ҫ�ڰ���otlv4ǰ
#include <otlv4.h>//��otlv4.h���Ƶ�/usr/local/include��

int main()
{
    otl_connect m_db;//���Ӷ���
    otl_connect::otl_initialize();//���ӳ�ʼ��
    m_db.rlogon("DSN=test");//��������odbc�Ͽ��˺ܾã�DSNָ�������Ǹ����úõ�odbc.ini

    char test_buf[1024] = { 0 };
    int test_num = 0;
    //��ѯ���ݿ�
    try
    {
        otl_stream select_info(50, "select * from test", m_db);
        while(!select_info.eof())
        {
            select_info >> test_buf >> test_num;//���ղ�ѯ˳�����������δ���
        }
    }   
    catch(otl_exception &p)
    {
        cout << p.msg;//�쳣���
    }

    cout << "test_buf : " << test_buf << "test_num" << test_num << endl;

    m_db.logoff();//�ر����ݿ�

    return 0; 
}

���ˣ�ʹ��OTL_ODBC����mysql���ݿ���в���������������

