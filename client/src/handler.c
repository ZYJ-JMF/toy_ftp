#include"handler.h"

int sendConnectRequest(int sockfd, char* targetIp, int serverPort)
{
	struct sockaddr_in addr;
	char response[1000];
	memset(response, 0, 1000);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(serverPort);
	if(inet_pton(AF_INET, targetIp, &addr.sin_addr) <= 0)
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

//发送User命令，检查回复，出错返回-1
int sendUserRequest(int sockfd, char* user)
{
	char userPrefix[100] = "USER ";
	char response[1000];
	memset(response, 0, 1000);
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
	memset(response, 0, 1000);
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
	char systCommand[50] = "SYST\r\n";
	if(write(sockfd, systCommand, strlen(systCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}

int sendTypeRequest(int sockfd)
{
	char typeCommand[100] = "TYPE I\r\n";
	if(write(sockfd, typeCommand, strlen(typeCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}

int sendMkdRequest(int sockfd, char* param)
{
	char endOfLine[4] = "\r\n";
	char mkdPrefix[100] = "MKD ";
	strcat(mkdPrefix, param);
	strcat(mkdPrefix, endOfLine);
	if(write(sockfd, mkdPrefix, strlen(mkdPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}
int sendRmdRequest(int sockfd, char* param)
{
	char endOfLine[4] = "\r\n";
	char rmdPrefix[100] = "RMD ";
	strcat(rmdPrefix, param);
	strcat(rmdPrefix, endOfLine);
	if(write(sockfd, rmdPrefix, strlen(rmdPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}
int sendCwdRequest(int sockfd, char* param)
{
	char endOfLine[4] = "\r\n";
	char cwdPrefix[100] = "CWD ";
	strcat(cwdPrefix, param);
	strcat(cwdPrefix, endOfLine);
	if(write(sockfd, cwdPrefix, strlen(cwdPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}

int sendPwdRequest(int sockfd, char* param)
{
	char endOfLine[4] = "\r\n";
	char pwdCommand[100] = "PWD";
	strcat(pwdCommand, endOfLine);
	if(write(sockfd, pwdCommand, strlen(pwdCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}
int sendListRequest(int sockfd, char* param)
{
	char endOfLine[4] = "\r\n";
	char listCommand[200] = "LIST ";
	if(!param)
		listCommand[4] = '\0';
	else
		strcat(listCommand, param);
	strcat(listCommand, endOfLine);
	if(write(sockfd, listCommand, strlen(listCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 		
	return 1;
}
int sendNlstRequest(int sockfd, char* param)
{
	char endOfLine[4] = "\r\n";
	char nlstCommand[200] = "NLST ";
	if(!param)
		nlstCommand[4] = '\0';
	else
		strcat(nlstCommand, param);
	strcat(nlstCommand, endOfLine);
	if(write(sockfd, nlstCommand, strlen(nlstCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 		
	return 1;
}

int sendPortRequest(int sockfd, char* param)
{
	char endOfLine[4] = "\r\n";
	char portCommand[200] = "PORT ";
	strcat(portCommand, param);
	strcat(portCommand, endOfLine);
	if(write(sockfd, portCommand, strlen(portCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 		
	return 1;
}

int sendPasvRequest(int sockfd)
{
	char pasvCommand[10] = "PASV\r\n";
	if(write(sockfd, pasvCommand, strlen(pasvCommand)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	
	return 1;
}
int sendRetrRequest(int sockfd, char* param)
{
	char endOfLine[4] = "\r\n";
	char retrPrefix[100] = "RETR ";
	strcat(retrPrefix, param);
	strcat(retrPrefix, endOfLine);
	if(write(sockfd, retrPrefix, strlen(retrPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}

int sendStorRequest(int sockfd, char* param)
{
	char endOfLine[4] = "\r\n";
	char storPrefix[100] = "STOR ";
	strcat(storPrefix, param);
	strcat(storPrefix, param);
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
	memset(response, 0, 1000);
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
	return 1;
}

int handleUserResponse(int sockfd)
{
	char response[1000];
	memset(response, 0, 1000);

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
	printf("FROM SERVER: %s\n", response);
	return 1;
}

int handlePassResponse(int sockfd)
{
	char response[1000];
	memset(response, 0, 1000);

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
	printf("FROM SERVER: %s\n", response);

	return 1;
}

int handleSystResponse(int sockfd)
{
	char response[1000];
	memset(response, 0, 1000);
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
	memset(response, 0, 1000);

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
	memset(response, 0, 1000);
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
	printf("FROM SERVER: %s", response);

	char serverIp[80];
	memset(serverIp, 0, strlen(serverIp));

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

int handlePasvResponseSimple(int sockfd, int* pFileSockfd)
{
	char response[1000];
	memset(response, 0, 1000);
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
	char serverIp[80];
	memset(serverIp, 0, 80);
	int filePort = 0;
	getIPFromPasvResponse(response, serverIp);
	filePort = getPortFromPasvResponse(response);
	//连接
	*pFileSockfd = createSocket();
	if(sendConnectRequest(*pFileSockfd, serverIp, filePort) == -1)
		return -1;

	return 1;
}

int handlePortResponse(int sockfd, char* param, int* pListenFd)
{
	char response[1000];
	memset(response, 0, 1000);
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
	int port = -1;
	int* pPort = &port;

	getPortFromPortMsg(param, pPort);
	printf("port is %d\n", port);
	*pListenFd = createSocket();
	if(bindSocket(*pListenFd, port) < 0)
	{
		return -1;
	}
	setSocketOption(*pListenFd);
	startSocketListening(*pListenFd, 10);
	return 1;
}

int handleStorResponse(int sockfd, int fileSockfd, char* param)
{
	char response[1000];
	memset(response, 0, 1000);
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

	if(sendFile(fileSockfd, param) == -1)
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


int handleRetrResponse(int sockfd, int fileSockfd, char* param)
{
	char response[1000];
	memset(response, 0, 1000);

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

	if(recvFile(fileSockfd, param) == -1)
	{
		printf("Recv file error(): %s(%d)\n", strerror(errno), errno);
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

int handleListResponse(int sockfd, int fileSockfd, char* param)
{
	char response[1000];
	memset(response, 0, 1000);
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
	printf("FROM SERVER: %s\n", response);
	char listContent[8192];
	memset(listContent, 0, strlen(listContent));
	if(read(fileSockfd, listContent, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	printf("%s\n", listContent);

	memset(response, 0, 1000);
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

int handleNlstResponse(int sockfd, int fileSockfd, char* param)
{
	char response[1000];
	memset(response, 0, 1000);
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
	printf("FROM SERVER: %s\n", response);
	char listContent[8192];
	memset(listContent, 0, strlen(listContent));
	if(read(fileSockfd, listContent, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	printf("%s\n", listContent);

	memset(response, 0, 1000);
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

int handleNlstResponseSimple(int sockfd, int fileSockfd, char* param)
{
	char response[1000];
	memset(response, 0, 1000);
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
	char listContent[8192];
	memset(listContent, 0, 8192);
	if(read(fileSockfd, listContent, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	printf("Current working directory:\n");
	printf("%s\n", listContent);

	memset(response, 0, 1000);
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
	return 1;
}

int handleMkdResponse(int sockfd)
{
	char response[1000];
	memset(response, 0, 1000);
	
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 250)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("FROM SERVER: %s\n", response);
	return 1;
}
int handleRmdResponse(int sockfd)
{
	char response[1000];
	memset(response, 0, 1000);
	
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 250)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("FROM SERVER: %s\n", response);
	return 1;
}
int handleCwdResponse(int sockfd)
{
	char response[1000];
	memset(response, 0, 1000);
	
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 250)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("FROM SERVER: %s\n", response);
	return 1;
}
int handlePwdResponse(int sockfd)
{
	char response[1000];
	memset(response, 0, 1000);
	
	if(read(sockfd, response, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 
	if(getDigit(response) != 257)
	{
		printf("No proper reply.\n");
		printf("current reply is : %s\n", response);
		return -1; 
	}
	printf("FROM SERVER: %s\n", response);
	return 1;
}


