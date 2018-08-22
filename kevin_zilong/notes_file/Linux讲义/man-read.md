READ(2)                                              Linux Programmer's Manual                                             READ(2)
## READ(2) Linux编程手册READ(2)

NAME
### 名字
       read - read from a file descriptor

       read - 从一个文件描述符读

SYNOPSIS
### 该要
       #include <unistd.h>

       ssize_t read(int fd, void *buf, size_t count);

DESCRIPTION
### 描述
       read() attempts to read up to count bytes from file descriptor fd into the buffer starting at buf.

       read()函数尝试从文件描述符fd中读最多count字节内容到buf所指内存中。

       On  files that support seeking, the read operation commences at the current file offset, and the file offset is incremented
       by the number of bytes read.  If the current file offset is at or past the end of file,  no  bytes  are  read,  and  read()
       returns zero.

       如果文件支持seeking（定位），那么读操作从当前的偏移值开始读，并且读操作完成之后，文件便宜值会增长读的长度。

       If  count is zero, read() may detect the errors described below.  In the absence of any errors, or if read() does not check
       for errors, a read() with a count of 0 returns zero and has no other effects.

       如果读函数的count参数是0，读函数read()会检测错误。如果文件描述符没有错误，那么返回0，并且没有其他作用。

       If count is greater than SSIZE_MAX, the result is unspecified.

       如果count参数大于SSIZE_MAX，那么结果是未定义的。

RETURN VALUE
### 返回值
       On success, the number of bytes read is returned (zero indicates end of file), and the file position is  advanced  by  this number.   It  is  not  an  error  if this number is smaller than the number of bytes requested; this may happen for example because fewer bytes are actually available right now (maybe because we were close to end-of-file, or because we are reading from  a  pipe,  or from a terminal), or because read() was interrupted by a signal.  On error, -1 is returned, and errno is
       set appropriately.  In this case it is left unspecified whether the file position (if any) changes.

       成功时，返回读到的字节数（0表示读到文件末尾），并且文件指针增加读到数量。返回值如果小于参数count，并不代表错误；在可读字节数，比参数count小的情况下，这种情况可能发生。（可读数量小的原因：文件当前指针到文件末尾长度，小于count；从一个管道读；从终端读），还有一种情况可能返回值小于参数count：当读操作被终端打断。
       错误时，返回-1，相应错误码记录在errno。

ERRORS
### 错误
       EAGAIN The file descriptor fd refers to a file other than a socket and has been marked nonblocking  (O_NONBLOCK),  and  the read would block.

       EAGAIN 文件描述符fd所指的文件是一个socket，并且标记了非阻塞，并且此时读应该阻塞。

       EAGAIN or EWOULDBLOCK
              The  file  descriptor  fd refers to a socket and has been marked nonblocking (O_NONBLOCK), and the read would block.
              POSIX.1-2001 allows either error to be returned for this case, and does not require these constants to have the same
              value, so a portable application should check for both possibilities.

              文件描述符fd是一个socket，并且标记非阻塞，并且此时如果是fd是阻塞时，应该是阻塞状态。

              POSIX.1-2001 允许这两个错误码的任意一个，从可移植方面考虑，应该检查两个返回值。

              int ret = read(...)
              if(ret == -1)
              {
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                  continue;
                }
              }

       EBADF  fd is not a valid file descriptor or is not open for reading.
       EBADF  fd不是一个可用的文件描述符，或者没有读方式打开。

         fd = open("a.txt", O_WRONLY);
         read(fd)

       EFAULT buf is outside your accessible address space.
       EFAULT buf不是可访问的内存
          read(fd, NULL, 300);

       EINTR  The call was interrupted by a signal before any data was read; see signal(7).
       EINTR read调用被终端打断，并且读还没有读到任何数据。

       EINVAL fd  is  attached  to  an  object which is unsuitable for reading; or the file was opened with the O_DIRECT flag, and
              either the address specified in buf, the value specified in count, or  the  current  file  offset  is  not  suitably
              aligned.

              无效值(invalid value)：
              尝试读一个不适合读的文件描述符（比如TCP服务器socket，还有epollfd）；
              文件以O_DIRECT方式打开，并且the address specified in buf, the value specified in count，或者当前文件偏移值，没有对齐。



       EINVAL fd  was created via a call to timerfd_create(2) and the wrong size buffer was given to read(); see timerfd_create(2)
              for further information.

              fd是通过timerfd_create创建，并且read（）函数的count参数，不合适。

       EIO    I/O error.  This will happen for example when the process is in a background process group, tries to read  from  its
              controlling  terminal,  and either it is ignoring or blocking SIGTTIN or its process group is orphaned.  It may also
              occur when there is a low-level I/O error while reading from a disk or tape.

              EIO表示IO错误，后台进程读终端。后台进程读取终端，会产生SIGTTIN信号。所以要产生该错误码，就应该忽略SIGTTIN或者阻塞该SIGTTIN信号，或者是该进程组是孤儿进程组。也可能在读写底层磁盘或者磁带时，产生IO错误。

       EISDIR fd refers to a directory.
       EISDIR fd指向目录。

       int fd = open(dirpath)

       Other errors may occur, depending on the object connected to fd.  POSIX allows a read() that is interrupted  after  reading
       some data to return -1 (with errno set to EINTR) or to return the number of bytes already read.

       有可能有其他错误发生，这个和fd描述的对象有关系。POSIX运行read()函数，在读取一些数据之后，同时被终端打断时，返回-1(错误码为EINTR)或者返回已经读的字节数。

CONFORMING TO
### 遵守标准
       SVr4, 4.3BSD, POSIX.1-2001.

NOTES
### 注意事项
       On  NFS  filesystems, reading small amounts of data will update the timestamp only the first time, subsequent calls may not
       do so.  This is caused by client side attribute caching, because most if not all NFS  clients  leave  st_atime  (last  file
       access  time) updates to the server and client side reads satisfied from the client's cache will not cause st_atime updates
       on the server as there are no server side reads.  UNIX semantics  can  be  obtained  by  disabling  client  side  attribute
       caching, but in most situations this will substantially increase server load and decrease performance.

       在NFS(网络文件系统)文件系统，第一次读小量数据会更新文件时间戳（访问时间），后续的调用不会更新。原因是大部分的网络文件系统，在客户端有缓存，后续的读，已经在缓存中直接读取，没有真正的取访问服务器端真实文件。

SEE ALSO
### 相关阅读
       close(2), fcntl(2), ioctl(2), lseek(2), open(2), pread(2), readdir(2), readlink(2), readv(2), select(2), write(2), fread(3)

       COLOPHON
              This  page  is  part  of  release 3.54 of the Linux man-pages project.  A description of the project, and information about
              reporting bugs, can be found at http://www.kernel.org/doc/man-pages/.
