#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client.h"

static void init(void){}
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

static void client(const char *subject, const char *receiver, const char *mail){

}

static int init_connection(const char *address, SOCKADDR_IN *sin){

}

static void end_connection(int sock){

}

static void send_mail(SOCKET sock, SOCKADDR_IN *sin, const char *buffer){

}

int main(int argc, char *argv[]){
    if(argc < 3){ // Le sujet, le destinataire et le corp du mail.
        return EXIT_FAILURE;
    }

    init();
    app(argv[1], argv[2], argv[3]);
    end();
}
