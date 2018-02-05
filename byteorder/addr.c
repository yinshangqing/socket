/*************************************************************************
	> File Name: addr.c
	> Author: yinshangqing
	> Mail: 841668821@qq.com 
	> Created Time: 2018年02月01日 星期四 22时02分48秒
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>

typedef unsigned int  uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

int main()
{
	ulong addr = inet_addr("192.168.1.77");
	printf("addr=%u\n",ntohl(addr));
	
	struct in_addr ipaddr;
	ipaddr.s_addr = addr;
	printf("ip: %s\n",inet_ntoa(ipaddr));

	return 0;
}
