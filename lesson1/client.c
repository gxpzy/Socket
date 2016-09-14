#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
int main(int argc, const char *argv[]) {
    printf("argc = %d argv = %s\n", argc, *argv);
    int sockfd;
    char buffer[1024];
    struct sockaddr_in server_addr;
    struct hostent *host;
    long portnumber,nbytes;
    if (argc != 3) {
        fprintf(stderr, "Usage:%shostname portnumber\a\n",argv[0]);
        exit(1);
    }
    
    if ((host = gethostbyname(argv[1])) == NULL) {
        fprintf(stderr, "get hostname error\n");
        exit(1);
    }
    
    if ((portnumber = atoi(argv[2])) < 0) {
        fprintf(stderr, "Usage:%s hostname portnumber\a\n", argv[0]);
        exit(1);
    }
    
    /*开始建立连接*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket created error");
        exit(1);
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr = *((struct in_addr*)host->h_addr);
    /*发起连接请求*/    
    if (connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1) {
        perror("connect error!\n");
        exit(1);
    }
       
    /*连接成功*/
    while(1){
    printf("等待接收数据\n");
    if ((nbytes = read(sockfd, buffer, 1024)) == -1) {
        perror("read error");
        printf("reading...\n");
        exit(1);
    }       
    buffer[nbytes] = '\0';
    printf("i have received:%s",buffer);        
    }     
    close(sockfd);
    return 0;
}