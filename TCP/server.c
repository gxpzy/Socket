#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXSIZE 1024
int main(int argc, const char * argv[]) {
    int sockfd,new_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t sin_size;
    int portnumber;
    char buf[MAXSIZE];
    if (argc != 2) {
        fprintf(stderr, "Usage:%s portnumber\a\n",argv[0]);
        exit(1);
    }
    
    if ((portnumber = atoi(argv[1])) < 0) {
        fprintf(stderr, "Usage:%s portnumber\a\n",argv[0]);
        exit(1);
    }
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Socket error:%s\n\a",strerror(errno));
        exit(1);
    }
    /*服务器端填充sockaddr 结构*/
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //    绑定
    if (bind(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "Bind error:%s\n\a",strerror(errno));
        exit(1);
    }else{
        printf("bind success local port %d\n", portnumber);
    }
    
    //    监听
    if (listen(sockfd, 5) == -1) {
        fprintf(stderr, "Listen error:%s\n\a",strerror(errno));
        exit(1);
    }else{
        printf("listening...\n");
    }
   
       sin_size = sizeof(struct sockaddr_in);
       if ((new_fd = accept(sockfd, (struct sockaddr *)(&client_addr), &sin_size)) == -1) {
           fprintf(stderr, "Accept error:%s\n\a",strerror(errno));
           exit(1);
       }
    printf("connected successful,please input the message [<1024bytes]:\n");
    while (1) {       
       if (fgets(buf, sizeof(buf), stdin) != buf) {
           printf("fgets error\n");
       }       
       if (write(new_fd, buf, strlen(buf)) == -1) {
           fprintf(stderr, "Write error:%s\n\a",strerror(errno));
           exit(1);
       }
       // close(new_fd);
   }
    
    return 0;
}
