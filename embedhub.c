#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "embedhub.h"
#include "bitmap.h"
#include "readcfg.h"

pthread_t pthid_timer;
pthread_t pthid_tcpconn;

void timer(void);//计时进程，留做备用。
void *tcprecvfromcli(void *arg);//TCP客户端接收线程

int tcpconn(void)
{
	int ser_sockfd;
	int opt = 1;
	int userid;
	socklen_t addlen;
	struct sockaddr_in ser_addr;
	
	ser_sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(ser_sockfd==-1)
	{
		printf("socket error:%s\n",strerror(errno));
		return -1;
	}
	
	setsockopt(ser_sockfd, SOL_SOCKET,SO_REUSEADDR, (const void *) &opt, sizeof(opt));
	
	bzero(&ser_addr,sizeof(ser_addr));
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	ser_addr.sin_port=htons(EMBSERPORT);
	
	if(bind(ser_sockfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr)) == -1)
	{
		printf("绑定套接字失败:%s\n",strerror(errno));
		return -1;
	}
	
	if(listen(ser_sockfd,5) == -1)
	{
		printf("监听失败：%s\n", strerror(errno));
		return -1;
	}
	
	printf("监听端口：%d\n",EMBSERPORT);
	fflush(stdout);
	while(1)
	{
		userid = getbitmap();
		//printf("User ID is :%d\n", userid);
		if(userid == -1)
		{
			printf("连接已达到系统最大限度。等待释放中。。。\n");
			while( (userid = getbitmap()) == -1)
			{
				printf(" * ");
				fflush(stdout);
				sleep(2);
			}
			printf("有客户端断开，可以进行新的连接\n");
		}
		
		addlen = sizeof(struct sockaddr);
		
		user[userid].fd = accept(ser_sockfd, (struct sockaddr *)&user[userid].cli_addr, &addlen);

		if(user[userid].fd == -1)
		{
			printf("连接建立失败：%s\n", strerror(errno));
		}
		else
		{
			printf("客户端( %d )连接已经建立！:%s:%d\n", userid, (char *)inet_ntoa(user[userid].cli_addr.sin_addr), user[userid].cli_addr.sin_port);
			setbitmap(userid);
		}
		
		pthread_create(&user[userid].pthid, NULL, &tcprecvfromcli, &userid);
		
		usleep(100);
		
	}
	
	close(ser_sockfd);
	
	return 0;
}

void signal_handler(int sig)
{
	bitmap_destroy();
	free(user);
	printf("Program have exit !\n");
	
	exit(0);
}

int sysinit(void)
{
	int maxusr, ret;
	
	printf("The Program complete Date(%s) Time(%s)\n", __DATE__, __TIME__);
	
	readcfgintdef("MaxUserNum", &maxusr);
	ret = bitmap_init(maxusr);
	if(ret < 0)
	{
		exit(EXIT_FAILURE);
	}
	printf("Max user number is :%d\n", maxusr);
	
	user = (embser *)malloc(sizeof(embser) * maxusr);
	if(user == NULL)
	{
		printf("malloc user error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	memset(user, 0, sizeof(embser) * maxusr);
	
	if (signal(SIGINT, signal_handler) == SIG_ERR) {
		exit(EXIT_FAILURE);
	}
	
	return 0;
}

int main(void)
{
	sysinit();
	
	pthread_create(&pthid_timer, NULL, (void *)timer, NULL);
	pthread_create(&pthid_tcpconn, NULL, (void *)tcpconn, NULL);
	
	pthread_join(pthid_tcpconn, NULL);
	
	return 0;
}

void *tcprecvfromcli(void *arg)
{
	int recv_len, err;
	int userid;
	
	pthread_detach(pthread_self());
	
	userid = *(int *)arg;

	while(1)
	{
		recv_len = recv(user[userid].fd, user[userid].buf, MAXBUFSIZE, 0);
		//send(user[userid].fd, user[userid].buf, recv_len, 0);
		if(recv_len <= 0)
		{
			err = errno;
			
			if((err != EAGAIN) && (err != EWOULDBLOCK) && (err != EINTR)){
				printf("TCP连接( %d )已经断开！\n", userid);fflush(stdout);
				
				resetbitmap(userid);
				close(user[userid].fd);
				break;
			}
		}

		user[userid].buf[recv_len]='\0';
		
		printf("客户端( %d )收到%d字节数据：%s\n", userid, recv_len, user[userid].buf);fflush(stdout);
	}
	close(user[userid].fd);
	
	return NULL;
}

void timer(void)
{
	unsigned int count = 0;
	
	pthread_detach(pthread_self());
	
	while(1)
	{
		sleep(1);
		count++;
	}
}