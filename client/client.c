#include"util.h"

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
int sendConnectRequest(int sockfd, char* serverIp, int serverPort)
{
	struct sockaddr_in addr;
	char response[1000];
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
	return 1;
}
int handleConnectResponse(int sockfd)
{
	char response[1000];
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	if(getDigit(response) != 220)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("Successfully connect.\n");
	return 1;
}

//发送User命令，检查回复，出错返回-1
int sendUserRequest(int sockfd, char* user)
{
	char userPrefix[100] = "USER ";
	char response[1000];
	strcat(userPrefix, user);
	if(write(sockfd, userPrefix, strlen(userPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 

	return 1;
}
int handleUserResponse(int sockfd)
{
	char response[1000];
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	if(getDigit(response) != 331)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("Successfully set user.\n");
	return 1;
}
//发送PASS命令,检查回复，出错返回-1
int sendPassRequest(int sockfd, char*password)
{
	char passPrefix[100] = "PASS ";
	char response[1000];
	strcat(passPrefix, password);
	if(write(sockfd, passPrefix, strlen(passPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;

}
int handlePassResponse(int sockfd)
{
	char response[1000];
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 230)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1;
	}
	printf("Successfully set pass.\n");
	return 1;
}
int sendPasvRequest(int sockfd)
{
	char* pasvCommand = "PASV";
	if(write(sockfd, pasvCommand, strlen(pasvCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	
	return 1;
}
int handlePasvResponse(int sockfd)
{
	char response[1000];
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	printf("FROM SERVER: %s", response);
	return 1;
}
int main(int argc, char **argv) 
{
	int sockfd;
	char sentence[8192];
	char command[10];
	char param[100];
	char* user = "anonymous";
	char* password = "th";
	char* serverIp = "127.0.0.1";
	int serverPort = 6789;
	printf("client will run.\n");
	sockfd = createSocket();
	if(sendConnectRequest(sockfd, serverIp, serverPort) == -1)
		return 1;
	if(handleConnectResponse(sockfd) == -1)
		return 1;
	if(sendUserRequest(sockfd,user) == -1)
		return 1;
	if(handleUserResponse(sockfd) == -1)
		return 1;
	if(sendPassRequest(sockfd, password) == -1)
		return 1;
	if(handlePassResponse(sockfd) == -1)
		return 1;
	while(1)
	{
		fgets(sentence, 4096, stdin);
		convertToUpperCase(sentence);
		removeLineFeed(sentence);
		if(getCommand(sentence, command, param) == -1) 
			continue;
			
		printf("command: %s\n", command);
		printf("command len: %lu\n", strlen(command));
		if(strcmp(command, "PASV") == 0)
		{
			if(sendPasvRequest(sockfd) == -1)
				return 1;
			if(handlePasvResponse(sockfd) == -1)
				return 1;
		}
		else if(strcmp(command, "SYST") == 0)
		{

		}
		else if(strcmp(command, "TYPE") == 0)
		{
			
		}
		else
		{
			printf("Error params.\n");
			return 1;
		}
	}
	close(sockfd);
	return 0;
}

