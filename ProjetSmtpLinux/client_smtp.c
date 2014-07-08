#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<signal.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<errno.h>
#include 	<string.h>

#define MAXLINE 200
void str_cli(FILE *, int);

int
main(int argc, char **argv)
{
	int	sockfd;
	struct sockaddr_in	servaddr;
	FILE *fp;
	

	if (argc != 3)
		{
		printf("usage: smtpClient <IPaddress> <port>\n");
		exit(-1);
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

void
str_cli(FILE *fp, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];

	if (read(sockfd, recvline, MAXLINE) == 0)
	{
		printf("erreur reception message de connexion au serveur\n");
		exit(-1);
	}
	fputs(recvline, stdout);
	memset(&recvline[0], 0, sizeof(recvline));
	while (fgets(sendline, MAXLINE, fp) != NULL) {

		
		write(sockfd, sendline, strlen(sendline)+1);

		if (read(sockfd, recvline, MAXLINE) == 0)
			{
			printf("str_cli: server terminated prematurely\n");
			exit(-1);
			}
		fputs(recvline, stdout);
		memset(&recvline[0], 0, sizeof(recvline));
	}
}


