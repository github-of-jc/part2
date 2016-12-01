/*
 * http-client.c - web page downloader
 * Jiayi Chen jc4489 Lab 6
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

void DieWithError(char *errorMessage);


void DieWithError(char *errorMessage)
{
	perror(errorMessage);
	exit(1);
}

int main(int argc, char **argv)
{
	printf("argc = %d\n", argc);

	if(argc != 4)
	{
		printf("usage: <host> <port number> <file path>\n");
		exit(1);
	}

	int sock; //socket descriptor
	struct sockaddr_in httpServAddr; //http server adddress
	unsigned short httpServPort; //http server port
	char *serverIP; 	//server IP address (dotted quad)
	char httpRequestStr[1000]; //string to send to http server
	char httpBuffer[1000]; //Buffer for http message ?
	unsigned int httpStringLen; //length of string from http
	int bytesRcvd=0;
        int totalBytesRcvd=0; //bytes read in single recv and total bytes read


	char *outFile=strrchr(argv[3], 47)+1;
	//printf("filename: %s\n", outFile);

	httpServPort = atoi(argv[2]); //use the given port number
	
	struct hostent *he; //dont know what this means
	char *serverName=argv[1]; //aka the host

	printf("severName: %s\n", serverName);

	//get server ip from server name
	if ((he = gethostbyname(serverName)) == NULL)
	{
		DieWithError("gethostbyname failed");
	}
	serverIP = inet_ntoa(*(struct in_addr *)he->h_addr);

	printf("serverIP: %s\n", serverIP);
	
	//open a socket to a host
	//create reliable, stream socket using TCP
	if((sock =socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
	{
		DieWithError("socket() failed");
	}
	
	printf("sock: %d\n", sock);


	//construct the server address structure
	memset(&httpServAddr, 0, sizeof(httpServAddr)); //"zero out structure", still don't know what i'm doing
	httpServAddr.sin_family =AF_INET; //Internet addr family (?)
	httpServAddr.sin_addr.s_addr = inet_addr(serverIP); //server IP address
	httpServAddr.sin_port = htons(httpServPort); //server port


	//establish the connection to the http server
	if(connect(sock, (struct sockaddr *) &httpServAddr, sizeof(httpServAddr))<0)
	{
		DieWithError("connect() failed");
	}

	printf("connected\n");

	//set up the httpRequestStr
	// GET /path/to/file/index.html HTTP/1.0
	//
	char *pathToFile=argv[3];
	printf("pathToFile: %s\n", pathToFile);

	sprintf(httpRequestStr, "GET %s HTTP/1.0\r\nHost: %s:80\r\n\r\n", pathToFile, argv[1]);

	printf("%s\n", httpRequestStr);


	
	//send request through the socket
	if(send(sock, httpRequestStr, strlen(httpRequestStr), 0) !=strlen(httpRequestStr))
	{
		DieWithError("send() sent a differrent num of bytes than expected");
	}

	//printf("hey you sent the request!!!\n");

	
	FILE *input = fdopen(sock, "r");
	char responseBuffer[1000];
	FILE *out=fopen(outFile, "w+");

	//save response, trim it, save to file
	totalBytesRcvd=0;
	printf("Received: ");
	//while(fgets(responseBuffer, 1, input) != NULL)
	bytesRcvd=1;
	while(bytesRcvd > 0 )
	{
		if((bytesRcvd = recv(input, httpBuffer, 1000-1, 0))<0)
		{
			DieWithError("recv() failed or connection closed prematurely");
		}
		
		totalBytesRcvd += bytesRcvd;
		httpBuffer[bytesRcvd] = '\0';
		printf("%d\n", bytesRcvd);
		//printf("%s", httpBuffer);
		fputs(httpBuffer, out);

	}
	fclose(out);
	fclose(input);

	printf("bR: %d, tbr: %d\n", bytesRcvd, totalBytesRcvd);
	printf("\n");

	close(sock);


	//close the socket (?)


	return 0;

}
