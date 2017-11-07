#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>

#define ipaddress "127.0.0.1"
#define portnum 12345
#define MAXSIZE 256

int main(int argc, char *argv[])
{
	struct sockaddr_in seraddr;
	int clifd,ret;
	char clibuf[MAXSIZE];
	
	clifd = socket(PF_INET,SOCK_STREAM,0);
	if(clifd < 0){
		printf("open client socket failed...\n");
		return -1;
	}
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons (portnum);
	inet_pton(PF_INET,ipaddress,&seraddr.sin_addr);
	
	ret = connect(clifd,(struct sockaddr *)&seraddr,sizeof(seraddr));
	if(ret!=0){
		close(clifd);
		printf("connect error...\n");
		return -1;
	}
	
	while(1){
		bzero(clibuf,sizeof(clibuf));
		scanf("%s",clibuf);
		write(clifd,clibuf,sizeof(clibuf));
		bzero(clibuf,sizeof(clibuf));
		read(clifd,clibuf,sizeof(clibuf));
		printf("echo:%s\n",clibuf); 
	}
	close(clifd);
	return 0;
}
