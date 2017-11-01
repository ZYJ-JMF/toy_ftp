#include"handler.h"

int sendConnectRequest(int sockfd, char* serverIp, int serverPort)
{
	struct sockaddr_in addr;
	char response[1000];
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(serverPort);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		printf("Error connect(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
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

int sendTypeRequest(int sockfd)
{
	char* typeCommand = "TYPE I";
	if(write(sockfd, typeCommand, strlen(typeCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}

int sendMkdRequest(int sockfd, char* param)
{
	char mkdPrefix[100] = "MKD ";
	strcat(mkdPrefix, param);
	if(write(sockfd, mkdPrefix, strlen(mkdPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}
int sendRmdRequest(int sockfd, char* param)
{
	char rmdPrefix[100] = "RMD ";
	strcat(rmdPrefix, param);
	if(write(sockfd, rmdPrefix, strlen(rmdPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}
int sendCwdRequest(int sockfd, char* param)
{
	char cwdPrefix[100] = "CWD ";
	strcat(cwdPrefix, param);
	if(write(sockfd, cwdPrefix, strlen(cwdPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}
int sendListRequest(int sockfd, char* param)
{
	char listCommand[200] = "LIST ";
	if(!param)
		listCommand[4] = '\0';
	else
		strcat(listCommand, param);
	if(write(sockfd, listCommand, strlen(listCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 		
	return 1;
}
int sendPortRequest(int sockfd, char* param)
{
	char portCommand[200] = "PORT ";
	strcat(portCommand, param);

	if(write(sockfd, portCommand, strlen(portCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 		
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
int sendRetrRequest(int sockfd, char* fileName)
{
	char retrPrefix[100] = "RETR ";
	strcat(retrPrefix, fileName);
	if(write(sockfd, retrPrefix, strlen(retrPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
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


/*-----------------------------------------------------------*/

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
	printf("FROM SERVER: %s\n", response);
	return 1;
}

int handleTypeResponse(int sockfd)
{
	char response[1000];
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 200)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("FROM SERVER: %s\n", response);
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

int handlePortResponse(int sockfd, char* param, int* pListenFd)
{
	printf("param is %s\n", param);
	printf("sockfd is %d\n", sockfd);
	char response[1000];

	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 

	printf("response is %s\n", response);
	if(getDigit(response) != 200)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("going to handle port response.\n");
	int port = -1;
	int* pPort = &port;

	getPortFromPortMsg(param, pPort);
	printf("port is %d\n", port);
	*pListenFd = createSocket();
	if(bindSocketToServer(*pListenFd, port) < 0)
	{
		return -1;
	}
	setSocketOption(*pListenFd);
	startSocketListening(*pListenFd, 10);
	return 1;
}

int handleStorResponse(int sockfd, int fileSockfd, char* fileName)
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
	printf("Successfully read from server. Reply is %s.\n", response);

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
	printf("Store succeed.\n");
	return 1;
}


int handleRetrResponse(int sockfd, int fileSockfd, char* fileName)
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
	printf("Successfully read from server. Reply is %s.\n", response);

	if(recvFile(fileSockfd, fileName) == -1)
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
		return -1; 
	}
	printf("Retr succeed.\n");
	return 1;
}

int handleListResponse(int sockfd, int fileSockfd)
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
	printf("Successfully read from server. Reply is %s.\n", response);

	char listContent[8192];
	if(read(fileSockfd, listContent, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	printf("List content is %s\n", listContent);

	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 226)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("FROM SERVER: %s\n", response);
	
	return 1;
}




