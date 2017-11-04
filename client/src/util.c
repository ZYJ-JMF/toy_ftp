#include"util.h"

//返回服务器返回的状态码，出错则返回-1
int getDigit(char*sentence)
{
	char digit[4];
	for(int i = 0; sentence[i] != ' '; i++)
	{
		digit[i] = sentence[i];
	}
	int digitInt = atoi(digit);
	return digitInt;
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
	if(sentence[len - 1] == '\n' || sentence[len - 1] == '\r')
	{
		sentence[len - 1] = '\0';
	}
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
void strreplace(char*sentence, char src, char dest)
{
	for(int i = 0; i < strlen(sentence); i++)
	{
		if(sentence[i] == src)
			sentence[i] = dest;
	}
}
void getIPFromPasvResponse(char* response, char* serverIp)
{
	int counter = 0;
	int i = 0;
	int j = 0;
	while(response[i] != '(') 
		i++;
	for(j = i + 1; ;j++)
	{
		if(response[j] == ',')
			counter++;
		if(counter == 4)
			break;
		serverIp[j - i - 1] = response[j];
	}
	serverIp[j - i - 1] = '\0';
	strreplace(serverIp, ',', '.');
}

int getPortFromPasvResponse(char* response)
{
	int counter = 0;
	int i = 0, j = 0;
	for(i = 0; i < strlen(response); i++)
	{
		if(response[i] == ',')
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
	for(j = 1; response[i + j] != ',';j++)
	{
		p1c[j - 1] = response[i + j];
	}
	p1c[j - 1] = '\0';
	int k = j;
	for(j = j + 1; response[i + j] != ')'; j++)
	{
		p2c[j - k - 1] = response[i + j];
	}
	p2c[j - k - 1] = '\0';
	p1 = atoi(p1c);
	p2 = atoi(p2c);
	int port = p1 * 256 + p2;
	return port;
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
	for(j = 1; param[i + j] != ',';j++)
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
	return 1;
}

void getFileNameFromPath(char* fileName ,char* filePath)
{
	if(filePath[0] != '/')
	{
		strcpy(fileName, filePath);
		return;
	}
	else
	{
		char temp[100];
		memset(temp, 0, 100);
		int i = 0;
		int recorder = 0;
		while(1)
		{
			if(filePath[i] == '/')
			{
				recorder = i;
				memset(temp, 0, 100);
			}
			else if(filePath[i] == '\0')
			{
				temp[i - recorder - 1] = '\0';
				break;
			}
			else
			{
				temp[i - recorder - 1] = filePath[i];
			}
			i++;
		}
		strcpy(fileName, temp);
	}
}

int recvFile(int connfd, char* fileName)
{
	FILE* f = fopen(fileName, "wb");
	char buffer[8192];
	int size;

	while(1)
	{
		memset(buffer, 0, 8192);
		size = read(connfd, buffer, 8192);
		if(size <= 0)
			break;
		fwrite(buffer, 1, size, f);
	}

	fclose(f);
	return 1;
}

int sendFile(int connfd, char* fileName)
{ 
	FILE* f = fopen(fileName, "rb");
	if(f == NULL)
	{
		return -1;
	}
	char buffer[8192];
	int size;
	printf("prepare to send.\n");
	while(1)
	{
		memset(buffer, 0, 8192);
		size = fread(buffer, 1, 8190, f);
		if(size <= 0)
			break;
		send(connfd, buffer, size, 0);
		printf("send %d bytes.\n", size);
	}
	return 1;
}

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

int bindSocket(int sockfd, int port)
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

//10是最大可以排队连接的个数，在这里（并发情况下）应该无意义
int startSocketListening(int sockfd, int maxQueueLength)
{
	if (listen(sockfd, maxQueueLength) == -1) 
	{
		printf("Error listen(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}

int closeSocket(int* pSocket)
{
	if(*pSocket != -1)
	{
		close(*pSocket);
		*pSocket = -1;
	}
	return 1;
}