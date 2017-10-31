## ftp实验报告
软件52班 张迁瑞 2015013226

## ftp服务端
### 功能简介
C语言实现并发ftp服务端，基本遵照RFC中约定进行实现，部分命令有所简化。

使用方法：

- 在server目录下运行make
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
只接受参数I,返回固定字符串"200 Type set to I.\r\n"其他参数均会返回错误。
#### MKD
参数为目录名称，在服务器工作目录下新建目录，正确建立返回码250，错误返回550.

注:出于安全考虑，MKD,RMD,CWD命令均不支持含有".."的参数。
#### RMD
参数为目录名称，在服务器工作目录下删除空目录，正确删除返回码250，错误返回550.
#### CWD
参数为目录名称，更换当前工作目录，正确返回码250，错误返回550.
#### PWD(Optional)
无参数，打印当前工作目录的绝对路径，返回码257.
#### PORT
参数为h1,h2,h3,h4,p1,p2形式，服务器将客户端端口信息记录下来。
#### PASV
无参数，服务器随机打开一个端口并返回给客户端。
#### LIST
#### RETR
参数为文件相对路径，将该文件发送给客户端。
#### STOR
参数为文件名，将客户端发送的文件存储在工作目录下。
#### QUIT/ABOR
无参数，关闭ftp全部连接(包括command和data).

## ftp客户端