/*服务器的回复字符串*/

#pragma once

char* welcomeMsg = "220 Anonymous FTP server ready.\r\n";
char* requirePassMsg = "331 Guest login ok, send your e-mail address as password.\r\n";
char* loginSuccessMsg = "230 Guest login ok, access restrictions apply.\r\n";
char* sysStatusMsg = "215 UNIX Type: L8.\r\n";
char* typeMsg = "200 Type set to I.\r\n";
char* portMsg = "200 PORT command successful.\r\n";
char* pasvMsgPart = "227 Entering passive mode";
char* startTransferPasvPart = "150 Opening BINARY mode data connection for ";
char* startTransferPortPart = "150 Opening BINARY mode data connection for ";
char* fileSentMsg = "226 Transfer complete.\r\n";
char* goodbyeMsg = "221 Goodbye.\r\n";

char* noPortError = "425 You need to use PORT or PASV command to establish TCP connection before every file transfer.\r\n";
char* networkError = "426 Your connection has been broken by network failure.\r\n";
char* fileNotExistError = "550 File does not exist.\r\n";
char* cannotConnectError = "425 Can't connect to given port.\r\n";
char* syntaxError = "500 Unacceptable syntax.\r\n";
char* typeError = "501 Wrong parameters.\r\n";
char* unknownUserError = "501 Unknown user.\r\n";
char* noAvailablePortError = "510 No available port.\r\n";
char* notLoginError = "530 You need to login first.\r\n";
