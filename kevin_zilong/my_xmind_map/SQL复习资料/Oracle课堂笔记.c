
������ʹ�ã�

	�ڹ��������� �� where having ����ʹ�ñ�����

	���Ӳ�ѯ�ж���ı����� ������ ����ѯ�Ĺ���������ʹ�á�where having ����ʹ�ñ���	��

�����ݵ� curd ��


���룺
	insert into ���� values����ֵ[, ��ֵ]��

	insert into ������������ values����ֵ[, ��ֵ]��

	insert into ������������ values��&���� [, &����]��

	insert into ���� select ���		

���£�
	update ���� set ���� = ��ֵ	where  ��������

ɾ����
	delete from  ���� where ��������

	delete / truncate 


����
	����ִ�� ���� SQL��䣬 Ҫô���ɹ���Ҫô��ʧ�ܡ�

	4�����ԣ�ԭ���ԡ�һ���ԡ������ԡ��־��ԡ�

	��������

		DML ��䣬 Oracle �Զ���������

	�ر�����

		��ʾ��  commit��rollback

		��ʽ��	DDL ��䡢 exit/quit ����ʽ commit���� �ϵ硢崻�������ʽrollback��

�� �� curd��

������

	create table ���������� ������[, ���� ������]��

	create table ���� as select xxxx where ������ 	������֪��ṹ��ͬ�ı�

	create table ���� as select xxxx where ������ 	������֪��ṹ��ͬ�ı��ͬʱ�� Я�������ݡ�

	create table ���� as where �Ӳ�ѯ/����ѯ	�������ű��ǵ����ݵı�  Я�����ű����ݡ�   ע�⣺��Ӧ�����˵��Ҫʹ�ñ�����

�޸ı�

	����У�	alter table ���� add ���� ������

	�޸��У�	alter table ���� modify ���� ������	--- �޸������͡�

	ɾ���У�	alter table ���� drop cloumn ����

	�������У�	alter table ���� rename cloumn ������ to ��������


ɾ����

	drop table ������			show recyclebin		purge recyclebin		

	drop table ���� purge��		����������վ��		-- ��ͨ�û�

������
	rename �ɱ��� to �±�����


��Լ����

	�ǿ� 	not null

	Ψһ��	unique

	����Լ�� 	primary  key == not null + unique

	�����	check 	��Oracle �У�

	���	Foreign key 			A �� ����  �� B ������		---- �����ÿա�����ɾ��




������

	������ create index �������� on  ���� ��������

	ɾ���� drop  index ��������


MySQL�İ汾��

	�����棺
		1. Դ���		

			window: 32/64

			Linux

			mac

		2. ͨ�ð�

		3. ��׼��

		4. �����

	��ҵ��

	��Ⱥ��

Oracle:	�����û������ݹ���ϵͳ

MySQL��	�������ݿ�����ݹ���ϵͳ

	1. ѡ�����ݿ�	2. ʹ�����ݱ�

���ݿ��curd��

	�������ݿ⣺

		create database mydb1;

		create database mydb2 character set utf8;  	ָ���ַ���


	�鿴���ݿ⣺

		show databases

		show create database mydb1;

	�޸����ݿ⣺

		alter database mydb1 character set utf8;	�޸��ַ���


	ɾ�����ݿ⣺

		drop database ���ݿ�����

	
��� curd��

	������

		create table ���������� ������[, ���� ������]��   ��Լ��	ע���������Ͳ�ͬ

	�鿴��

		show tables��

		desc ����

		show create table ����

	�޸ı�
		rename table �ɱ��� to �±���

		alter table ���� add ���� ������

		alter table ���� modify ���� ��������

		alter table ���� delete column ����

		alter table ���� rename column ���� to ������

		alter table employee character set gbk  		�޸��ַ���

	ɾ����

		drop table ����

�����ݵ� curd��

	insert ���ݣ�

		insert into �� values(��ֵ);

		insert into �������� values(��ֵ);

	update ���ݣ�

		update ���� set ���� = ����ֵ


����ѯ��

	
�����ӣ�		--- ��ֵ����/����ֵ����

	select ����

	from ��1�� ��2					��,�� ---�� inner 	join ����ʡ�ԡ�

	where  ��1��= ��2��				��where�� ---> on

�����ӣ�

	select ����

	from ��1�� ��2					��,�� ---�� left [outer] join/ right [outer] join

	where  ��1��= ��2��				��where�� ---> on	


	ѡ�����⡢���⣬ȡ���� from �� �����д˳���� on ֮���б���ʽ����д˳�� �޹أ�����












