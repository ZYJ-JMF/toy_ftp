#include"util.h"
#include"reply.h"

void serveOneClient(int connfd)
{
	char sentence[8192];
	char command[100];
	char param[100]; //param of command
	int hasLogedin = 0; //是否登录，登录为1，未登录为0
	int hasInputPass = 0; //是否给出密码
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
			else
			{
				sendMsg(connfd, syntaxError);
			}
		}
	}
}
int main(int argc, char **argv) 
{
	printf("server will run\n");
	int listenfd, connfd;
	struct sockaddr_in addr;
	int pid; //multiple processes

	if ((listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
	{
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = 6789;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
	{
		printf("Error bind(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	if (listen(listenfd, 10) == -1) 
	{
		printf("Error listen(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	while (1)
	{
		//多个客户端
		if ((connfd = accept(listenfd, NULL, NULL)) == -1)
		{
			printf("Error accept(): %s(%d)\n", strerror(errno), errno);
			continue;
		}
		pid = fork();
		if(pid < 0)
			printf("Error fork()");
		else if(pid == 0) //子进程
		{
			close(listenfd);
			serveOneClient(connfd);
			printf("serving one is over.\n");
			close(connfd);
			return 0;
		}
		close(connfd);
	}
	close(listenfd);
	return 0;
}

