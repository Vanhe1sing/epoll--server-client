#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mysql/mysql.h>

#define MSGTYPE_CLIENTFOOD 1
#define MSGTYPE_SERVERFOOD 2
#define MSGFUNC_LOGIN      3
#define MSGFUNC_LOADING    4
#define MSGFUNC_REARCH     5
#define MSGFUNC_BUYFOOD    6
#define MSGFUNC_CANCEL     7 
#define MSGFUNC_RECEIVE    8
#define MSGFUNC_REFUSE     9
#define MSGFUNC_CXINDENT   10

typedef struct msghead
{
	int msglen;
	int msgtype;
	int msgfunc;
	char name[20];
	char keys[20];
	char storename[20];
	int mnuenum;
}MSGHEAD;



typedef struct node
{
	MSGHEAD msghead;
	int fd;
	struct node * tail;
	struct node * next;
}NODE;

NODE head;

void setnoblocking(int sock);

int creatServer(int port);


