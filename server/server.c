#include"util_server.h"
#include"reply.h"
#include"configs.h"


//从命令行获取参数，错误返回-1，正确返回1
//TODO:非法命令的处理
int getParamsFromCli(int argc, char**argv)
{
	//接收命令行参数
	struct option opts[] = {
		{"port", required_argument, 0, VALUE_PORT},
		{"root", required_argument, 0, VALUE_ROOT},
		{0, 0, 0, 0}};
	const char* optstring = "r:p:";
	int index = 0;
	int opt = -1;
	while((opt = getopt_long_only(argc, argv, optstring, opts, &index)) != -1)
	{
		switch(opt)
		{
			case VALUE_PORT:
				printf("port\n");
				port = atoi(optarg);
				break;
			case VALUE_ROOT:
				printf("root\n");
				strcpy(rootPath, optarg);
				break;
			case '?':
				printf("Param error.\n");
				return -1;
				break;
			default:
				printf("Param error.\n");
				return -1;
				break;
		}
	}
	return 1;
}
//登录成功返回1，失败返回0
//TODO:建立用户表
int handleUserRequest(int connfd, char* param)
{
	if(strcmp(param, "ANONYMOUS") == 0)
    {
	   	if(sendMsg(connfd, requirePassMsg) == -1)
	   		return -1;
	   	return 1;
	}
	else
	{
	    sendMsg(connfd, unknownUserError);
	    return -1;
	}
}
//TODO:检查密码格式
int handlePassRequest(int connfd, char* param)
{
	if(sendMsg(connfd, loginSuccessMsg) == -1)
		return -1;
	return 1;
}

int handleSystRequest(int connfd, char* param)
{
	if(sendMsg(connfd, sysStatusMsg))
		return -1;
	return 1;
}

int handleTypeRequest(int connfd, char* param)
{
	if(strcmp(param, "I") == 0)
	{
		sendMsg(connfd, typeMsg);
		return -1;
	}
	else if(strcmp(param, "") == 0)
	{
		sendMsg(connfd, syntaxError);
		return -1;
	}
	else
		sendMsg(connfd, typeError);
	return 1;
}

int handlePortRequest(int connfd, char* param, char* portModeInfo)
{
	if(checkPortParam(param) == -1)
	{
		sendMsg(connfd, typeError);
		return -1;
	}
	else
	{
		strcpy(portModeInfo, param);
		if(sendMsg(connfd, portMsg) == -1)
			return -1;
	}
	return 1;
}

int handlePasvRequest(int connfd, char* param, int* psockfd)
{
	int pasvPort = 30000;
	*psockfd = createSocket();
	while(1)
	{
		if(bindSocketToServer(*psockfd, pasvPort) > 0)
		{
			setSocketOption(*psockfd);
			startSocketListening(*psockfd, 10);
			break;
		}
		else
			pasvPort += 1;
		if(pasvPort >= 65535)
		{
			sendMsg(connfd, noAvailablePortError);
			return -1;
		}
	}
	printf("Listening port: %d\n", pasvPort);
	char serverIpWithComma[80];
	strcpy(serverIpWithComma, serverIp);
	strreplace(serverIpWithComma, '.', ',');
	int p1 = pasvPort / 256;
	int p2 = pasvPort % 256;
	char leftBracket[2] = "(";
	char rightBracket[2] = ")";
	char comma[2] = ",";
	char endOfLine[10] = "\r\n";
	char p1c[10];
	char p2c[10];
	intToString(p1, p1c);
	intToString(p2, p2c);
	char pasvMsg[200] = "\0";
	strcat(pasvMsg, pasvMsgPart);
	strcat(pasvMsg, leftBracket);
	strcat(pasvMsg, serverIpWithComma);
	strcat(pasvMsg, comma);
	strcat(pasvMsg, p1c);
	strcat(pasvMsg, comma);
	strcat(pasvMsg, p2c);
	strcat(pasvMsg, rightBracket);
	strcat(pasvMsg, endOfLine);
	if(sendMsg(connfd, pasvMsg) == -1)
		return -1;
	return 1;
}
//建立新的socket，返回该socket的描述符
int createSocket()
{
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
	{
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	printf("socketfd is: %d\n", sockfd);
	return sockfd;
}

//将socket绑定到服务器指定端口
int bindSocketToServer(int sockfd, int port)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
	{
		printf("Error bind(): %s(%d)\n", strerror(errno), errno);
		return -1;	
	}
	return 1;
}
//设置socket为可立即复用
int setSocketOption(int sockfd)
{
	int reuse = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse) == -1)
	{
		printf("Error set(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}

//10是最大可以排队连接的个数，在这里（并发情况下）应该无意义
int startSocketListening(int sockfd, int maxQueueLength)
{
	if (listen(sockfd, maxQueueLength) == -1) 
	{
		printf("Error listen(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}
void serveOneClient(int connfd)
{
	char sentence[8192];
	char command[100];
	char param[100]; //param of command
	int hasLogedin = -1; //是否登录，登录为1，未登录为-1
	int hasInputPass = -1; //是否给出密码
	int isPortMode = -1;  //是否为端口模式
	int isPasvMode = -1;  //是否为passive模式
	char portModeInfo[8192];   //端口模式传送文件所用信息
	char pasvModeInfo[8192];   //passive模式传送文件所用信息

	int pasvPort;
	int pasvListenfd = -1;  //pasv中用于传送文件的socket
	int fileConnfd = -1;    //连接客户端的socket

	sendMsg(connfd, welcomeMsg);
	printf("Welcome message sent\n");
	while(1)
	{
		command[0] = '\0';
		param[0] ='\0';
		if(getSentence(connfd, sentence) < 0)
			continue;
        convertToUpperCase(sentence);
        removeLineFeed(sentence);
        printf("sentence: %s\n", sentence);
        if(getCommand(sentence, command, param) < 0)
        	continue;

        if(hasLogedin == -1)
        {
        	if(strcmp(command, "USER") == 0)
        		hasLogedin = handleUserRequest(connfd, param);
        	else
        		sendMsg(connfd, notLoginError);
        }
        else if(hasInputPass == -1)
        {
        	if(strcmp(command, "USER") == 0)
        		hasLogedin = handleUserRequest(connfd, param);
        	if(strcmp(command, "PASS") == 0)
        		hasInputPass = handlePassRequest(connfd, param);
        	else
        		sendMsg(connfd, notLoginError);
        }
        else
        {
			//命令处理部分
			if(strcmp(command, "QUIT") == 0 || strcmp(command, "ABOR") == 0)
			{
				hasLogedin = 0;
				hasInputPass = 0;
				sendMsg(connfd, goodbyeMsg);
				break;
			}
			else if(strcmp(command, "SYST") == 0)
				handleSystRequest(connfd, param);

			else if(strcmp(command, "TYPE") == 0)
				handleTypeRequest(connfd, param);

			else if(strcmp(command, "PORT") == 0)
			{
				if(handlePortRequest(connfd, param, portModeInfo))
				{
					isPasvMode = -1;
					isPortMode = 1;
				}
			}

			else if(strcmp(command, "PASV") == 0)
			{
				//如果之前有连接，则断掉连接
				if(fileConnfd != -1)
				{
					close(fileConnfd);
					fileConnfd = -1;
				}
				if(pasvListenfd != -1)
				{
					close(pasvListenfd);
					pasvListenfd = -1;
				}
				int* pPasvListenfd = &pasvListenfd;
				if(handlePasvRequest(connfd, param, pPasvListenfd))
				{
					isPasvMode = 1;
					isPortMode = -1;
				}
				if ((fileConnfd = accept(pasvListenfd, NULL, NULL)) == -1)
				{
					printf("Error accept(): %s(%d)\n", strerror(errno), errno);
					continue;
				}
				printf("succeed in accept. File %d \n", fileConnfd);
			}
			else if(strcmp(command, "RETR"))
			{
				if(isPortMode == 1)
				{

				}
				else if(isPasvMode == 1)
				{

				}
				else
				{
					sendMsg(connfd, noPortError);
				}
			}
			else if(strcmp(command, "STOR"))
			{
				if(isPortMode == 1)
				{

				}
				else if(isPasvMode == 1)
				{

				}
				else
				{
					sendMsg(connfd, noPortError);
				}
			}
			else
			{
				sendMsg(connfd, syntaxError);
			}
		}
	}
}

int main(int argc, char **argv) 
{	
	if(getParamsFromCli(argc, argv) < 0)
		return 1;
	
	int listenfd, connfd;
	int pid;

	listenfd = createSocket();
	if(bindSocketToServer(listenfd, port) == -1)
		return 1;
	setSocketOption(listenfd);
	startSocketListening(listenfd, 10);
	printf("server will run with port %d and root directory %s\n", port, rootPath);
	while (1)
	{
		if ((connfd = accept(listenfd, NULL, NULL)) == -1)
		{
			printf("Error accept(): %s(%d)\n", strerror(errno), errno);
			continue;
		}
		//创建子进程,connfd和listenfd引用增加
		pid = fork();
		if(pid < 0)
		{
			printf("Error fork(): %s(%d)\n", strerror(errno), errno);
			continue;
		}
		//子进程，需要关闭监听listenfd，处理客户端的请求
		else if(pid == 0)
		{
			close(listenfd);
			serveOneClient(connfd);
			printf("serving one over.\n");
			close(connfd);
			return 0;
		}
		//父进程，需要关闭connfd,这样父进程可以继续监听下一个客户端的连接请求
		else
			close(connfd);
	}
	close(listenfd);
	return 0;
}

