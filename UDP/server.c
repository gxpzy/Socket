#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define SERVER_PORT 8888 //端口号
#define MAX_MSG_SIZE 1024 //缓冲区大小
int main(int argc, const char * argv[]) {
    int sockfd, n;
    socklen_t addrlen;
    struct sockaddr_in addr;//服务端套接口地址数据结构
    char msg[MAX_MSG_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);//开始建立socket描述符，SOCK_DGRAM udp
    if (sockfd < 0) {
        fprintf(stderr, "socket error:%s\n", strerror(errno));
        exit(1);
    }
    
    addrlen = sizeof(struct sockaddr_in);
    bzero(&addr, addrlen);
    //设置套接口数据结构
    addr.sin_family = AF_INET;//IPv4
    addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY 表示本机所有ip(一台计算机可能会有好几个网卡）
    addr.sin_port = htons(SERVER_PORT);//将本机上的8888 转换成网络上的8888 -> 47318
    
    if (bind(sockfd, (struct sockaddr *)(&addr), addrlen) < 0) {//创建好一个套接口后绑定到本机上，端口上
        fprintf(stderr, "bind error:%s\n", strerror(errno));
        exit(1);
    }else{
        printf("success");
    }
    
    while (1) {
        //从网络中读取数据
        bzero(msg, MAX_MSG_SIZE);//缓冲区清零
        n = recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)(&addr), &addrlen);
        fprintf(stdout, "receive msg from client:%s",msg);
        //从终端读取用户输入的数据，发送
        fgets(msg, MAX_MSG_SIZE, stdin);
        printf("server endpoint input message:%s",msg);
        sendto(sockfd, msg, n, 0, (struct sockaddr *)(&addr), addrlen);
        
    }
    return 0;
}
