#ifndef CLIENT_H
#define CLIENT_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define PORT	 1566

#define BUF_SIZE 1024

static void init(void);
static void end(void);
static void client(const char *address, const char *name);
static int init_connection(const char *address, SOCKADDR_IN *sin);
static void end_connection(int sock);
static void send_mail(SOCKET sock, SOCKADDR_IN *sin, const char *buffer);

#endif
