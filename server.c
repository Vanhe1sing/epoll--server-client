#include "mysock.h"
#define MAXLINE 1024
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

void setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock,F_GETFL);
	if(opts < 0)
	{
		perror("fcntl(sock,GETFL)");
		exit(1);
	}
	opts = opts | O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts) < 0)
	{
		perror("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}

int creatServer(int port)
{
	int i,maxi,listenfd,connfd,sockfd,epfd,nfds,portnumber;
	ssize_t n;
	char line[MAXLINE];
	socklen_t clilen;
	struct epoll_event ev,events[20];
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	clilen = sizeof(clientaddr);
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	setnonblocking(listenfd);
	epfd = epoll_create(256);
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	char *local_addr = "127.0.0.1";
	inet_aton(local_addr,&(serveraddr.sin_addr)),(sizeof(serveraddr));
	serveraddr.sin_port = htons(port);
	bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	listen(listenfd,LISTENQ);
	maxi = 0;
	for(;;)
	{
		nfds = epoll_wait(epfd,events,20,500);
		for(i = 0 ; i < nfds ; i++)
		{
			printf("@@@@@@\n");
			if(events[i].data.fd == listenfd)
			{
				connfd = accept(listenfd,(struct sockaddr *)&clientaddr,&clilen);
				if(connfd < 0)
				{
					perror("connfd < 0");
					close(listenfd);
					exit(1);
				}
				setnonblocking(connfd);
				
				char *str = inet_ntoa(clientaddr.sin_addr);
				printf("accept a connetion from %s\n",str);
				ev.data.fd = connfd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
			}
			else if(events[i].events&EPOLLIN)
			{
				printf("EPOLLIN\n");
				memset(line,0,MAXLINE);
				if((sockfd = events[i].data.fd) < 0)
					continue;
				if((n = read(sockfd,line,MAXLINE)) < 0)
				{
					if(errno == ECONNRESET)
					{
						close(sockfd);
						events[i].data.fd = -1;
					}
					else
					printf("readline error\n");
				}
				else if(n == 0)
				{
					close(sockfd);
					events[i].data.fd = -1;		
				}
				printf("n = %d\n",n);
//				line[n] = '\0';
//				printf("read:%s\n",line);//此后要对读出的数据处理在此后面进
				int tailLen = 0;
				while(1)
				{
					if(tailLen >= n)
					{
						break;
					}
					NODE *pnode = malloc(sizeof(NODE));
					if(pnode == NULL)
					{
						printf("malloc error\n");
						return -1;
					}
					memset(pnode,0,sizeof(NODE));
					int msglen;
					msglen = *((int *)line);
					msglen = ntohl(msglen);
                    printf("msglen = %d\n", msglen);
					memcpy(&(pnode->msghead), line+tailLen/*开始地址*/,msglen/*拷贝数据长度*/);
                    pnode->msghead.msglen = ntohl(pnode->msghead.msglen);
					pnode->msghead.msgtype = ntohl(pnode->msghead.msgtype);
					pnode->msghead.msgfunc = ntohl(pnode->msghead.msgfunc);
					pnode->msghead.mnuenum = ntohl(pnode->msghead.mnuenum);

					tailLen = tailLen + msglen;//已经读取的长度
					pnode->fd = sockfd;//文件描述传输很重要！！！！
					//insert lianbiao
					if(head.next == NULL)
					{
						head.next = pnode;
						head.tail = pnode;
					}
					else
					{
						head.tail->next = pnode;
						head.tail = pnode;
					}
				}
				ev.data.fd = sockfd;
				epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
			}
			else if(events[i].events&EPOLLOUT)
			{
				sockfd = events[i].data.fd;
				write(sockfd,line,n);
				ev.data.fd = sockfd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
			}
		}
	}
	close(sockfd);
	close(listenfd);
	close(connfd);
	return 0;
}
