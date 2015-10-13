#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
int main(int argc,char **argv){
	struct in_addr inaddr;
	unsigned int addr;
	if(argc!=2){
		fprintf(stderr,"Usage:%s<hex number>\n",argv[0]);
		exit(0);
	}
	if(inet_aton(argv[1],&inaddr)==0){
//		app_error("inet_aton error");
		exit(0);
	}
	addr=ntohl(inaddr.s_addr);
	printf("0x%x\n",addr);
	exit(0);
}
