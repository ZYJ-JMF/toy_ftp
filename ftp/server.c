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

//建立新的socket，返回该socket的描述符
int createSocket()
{
	int listenfd;
	if ((listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
	{
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	return listenfd;
}
//将socket绑定到服务器
int bindSocketToServer(int listenfd, int port)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
	{
		printf("Error bind(): %s(%d)\n", strerror(errno), errno);
		return -1;	
	}
	return 1;
}
//设置socket为可立即复用，最大排队连接个数为10
int setSocketOption(int fd)
{
	int reuse = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse) == -1)
	{
		printf("Error set(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	//10是最大可以排队连接的个数，在这里（并发情况下）应该无意义
	if (listen(fd, 10) == -1) 
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
	int hasLogedin = 0; //是否登录，登录为1，未登录为0
	int hasInputPass = 0; //是否给出密码
	int isPortMode = 0;  //是否为端口模式
	int isPasvMode = 0;  //是否为passive模式
	char portModeInfo[8192];   //端口模式传送文件所用端口信息
	char pasvModeInfo[8192];   //passive模式传送文件端口信息

	int pasvPort;
	int pasvFilefd;  //用于传送文件的socket
	sendMsg(connfd, welcomeMsg);
	printf("Welcome message sent\n");
	while(1)
	{
		command[0] = '\0';
		param[0] ='\0';
		if(getSentence(connfd, sentence) < 0)
			continue;
		printf("sentence length before: %lu\n", strlen(sentence));
        convertToUpperCase(sentence);
        removeLineFeed(sentence);
        printf("sentence length after: %lu\n", strlen(sentence));
        printf("sentence: %s\n", sentence);
        if(getCommand(sentence, command, param) < 0)
        	continue;
        //TODO:建立用户表
        if(hasLogedin == 0)
        {
        	if(strcmp(command, "USER") == 0)
        	{
        		if(strcmp(param, "ANONYMOUS") == 0)
        		{
	        		sendMsg(connfd, requirePassMsg);
	        		hasLogedin = 1;
	        	}
	        	else
	        	{
	        		sendMsg(connfd, unknownUserError);
	        	}
        	}
        	else
        	{
        		sendMsg(connfd, notLoginError);
        	}
        }
        else if(hasInputPass == 0)
        {
        	//TODO:检查密码格式
        	if(strcmp(command, "PASS") == 0)
        	{
        		sendMsg(connfd, loginSuccessMsg);
        		hasInputPass = 1;
        	}
        	else
        	{
        		sendMsg(connfd, notLoginError);
        	}
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
			{
				sendMsg(connfd, sysStatusMsg);
			}
			else if(strcmp(command, "TYPE") == 0)
			{
				if(strcmp(param, "I") == 0)
					sendMsg(connfd, typeMsg);
				else if(strcmp(param, "") == 0)
					sendMsg(connfd, syntaxError);
				else
					sendMsg(connfd, typeError);
			}
			else if(strcmp(command, "PORT") == 0)
			{
				if(checkPortParam(param) == -1)
				{
					sendMsg(connfd, typeError);
					continue;
				}
				else
				{
					isPasvMode = 0;
					isPortMode = 1;
					strcpy(portModeInfo, param);
					sendMsg(connfd, portMsg);
				}
			}
			else if(strcmp(command, "PASV") == 0)
			{
				isPasvMode = 1;
				isPortMode = 0;
				//尝试绑定并选择一个可用端口
				pasvPort = 30000;
				pasvFilefd = createSocket();
				while(1)
				{
					printf("start.\n");
					if(bindSocketToServer(pasvFilefd, pasvPort) > 0)
					{
						printf("going to break");
						break;
					}
					else
						pasvPort += 1;
					printf("error.\n");
				}
				printf("Listening port%d", pasvPort);
				/*
				int p1 = tempPort / 256;
				int p2 = tempPort % 256;
				char p1c[10];
				char p2c[10];
				itoa(p1, p1c, 10);
				itoa(p2, p2c, 10);
				char pasvMsg[200];
				strcat(pasvMsg, pasvMsgPart);
				strcat(pasvMsg, serverIp);
				strcat(pasvMsg, p1c);
				strcat(pasvMsg, p2c);*/

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

