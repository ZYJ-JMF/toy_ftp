#include "util.h"

//字符串中字符替换，将所有src替换为dest
void strreplace(char*sentence, char src, char dest)
{
	for(int i = 0; i < strlen(sentence); i++)
	{
		if(sentence[i] == src)
			sentence[i] = dest;
	}
}

void intToString(int n, char* s)
{
	int integer = n;
	int r = 0;
	int len = 1;
	while(integer / 10 != 0)
	{
		integer /= 10;
		len++;
	}
	integer = n;
	for(int i = len - 1; i >= 0; i--)
	{
		s[i] = integer % 10 + '0';
		integer /= 10;
	}
	s[len] = '\0';
}

void convertToUpperCase(char* sentence)
{
	int len = strlen(sentence);
	for (int p = 0; p < len; p++) 
		sentence[p] = toupper(sentence[p]);
}
//删去命令末尾的'\n'
void removeLineFeed(char* sentence)
{
	int len = strlen(sentence);
	while(sentence[len - 1] == '\n' || sentence[len - 1] == '\r')
	{
		sentence[len - 1] = '\0';
		len--;
	}
}

int getIpFromPortMsg(char* param, char* clientIp)
{
	int counter = 0;
	int i = 0;
	while(1)
	{
		if(param[i] == '\0')
		{
			return -1;
		}
		if(counter == 4)
		{
			clientIp[i - 1] = '\0';
			break;
		}
		if(param[i] == ',')
		{
			clientIp[i] = '.';
			counter++;
		}
		else
		{
			clientIp[i] = param[i];
		}
		i++;
	}
	return 1;
}

int getPortFromPortMsg(char* param, int* pClientPort)
{
	int counter = 0;
	int i = 0, j = 0;
	for(i = 0; i < strlen(param); i++)
	{
		if(param[i] == ',')
		{
			counter++;
		}
		if(counter == 4)
		{
			break;
		}
	}
	int p1;
	int p2;
	char p1c[10];
	char p2c[10];
	memset(p1c, 0, strlen(p1c));
	memset(p2c, 0, strlen(p2c));
	for(j = 1; param[i + j] != ','; j++)
	{
		p1c[j - 1] = param[i + j];
	}
	p1c[j - 1] = '\0';
	int k = j;
	for(j = j + 1; param[i + j] != '\0'; j++)
	{
		p2c[j - k - 1] = param[i + j];
	}
	p2c[j - k - 1] = '\0';
	p1 = atoi(p1c);
	p2 = atoi(p2c);
	*pClientPort = p1 * 256 + p2;
	printf("client port is %d\n", *pClientPort);
	return 1;
}

//从sentence中得到前面的command和parameter,出错返回-1
int getCommandFromSentence(char* sentence, char* command, char* parameter)
{
	memset(command, 0, strlen(command));
	memset(parameter, 0, strlen(parameter));	
	int len = strlen(sentence);
	int spacePos = -1;
	for(int i = 0; i < len; i++)
	{
		if(sentence[i] == ' ')
		{
			spacePos = i;
			break;
		}
	}
	if(spacePos > 0)
	{
		for(int i = 0; i < spacePos; i++)
		{
			command[i] = sentence[i];
		}
		command[spacePos] = '\0';
		for(int i = spacePos + 1; i <= len; i++)
		{
			parameter[i - spacePos - 1] = sentence[i];
		}
	}
	else
	{
		for(int i = 0; i <= len; i++)
		{
			command[i] = sentence[i];
		}
	}
	return 1;
}

void makePasvPortMsg(char* pasvMsg, int pasvPort)
{
	char serverIpWithComma[80];
	memset(serverIpWithComma, 0, strlen(serverIpWithComma));
	strcpy(serverIpWithComma, serverIp);
	strreplace(serverIpWithComma, '.', ',');
	int p1 = pasvPort / 256;
	int p2 = pasvPort % 256;
	char leftBracket[2] = "(";
	char rightBracket[2] = ")";
	char comma[2] = ",";
	char endOfLine[10] = "\r\n";
	char p1c[10];
	memset(p1c, 0, strlen(p1c));
	char p2c[10];
	memset(p2c, 0, strlen(p2c));
	intToString(p1, p1c);
	intToString(p2, p2c);
	strcat(pasvMsg, pasvMsgPart);
	strcat(pasvMsg, leftBracket);
	strcat(pasvMsg, serverIpWithComma);
	strcat(pasvMsg, comma);
	strcat(pasvMsg, p1c);
	strcat(pasvMsg, comma);
	strcat(pasvMsg, p2c);
	strcat(pasvMsg, rightBracket);
	strcat(pasvMsg, endOfLine);
	printf("pasvmsg is %s", pasvMsg);
}

void makeStartTransferMsg(char* startTransferMsg, char* fileName)
{
	memset(startTransferMsg, 0, strlen(startTransferMsg));
	char endOfLine[10] = "\r\n";
	strcat(startTransferMsg, startTransferPart);
	strcat(startTransferMsg, fileName);
	strcat(startTransferMsg, endOfLine);
}

void makeAbsolutePath(char* filePath, char* rootPath, char* fileName)
{
	char slash[2] = "/";
	strcat(filePath, rootPath);
	strcat(filePath, slash);
	strcat(filePath, fileName);
}
//参数检查
int checkPassParam(char* param)
{
	return 1;
}
int checkPortParam(char* param)
{
	return 1;
}
int checkListParam(char* param)
{
	return 1;
}
int checkPath(char* path)
{
	const char pattern[10] = "..";
	if(strstr(path, pattern) != NULL)
		return -1;
	else
		return 1;
}
//信息传输
int recvSentence(int connfd, char* sentence)
{
	int n;
	n = recv(connfd, sentence, 8191 , 0);
	if(n < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	sentence[n] = '\0';
    return 1;
}

int sendMsg(int connfd, char* sentence)
{
	int len = strlen(sentence);
	int n = send(connfd, sentence, len, 0);
	if(n < 0)
	{
		printf("Error send():%s(%d)\n" , strerror(errno), errno);
		return -1;
	}
	return 1;
}

int recvFile(int connfd, char* fileName)
{
	FILE* f = fopen(fileName, "wb");
	char buffer[8192];
	memset(buffer, 0, strlen(buffer));
	int size;

	while(1)
	{
		size = read(connfd, buffer, 8191);
		if(size == 0)
			break;
		fwrite(buffer, 1, size, f);
	}

	fclose(f);
	printf("recv success.\n");
	return 1;
}

int sendFile(int connfd, char* fileName)
{
	FILE * f = fopen(fileName, "rb");
	char buffer[8192];
	memset(buffer, 0, strlen(buffer));

	int size;
	while(1)
	{
		size = fread(buffer, 1, 8190, f);
		if(size <= 0)
			break;
		send(connfd, buffer, size, 0);
	}
	return 1;
}

int sendDirectoryInfo(int fileConnfd, char* directoryPath, char* listData)
{
	char blank[2] = " ";
	char endOfLine[10] = "\r\n";
	char regFilePrefix[100] = "rw-r--r-- 1 owner group       ";
	char dirPrefix[100] = "rwxr-xr-x 1 owner group        ";
	DIR* dir;
	struct dirent* ptr;
	struct stat statbuf;
	dir = opendir(directoryPath);
	ptr = readdir(dir);
	while(ptr)
	{
		if(strcmp(ptr -> d_name, ".") == 0 || strcmp(ptr -> d_name, "..") == 0)
		{
			ptr = readdir(dir);
			continue;
		}
		char fileAbsolutePath[100];
		memset(fileAbsolutePath, 0, strlen(fileAbsolutePath));
		makeAbsolutePath(fileAbsolutePath, directoryPath, ptr -> d_name);
		if(stat(fileAbsolutePath, &statbuf) == -1)
		{
			printf("Error read file(): %s(%d)\n", strerror(errno), errno);
			return -1;
		}
		//file
		if(ptr -> d_type == 8)
		{
			char fileInfo[300];
			char sizeStr[100];
			char timeStr[100];
			time_t rawTime;
			struct tm *pTime;
			memset(fileInfo, 0, strlen(fileInfo));
			memset(sizeStr, 0, strlen(sizeStr));
			memset(timeStr, 0, strlen(timeStr));
			strcat(fileInfo, regFilePrefix);
			intToString(statbuf.st_size, sizeStr);
			strcat(fileInfo, sizeStr);
			strcat(fileInfo, blank);
			rawTime = statbuf.st_mtime;
			pTime = localtime(&rawTime);
			strftime(timeStr, 100, "%b %d %H:%M", pTime);
			strcat(fileInfo, timeStr);
			strcat(fileInfo, blank);
			strcat(fileInfo, ptr -> d_name);
			strcat(fileInfo, endOfLine);
			strcat(listData, fileInfo);
		}
		//dir
		else if(ptr -> d_type == 4)
		{
			char fileInfo[300];
			char timeStr[100];
			time_t rawTime;
			struct tm *pTime;
			memset(fileInfo, 0, strlen(fileInfo));
			memset(timeStr, 0, strlen(timeStr));
			strcat(fileInfo, dirPrefix);
			rawTime = statbuf.st_mtime;
			pTime = localtime(&rawTime);
			strftime(timeStr, 100, "%b %d %H:%M", pTime);
			strcat(fileInfo, timeStr);
			strcat(fileInfo, blank);
			strcat(fileInfo, ptr -> d_name);
			strcat(fileInfo, endOfLine);
			strcat(listData, fileInfo);
		}
		ptr = readdir(dir);
	}
	return 1;
}

int sendFileInfo(int fileConnfd, char* filePath, char* fileName, char* listData)
{
	char blank[2] = " ";
	char endOfLine[10] = "\r\n";
	char regFilePrefix[100] = "rw-r--r-- 1 owner group       ";
	struct stat statbuf;
	if(stat(filePath, &statbuf) == -1)
	{
		printf("Error read file(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	char fileInfo[300];
	char sizeStr[100];
	char timeStr[100];
	time_t rawTime;
	struct tm *pTime;
	memset(fileInfo, 0, strlen(fileInfo));
	memset(sizeStr, 0, strlen(sizeStr));
	memset(timeStr, 0, strlen(timeStr));
	strcat(fileInfo, regFilePrefix);
	intToString(statbuf.st_size, sizeStr);
	strcat(fileInfo, sizeStr);
	strcat(fileInfo, blank);
	rawTime = statbuf.st_mtime;
	pTime = localtime(&rawTime);
	strftime(timeStr, 100, "%b %d %H:%M", pTime);
	strcat(fileInfo, timeStr);
	strcat(fileInfo, blank);
	strcat(fileInfo, fileName);
	strcat(fileInfo, endOfLine);
	strcat(listData, fileInfo);
	return 1;
}

int sendDirectoryInfoSimple(int fileConnfd, char* directoryPath, char* listData)
{
	char blank[2] = " ";
	char endOfLine[10] = "\r\n";
	DIR* dir;
	struct dirent* ptr;
	struct stat statbuf;
	dir = opendir(directoryPath);
	ptr = readdir(dir);
	while(ptr)
	{
		if(strcmp(ptr -> d_name, ".") == 0 || strcmp(ptr -> d_name, "..") == 0)
		{
			ptr = readdir(dir);
			continue;
		}
		char fileAbsolutePath[100];
		memset(fileAbsolutePath, 0, strlen(fileAbsolutePath));
		makeAbsolutePath(fileAbsolutePath, directoryPath, ptr -> d_name);
		if(stat(fileAbsolutePath, &statbuf) == -1)
		{
			printf("Error read file(): %s(%d)\n", strerror(errno), errno);
			return -1;
		}
		//file
		if(ptr -> d_type == 8)
		{
			char fileInfo[300];
			memset(fileInfo, 0, strlen(fileInfo));
			strcat(fileInfo, ptr -> d_name);
			strcat(fileInfo, endOfLine);
			strcat(listData, fileInfo);
		}
		//dir
		else if(ptr -> d_type == 4)
		{
			char fileInfo[300];
			memset(fileInfo, 0, strlen(fileInfo));
			strcat(fileInfo, ptr -> d_name);
			strcat(fileInfo, endOfLine);
			strcat(listData, fileInfo);
		}
		ptr = readdir(dir);
	}
	return 1;
}

int sendFileInfoSimple(int fileConnfd, char* filePath, char* fileName, char* listData)
{
	char blank[2] = " ";
	char endOfLine[10] = "\r\n";
	struct stat statbuf;
	if(stat(filePath, &statbuf) == -1)
	{
		printf("Error read file(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	char fileInfo[300];
	memset(fileInfo, 0, strlen(fileInfo));
	strcat(fileInfo, fileName);
	strcat(fileInfo, endOfLine);
	strcat(listData, fileInfo);
	return 1;
}
//建立新的socket，返回该socket的描述符
int createSocket()
{
	int sockfd;
	if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
	{
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	return sockfd;
}
//将socket绑定到服务器指定端口
int bindSocketToServer(int sockfd, int port)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
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

int startSocketListening(int sockfd, int maxQueueLength)
{
	if (listen(sockfd, maxQueueLength) == -1) 
	{
		printf("Error listen(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}

void closeSocket(int* pSockfd)
{
	if(*pSockfd > 0)
	{
		close(*pSockfd);
		*pSockfd= -1;
	}

}

int sendConnectRequest(int sockfd, char* targetIp, int targetPort)
{
	struct sockaddr_in addr;
	char response[1000];
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(targetPort);
	printf("target IP is %s", targetIp);
	if(inet_pton(AF_INET, targetIp, &addr.sin_addr) <= 0)
		return -1;
	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		printf("Error connect(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}
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
				port = atoi(optarg);
				break;
			case VALUE_ROOT:
				memset(rootPath, 0, strlen(rootPath));
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

