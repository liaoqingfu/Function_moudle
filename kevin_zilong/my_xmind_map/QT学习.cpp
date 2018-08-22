
//主函数，最好不要在main函数内弄，不然后面的增加功能弄不了，全部转到窗口的构造函数内才行
int main(int argc, char *argv[])
{
     QApplication a(argc, argv);//管理图形用户界面应用程序的控制流和主要设置,在其中来自窗口系统和其它资源的所有事件处理和调度,处理应用程序的初始化和结束，并且提供对话管理
    Widget w;   //定义一个窗口类
   //w.show();  //显示窗口
    w.resize(600,600);  //设置窗口的大小尺寸
    //w.resize(QSize(600,600)); //将尺寸转换成QSize类型,在给窗口传QSize类的大小尺寸,和上面是一样的
    QPushButton btn;

    btn.setParent(&w); //在w窗口内设置一个按钮
    btn.setText("确定");  //设置按钮的属性文字,给按钮取名
    btn.move(100,200);
    w.show();  //显示窗口,最后显示
    return a.exec();  //程序进入消息循环，等待对用户输入进行响应
}