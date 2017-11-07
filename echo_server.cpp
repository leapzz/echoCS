#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#define MAXSIZE 5
#define MAXBUFFER 256
#define portnum 12345

int main(int argc, char *argv[])
{
	int sockfd,err,connfd,ret;
	struct sockaddr_in myaddr,peeraddr;//myaddr为server peeraddr用来装peer的ip和端口等 
	char readBuf[MAXBUFFER] = {0};
	char ip[40] = {0};
	socklen_t addrlen;
	
	if(argc != 2){
		printf("u didn't write portnum...\n");
		return -1;
	}
	
	sockfd = socket(PF_INET,SOCK_STREAM,0);
	if(sockfd < 0){
		printf("create socket failed...\n");
		return -1;
	}
	
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons (portnum);
	inet_pton(PF_INET,argv[1],&myaddr.sin_addr);
	err = bind(sockfd,(struct sockaddr *)&myaddr,sizeof(myaddr));
	if(err < 0){
		printf("bind failed...\n");
		return -1;
	}
	
	if(listen(sockfd,MAXSIZE)!=0){
		printf("listen error...\n");
		return -1;
	}
	
	addrlen = sizeof(struct sockaddr);
	while(1){
		connfd = accept(sockfd,(struct sockaddr *)&peeraddr,&addrlen);
		printf("%s connects success...\n",inet_ntop(PF_INET,&peeraddr.sin_addr,ip,sizeof(ip)));
		while((ret=read(connfd,readBuf,MAXBUFFER))){
			write(connfd,readBuf,MAXBUFFER);
			bzero(readBuf,MAXBUFFER);
		}
		if(ret==0){
			printf("客户端关闭连接...\n");
		}
		else{
			printf("read error...\n");
		}
		close(connfd);
	}
	close(sockfd);
	return 0;
}
