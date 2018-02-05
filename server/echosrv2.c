/*************************************************************************
	> File Name: echosrv2.c
	> Author: yinshangqing
	> Mail: 841668821@qq.com 
	> Created Time: 2018年02月01日 星期四 22时45分02秒
 ************************************************************************/
// 回射服务器
// 一个连接一个进程来处理并发（子进程来进行通信处理）
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)
void do_service(int connfd);

int main()
{
	int listenfd;// 监听套接字
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
	pid_t pid;
	while(1)
	{
		if((connfd = accept(listenfd,(struct sockaddr *)&peeraddr,&peerlen)) < 0)
		{
			ERR_EXIT("accept");
		}
		printf("ip=%s port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
		// 创建一个进程
		pid = fork();
		if(pid == -1)
		{
			ERR_EXIT("fork");
		}
		if(pid == 0)// 子进程
		{
			close(listenfd);
			do_service(connfd);
			// 销毁与客户端对应的开辟的进程,结束与之对应的会话
			exit(EXIT_SUCCESS);
		}
		else		// 父进程
		{
			close(connfd);
		}
	}
	return 0;
}

void do_service(int connfd)
{
	char recvbuf[1024];
	while(1)
	{
		memset(recvbuf,0,sizeof(recvbuf));
		int ret = read(connfd,recvbuf,sizeof(recvbuf));
		if(ret == 0)
		{
			printf("client close\n");
			break;
		}
		else if(ret == -1) // 读取失败时，避免死循环,退出
		{
			ERR_EXIT("read");
		}
		fputs(recvbuf,stdout);
		write(connfd,recvbuf,ret);
		
	}

}

