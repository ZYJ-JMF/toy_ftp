#include"util.h"
#include"configs.h"

#define NO_MODE 0
#define PASV_MODE 1
#define PORT_MODE 2

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
	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		printf("Error connect(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}
//第一次连接服务器，接收欢迎信息
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
int handlePasvResponse(int sockfd,int* pFileSockfd)
{
	char response[1000];
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 227)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("FROM SERVER: %s\n", response);
	char serverIp[80];
	int filePort = 0;
	getIPFromPasvResponse(response, serverIp);
	filePort = getPortFromPasvResponse(response);
	printf("serverIp is %s\n", serverIp);
	printf("port is %d\n", filePort);
	//连接
	*pFileSockfd = createSocket();
	if(sendConnectRequest(*pFileSockfd, serverIp, filePort) == -1)
		return -1;

	return 1;
}
int sendSystRequest(int sockfd)
{
	char* systCommand = "SYST";
	if(write(sockfd, systCommand, strlen(systCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}
int handleSystResponse(int sockfd)
{
	char response[1000];
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 215)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("FROM SERVER: %s", response);
	return 1;
}

int sendStorRequest(int sockfd, char* fileName)
{
	char storPrefix[100] = "STOR ";
	strcat(storPrefix, fileName);
	if(write(sockfd, storPrefix, strlen(storPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}

int handleStorResponsePasv(int sockfd, int fileSockfd, char* fileName)
{
	char response[1000];
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 150)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("successfully read from server.\n");
	if(sendFile(fileSockfd, fileName) == -1)
	{
		printf("Send file error(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	close(fileSockfd);
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 226)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		printf("send failed.\n");
		return -1; 
	}
	return 1;
}
int handleStorResponsePort(int sockfd, int fileSockfd, char* fileName)
{
	return 1;
}
int sendTypeRequest(int sockfd)
{
	return 1;
}
int handleTypeResponse(int sockfd)
{
	return 1;
}
int main(int argc, char **argv) 
{
	int sockfd = -1;
	int fileSockfd = -1;
	int* pFileSockfd = &fileSockfd;

	char sentence[8192];
	char command[10];
	char param[100];

	//1代表port模式，2代表pasv模式
	int transferMode = NO_MODE;

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
		removeLineFeed(sentence);
		if(getCommand(sentence, command, param) == -1) 
			continue;
		convertToUpperCase(command);
		printf("command: %s\n", command);
		printf("command len: %lu\n", strlen(command));
		if(strcmp(command, "PASV") == 0)
		{
			if(sendPasvRequest(sockfd) == -1)
				continue;
			if(handlePasvResponse(sockfd, pFileSockfd) == -1)
				continue;
			transferMode = PASV_MODE;
		}
		else if(strcmp(command, "SYST") == 0)
		{
			if(sendSystRequest(sockfd) == -1)
				continue;
			if(handleSystResponse(sockfd) == -1)
				continue;
		}
		else if(strcmp(command, "TYPE") == 0)
		{
			if(sendTypeRequest(sockfd) == -1)
				continue;
			if(handleTypeResponse(sockfd) == -1)
				continue;
		}
		else if(strcmp(command, "STOR") == 0)
		{
			if(sendStorRequest(sockfd, param) == -1)
				continue;
			switch(transferMode)
			{
				case PASV_MODE:
					handleStorResponsePasv(sockfd, fileSockfd, param);
					break;
				default:
					break;
			}
			
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

