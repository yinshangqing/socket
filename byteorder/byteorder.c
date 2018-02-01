/*************************************************************************
	> File Name: byteorder.c
	> Author: yinshangqing
	> Mail: 841668821@qq.com 
	> Created Time: 2018年02月01日 星期四 22时02分48秒
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>

typedef unsigned int  uint;
typedef unsigned char  uchar;

int main()
{
	uint x = 0x12345678;
	uchar *p = (uchar *)&x;
	printf("%0x %0x %0x %0x\n",p[0],p[1],p[2],p[3]);
	
	uint y = htonl(x); // y网络字节序,大端模式---x86 小端
	p = (uchar *)&y;
	printf("%0x %0x %0x %0x\n",p[0],p[1],p[2],p[3]);

	return 0;
}
