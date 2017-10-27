#include "configs.h"
#include "handler.c"

#define NO_MODE 0
#define PASV_MODE 1
#define PORT_MODE 2

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

void serveOneClient(int connfd)
{
	char sentence[8192];
	char command[100];
	char param[100];
	int hasLogedIn = -1; //是否登录，登录为1，未登录为-1
	int hasInputPass = -1; //是否给出密码
	int isTransferring = -1; //是否正在传输文件
	int fileMode = NO_MODE;

	//pasv模式使用
	int pasvPort;
	int pasvListenfd = -1; 
	int* pPasvListenfd = &pasvListenfd;

	//port模式使用
	char clientIp[8192];
	int clientPort = -1;

	//连接客户端的socket,两模式共用
	int fileConnfd = -1;  
	int* pFileConnfd = &fileConnfd;  

	handleFirstConnectRequest(connfd);
	while(1)
	{
		if(handleInputSentence(connfd, sentence) == -1) 
			continue;
		if(parseInputSentence(sentence, command, param) == -1)
			continue;
        if(isTransferring == 1)
        {
        	printf("Is transferring file.\n");
        	continue;
        }
        if(hasLogedIn == -1)
        {
        	if(strcmp(command, "USER") == 0)
        		hasLogedIn = handleUserRequest(connfd, param);
        	else
        		sendMsg(connfd, notLoginError);
        }
        else if(hasInputPass == -1)
        {
        	if(strcmp(command, "USER") == 0)
        		hasLogedIn = handleUserRequest(connfd, param);
        	if(strcmp(command, "PASS") == 0)
        		hasInputPass = handlePassRequest(connfd, param);
        	else
        		sendMsg(connfd, notLoginError);
        }
        else
        {
			if(strcmp(command, "QUIT") == 0 || strcmp(command, "ABOR") == 0)
			{
				hasLogedIn = -1;
				hasInputPass = -1;
				sendMsg(connfd, goodbyeMsg);
				break;
			}
			else if(strcmp(command, "SYST") == 0)
				handleSystRequest(connfd, param);

			else if(strcmp(command, "TYPE") == 0)
				handleTypeRequest(connfd, param);

			else if(strcmp(command, "PORT") == 0)
			{
				closeFileSockets(pFileConnfd, pPasvListenfd);
				int* pClientPort = &clientPort;
				if(handlePortRequest(connfd, param, clientIp, pClientPort) == 1)
				{
					fileMode = PORT_MODE;
				}
				else
				{
					sendMsg(connfd, portSyntaxError);
				}
			}
			else if(strcmp(command, "PASV") == 0)
			{
				//如果之前有连接，则断掉连接
				closeFileSockets(pFileConnfd, pPasvListenfd);
				if(handlePasvRequest(connfd, param, pPasvListenfd) == 1)
				{
					fileMode = PASV_MODE;
				}
				if ((fileConnfd = accept(pasvListenfd, NULL, NULL)) == -1)
				{
					printf("Error accept(): %s(%d)\n", strerror(errno), errno);
					continue;
				}
			}
			else if(strcmp(command, "RETR") == 0)
			{
				if(fileMode == PORT_MODE)
				{

				}
				else if(fileMode == PASV_MODE)
				{

				}
				else
					sendMsg(connfd, noPortError);
			}
			else if(strcmp(command, "STOR") == 0)
			{
				switch(fileMode)
				{
					case PASV_MODE:
						handleStorRequestPasv(connfd, fileConnfd, param);
						closeFileSockets(pFileConnfd, pPasvListenfd);
						break;
					case PORT_MODE:
						break;
					default:
						sendMsg(connfd, noPortError);
						break;
				}
			}
			else
				sendMsg(connfd, syntaxError);
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

