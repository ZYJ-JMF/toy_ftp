## toy_ftp(with gui client)

### ftp服务端
使用方法：

- 在server目录下运行命令make
- 运行命令./server,可选参数-port(改变端口,默认为21), -root(当前工作目录，默认为/tmp) 注：在mac下使用21端口需要在sudo模式下

#### 代码结构
自上而下为**server-handler-util**三层架构，util中实现字符串处理函数，封装socket相关内容。handler实现命令处理。server调用相关handler函数处理命令。

服务多个client通过多进程实现，对每个连接的client，运行fork()，子进程处理client请求，父进程继续监听新的client连接。
#### 实现命令
##### USER
只接受参数anonymous，返回码331,其他参数均会返回错误。
##### PASS
接受任何参数，返回码230。
##### SYST
无参数，返回码215.
##### TYPE
接受参数I/A,返回固定字符串"200 Type set to I/A.\r\n"其他参数均会返回错误。
##### MKD,CMD,RMD
创建、更换工作目录，删除空目录。
##### PORT
参数为h1,h2,h3,h4,p1,p2形式，服务器将客户端端口信息记录下来，返回码200。
##### PASV
无参数，服务器随机打开一个端口并返回给客户端，返回码227。
##### LIST
参数为文件（夹）相对路径或无参数，参数为文件名，则返回文件信息;参数为文件夹名，则返回文件夹下文件信息；无参数则返回当前工作路径下文件信息。返回信息符合[/bin/ls格式](http://cr.yp.to/ftp/list/binls.html)。
##### RETR
参数为文件相对路径，将该文件发送给客户端。
##### STOR
参数为文件名，将客户端发送的文件存储在工作目录下。
##### QUIT/ABOR
无参数，关闭ftp全部连接(包括command和data)，返回码221.
##### PWD(Optional)
无参数，打印当前工作目录的绝对路径，返回码257.
##### NLST(Optional)
与LIST参数相同，返回文件简要信息（文件名）。

### ftp客户端
#### 功能简介
命令行客户端: 实现了全部命令，并在原有ftp命令的基础上进行了封装。

图形界面客户端: 使用QT完成，可以进行上传、下载、浏览文件夹等操作。
#### 实现命令
##### 必做命令
对应server：USER、PASS、SYST、TYPE、MKD、RMD、CWD、PORT、PASV、LIST、RETR、STOR、QUIT、ABOR、PWD、NLST。在客户端命令行中可以直接发送这些命令。

##### 封装命令
注：出于安全性考虑，封装命令全部使用pasv模式。
##### browse
浏览服务器上文件：基于NLST。无参数时显示当前工作目录下的文件和文件夹。
##### download
从客户端下载文件：基于RETR。
##### upload
向服务器上传文件：基于STOR。
##### help
查看ftp客户端使用说明。

#### 图形界面
- 登录
- 查看文件列表
- 打开子目录/回退
- 上传/下载
- 创建／删除服务器上文件夹

