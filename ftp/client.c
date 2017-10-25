#include"util_client.h"

//测试时暂时使用固定的用户名和密码
char* user = "anonymous";
char* password = "th";

int sendUserRequest(sockfd)
{
	char userPrefix[100] = "USER ";
	strcat(userPrefix, user);
	if(write(sockfd, userPrefix, strlen(userPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}
int sendPassRequest(sockfd)
{
	char passPrefix[100] = "PASS ";
	strcat(passPrefix, password);
	if(write(sockfd, passPrefix, strlen(passPrefix)) < 0)
	{
		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		return -1;
	} 	
	return 1;
}
int main(int argc, char **argv) 
{
	int sockfd;
	struct sockaddr_in addr;
	char sentence[8192];
	printf("client will run.\n");
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = 21;
	if (inet_pton(AF_INET, "192.241.151.16", &addr.sin_addr) <= 0) 
	{
		printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}
	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		printf("Error connect(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	if(read(sockfd, sentence, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}
	printf("FROM SERVER: %s\n", sentence);
	if(sendUserRequest(sockfd) == -1)
	{
		//向用户输出错误信息
		return 0;
	}
	if(read(sockfd, sentence, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return 1;
	} 
	printf("FROM SERVER: %s", sentence);
	sendPassRequest(sockfd);
	if(read(sockfd, sentence, 8191) < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return 1;
	} 
	printf("FROM SERVER: %s", sentence);
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

