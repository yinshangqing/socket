/*************************************************************************
	> File Name: p2psrv.c
	> Author: yinshangqing
	> Mail: 841668821@qq.com 
	> Created Time: 2018年02月01日 星期四 22时45分02秒
 ************************************************************************/
// 点对点通信
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

void handler(int sig)
{
	printf("recv a sig=%d\n",sig);
	exit(EXIT_SUCCESS);
}

int main()
{
	int listenfd;
	//listenfd = socket(PF_INET,SOCK_STREAM,0);
	if((listenfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
	{
		ERR_EXIT("socket");
	}
	struct sockaddr_in seraddr;
	memset(&seraddr,0,sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(8080);
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);// 任何地址
	//seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//inet_aton("127.0.0.1",&seraddr.sin_addr);
	
	int on = 1;
	// REUSEADDR 在 TIME_WAIT 状态消失之前让服务器可以重启
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
	{
		ERR_EXIT("setsockopt");
	}

	if(bind(listenfd,(struct sockaddr *)&seraddr,sizeof(seraddr)) < 0)
	{
		ERR_EXIT("bind");
	}
	if(listen(listenfd,SOMAXCONN) < 0)
	{
		ERR_EXIT("listen");
	}
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	
	int connfd;// 已连接套接字
	if((connfd = accept(listenfd,(struct sockaddr *)&peeraddr,&peerlen)) < 0)
	{
		ERR_EXIT("accept");
	}
	printf("ip=%s port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
	pid_t pid;
	pid = fork();
	if(pid == -1)
	{
		ERR_EXIT("fork");
	}
	if(pid == 0)
	{	
		signal(SIGUSR1,handler);
		char sendbuf[1024] = {0};
		while(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)
		{
			write(connfd,sendbuf,strlen(sendbuf));
			memset(sendbuf,0,sizeof(sendbuf));
		}
		printf("child close\n");
		exit(EXIT_SUCCESS);
	}
	else
	{
		char recvbuf[1024];
		while(1)
		{
			memset(recvbuf,0,sizeof(recvbuf));
			int ret = read(connfd,recvbuf,sizeof(recvbuf));
			if(ret == -1)
			{
				ERR_EXIT("read");
			}
			else if(ret == 0)	// 对方关闭了
			{
				printf("peer close\n");
				break;
			}
			else				// 打印
			{
				fputs(recvbuf,stdout);
			}
		}
		printf("parent close\n");
		kill(pid,SIGUSR1);
		exit(EXIT_SUCCESS);		// 跳出循环则退出
	}

	return 0;
}


