Makefile ������ʾError��Warning�ȴ����Լ�������Ϣ



ԭ�ĳ�����https://my.oschina.net/bingwu/blog/788347        


    �������� Makefile ������򣬵��ǳ������һɫ�ı��������Ϣ���˲��Ҵ����ʱһ��ͷ�󣡻���GitHub���� color_cpmpile ��Ŀ�������츣������л����Ŀ��ԭ���ߣ�
    GitHub�ϵ���Ŀ��ַ  https://github.com/chinaran/color-compile
    ��������������Դ���룺http://pan.baidu.com/s/1kUTFtQ3
    1�����롣�ܼ򵥣�������ĿĿ¼��

$ \make /*ʹ�õ���ϵͳ�� make ������,���� alias ����ı���*/
     2����װ��������ɺ���ǰ�װ�ˣ�install �Ĺ����� copy ������ /usr/local/bin �У����� shell �������ļ����˴�һ���� ~/.bashrc��ĩβ��ӱ�����

$ sudo \make install /*��װ*/
    ��ʱ�鿴 ~/.bashrc �ļ������ļ���ĩβӦ�ܿ����¼���ı���
alias gcc="color_compile gcc"
alias g++="color_compile g++"
alias make="color_compile make"
    3�����ý�������������Ҳ�Ǻܼ򵥣�ֻҪ�� ~/.bashrc �ļ�ĩβ����һ�����ݼ��ɣ����Ǻ�˼����������Ҳ�ɸ��ݸ�ʽ����Լ��ı�������
 # ����ʱ��ɫ��� 2016��11��15�� ���ڶ� 14:36:00 CST
alias arm-hisiv100nptl-linux-gcc="color_compile arm-hisiv100nptl-linux-gcc"
alias arm-hisiv100nptl-linux-g++="color_compile arm-hisiv100nptl-linux-g++"
alias arm-hismall-linux-gcc="color_compile arm-hismall-linux-gcc"
alias arm-hismall-linux-g++="color_compile arm-hismall-linux-g++"
    4����Ч����Ҫ source һ���������ļ���Ч��Ҳ�������´��նˣ�

$ source ~/.bashrc
    ��ʱ��ȥ make ���빤�̣��ͻᷢ���кܶ���Ŀ�������Ϣ�ˡ�����ʹ���Դ������Ӳ鿴Ч��

$ make TEST /* ==gcc -Wall test.c */
    �������

    
    5��ж�ػ���װ
    a) ɾ�� ~/.bashrc �����������ӵ� alias �����������������δ֪Ӱ�졣
    b) ɾ�� /usr/local/bin/color_compile ��ִ�г���

$ sudo rm /usr/local/bin/color_compile ����
    1��gcc �汾Ϊ 4.9.0 ����ʱ������ʹ�� gcc ���õĸ�����ʾ�����ϲ��� -fdiagnostics-color=auto ����
    2����Ҫ�޸�������ɫ�����ֻҪ�޸� out_color_info.c Դ�뼴�ɣ������в�����ɫ�ĺ궨�塣




�� ~/Ŀ¼���棬���� .bashrc�ļ����޸������ļ���������ݣ�
���ļ���������ӣ�
alias gcc="color_compile gcc"        //gcc���������ɫ��ʾ
alias g++="color_compile g++"    //g++���������ɫ��ʾ
//arm���������ɫ��ʾ
alias arm-hisiv100nptl-linux-gcc="color_compile arm-hisiv100nptl-linux-gcc"
alias arm-hisiv100nptl-linux-g++="color_compile arm-hisiv100nptl-linux-g++"
alias arm-hismall-linux-gcc="color_compile arm-hismall-linux-gcc"

alias arm-hismall-linux-g++="color_compile arm-hismall-linux-g++"
//mips���������ɫ��ʾ
alias mipsel-linux-gcc="color_compile mipsel-linux-gcc"

alias mipsel-linux-g++="color_compile mipsel-linux-g++"

alias make="color_compile make"    //make������ɫ��ʾ

