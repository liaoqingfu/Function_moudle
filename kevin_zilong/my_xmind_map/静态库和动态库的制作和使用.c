��̬���������
	1. Դ����  .c ���� .cpp
	2. ��Դ�ļ����� .o���ļ�  gcc *.c -c ������ .o���ļ� ע�����ͷ�ļ���������Ļ�����Ҫָ��ͷ�ļ�·��  gcc *.c -c -I ../include/    //ָ��ͷ�ļ�Ϊ�ϼ�Ŀ¼��include���ļ���
	3. �� .o���ļ����  ar rcs ��̬������� .o Դ�ļ�
	 ���磺 ar rcs libmytest.a  *.o;  //�����ͻ�����һ��.a�ľ�̬���ļ�
	 //��̬����������� libxxxx.a  ǰ�������lib��ͷ����������� .a�������м����д�Լ�����������
ʹ�� nm����鿴��̬���ڵ��ļ���Ϣ nm libmytest.a
	 
��̬���ʹ�ã�
	1. main.c�ļ��������ͷ�ļ� 
	2. gcc main.c -I ./include/ -L ./lib/ -l mytest -o app;
	//����main.c ָ��ͷ�ļ�·�� -I ���� I���ڵ�ǰĿ¼��include�ļ������棬 ָ�����ļ���·�� -L����L�� �ڵ�ǰĿ¼��lib�ļ������棬 ָ�����ļ����� ��-l(С l ��ĸ)����������Ҫȥ���涨�Ĳ��ּ�ǰ��ȥ�� lib  ����ȥ�� .a Ȼ��-o��С o) ���ɿ�ִ���ļ�����������ȡ��
	
vi����ͬʱ������ʾ����ļ���� vi -O(��д��ʾ������ʾ�� *.c
 Сд -o �Ǻ�����ʾ����ļ�
 
 
 ��̬��Ĵ�����
gcc func.c -o libfunc.so -fpic -shared
gcc main.c -o a.out -lfunc -L.
./a.out error
export LD_LIBRARY_PATH=. (��һ�ֽ���취)
sudo cp libfunc.so /usr/lib (�ڶ��ֽ���취)

��̬��Ĵ���:
gcc func.c -c  ���� func.o
ar rcs libfunc.a func.o
�����̬��Ͷ�̬���������������Ӷ�̬��
gcc main.c -lfunc -static -L. ָ��ʹ�þ�̬��

�����̬��Ͷ�̬������������Ҫͬʱ�õ���̬��Ͷ�̬��
gcc main.c -Wl,-Bstatic -lfunc -L. -Wl,-Bdynamic -lfunc -L.



��ȫman�ֲ�
sudo apt-get install glibc-doc

sudo apt-get install manpages-posix-dev
	
	
	
��̬���������
	1. �������� libxxx.so  //ǰ�������lib��ͷ����������� .so����
	2. ��Դ�ļ����� .o �ļ� gcc *.c -c -fpic ����  gcc *.c -c fPIC �������������� 
	ע�����ͷ�ļ���������Ļ�����Ҫָ��ͷ�ļ�·��  gcc *.c -c -fpic -I ../include/    //ָ��ͷ�ļ�Ϊ�ϼ�Ŀ¼��include���ļ���
	3. ��.o ���ļ�����ɶ�̬�� 
		gcc -shared  *.o -o libxxx.so  
	 //�����ͻ�����һ��.so�Ķ�̬���ļ�
	 
ʹ�� nm����鿴��̬���ڵ��ļ���Ϣ nm libxxx.so

��̬���ʹ�ã�
	1. main.c�ļ��������ͷ�ļ� 
	2. gcc main.c -I ./include/ -L ./lib/ -l mytestm -o app;
	//����main.c ָ��ͷ�ļ�·�� -I ���� I���ڵ�ǰĿ¼��include�ļ������棬 ָ�����ļ���·�� -L����L�� �ڵ�ǰĿ¼��lib�ļ������棬 ָ�����ļ����� ��-l(С l ��ĸ)����������Ҫȥ���涨�Ĳ��ּ�ǰ��ȥ�� lib  ����ȥ�� .a Ȼ��-o��С o) ���ɿ�ִ���ļ�����������ȡ��
	�ŷ��;�̬����һ���� �����ǻᱨ�����ض�̬��ʧ�����⣩��
	./app: error while loading shared libraries: libmycalcm.so: cannot open shared object file: No such file or directory
	
	3. ��Ҫ����̬�⿽������Ŀ¼�µ�lib�ļ����棬Ȼ����ִ�г����OK�ˣ�������ò�Ҫ����Ū���������ڴ�ռ�á�
	
	4. ʹ�û��������� 
		��1��.��ʱ���ã� ���� export LD_LIBRARY_PATH=��̬���·����ֻҪ·������Ҫ����Ŀ��ļ�������Ǿ���·������
		���磺 export LD_LIBRARY_PATH=/home/zilong/test/lib:$LD_LIBRARY_PATH  //��ʱ���ã��Ⱥ����߲����пո�
		��2��.�������ã�  ��½shellʱϵͳ����� ~/.bashrc �ļ������Խ����÷ŵ�����ļ��ڣ���һ���ն˾ͻ��Զ����û��������ˣ����û�Ŀ¼�ĸ�Ŀ¼��
		�����һ���������ã�export LD_LIBRARY_PATH=/home/zilong/test/lib/
		
	