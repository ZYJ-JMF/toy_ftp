#include"util_client.h"

//建立新的socket，返回该socket的描述符,出错结束进程
int createSocket()
{
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
	{
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	return sockfd;
}
//ftp连接，出错返回-1
int ftpConnect(int sockfd, char* serverIp, int serverPort,char* sentence)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = serverPort;
	if (inet_pton(AF_INET, serverIp, &addr.sin_addr) <= 0) 
	{
		printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		printf("Error connect(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	if(read(sockfd, sentence, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	if(getDigit(sentence) != 220)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", sentence);
		return -1; 
	}
	return 1;
}

//发送User命令，检查回复，出错返回-1
int sendUserRequest(int sockfd, char* user, char*sentence)
{
	char userPrefix[100] = "USER ";
	strcat(userPrefix, user);
	if(write(sockfd, userPrefix, strlen(userPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(read(sockfd, sentence, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	if(getDigit(sentence) != 331)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", sentence);
		return -1; 
	}
	printf("FROM SERVER: %s", sentence);
	return 1;
}
//发送PASS命令,检查回复，出错返回-1
int sendPassRequest(int sockfd, char*password, char*sentence)
{
	char passPrefix[100] = "PASS ";
	strcat(passPrefix, password);
	if(write(sockfd, passPrefix, strlen(passPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	if(read(sockfd, sentence, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(sentence) != 230)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", sentence);
		return -1;
	}
	return 1;
}

int main(int argc, char **argv) 
{
	int sockfd;
	char sentence[8192];
	char* user = "anonymous";
	char* password = "th";
	char* serverIp = "127.0.0.1";
	int serverPort = 6789;
	printf("client will run.\n");
	sockfd = createSocket();
	if(ftpConnect(sockfd, serverIp, serverPort, sentence) == -1)
		return 1;
	printf("Successfully connect.\n");
	if(sendUserRequest(sockfd,user,sentence) == -1)
		return 1;
	printf("Successfully send user.\n");
	if(sendPassRequest(sockfd, password, sentence) == -1)
		return 1;
	printf("Successfully send pass.\n");
	while(1)
	{
		fgets(sentence, 4096, stdin);
		printf("sentence len: %lu\n", strlen(sentence));
		if(write(sockfd, sentence, strlen(sentence)) < 0)
		{
			printf("Error write(): %s(%d)\n", strerror(errno), errno);
			return 1;
 		} 	
		if(read(sockfd, sentence, 8191) < 0)
		{
			printf("Error read(): %s(%d)\n", strerror(errno), errno);
			return 1;
		} 
		printf("FROM SERVER: %s", sentence);
	}
	close(sockfd);
	return 0;
}

