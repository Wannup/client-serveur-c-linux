#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXLINE 200

// RFC 5321

void traitementclient(int connfd)
{
	char commandes[5][15] = {{"EHLO"}, {"MAIL FROM"}, {"RCPT TO"}, {"DATA"}, {"QUIT"}};
	char reponses[6][100] = {
		{"220 SMTP Service Ready\n"},
		{"250 YourSmtpServer\n"},
		{"250 Requested mail action okay, completed\n"},
		{"354 Enter mail, end with . on a line by itself\n"},
		{"221 Service closing transmission channel\n"},
		{"500 Syntax error, command unrecognized\n"} 
	};

	write(connfd,reponses[0],strlen(reponses[0]));
	bool quit = false;
	
	while(!quit){
		char recvline[MAXLINE];
		if (read(connfd, recvline, MAXLINE) == 0)
			quit = true;
			
		if(!quit){
			//traitement sur recvline : verifier présence d'une commande SMTP
			int commande = -1;
			int i, j;
			const char c= ' ';
		
			if (strlen(recvline) > 4){
				for(i=0; i<5; i++){
					if(recvline[0] == commandes[i][0]){
						for(j=1; j< strlen(commandes[i]); j++){
							if(recvline[j] != commandes[i][j]){
								break;
							}
							else if((j+1)==strlen(commandes[i]))
								commande = i;
						}			
					}
					if(commande != -1) 
						break;
				}
			}
			// commande smtp présente ou pas
			switch (commande)
			{
			case 0:
			  	write(connfd,reponses[1],strlen(reponses[1]));
			  	printf("EHLO \n");
			  break;
			case 1:
			    write(connfd,reponses[2],strlen(reponses[2]));
			  	printf("MAIL FROM \n");
			  break;
			case 2:
			  	write(connfd,reponses[2],strlen(reponses[2]));
			  	printf("RCPT TO \n");
			  break;
			case 3:
			  	write(connfd,reponses[3],strlen(reponses[3]));
			  	printf("DATA \n");
			  break;
			case 4:
				write(connfd,reponses[4],strlen(reponses[4]));
			  	printf("QUIT \n");
				quit = true;
			  break;
			default:
			  write(connfd,reponses[5],strlen(reponses[5]));
			  break;
			}
		}
	}
}

main(int argc, char *argv[]){

	int listenfd,connfd,port;
	socklen_t len;
	struct sockaddr_in servaddr,cliaddr;
	char adresseIP[16],buff[50];
	struct sigaction zombies;

	if (argc!=2)
	{
		printf("Usage : serveurSmtp <port>\n");
		exit(-1);
	}

	//les fils ne passent pas par l'état zombie
	signal(SIGCHLD,SIG_IGN);

	listenfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));//mise a zero de la structure

	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(atoi(argv[1]));

	bind(listenfd,(struct sockaddr *)& servaddr,sizeof(servaddr));
	listen(listenfd,10);

	for(;;)
	{
		len=sizeof(cliaddr);	
		connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&len);
		port=ntohs(cliaddr.sin_port);
		inet_ntop(AF_INET,&cliaddr.sin_addr,adresseIP,sizeof(adresseIP));
		printf("connexion de %s, port %d\n",adresseIP,port);
		if(fork()==0)
		{
			close(listenfd);
			traitementclient(connfd);//fonction chargée de travailler avec le client
			close(connfd);
			exit(0);
		}
	
		close(connfd);
	}
}
