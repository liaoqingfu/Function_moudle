linux下安装boost
（一）. 在Terminal中分别执行以下3步安装boost：

1. 下载后进入压缩包所在目录解压: sudo tar -zxvf boost_1_52_0.tar.gz

2. 进入./boost_1_52_0，运行: sudo ./bootstrap.sh --prefix=/usr

　　/usr 可替换为任意路径　　(事前运行./bootstrap.sh --help 看下参数说明）

3. sudo ./b2 install


linux下如何安装libcurl
./configure --prefix=/usr
然后输入make; make install