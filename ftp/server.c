#include"util.h"
#include"reply.h"


int main(int argc, char **argv) 
{
	printf("server will run\n");
	int t; //用于判断是否出错
	int listenfd, connfd;
	struct sockaddr_in addr;
	char sentence[8192];
	char command[100];
	char param[100]; //param of command
	int p;
	int sentenceLen;
	int commandLen;
	int hasLogedin; //是否登录，登录为1，未登录为0

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
		if ((connfd = accept(listenfd, NULL, NULL)) == -1)
		{
			printf("Error accept(): %s(%d)\n", strerror(errno), errno);
			continue;
		}
		// 欢迎信息
		t = send(connfd, welcomeMsg, 100, 0);
		hasLogedin = 0;
		printf("Welcome message sent\n");
		while(1)
		{
			command[0] = '\0';
			param[0] ='\0';
			sentenceLen = getSentence(connfd, sentence);
			if(sentenceLen == -1)
			{
				printf("Error read():%s(%d)\n" , strerror(errno), errno); 
				close(connfd);
				continue;
			}
	        convertToUpperCase(sentence);
	        t = getCommand(sentence, command, param);
	        //TODO:t的错误处理

			commandLen = strlen(command);
			printf("%s", command);

			//命令处理部分
			if(strcmp(command, "QUIT") == 0 || strcmp(command, "ABOR") == 0)
			{
				t = sendMsg(connfd, goodbyeMsg);
				break;
			}
			else if(strcmp(command, "SYST") == 0)
			{
				t = sendMsg(connfd, sysStatusMsg);
			}
			else if(strcmp(command, "TYPE") == 0)
			{
				if(strcmp(param, "I") == 0)
					t = sendMsg(connfd, typeMsg);
				else if(strcmp(param, "") == 0)
					t = sendMsg(connfd, syntaxError);
				else
					t = sendMsg(connfd, typeError);
			}
		}
		close(connfd);
	}
	close(listenfd);
}

