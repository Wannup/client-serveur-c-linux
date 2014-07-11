#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXLINE 500

bool sendAndReceive(int sock, char * buffer);

//verification si la reponse contient le code 221 au debut
bool isQuitCommand(char * recv){
	
	char reponse[3] = {"221"};
	int m;
	if(strlen(recv) > 3){
		for(m=0; m<3; m++){
			if(recv[m] != reponse[m])
				return false;
		}
		return true;
	}
}

bool isEndMail(char * send){

	char endMail[1] = {"."};
	int l;
	if(strlen(send) == 2){
		for(l=0; l<1; l++){
			if(send[l] != endMail[l])
				return false;		
		}
		return true;
	}
}

//verification si la reponse contient le code 354 au debut si oui saisie mail jusqu'à la saisie de " . "
void isDataCommand(char * recv, int sockfd){
	
	char reponse[3] = {"354"};
	int k;
	bool equal = true;
	char sendline[MAXLINE];

	if(strlen(recv) > 3){
		for(k=0; k<strlen(reponse); k++){
			if(recv[k] != reponse[k]){
				equal = false;
				break;
			}
		}
		if(equal){
			printf("Saisie du message:\n");
			while(fgets(sendline, MAXLINE, stdin) != NULL && !isEndMail(sendline)){
				write(sockfd, sendline, strlen(sendline)+1);
				memset(&sendline[0], 0, sizeof(sendline));			
			}
			sendAndReceive(sockfd, "\r\n.\r\n");
		}			
	}	
}

void sendSmtpBuffer(int sock, char * contentToSend){
   
	int size = strlen(contentToSend);
   	if(size > 0)
		send(sock, contentToSend, size, 0);
}

bool sendAndReceive(int hSocket, char * pszBuffer){
   
	char Buffer[MAXLINE]; int cb, cbBuffer;
   	sendSmtpBuffer(hSocket, pszBuffer);

   	/* Wait the \r\n*/
   	cbBuffer = 0;
   	while(1){
		cb = recv(hSocket, Buffer+cbBuffer, sizeof(Buffer)-1-cbBuffer, 0);
		if(cb <= 0)
   			break;
		
		cbBuffer += cb;
		if(memcmp(Buffer+cbBuffer-2, "\r\n", 2) == 0)
			break;
   	}

	if(cbBuffer > 0){
		Buffer[cbBuffer] = 0;
		write(1, Buffer, cbBuffer);
		isDataCommand(Buffer, hSocket);

		if(isQuitCommand(Buffer))
			return false;

		return true;
   	}

   	else{
		printf("Connection close\n");
		return false;
   	}
}

void actionsClient(int sockfd){

	char sendline[MAXLINE], recvline[MAXLINE];
	memset(&recvline[0], 0, sizeof(recvline));
	
	if (read(sockfd, recvline, MAXLINE) == 0){
		printf("Server terminated prematurely\n");
		exit(0);
	}

	fputs(recvline, stdout);

	while (fgets(sendline, MAXLINE, stdin) != NULL && sendAndReceive(sockfd, sendline))
		memset(&sendline[0], 0, sizeof(sendline));
	
}

int main(int argc, char **argv) {

	int	sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 3){
		printf("usage: smtpClient <IPaddress> <port>\n");
		exit(0);
	}
		
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

	actionsClient(sockfd);		

	exit(0);
}

