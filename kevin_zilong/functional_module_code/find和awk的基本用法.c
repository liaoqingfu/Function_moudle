	
sed:

	格式：  1. sed  参数  '脚本语句(/pattern/action)' 待操作文件

			sed -i '4a xxxxxxxxxxxxxxxxxxxxxxxxx' case.sh

			    -n  静默

			    -r  扩展正则

		2. sed  参数 -f‘脚本文件’待操作的文件


awk：

	格式：	1. awk 参数 ‘脚本语句(/pattern/{actions})’ 待操作文件

		2. awk 参数 -f‘脚本文件’ 待操作文件

		printf("%s\t%s\n", $11, $2);	

		print $11 $2

		BEGIN:	pattern 未匹配文件之前，执行某些操作。
			
			awk 'BEGIN {FS=":"} {print $7}' /etc/passwd

			awk -F: {print $7} /etc/passwd

		END:	pattern 未匹配文件结束，执行某些操作。
		
			ps aux | awk '$2>6000 && $2<=7000 {num=num+1} END {print num}' 

			ps aux | awk  'BEGIN {FS=" "} $2>6000 && $2<=7000 {x=x+1} END {print x}' 

			ps aux | awk -F" " '$2>6000 && $2<=7000 {x=x+1} END {print x}'


C语言中使用正则表达式：


	int regcomp (regex_t *compiled, const char *pattern, int cflags)

		参数1：  结构体  --- 编译(字符串---结构体)存储正则表达式

		参数2：	 正则表达式串。

		参数3：	 标志位：  

				1. 扩展正则：REG_EXTENDED 

				2. 忽略大小写：REG_ICASE 

				3. 识别换行符：REG_NEWLINE   ---> ^ $ 

				4. 指定是否存储匹配结果：REG_NOSUB  --->  regexec函数参3取0值，并且参4取NULL


		返回值：成功0；失败错误号


	int regexec (regex_t *compiled, char *string, size_t nmatch, regmatch_t matchptr[], int eflags)

		参数1： regcomp编译后传出的 结构体

		参数2： 待用正则表达式 进行匹配的字符串

		参数3： 数组大小

		参数4： 用来存储返回结果的数组

		参数5： 标志位：

				1. REG_NOTBOL 使^无效

				2. REG_NOTEOL 使$无效

		返回值：成功0；失败错误号


	void regfree (regex_t *compiled)  ---> 释放结构体。


	size_t regerror (int errcode, regex_t *compiled, char *buffer, size_t length)  --> 针对正则表达式的错误处理函数。


grep:

	 grep -R "struct task_struct {"  /usr/src/ -n

		
find:

	1. -name

		find ./ -name *.mp3

	2. -type
		
		find ./ -type f/d/p/c/b/s/l

	3. -size
		
		默认单位：512B-->0.5K-->一个扇区大小

		-M：不能m  find /home/itcast -size +3M -size -7M

		-k：不能K  find /home/itcast -size +47k -size -89k

		
	4. -maxdepth：

		find ./ -maxdepth 2 -type d 

	5. -exec、-ok

		find ./ -maxdepth 1 -name "*.sh" -exec rm -r {} \;

		find ./ -maxdepth 1 -name "*.sh" -ok rm -r {} \;

	7. xargs

		find ./ -maxdepth 1 -type f -print0 | xargs -0 ls -lh

	6. -print0


	8. -atime  -- 访问时间
	   -amin

	   -mtime  -- 文件内容修改时间
	   -mmin

	   -ctime  -- 文件属性修改时间
	   -cmin

		 
		find ./ -name "syslog.*.gz" -mtime -5 -exec ls -lh {} \;
		

