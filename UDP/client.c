#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define MAX_BUF_SIZE 1024

int main(int argc, const char * argv[]) {
    int sockfd, port, n;
    socklen_t addrlen;
    char buffer[MAX_BUF_SIZE];
    struct sockaddr_in addr;
    if (argc != 3) {
        fprintf(stderr, "usage:%s server_ip server_port\n",argv[0]);
        exit(1);
    }
    
    if ((port = atoi(argv[2])) < 0) {
        fprintf(stderr, "usage:%s server_ip server_port\n",argv[0]);
        exit(1);
    }
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sockfd < 0) {
        fprintf(stderr, "socket error:%s\n",strerror(errno));
        exit(1);
    }
    
    addrlen = sizeof(struct sockaddr_in);
    bzero(&addr, addrlen);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_aton(argv[1], &addr.sin_addr) < 0) {
        fprintf(stderr, "ip error:%s\n",strerror(errno));
        exit(1);
    }
    
    while (1) {
        bzero(buffer, MAX_BUF_SIZE);
        fgets(buffer, MAX_BUF_SIZE, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)(&addr), addrlen);
        printf("client endpoint input msg:%s",buffer);
        
        n = recvfrom(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)(&addr), &addrlen);
        fprintf(stdout, "receive msg from server:%s", buffer);
    }
    return 0;
}
