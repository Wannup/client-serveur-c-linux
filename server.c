#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"
#include "client.h"

static void init(void){
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void){
#ifdef WIN32
   WSACleanup();
#endif
}

static void server(void){

}

static int init_connection(void){
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET){
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR){
      perror("bind()");
      exit(errno);
   }

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR){
      perror("listen()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock){
   closesocket(sock);
}

static void recv_mail(SOCKET sock, char *buffer){
   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0){
      perror("recv()");
      n = 0;
   }
}

static void send_mail(SOCKET sock, const char *buffer){
   if(send(sock, buffer, strlen(buffer), 0) < 0){
      perror("send()");
      exit(errno);
   }
}

int main(int argc, char *argv[]){
    init();
    app();
    end();

    return EXIT_SUCCESS;
}
