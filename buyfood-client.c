#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include "mysock.h"

int fd;
int flag = 0;
int flag_loading = 0;
int flag_Select = 0;

void Select(int b)
{
	int real_write;
    MSGHEAD msghead;
	switch(b)
	{
		case 1://查询菜单
		{
			printf("餐厅：xnzf / shyj\n");
			printf("请选择要查询信息的餐厅名称：\n");
			scanf("%s",msghead.storename);
			msghead.msgtype = MSGTYPE_CLIENTFOOD;
			msghead.msgfunc = MSGFUNC_REARCH;
			msghead.msglen = htonl(sizeof(msghead));
			msghead.msgtype =htonl(msghead.msgtype);
			msghead.msgfunc =htonl(msghead.msgfunc);
			msghead.mnuenum =htonl(msghead.mnuenum);
			real_write = write(fd,&msghead,sizeof(msghead));
			if(real_write > 0)
			{
				printf("已发送查询餐厅名称！！\n");
			}
			while(flag == 1)
			{
				;
			}
			if(flag == 4)
			{
				flag_Select = 0;
	//			printf("请选择服务项目：1查询/2下单/3取消订单/4退出\n");
	//			scanf("%d",&e);
				flag = 1;
	//			Select(e);
			}
			break;	
		}
		case 2://订餐
		{
			printf("请输入要订餐的客户名：\n");
			scanf("%s",msghead.name);
			printf("请输入要订餐的餐厅名：\n");
			scanf("%s",msghead.storename);
			printf("请输入要订餐的餐号：\n");
			scanf("%d",&msghead.mnuenum);
			msghead.msglen = sizeof(msghead);
			msghead.msgtype = MSGTYPE_CLIENTFOOD;
			msghead.msgfunc = MSGFUNC_BUYFOOD;
			msghead.msglen = htonl(msghead.msglen);
			msghead.msgtype = htonl(msghead.msgtype);
			msghead.msgfunc = htonl(msghead.msgfunc);
			msghead.mnuenum = htonl(msghead.mnuenum);
			printf("111\n");
			real_write = write(fd,&msghead,sizeof(msghead));
			printf("222\n");
			if(real_write > 0)
			{
				printf("下单信息已经发送！！");
			}
			while(flag == 1)
			{
				;
			}
			if(flag == 5)
			{
				flag_Select = 0;
			//	printf("请选择服务项目：1查询/2下单/3取消订单/4退出\n");
			//	scanf("%d",&e);
				flag = 1;
			//	Select(e);
			}
			break;
		}
		case 3://查询订单
		{
			printf("请输入下单客户姓名:\n");
			scanf("%s",msghead.name);
			msghead.msglen = sizeof(msghead);
			msghead.msgtype = MSGTYPE_CLIENTFOOD;
			msghead.msgfunc = MSGFUNC_CXINDENT;
			msghead.msglen = htonl(msghead.msglen);
			msghead.msgtype = htonl(msghead.msgtype);
			msghead.msgfunc = htonl(msghead.msgfunc);
			msghead.mnuenum = htonl(msghead.mnuenum);
			real_write = write(fd,&msghead,sizeof(msghead));
			if(real_write > 0)
			{
				printf("申请信息已经发送！！");
			}
			while(flag == 1)
			{
				;
			}
			if(flag == 7)
			{
				flag_Select = 0;
				flag = 1;
			}
			break;
		}
		case 4://取消订单
		{
			printf("请输入要取消的订单的用户名:\n");
			scanf("%s",msghead.name);
			msghead.msglen = sizeof(msghead);
			msghead.msgtype = MSGTYPE_CLIENTFOOD;
			msghead.msgfunc = MSGFUNC_CANCEL;
			msghead.msglen = htonl(msghead.msglen);
			msghead.msgtype = htonl(msghead.msgtype);
			msghead.msgfunc = htonl(msghead.msgfunc);
			msghead.mnuenum = htonl(msghead.mnuenum);
			real_write = write(fd,&msghead,sizeof(msghead));
			if(real_write > 0)
			{
				printf("取消订单申请已发送！！\n");
			}
			while(flag == 1)
			{
				;
			}
			if(flag == 6)
			{
				flag_Select = 0;
			//	printf("请选择服务项目：1查询/2下单/3取消订单/4退出\n");
			//	scanf("%d",&e);
				flag = 1;
			//	Select(e);		
			}
			break;
		}
		case 5://退出
		{
			exit(0);
		}
	}
	return;
}

void loading(int a)
{
	int real_write;
    int real_read;
    char buf[1024];
	char name[20];
	char keys[20];
	char newname[20];
	char newkeys[20];
    MSGHEAD msghead;
	switch(a)
	{
		case 1:
		{
			memset(&msghead,0,sizeof(msghead));
			printf("请输入用户名：\n");
			scanf("%s",name);
			printf("请输入密码\n");
			scanf("%s",keys);
			msghead.msglen = sizeof(msghead);
			msghead.msgtype = MSGTYPE_CLIENTFOOD;
			msghead.msgfunc = MSGFUNC_LOADING;
			strcpy(msghead.name,name);
			strcpy(msghead.keys,keys);
			msghead.msglen = htonl(sizeof(msghead));
			msghead.msgtype =htonl(msghead.msgtype);
			msghead.msgfunc =htonl(msghead.msgfunc);
			msghead.mnuenum =htonl(msghead.mnuenum);
			real_write = write(fd,&msghead,sizeof(msghead));
			if(real_write > 0)
			{
				printf("clientfood send msg to server\n");
			}
			while(flag == 0)
			{
				;
			}
			if(flag == 1)
			{
				flag_loading = 1;
		//		printf("请选择服务项目：1.查询/2.下单/3.取消订单/4退出\n");
		//		scanf("%d",&b);
		//		Select(b);
			}
			if(flag == 2)
			{
				flag_loading = 0;
		//		a = 1;
				flag = 0;
		//		loading(a);
			}
			break;
		}
		case 2:
		{
			memset(&msghead,0,sizeof(msghead));
			printf("您需要注册一个新的账户！！\n");
			printf("请输入新的用户名：\n");
			scanf("%s",newname);
			printf("请输入新的密码：\n");
			scanf("%s",newkeys);
			msghead.msglen = sizeof(msghead);
			msghead.msgtype = MSGTYPE_CLIENTFOOD;
			msghead.msgfunc = MSGFUNC_LOGIN;
			strcpy(msghead.name,newname);
			strcpy(msghead.keys,newkeys);
			msghead.msglen = htonl(msghead.msglen);
			msghead.msgtype = htonl(msghead.msgtype);
			msghead.msgfunc = htonl(msghead.msgfunc);
			msghead.mnuenum = htonl(msghead.mnuenum);
			real_write = write(fd,&msghead,sizeof(msghead));
			if(real_write > 0)
			{
				printf("已发送注册信息到服务器！！！\n");
			}
			while(flag == 0)
			{
				;
			}
			if(flag == 3)
			{
				flag_loading = 0;
		//		a = 1;
				flag = 0;
		//		loading(a);
			}
			break;
		}
	}
	return;
}

void *handler(void *arg)
{
	int a;
	int b;
	while(flag_loading == 0)
	{
		printf("请选择用户：1.老用户/2.新用户\n");
		scanf("%d",&a);
		loading(a);
	}
	while(flag_Select == 0)	
	{
		printf("请选择服务项目：1查询菜单/2下单/3查询订单/4取消订单/5退出\n");
		scanf("%d",&b);
    	Select(b);
	}
	return NULL;
}

int main(int argc,char **argv)
{
	int ret;
	struct sockaddr_in clientAddr;
	
	if(argc != 3)
	{
		printf("para error\n");
		return -1;
	}
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd < 0)
	{
		printf("socket error\n");
		return -1;
	}	

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(atoi(argv[2]));
	clientAddr.sin_addr.s_addr = inet_addr(argv[1]);
	
	ret = connect(fd,(struct sockaddr *)&clientAddr,sizeof(clientAddr));
	if(ret < 0)
	{
		printf("connect error\n");
		return -1;
	}//以上建立好了网络通信通道
	pthread_t pth_id;
	char buf[1024];
	ret = pthread_create(&pth_id,NULL,handler,NULL);
	if(ret < 0)
	{
		printf("create error\n");
		return -1;
	}
	while(1)
	{
		memset(buf,0,1024);
//		printf("read start\n");
		ret = read(fd,buf,1024);
//		printf("buf = %s\n",buf);
//		printf("read ok\n");
		if(ret > 0)
		{
			printf("%s\n",buf);
		//	printf("buf = %s\n",buf);
            if(strcmp(buf,"loading success!!") == 0)
            {
				flag = 1;
			}
			if(strcmp(buf,"loading error!!") == 0)
			{
				flag = 2;
			}
			if(strcmp(buf,"login success!!") == 0)
			{
				flag = 3;
			}
			if(strcmp(buf,"food information back!!") == 0)
			{
				flag = 4;
			}
			if(strcmp(buf,"buyfood success!!") == 0)
			{
				flag = 5;
			}
			if(strcmp(buf,"cancel success!!") == 0)
			{
				flag = 6;
			}
			if(strcmp(buf,"cxindent success!!") == 0)
			{
				flag = 7;
			}
		}
		if(ret = 0)
		{
			printf("client no msg\n");
			sleep(1);
		}
	}

	close(fd);
	return 0;
}
