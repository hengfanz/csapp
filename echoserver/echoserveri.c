#include"csapp.h"
#include"echo.c"
int main(int argc,char **argv)
{
    int listenfd,connfd,port,clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    if(argc>100){
        fprintf(stderr,"usage:%s<port>\n",argv[0]);
        exit(0);
    }
    //port=atoi(argv[1]);
    port=8911;
    listenfd=open_listenfd(port);

    while(1){
        clientlen=open_listenfd(port);
        connfd=accept(listenfd,(SA*)&clientaddr,&clientlen);
        hp=gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr,sizeof(clientaddr.sin_addr.s_addr),AF_INET);
        haddrp=inet_ntoa(clientaddr.sin_addr);
        if(haddrp!=NULL){
            printf("server connected to (%s)\n",haddrp);
        }
        else
            printf("hello\n");
        echo(connfd);
        Close(connfd);
    }
    exit(0);
}
