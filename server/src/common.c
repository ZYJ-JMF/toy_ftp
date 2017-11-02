#include "common.h"

char* startTransferPart = "150 Opening BINARY mode data connection for ";

char* typeIMsg = "200 Type set to I.\r\n";
char* typeAMsg = "200 Type set to A.\r\n";
char* portMsg = "200 PORT command successful.\r\n";
char* sysStatusMsg = "215 UNIX Type: L8\r\n";
char* welcomeMsg = "220 Anonymous FTP server ready.\r\n";
char* goodbyeMsg = "221 Goodbye.\r\n";
char* fileSentMsg = "226 Transfer complete.\r\n";
char* directorySentMsg = "226 Directory sent complete.\r\n";
char* pasvMsgPart = "227 Entering passive mode";
char* loginSuccessMsg = "230 Guest login ok, access restrictions apply.\r\n";
char* mkdSuccessMsgPart = "250 Successfully make directory ";
char* rmdSuccessMsgPart = "250 Successfully remove directory ";
char* cwdSuccessMsgPart = "250 Successfully change working directory to ";
char* pwdSuccessMsgPart = "257 ";

char* requirePassMsg = "331 Guest login ok, send your e-mail address as password.\r\n";

char* cannotConnectError = "425 Can't connect to given port.\r\n";
char* noPortError = "425 You need to use PORT or PASV command to establish TCP connection before every file transfer.\r\n";
char* networkError = "426 Your connection has been broken by network failure.\r\n";
char* readDirError = "451 Can not read the directory from disk.\r\n";

char* syntaxError = "500 Unacceptable syntax.\r\n";
char* typeError = "501 Wrong parameters.\r\n";
char* portSyntaxError = "501 Wrong port parameters.\r\n";
char* unknownUserError = "501 Unknown user.\r\n";
char* invalidPathError = "504 The path is invalid(contains ../?).\r\n";
char* noAvailablePortError = "510 No available port.\r\n";
char* notLoginError = "530 You need to login first.\r\n";
char* fileNotExistError = "550 File does not exist.\r\n";
char* mkdFailError = "550 Creation failed.\r\n";
char* rmdFailError = "550 Removal failed.\r\n";
char* cwdFailError = "550 Change directory failed.\r\n";

//服务器的配置
int port = 6789; 
char rootPath[1000] = "/Users/pingguo/Desktop/CN1_ftp/server/src/tmp";
char serverIp[100] = "127.0.0.1";