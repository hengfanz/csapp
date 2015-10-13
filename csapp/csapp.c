#include"csapp.h"
int Open_clientfd(char *hostname,int port){
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;

    if((clientfd=socket(AF_INET,SOCK_STREAM,0))<0)
        return -1;
    if((hp=gethostbyname(hostname))==NULL){
        return -2;
    }
    bzero((char*)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    bcopy((char*)hp->h_addr_list[0],(char*)&serveraddr.sin_addr.s_addr,hp->h_length);


    serveraddr.sin_port=htons(port);

    if(connect(clientfd,(SA*)&serveraddr,sizeof(serveraddr))<0)
        return -1;
    return clientfd;
}

int open_listenfd(int port){
    int listenfd,optval=1;
    struct sockaddr_in serveraddr;
    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0){
        return -1;
    }
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(int)<0)){
        return -1;
    }
    bzero((char*)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serveraddr.sin_port=htons((unsigned short)port);
    if(bind(listenfd,(SA*)&serveraddr,sizeof(serveraddr))<0)
        return -1;
    if(listen(listenfd,LISTENQ)<0)
        return -1;
    return listenfd;
}
static ssize_t rio_read(rio_t *rp,char *usrbuf,size_t n)
{
    int cnt;
    while(rp->rio_cnt<=0){
        rp->rio_cnt=read(rp->rio_fd,rp->rio_buf,sizeof(rp->rio_buf));
    if(rp->rio_cnt<0){
        if(errno!=EINTR)
            return -1;
    }
    else if(rp->rio_cnt==0)
        return 0;
    else
        rp->rio_bufptr=rp->rio_buf;

    }
    cnt=n;
    if(rp->rio_cnt<n)
        cnt=rp->rio_cnt;
    memcpy(usrbuf,rp->rio_bufptr,cnt);
    rp->rio_bufptr+=cnt;
    rp->rio_cnt-=cnt;
    return cnt;
}
void rio_readinitb(rio_t *rp,int fd){
    rp->rio_fd=fd;
    rp->rio_cnt=0;
    rp->rio_bufptr=rp->rio_buf;
}
ssize_t rio_readlineb(rio_t *rp,void *usrbuf,size_t maxlen){
    int n,rc;
    char c,*bufp=usrbuf;
    for(n=1;n<maxlen;n++){
        if((rc=rio_read(rp,&c,1))==1){
            *bufp++=c;
            if(c=='\n'){
                n++;
                break;
            }
        }
        else if(rc==0){
            if(n==1){
                return 0;
            }
            else{
                break;
            }
        }else
            return -1;
    }
    *bufp='\0';
    return n-1;
}
void Close(int fd)
{
    int rc;
    if((rc==close(fd))<0)
        unix_error("Close error");
}

ssize_t rio_writen(int fd,void *usrbuf,size_t n){
    size_t nleft=n;
    ssize_t nwritten;
    char *bufp=usrbuf;

    while(nleft>0){
        if((nwritten=write(fd,bufp,nleft))<=0){
            if(errno==EINTR){
                nwritten=0;
            }
            else{
                return -1;
            }
            nleft-=nwritten;
            bufp+=nwritten;
        }
    }
    return n;

}
char* Fgets(char *ptr,int n,FILE *stream){
    char *rptr;
    if(((rptr=fgets(ptr,n,stream)))==NULL&&ferror(stream))
        app_error("Fgets error");
    return rptr;
}

void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

void app_error(char *msg){
    fprintf(stderr,"%s\n",msg);
    exit(0);
}
void Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
}

void Rio_writen(int fd,void *usrbuf,size_t n){
    if(rio_writen(fd,usrbuf,n)!=n)
        unix_error("Rio_writen error");
}

ssize_t Rio_readlineb(rio_t *rp,void *usrbuf,size_t maxlen){
    ssize_t rc;
    if((rc=rio_readlineb(rp,usrbuf,maxlen))<0)
        unix_error("Rio_readlineb error");
    return rc;
}


void Fputs(const char *ptr,FILE *stream)
{
    if(fputs(ptr,stream)==EOF)
        unix_error("Fputs error");
}
