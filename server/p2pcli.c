/*************************************************************************
	> File Name: p2pcli.c
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
	int sockfd;
	if((sockfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
	{
		ERR_EXIT("socket");
	}
	struct sockaddr_in seraddr;
	memset(&seraddr,0,sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(8080);
	seraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 指定服务器的地址
	
	if(connect(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr)) < 0)
	{
		ERR_EXIT("connect");
	}
	
	pid_t pid;
	pid = fork();
	if(pid == -1)
	{
		ERR_EXIT("fork");
	}
	else if(pid == 0)	// 父进程
	{
		char recvbuf[1024];
		while(1)
		{
			memset(recvbuf,0,sizeof(recvbuf));
			int ret = read(sockfd,recvbuf,sizeof(recvbuf));
			if(ret == -1)
			{
				ERR_EXIT("read");
			}
			else if(ret == 0) // 对等方关闭
			{
				printf("peer close\n");
				break;
			}
			else
			{
				fputs(recvbuf,stdout);
			}
		}
		close(sockfd);
		kill(getppid(),SIGUSR1);
	}
	else				// 子进程
	{
		signal(SIGUSR1,handler);
		char sendbuf[1024] = {0};
		while(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)
		{
			write(sockfd,sendbuf,strlen(sendbuf));
			memset(sendbuf,0,sizeof(sendbuf));
		}
		close(sockfd);
	}

	return 0;	
}
