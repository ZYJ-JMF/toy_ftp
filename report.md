## ftp实验报告
软件52班 张迁瑞 2015013226

## ftp服务端
### 功能简介
C语言实现并发ftp服务端，基本遵照RFC中约定进行实现，部分命令有所简化。

使用方法：

- 在server目录下运行命令make
- 运行命令./server,可选参数-p(改变端口,默认为21), -r(当前工作目录，默认为/tmp)

### 代码结构
自上而下为**server-handler-util**三层架构，util中实现字符串处理函数，封装socket相关内容。handler实现命令处理。server调用相关handler函数处理命令。

common中定义了服务器的返回字符串和相关参数。

服务多个client通过多进程实现，对每个连接的client，运行fork()，子进程处理client请求，父进程继续监听新的client连接。
### 实现命令
#### USER
只接受参数anonymous，返回字符串"331 Guest login ok, send your e-mail address as password.\r\n",其他参数均会返回错误。
#### PASS
接受任何参数，返回字符串"230 Guest login ok, access restrictions apply.\r\n"。
#### SYST
无参数，返回固定字符串"215 UNIX TYPE: L8\r\n"(由于autograde.py的影响，句末没有句号)。
#### TYPE
接受参数I/A,返回固定字符串"200 Type set to I/A.\r\n"其他参数均会返回错误。
#### MKD
参数为目录名称，在服务器工作目录下新建目录，正确建立返回码250，错误返回550.

注:出于安全考虑，MKD,RMD,CWD命令均不支持含有".."的参数。
#### RMD
参数为目录名称，在服务器工作目录下删除空目录，正确删除返回码250，错误返回550.
#### CWD
参数为目录名称，更换当前工作目录，正确返回码250，错误返回550.
#### PORT
参数为h1,h2,h3,h4,p1,p2形式，服务器将客户端端口信息记录下来，返回码200。
#### PASV
无参数，服务器随机打开一个端口并返回给客户端，返回码227。
#### LIST
参数为文件（夹）相对路径或无参数，参数为文件名，则返回文件信息;参数为文件夹名，则返回文件夹下文件信息；无参数则返回当前工作路径下文件信息。返回信息符合[/bin/ls格式](http://cr.yp.to/ftp/list/binls.html)。
#### RETR
参数为文件相对路径，将该文件发送给客户端。
#### STOR
参数为文件名，将客户端发送的文件存储在工作目录下。
#### QUIT/ABOR
无参数，关闭ftp全部连接(包括command和data)，返回码221.
#### PWD(Optional)
无参数，打印当前工作目录的绝对路径，返回码257.
#### NLST(Optional)
与LIST参数相同，返回文件简要信息（文件名）。

### Optional Question
##### 如何在不阻塞服务器的情况下传输大文件?
在ftp服务端本身多进程的基础上，可以再使用新的进程来处理文件传输。每次收到retr或stor请求时，服务器利用fork()新开一个进程处理文件传输，父进程继续接受命令。在大文件传输完成或失败后，子进程向客户端发送信息并结束。
## ftp客户端
### 功能简介
实现命令行界面的FTP客户端，可以实现连接ftp服务器，文件上传下载等功能。在原有ftp命令的基础上进行了简单的封装。
### 代码结构
客户端为**client-handler-util**三层架构，handler由send和handle两部分函数组成，分别对应发送相关命令和对结果的处理。util实现功能函数。
### 实现命令
#### 必做命令
对应server相关功能：USER、PASS、SYST、TYPE、MKD、RMD、CWD、PORT、PASV、LIST、RETR、STOR、QUIT、ABOR。在客户端命令行中可以直接发送这些命令。
#### 选做命令
PWD、NLST:与服务器部分选做命令对应。
#### 封装命令
注：出于安全性考虑，封装命令全部使用pasv。
##### browse
浏览服务器上文件：基于NLST。
##### download
从客户端下载文件：基于RETR。
##### upload
向服务器上传文件：基于STOR。
##### help
查看ftp客户端使用说明。
