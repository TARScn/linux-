## uinx的io操作

### 目录

### IO重定向和管道

__文件描述符__:

进程读取和写入（IO操作）的入口叫做文件描述符。类似于打电话的操作，用户要和某某企业沟通，首先要找到该公司的电话号码，拨号接通后开始交换信息。

其中文件描述符的作用相当于电话号码。

<img src="https://github.com/TARScn/unix-linux-Programming-MyReadingNote/blob/18481313eb26575f2eac12324c67220efeb02c75/readme_picture/io_file_dsrp.png" alt="文件描述符" width="50%">

而改变进程读取文件描述符的对象则称作IO重定向，即打电话还是那个号码，但是接听对象改变了。

__管道__:

管道可以认为是两个文件描述符，其中一个用来读，一个用来写。利用管道的特性，两个进程可以使用管道通信.

<img src="https://github.com/TARScn/unix-linux-Programming-MyReadingNote/blob/268b6b3179137fddfd0de8b33f173f500b0633db/readme_picture/pipe.png" alt="管道" width="50%">
