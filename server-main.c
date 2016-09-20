#include "mysock.h"

typedef struct foodinf
{
	int ID;
	char name[20];
	char price[20];
	struct foodinf *next;
}FOODINF;

typedef struct indent
{
	char username[20];
	char storename[20];
	int foodnum;
	char state[20];
	struct indent *next;
}INDENT;

int Research(NODE* pnode)
{
	char sql[1024];
	int ret;
	MYSQL_RES *ptr;
	MYSQL_ROW result;
	MYSQL db,*pdb;
	FOODINF *head = NULL;
	FOODINF *p;
	mysql_init(&db);
	pdb = mysql_real_connect(&db,"localhost","root","123456","resturant",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect failed!!\n");
	}	
	else
	{
		printf("mysql connect success!!\n");
	}
	sprintf(sql,"select * from %s;",pnode->msghead.storename);
	ret = mysql_query(&db,sql);
	MYSQL_FIELD *field;
	int row,column;
	int i,j;
	char foodinf[1024];
	memset(foodinf,0,1024);
	ptr = mysql_store_result(&db);
	row = mysql_num_rows(ptr);
	column = mysql_num_fields(ptr);
	printf("row = %d column = %d\n",row,column);
	for(;field = mysql_fetch_field(ptr) ; )
	{
		printf("%s\t",field->name);
		strcat(foodinf,field->name);
		strcat(foodinf,"     ");
	}
	printf("\n");
	strcat(foodinf,"\n");
	for(i = 0;i < row;i++)
	{
		result = mysql_fetch_row(ptr);
		for(j = 0;j<column;j++)
		{
			printf("%s\t",result[j]);
			strcat(foodinf,result[j]);
			strcat(foodinf,"     ");
		}
		printf("\n");
		strcat(foodinf,"\n");
	}
	ret = write(pnode->fd,foodinf,strlen(foodinf));
	if(ret < 0)
	{
		printf("write error!!\n");
		mysql_free_result(ptr);
		mysql_close(&db);
		return 0;
	}
	char *pstr;
	printf("pnode->msghead.storename = %s\n",pnode->msghead.storename);
	if(strcmp(pnode->msghead.storename,"indent") == 0)
	{
		pstr = "indent information back!!";
	}
	else
	{
		pstr = "food information back!!";
	}
	ret = write(pnode->fd,pstr,strlen(pstr));	
	if(ret < 0)
	{
		printf("write error!!\n");
		mysql_free_result(ptr);
		mysql_close(&db);
		return 0;
	}
	mysql_free_result(ptr);
	mysql_close(&db);
	return 0;
}

void Buyfood(char *username,char *storename,int foodnum)
{
	char sql[1024];
	MYSQL db,*pdb;
	INDENT *head = NULL;
	INDENT *p;
	mysql_init(&db);
	pdb = mysql_real_connect(&db,"localhost","root","123456","resturant",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect error\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	p = (INDENT *)malloc(sizeof(INDENT));
	assert(p != NULL);
	strcpy(p->username,username);
	strcpy(p->storename,storename);
	p->foodnum = foodnum;
	strcpy(p->state,"ING");
	p->next = head;
	head = p;
	p = head;
	sprintf(sql,"insert into indent values(\'%s\',\'%s\',%d,\'%s\')",p->username,p->storename,p->foodnum,p->state);
	if(mysql_query(&db,sql))
	{
		printf("insert failed\n");
		return;
	}
	p = p->next;
	mysql_close(&db);
	return;
}

void Chaxun(NODE *pnode)
{	
	char sql[1024];
	int ret;
	MYSQL_RES *ptr;
	MYSQL_ROW result;
	MYSQL db,*pdb;
	mysql_init(&db);
	pdb = mysql_real_connect(&db,"localhost","root","123456","resturant",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect failed!!\n");
	}	
	else
	{
		printf("mysql connect success!!\n");
	}
	sprintf(sql,"select username,storename,foodnum,state from indent where username = \'%s\';",pnode->msghead.name);
	ret = mysql_query(&db,sql);
	MYSQL_FIELD *field;
	int row,column;
	int i,j;
	char foodinf[1024];
	memset(foodinf,0,1024);
	ptr = mysql_store_result(&db);
	row = mysql_num_rows(ptr);
	column = mysql_num_fields(ptr);
	printf("row = %d column = %d\n",row,column);
	for(;field = mysql_fetch_field(ptr) ; )
	{
		printf("%s\t",field->name);
		strcat(foodinf,field->name);
		strcat(foodinf,"     ");
	}
	printf("\n");
	strcat(foodinf,"\n");
	for(i = 0;i < row;i++)
	{
		result = mysql_fetch_row(ptr);
		for(j = 0;j<column;j++)
		{
			printf("%s\t",result[j]);
			strcat(foodinf,result[j]);
			strcat(foodinf,"     ");
		}
		printf("\n");
		strcat(foodinf,"\n");
	}
	ret = write(pnode->fd,foodinf,strlen(foodinf));
	if(ret < 0)
	{
		printf("write error!!\n");
		mysql_free_result(ptr);
		mysql_close(&db);
		return;
	}
	mysql_free_result(ptr);
	mysql_close(&db);
	return;
}

void Cancel(NODE* pnode)
{
	char uname[20];
	char ustate[20];
	char sql[1024];
	MYSQL db,*pdb;
	mysql_init(&db);
	pdb = mysql_real_connect(&db,"localhost","root","123456","resturant",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect error\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	strcpy(uname,pnode->msghead.name);
	strcpy(ustate,"cancel");
	sprintf(sql,"update indent set state = \'%s\' where username = \'%s\'",ustate,uname);
	if(mysql_query(&db,sql))
	{
		printf("delete failed\n");
		return;
	}
	mysql_close(&db);
	return;
}

void Receive_Refuse(NODE* pnode, char *function)
{
	char uname[20];
	char ustate[20];
	char sql[1024];
	MYSQL db,*pdb;
	mysql_init(&db);
	pdb = mysql_real_connect(&db,"localhost","root","123456","resturant",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect error\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	strcpy(uname,pnode->msghead.name);
	strcpy(ustate,function);
	sprintf(sql,"update indent set state = \'%s\' where username = \'%s\'",ustate,uname);
	if(mysql_query(&db,sql))
	{
		printf("delete failed\n");
		return;
	}
	mysql_close(&db);
	return;
}

void *handler(void *arg)
{
	NODE *pnode;
	int ret,i,retw,retr,fd;
	char np[20];
	char buf[20];
	char pstr[50];
	char name[10];
	FILE *fp;
	memset(pstr,0,50);
	while(1)
	{
		if(head.next != NULL)
		{
			pnode = head.next;
			head.next = head.next->next;
			memset(pstr,0,50);
			if(pnode->msghead.msgtype == MSGTYPE_CLIENTFOOD)//用户客户端
			{
				if(pnode->msghead.msgfunc == MSGFUNC_LOADING)//登录
				{
					strcat(pnode->msghead.name," ");
					strcat(pnode->msghead.name,pnode->msghead.keys);
					strcat(pnode->msghead.name,"\n");
					fp = fopen("loadinguser","r");
					if(fp == NULL)
					{
						return NULL;
					}
					while(fgets(np,sizeof(np),fp) != NULL)
					{
						if(strcmp(pnode->msghead.name,np) != 0)
						{
							memset(np,0,sizeof(np));
							continue;
						}
						strcpy(pstr,"loading success!!");
						ret = write(pnode->fd,pstr,strlen(pstr));	
						break;
					}
					if(strcmp(pstr,"loading success!!") != 0)
					{
						strcpy(pstr,"loading error!!");
						ret = write(pnode->fd,pstr,strlen(pstr));
						if(ret < 0)
						{
							printf("write error\n");
							return NULL;
						}	
					}
					fclose(fp);
					continue;
				}
				if(pnode->msghead.msgfunc == MSGFUNC_LOGIN)//注册
				{
					strcat(pnode->msghead.name," ");
					strcat(pnode->msghead.name,pnode->msghead.keys);
					strcat(pnode->msghead.name,"\n");
					fd = open("loadinguser",O_WRONLY | O_APPEND);
					if(fd < 0)
					{
						printf("open error\n");
						return NULL;		
					}
					retw = write(fd,pnode->msghead.name,strlen(pnode->msghead.name));
					strcpy(pstr,"login success!!");
					ret = write(pnode->fd,pstr,strlen(pstr));
					if(ret < 0)
					{
						printf("write error\n");
						close(fd);
						return NULL;
					}	
				}
				if(pnode->msghead.msgfunc == MSGFUNC_REARCH)//查询菜单
				{
					Research(pnode);
				}
				if(pnode->msghead.msgfunc == MSGFUNC_BUYFOOD)//下单
				{
					Buyfood(pnode->msghead.name,pnode->msghead.storename,pnode->msghead.mnuenum);
					strcpy(pstr,"buyfood success!!");
					ret = write(pnode->fd,pstr,strlen(pstr));				
				}
				if(pnode->msghead.msgfunc == MSGFUNC_CXINDENT)//查询订单
				{
					Chaxun(pnode);
					strcpy(pstr,"cxindent success!!");
					ret = write(pnode->fd,pstr,strlen(pstr));
				}
				if(pnode->msghead.msgfunc == MSGFUNC_CANCEL)//取消订单
				{
					Cancel(pnode);
					strcpy(pstr,"cancel success!!");
					ret = write(pnode->fd,pstr,strlen(pstr));				
				}
			}
			if(pnode->msghead.msgtype == MSGTYPE_SERVERFOOD)//服务员程序
			{
				if(pnode->msghead.msgfunc == MSGFUNC_LOADING)//登录
				{
					strcat(pnode->msghead.name," ");
					strcat(pnode->msghead.name,pnode->msghead.keys);
					strcat(pnode->msghead.name,"\n");
					fp = fopen("loadinguser","r");
					if(fp == NULL)
					{
						return NULL;
					}
					while(fgets(np,sizeof(np),fp) != NULL)
					{
						if(strcmp(pnode->msghead.name,np) != 0)
						{
							memset(np,0,sizeof(np));
							continue;
						}
						strcpy(pstr,"loading success!!");
						ret = write(pnode->fd,pstr,strlen(pstr));	
						break;
					}
					if(strcmp(pstr,"loading success!!") != 0)
					{
						strcpy(pstr,"loading error!!");
						ret = write(pnode->fd,pstr,strlen(pstr));
						if(ret < 0)
						{
							printf("write error\n");
							return NULL;
						}	
					}
					fclose(fp);
					continue;
				}
				if(pnode->msghead.msgfunc == MSGFUNC_LOGIN)//注册
				{
					strcat(pnode->msghead.name," ");
					strcat(pnode->msghead.name,pnode->msghead.keys);
					strcat(pnode->msghead.name,"\n");
					fd = open("loadinguser",O_WRONLY | O_APPEND);
					if(fd < 0)
					{
						printf("open error\n");
						return NULL;		
					}
					retw = write(fd,pnode->msghead.name,strlen(pnode->msghead.name));
					strcpy(pstr,"login success!!");
					ret = write(pnode->fd,pstr,strlen(pstr));
					if(ret < 0)
					{
						printf("write error\n");
						close(fd);
						return NULL;
					}	
				}
				if(pnode->msghead.msgfunc == MSGFUNC_REARCH)//查询订单
				{
					Research(pnode);
				}
				if(pnode->msghead.msgfunc == MSGFUNC_RECEIVE)//接单
				{
					char *function = "receive";
					Receive_Refuse(pnode,function);
					strcpy(pstr,"receive client success!!");
					ret = write(pnode->fd,pstr,strlen(pstr));				
				}
				if(pnode->msghead.msgfunc == MSGFUNC_REFUSE)//拒单
				{
					char *function = "refuse";
					Receive_Refuse(pnode,function);
					strcpy(pstr,"refuse client success!!");
					ret = write(pnode->fd,pstr,strlen(pstr));				
				}
			}
		}
	}
	return NULL;
}

void *createServer(void *arg)
{
	int ret;
	ret = creatServer(atoi((char *)arg));
	return NULL;
}

int main(int argc, char **argv)
{
	int ret;
	pthread_t pth_id;
	pthread_t pth_id1;

	memset(&head,0,sizeof(head));

	ret = pthread_create(&pth_id,NULL,handler,NULL);
	if(ret < 0)
	{
		printf("create error\n");
		return -1;
	}

	ret = pthread_create(&pth_id,NULL,createServer,argv[1]);
	if(ret < 0)
	{
		printf("create error\n");
		return -1;
	}

	pthread_join(pth_id,NULL);
	pthread_join(pth_id1,NULL);
	return 0;
}


