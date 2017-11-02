#include "client.h"

int main(int argc, char **argv) 
{
	int sockfd = -1;
	int fileSockfd = -1;
	int* pFileSockfd = &fileSockfd;
	int listenFd = -1;
	int* pListenFd = &listenFd;

	char sentence[8192];
	char command[10];
	char param[100];

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
		if(getCommandFromSentence(sentence, command, param) == -1) 
			continue;
		convertToUpperCase(command);
		printf("command: %s\n", command);
		printf("command len: %lu\n", strlen(command));
		if(strcmp(command, "PASV") == 0)
		{
			closeSocket(pFileSockfd);
			closeSocket(pListenFd);
			if(sendPasvRequest(sockfd) == -1)
				continue;
			if(handlePasvResponse(sockfd, pFileSockfd) == -1)
				continue;
			transferMode = PASV_MODE;
		}
		else if(strcmp(command, "PORT") == 0)
		{
			closeSocket(pFileSockfd);
			closeSocket(pListenFd);
			if(sendPortRequest(sockfd, param) == -1)
				continue;
			if(handlePortResponse(sockfd, param, pListenFd) == -1)
				continue;
			transferMode = PORT_MODE;
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
		else if(strcmp(command, "LIST") == 0)
		{
			if(sendListRequest(sockfd, param) == -1)
				continue;
			switch(transferMode)
			{
				case PASV_MODE:
					handleListResponse(sockfd, fileSockfd);
					break;
				case PORT_MODE:
					if ((fileSockfd = accept(listenFd, NULL, NULL)) == -1)
					{
						printf("Error accept(): %s(%d)\n", strerror(errno), errno);
						continue;
					} 
					handleListResponse(sockfd, fileSockfd);
					break;
				default:
					break;
			}
			transferMode = NO_MODE;

		}
		else if(strcmp(command, "STOR") == 0)
		{
			if(sendStorRequest(sockfd, param) == -1)
				continue;
			switch(transferMode)
			{
				case PASV_MODE:
					handleStorResponse(sockfd, fileSockfd, param);
					break;
				case PORT_MODE:
					if ((fileSockfd = accept(listenFd, NULL, NULL)) == -1)
					{
						printf("Error accept(): %s(%d)\n", strerror(errno), errno);
						continue;
					} 
					handleStorResponse(sockfd, fileSockfd, param);
					break;
				default:
					break;
			}
			transferMode = NO_MODE;
			
		}
		else if(strcmp(command, "RETR") == 0)
		{
			if(sendRetrRequest(sockfd, param) == -1)
				continue;
			switch(transferMode)
			{
				case PASV_MODE:
					handleRetrResponse(sockfd, fileSockfd, param);
					break;
				case PORT_MODE:
					if ((fileSockfd = accept(listenFd, NULL, NULL)) == -1)
					{
						printf("Error accept(): %s(%d)\n", strerror(errno), errno);
						continue;
					} 
					handleRetrResponse(sockfd, fileSockfd, param);
					break;
				default:
					break;
			}
			transferMode = NO_MODE;
		}
		else if(strcmp(command, "MKD") == 0)
		{
			if(sendMkdRequest(sockfd, param) == -1)
				continue;
		}
		else if(strcmp(command, "RMD") == 0)
		{
			if(sendRmdRequest(sockfd, param) == -1)
				continue;
		}
		else if(strcmp(command, "CWD") == 0)
		{
			if(sendCwdRequest(sockfd, param) == -1)
				continue;
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

