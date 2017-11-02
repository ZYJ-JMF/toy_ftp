#include "client.h"

int getServerInfo()
{
	printf("Server ip(default:127.0.0.1):");
	fgets(serverIp, 4096, stdin);
	if(serverIp[0] == '\n'|| serverIp[0] == '\r')
		strcpy(serverIp,"127.0.0.1");
	printf("server ip is %s\n", serverIp);
	printf("Server port(default:6789):");
	fgets(serverPortStr, 4096, stdin);
	if(serverPortStr[0] == '\n' || serverPortStr[0] == '\r')
		serverPort = 6789;
	else
		serverPort = atoi(serverPortStr);
	printf("server port is %d.\n", serverPort);
	return 1;
}

int getUserInfo()
{
	printf("User name(default:anonymous):");
	fgets(user, 4096, stdin);
	if(user[0] == '\n' || user[0] == '\r')
		strcpy(user, "anonymous");
	printf("Password(default:anonymous@):");
	fgets(password, 4096, stdin);
	if(password[0] == '\n' || password[0] == '\r')
		strcpy(password, "anonymous@");

	return 1;
}

int handleBrowseRequest(int sockfd, int* pFileSockfd ,char* param)
{
	if(sendPasvRequest(sockfd) == -1) return -1;
	if(handlePasvResponse(sockfd, pFileSockfd) == -1) return -1;
	if(sendNlstRequest(sockfd, param) == -1) return -1;
	if(handleNlstResponse(sockfd, *pFileSockfd, param) == -1) return -1;
	closeSocket(pFileSockfd);
	return 1;
}
int handleDownloadRequest(int sockfd, int* pFileSockfd, char* param)
{
	if(sendPasvRequest(sockfd) == -1) return -1;
	if(handlePasvResponse(sockfd, pFileSockfd) == -1) return -1;
	if(sendRetrRequest(sockfd, param) == -1) return -1;
	if(handleRetrResponse(sockfd, *pFileSockfd, param) == -1) return -1;
	closeSocket(pFileSockfd);
	return 1;
}
int handleUploadRequest(int sockfd, int* pFileSockfd, char* param)
{
	if(sendPasvRequest(sockfd) == -1) return -1;
	if(handlePasvResponse(sockfd, pFileSockfd) == -1) return -1;
	if(sendStorRequest(sockfd, param) == -1) return -1;
	if(handleStorResponse(sockfd, *pFileSockfd, param) == -1) return -1;
	closeSocket(pFileSockfd);
	return 1;
}

void showWelcomeMsg()
{
	printf("Login OK. Welcome to the magic world of FTP.\n");
	printf("If you are an FTP-expert, you can use commands in RFC 959.\n");
	printf("And we will send the raw reply from the server to you.\n");
	printf("You will be able to handle it c'z you are an expert, right?\n");
	printf("If you are a green-hand, please enter 'help' to see what you can do.\n");
}

void showHelpMsg()
{
	printf("You can use following commands:\n");
	printf("browse [path]: show what you can get from server.\n");
	printf("download [path]: get file from server.\n");
	printf("upload [path]: upload local files to server.\n");
	printf("mkd [dirname]: create new directory in working directory.\n");
	printf("cwd [dirname]: change current working directory.\n");
	printf("rmd [dirname]: remove empty directory in server.\n");
	printf("quit: log out.\n");
}

int main(int argc, char **argv) 
{
	if(getServerInfo() == -1)
		return 1;
	int sockfd = -1;
	int fileSockfd = -1;
	int* pFileSockfd = &fileSockfd;
	int listenFd = -1;
	int* pListenFd = &listenFd;

	char sentence[8192];
	char command[10];
	char param[100];

	int transferMode = NO_MODE;
	sockfd = createSocket();
	if(sendConnectRequest(sockfd, serverIp, serverPort) == -1)
		return 1;
	if(handleConnectResponse(sockfd) == -1)
		return 1;
	printf("You have successfully connected to that server.\n");
	if(getUserInfo() == -1) return 1;
	if(sendUserRequest(sockfd, user) == -1) return 1;
	if(handleUserResponse(sockfd) == -1)   return 1;
	if(sendPassRequest(sockfd, password) == -1) return 1;
	if(handlePassResponse(sockfd) == -1) return 1;	
	showWelcomeMsg();
	while(1)
	{
		fgets(sentence, 4096, stdin);
		removeLineFeed(sentence);
		if(getCommandFromSentence(sentence, command, param) == -1) 
			continue;
		convertToUpperCase(command);
		printf("command: %s\n", command);
		if(strcmp(command, "HELP") == 0)
		{
			showHelpMsg();
		}
		else if(strcmp(command, "BROWSE") == 0)
		{
			closeSocket(pFileSockfd);
			closeSocket(pListenFd);
			if(handleBrowseRequest(sockfd, pFileSockfd, param) == -1)
			{
				printf("Browse request failed.\n");
				continue;
			}
		}
		else if(strcmp(command, "DOWNLOAD") == 0)
		{
			closeSocket(pFileSockfd);
			closeSocket(pListenFd);
			if(handleDownloadRequest(sockfd, pFileSockfd, param) == -1)
			{
				printf("Download request failed.\n");
				continue;
			}
		}
		else if(strcmp(command, "UPLOAD") == 0)
		{
			closeSocket(pFileSockfd);
			closeSocket(pListenFd);
			if(handleUploadRequest(sockfd, pFileSockfd, param) == -1)
			{
				printf("Upload request failed.\n");
				continue;
			}
		}
		else if(strcmp(command, "PASV") == 0)
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
					handleListResponse(sockfd, fileSockfd, param);
					break;
				case PORT_MODE:
					if ((fileSockfd = accept(listenFd, NULL, NULL)) == -1)
					{
						printf("Error accept(): %s(%d)\n", strerror(errno), errno);
						continue;
					} 
					handleListResponse(sockfd, fileSockfd, param);
					break;
				default:
					break;
			}
			transferMode = NO_MODE;
		}
		else if(strcmp(command, "NLST") == 0)
		{
			if(sendNlstRequest(sockfd, param) == -1)
				continue;
			switch(transferMode)
			{
				case PASV_MODE:
					handleNlstResponse(sockfd, fileSockfd, param);
					break;
				case PORT_MODE:
					if ((fileSockfd = accept(listenFd, NULL, NULL)) == -1)
					{
						printf("Error accept(): %s(%d)\n", strerror(errno), errno);
						continue;
					} 
					handleNlstResponse(sockfd, fileSockfd, param);
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
			if(handleMkdResponse(sockfd) == -1)
				continue;
		}
		else if(strcmp(command, "RMD") == 0)
		{
			if(sendRmdRequest(sockfd, param) == -1)
				continue;
			if(handleRmdResponse(sockfd) == -1)
				continue;
		}
		else if(strcmp(command, "CWD") == 0)
		{
			if(sendCwdRequest(sockfd, param) == -1)
				continue;
			if(handleCwdResponse(sockfd) == -1)
				continue;
		}
		else if(strcmp(command, "PWD") == 0)
		{
			if(sendPwdRequest(sockfd, param) == -1)
				continue;
			if(handlePwdResponse(sockfd) == -1)
				continue;
		}
		else if(strcmp(command, "QUIT") == 0)
		{
			printf("Bye.\n");
			sleep(1);
			break;
		}
		else
		{
			printf("Sorry, the command is wrong.\n");
			continue;
		}
	}
	close(sockfd);
	return 0;
}

