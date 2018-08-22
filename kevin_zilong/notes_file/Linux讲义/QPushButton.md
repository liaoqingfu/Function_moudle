
<!-- toc orderedList:0 -->

- [QPushButton Class](#qpushbutton-class)
	- [属性](#属性)
	- [公共接口函数原型](#公共接口函数原型)
	- [可以重载的公共接口函数](#可以重载的公共接口函数)
	- [公共槽函数](#公共槽函数)
	- [保护函数](#保护函数)
	- [保护的虚函数](#保护的虚函数)
	- [额外继承的成员](#额外继承的成员)
- [详细内容](#详细内容)
	- [详细描述](#详细描述)
	- [属性](#属性-1)
		- [audoDefault : bool](#audodefault-bool)
		- [default: bool](#default-bool)

<!-- tocstop -->

# QPushButton Class

QPushButton提供一个命令按钮。

父类：QAbstractButton
子类：QCommandLinkButton

## 属性
- autoDefault: bool
- default: bool
- flat: bool
- 11个属性继承自QAbstractButton
- 58个属性继承自QWidget
- 1个属性继承自QObject

## 公共接口函数原型

| 返回类型     | 函数原型     |
| :------------- | :------------- |
|       | QPushButton(QWidget *parent = Q_NULLPTR)       |
|       | QPushButton(const QString &text, QWidget *parent = Q_NULLPTR)|
|       | QPushButton(const QIcon &icon, const QString &text, QWidget *parent = Q_NULLPTR)|
|   bool    | ~QPushButton()|
|   bool    | autoDefault() const|
|   bool    | isDefault() const|
|   bool    |  isFlat() const
|    QMenu*   | menu() const|
|   void    | setAutoDefault(bool)|
|      void |setDefault(bool) |
|      void | setFlat(bool)|
|       void| setMenu(QMenu *menu)|


## 可以重载的公共接口函数

virtual QSize minimumSizeHint() const
virtual QSize sizeHint() const

- 21 public functions inherited from QAbstractButton
- 211 public functions inherited from QWidget
- 31 public functions inherited from QObject
- 14 public functions inherited from QPaintDevice

## 公共槽函数
void showMenu()

- 5 public slots inherited from QAbstractButton
- 19 public slots inherited from QWidget
- 1 public slot inherited from QObject
## 保护函数
void initStyleOption(QStyleOptionButton *option) const

## 保护的虚函数


virtual bool event(QEvent *e)
virtual void focusInEvent(QFocusEvent *e)
virtual void focusOutEvent(QFocusEvent *e)
virtual bool hitButton(const QPoint &pos) const
virtual void keyPressEvent(QKeyEvent *e)
virtual void paintEvent(QPaintEvent *)

- 14 protected functions inherited from QAbstractButton
- 35 protected functions inherited from QWidget
- 9 protected functions inherited from QObject
- 1 protected function inherited from QPaintDevice


## 额外继承的成员

- 4 signals inherited from QAbstractButton
- 3 signals inherited from QWidget
- 2 signals inherited from QObject
- 1 public variable inherited from QObject
- 5 static public members inherited from QWidget
- 10 static public members inherited from QObject
- 1 protected slot inherited from QWidget
- 2 protected variables inherited from QObject
- 1 protected type inherited from QPaintDevice

# 详细内容
##  详细描述
QPushButton窗口提供一个命令按钮。按钮或者说命令按钮，可能是在任何图形界面中，都是最常用的窗口部件。按（点）一个按钮，发送命令给计算机，或者回应计算机。典型的按钮是OK，应用，取消，关闭，是，否，帮助。

命令按钮一般是一个矩形，一般带有文字来描述它的动作。可以设置一个快捷键通过&符号。比如：
```
 QPushButton *button = new QPushButton("&Download", this);
```

在这里例子里，快捷键是ALT+D。可以通过QShortcut文件来查看更加详细的内容，如果你想在按钮上显示&符号，应该用'&&'。

按钮显示一个文本标签，并且可以显示一个小图标。文本和图标可以通过构造函数来设置，也可通过setText()和setIcon()来改变。如果被禁用，那么这个文本和图标的显示，被修改成禁用的风格（禁用的风格的样子，按照该平台GUI风格）

一个按钮被鼠标激活时、或者空格激活、或者被快捷键激活，发送clicked()信号。如果需要相应该按钮，应该使用槽链接该信号。除了clicked()信号，按钮也提供不常用的信号，比如pressed()和released()。

Command buttons in dialogs are by default auto-default buttons,比如，按钮得到键盘的焦点时，会自动变成默认按钮。默认按钮可以有Enter和Return按钮激活。可以通过修改setAutoDefault()。可以通过setAutoDefault来修改audoDefault功能。注意：audo-default的button，由于要显示更多的内容来表示它是auto-default的，所以要预留一些小空间。

Being so central, the button widget has grown to accommodate a great many variations in the past decade. The Microsoft style guide now shows about ten different states of Windows push buttons and the text implies that there are dozens more when all the combinations of features are taken into consideration.

The most important modes or states are:
- Available or not (grayed out, disabled).
- Standard push button, toggling push button or menu button.
- On or off (only for toggling push buttons).
- Default or normal. The default button in a dialog can generally be "clicked" using the Enter or Return key.
- Auto-repeat or not.
- Pressed down or not.

As a general rule, use a push button when the application or dialog window performs an action when the user clicks on it (such as Apply, Cancel, Close and Help) and when the widget is supposed to have a wide, rectangular shape with a text label.

 Small, typically square buttons that change the state of the window rather than performing an action (such as the buttons in the top-right corner of the QFileDialog) are not command buttons, but tool buttons. Qt provides a special class (QToolButton) for these buttons.


 If you need toggle behavior (see setCheckable()) or a button that auto-repeats the activation signal when being pushed down like the arrows in a scroll bar (see setAutoRepeat()), a command button is probably not what you want. When in doubt, use a tool button.


 命令按钮的一种变化是菜单按钮，它不单提供一个命令，而是多个。通过点击它弹出菜单选项。通过setMenu来关联一个菜单到按钮。


 ## 属性
 ### audoDefault : bool
 表示该按钮是否autoDefault属性，通过setAutoDefault可以设置，通过autoDefault可以获取，当父对象是QDialog，按钮audoDefault默认是true，如果父对象是其他，audoDefault默认是false。

 ### default: bool
 这个属性描述default属性，Default属性和autoDefault属性决定了用户按Return的行为。
