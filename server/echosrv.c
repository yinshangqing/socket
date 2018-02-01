/*************************************************************************
	> File Name: echosrv.c
	> Author: yinshangqing
	> Mail: 841668821@qq.com 
	> Created Time: 2018年02月01日 星期四 22时45分02秒
 ************************************************************************/
// 回射服务器
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
	char recvbuf[1024];
	while(1)
	{
		memset(recvbuf,0,sizeof(recvbuf));
		int ret = read(connfd,recvbuf,sizeof(recvbuf));
		fputs(recvbuf,stdout);
		write(connfd,recvbuf,ret);
		
	}
	close(connfd);
	close(listenfd);
	return 0;
}


