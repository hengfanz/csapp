#ifndef CSAPP_H_INCLUDED
#define CSAPP_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<netdb.h>
#include <errno.h>
#define MAXLINE 8192 /* MAX text line length */
#define LISTENQ  1024  /* Second argument to listen() */
typedef struct sockaddr SA;

#define RIO_BUFSIZE 8192
typedef struct{
    int rio_fd;             /*Desciptor for this internel buf*/
    int rio_cnt;            /*Unread bytes in internal buf*/
    char *rio_bufptr;       /*Next unread byte in intrenal buf */
    char rio_buf[RIO_BUFSIZE];/*Internal buffer */

}rio_t;


#endif // CSAPP_H_INCLUDED
