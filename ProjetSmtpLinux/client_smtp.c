#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#define MAXLINE 200

//verification si la reponse contient le code 221 au debut
bool isQuitCommand(char * recv){
	
	char reponse[3] = {"221"};
	int m;
	if(strlen(recv) > 3){
		for(m=0; m<strlen(reponse); m++){
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
	char sendline[MAXLINE], recvline[MAXLINE];

	if(strlen(recv) > 3){
		for(k=0; k<strlen(reponse); k++){
			if(recv[k] != reponse[k]){
				equal = false;
				break;
			}
		}
		if(equal){
			printf("Saisi du message:\n");
			while(fgets(sendline, MAXLINE, stdin) != NULL && !isEndMail(sendline)){
				write(sockfd, sendline, strlen(sendline)+1);
				memset(&sendline[0], 0, sizeof(sendline));			
			}
			write(sockfd, sendline, strlen(sendline)+1);
			if (read(sockfd, recvline, MAXLINE) == 0){
				printf("Server terminated prematurely\n");
				exit(0);
			}	
			fputs(recvline, stdout);	
		}			
	}
	
}

void str_cli(FILE *fp, int sockfd){

	char sendline[MAXLINE], recvline[MAXLINE];

	if (read(sockfd, recvline, MAXLINE) == 0)
	{
		printf("Server terminated prematurely\n");
		exit(0);
	}
	fputs(recvline, stdout);
	memset(&recvline[0], 0, sizeof(recvline));
	while (fgets(sendline, MAXLINE, fp) != NULL) {

		write(sockfd, sendline, strlen(sendline)+1);

		if (read(sockfd, recvline, MAXLINE) == 0)
			{
			printf("Server terminated prematurely\n");
			exit(0);
			}
		fputs(recvline, stdout);
		isDataCommand(recvline, sockfd);

		if(isQuitCommand(recvline))
			break;

		memset(&recvline[0], 0, sizeof(recvline));
		memset(&sendline[0], 0, sizeof(sendline));
	}
}

int main(int argc, char **argv) {

	int	sockfd;
	struct sockaddr_in	servaddr;
	FILE *fp;
	

	if (argc != 3)
		{
		printf("usage: smtpClient <IPaddress> <port>\n");
		exit(0);
		}
		
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

	fp=stdin;
	str_cli(fp, sockfd);		

	exit(0);
}

