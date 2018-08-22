1. 版本管理git作为个人开发使用：
	1. 安装命令：sudo apt-get install git
	2. 配置本机的用户名和Email地址
		git config --global user.name "Your Name"
		git config --global user.email "email@example.com"
	3. 创建需要使用git的目录，又叫仓库(repository)，这个目录里面的所有文件都可以被Git管理起来，每个文件的修改、删除都能被跟踪。
	我的仓库在：cd /home/zilong/test/
				mkdir git  //创建了一个仓库文件夹
	4. 使用init命令将当前目录初始化为Git仓库
		命令：git init  初始化git仓库，该命令在当前目录下，创建一个git仓库。
		Initialized empty Git repository in /home/zilong/test/git/.git/
		(显示信息意思为：初始化了一个空的Git仓库，git目录下多了一个.git目录，时用来管理版本库的)
		
	5. 添加文件，将修改的代码加入到仓库
		 git add .        添加所有的文件、文件夹
		 git add <file>   添加指定名称的文件,<>内部写文件全称
		 注：如果文件没有做出任何修改，则默认不会添加任何文件
		 git add 09.c
		 
	6. 提交文件
		git commit –m “commit info”      提交本次事务，即将add的文件提交到git仓库，引号内部表示本次提交的提示信息
		
	7. 提交内容撤销操作
			通过git revert进行撤销
		
		
	8. 查询提交状态
		git status
		 显示提交的状态：已经添加，等待提交事务的文件(绿色字体表示)；已经改变但是没有添加(not staged)的文件(红色字体表示)；
	
	9. 查询该文件和git仓库中的文件的区别，即做了什么修改

		git diff <文件全称>      如果已经add了，就打印不出有什么修改了，这一步骤应该在add之前，即添加之前可以用来看看做了什么修改。
		
	10. 打印历史记录
			命令：git log
			Commit xxx              commit id 版本号
			Author:xxx<xxx@xxx.com> 提交人和邮箱
			Date：xxx                提交的时间
				XXXXXXXXXXXXXX      提交的信息(所以说，提交信息很重要！！！)
			$ cat <文件全名称>      显示整个文件的内容
				
	11. 版本回退
		命令：git reset --hard head^
		在Git中，HEAD表示当前版本，就是最新提交的版本，即使用git log打印出来的位于第一位的版本，上一个版本就是HEAD^，上上个版本就是HEAD^^，  
		当前向上100个可以写成HEAD~100。当然，还有一种方式就是直接使用commit id来代替HEAD^，比如版本号是cadab353589f3eef075817b890dafe8b722d802b，  
		那么就可以直接使用命令：  
		$ git reset --hard cadab353589f            使用前几位表示即可，git会自动查找  
		注：版本回退以后，使用git log打印的历史记录都是回退版本之前的数据，之后的都没有了，不过放心，git总有后悔药可以吃哒~  
		1.如果命令行窗口没有关闭，直接去前面找commit id即可；  
		2.如果命令行窗口关闭了，或者第二天后悔了，可以进入到该目录下，使用git reflog命令来查看以前的每一次命令，可以获得每次提交的commit id，  
		就可以版本回退了。  
		$ git reflog                           可以查看命令历史，包含提交的commit id  
		
	12. 可以通过分支来进行大版本管理
		git branch查看当前仓库的分支
		git branch {branchname} 在当前分支的状态下，创建分支
		git checkout {branchname}切换分支
		git branch -d {branchname}删除分支
		git branch -D {branchname} 强制删除有代码修改的分支，慎重使用

		
	13. github
		注册账号（邮箱）
		设置SSH-key
			ssh-keygen -t rsa -b 4096 -C "注册时的邮箱"

			eval "$(ssh-agent -s)"

			ssh-add ~/.ssh/id_rsa

			将公钥($HOME/.ssh/rsa.pub)设置到你的github账号中。
		在github创建仓库
		git clone git@github.com:suibian123/sb.git
		git clone：从远程拷贝仓库
		git push origin master:master：把当前仓库的代码提交到远程仓库，其中origin是远程仓库的名称，前面的master表示本地仓库的分支，后面的master表示远程仓库的分支
	
	14. 合作开发

		git init --bare 代码库名字：在服务器端创建代码库
		git clone 账户@服务器ip:/代码库路径：客户端clone服务器代码
		git push 远程服务器名字 本地分支:远程分支：将本地库代码提交到名字为origin的服务器上，提交代码为本地分支，提交到远程的远程分支
		git pull 远程服务器名字 分支名字：将服务器的分支，拉取到本地
			问题：冲突问题
			当多人同时编辑一个文件的相同位置，会导致冲突			为了避免冲突，尽量不让多人编辑同一个文件，比如按照模块分目录，每个人编写一个目录。
			但是在代码维护阶段，无法避免这种事情发生。
		git merge 分支号，将参数指定的分支，合并到当前分支，可能会有冲突发生。
		git fetch，获取远程服务器的分支信息
		
15. git命令总结
    git init：可以用于服务器创建git仓库，也可以客户端本地创建git仓库。git init --bare服务器创建仓库。

    git add：将修改的代码纳入管理，最常用的是git add -A。

    git commit：从编辑区提交代码到仓库

    git push：从本地仓库提交到远程仓库

    git pull：从远程仓库同步到本地仓库

    git branch：显示分支

    git branch -a：显示分支，包括远程分支

    git branch 分支名字：创建分支

    git checkout 分支名字：切换分支，如果该名字在本地分支没有，而在远程分支有，那么远程分支同步到本地分支

    git branch -d 分支名字：删除分支，如果该分支的代码没有被保存，那么删除会失败

    git branch -D 分支名字：删除分支，无论该分支是否有代码没有被同步

    git push orign :分支名称：删除分支，删除的是远程分支，注意，：前，不需要加任何东西

    git remote add 服务器名称 服务器地址：增加远程服务器

    git remote remove 服务器名称：删除远程服务器

    git merge 分支名称：合并两个分支代码，将参数所指的分支代码合并到当前分支

    git fetch：获取服务器信息，主要是分支信息

    git status：查看那些代码有变化

    git log：查看git提交日志

    git diff：比较两个分支的差异

			
			
		
		
