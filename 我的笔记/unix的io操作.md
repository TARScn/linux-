## uinx的io操作

### 目录
- [IO重定向和管道](#IO重定向和管道)
- [常用函数](#常用函数)
### IO重定向和管道

__文件描述符__:

进程读取和写入（IO操作）的入口叫做文件描述符。类似于打电话的操作，用户要和某某企业沟通，首先要找到该公司的电话号码，拨号接通后开始交换信息。

其中文件描述符的作用相当于电话号码。

<img src="https://github.com/TARScn/unix-linux-Programming-MyReadingNote/blob/18481313eb26575f2eac12324c67220efeb02c75/readme_picture/io_file_dsrp.png" alt="文件描述符" width="50%">

而改变进程读取文件描述符的对象则称作IO重定向，即打电话还是那个号码，但是接听对象改变了。

__管道__:

管道可以认为是两个文件描述符，其中一个用来读，一个用来写。利用管道的特性，两个进程可以使用管道通信.

<img src="https://github.com/TARScn/unix-linux-Programming-MyReadingNote/blob/268b6b3179137fddfd0de8b33f173f500b0633db/readme_picture/pipe.png" alt="管道" width="50%">

### 常用函数

__dup__:

dup() 函数会创建一个与指定文件描述符相同的新文件描述符，默认使用最低文件描述符进行复制。

`int dup(int oldfd);`

_参数_：

oldfd：要复制的已有文件描述符。

_返回值_：

成功时，返回新的文件描述符（一个非负整数）。

失败时，返回 -1 并设置 errno 以指示错误原因

__dup2__:

与dup()相比，dup2()可以指定复制使用的文件描述符。

`int dup2(int oldfd, int newfd);`

_参数_：

oldfd：要复制的已有文件描述符。

newfd：目标文件描述符，新描述符的值由调用者指定。

_返回值_：

成功时，返回 newfd。

失败时，返回 -1 并设置 errno 以指示错误原因。

<img src="https://github.com/TARScn/unix-linux-Programming-MyReadingNote/blob/4ea8ff6babc23a11815e0f0f17090da4b3741a1f/readme_picture/dup.png" alt="dup" width="50%">

__pipe()__:

创建一个管道，
它允许两个相关联的进程（通常是父子进程）通过一个单向的数据通道进行通信。pipe() 在创建管道、实现数据流转以及进行命令连接（如在 shell 中使用管道符 |）时非常有用。

`int pipe(int pipefd[2]);`

_参数_：

pipefd[0]：管道的读端。

pipefd[1]：管道的写端。

_返回值_：

成功时，返回 0。

失败时，返回 -1 并设置 errno 以指示错误原因。
