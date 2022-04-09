//
//  client.c
//  socket
//
//  Created by Jho on 2020/04/13.
//  Copyright © jho All rights reserved.
//
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
 
void ErrorHandling(char* message);
 
int main(int argc, char* argv[])
{
    
    int hSocket; //서버에 접속할 소켓
    struct sockaddr_in serveraddr; //접속할 서버의 주소
    char opmsg[BUF_SIZE];
    int result, opndCnt, i;
    unsigned char flag;
    int recv_len, recv_cnt;
    if(argc!=3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    hSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(hSocket<0)
        ErrorHandling("hSocket() error");
    
    memset(&serveraddr, 0, sizeof(serveraddr)); // 서버쪽 주소 구조체를 0 으로 초기화
    serveraddr.sin_family=AF_INET; //IPv4 정보체계저장
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); //주소할당
    serveraddr.sin_port=htons(atoi(argv[2])); //포트할당
    
    
    if(connect(hSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        ErrorHandling("connect() error!");
    else
        puts("Connected.............");
    
    while(1)
    {
        flag =' ';
        fputs("Operand count (Q to quit) : ",stdout);
        scanf(" %c",&flag);
 
    
        if(flag == 'q' || flag =='Q')
            break;
        else
            opndCnt = (int)flag-48;
        
        opmsg[0] = (char)opndCnt;
        //피연산자 개수만큼 숫자를 입력
       for(i = 0; i < opndCnt; i++)
        {
            printf("Operand %d : ", i+1);
            scanf("%d", (int*)&opmsg[i*OPSZ+1]);
        }
 
        fgetc(stdin);
        fputs("Operator : ", stdout); //연산 기호 입력
        scanf("%c", &opmsg[opndCnt*OPSZ+1]); //전체 데이터 전송
        write(hSocket, opmsg, opndCnt*OPSZ+2);
        read(hSocket, &result, RLT_SIZE);
        
        //client로 부터 오는 데이터는 연산 결과인 4byte int형
        while(RLT_SIZE > recv_len)
        {
            recv_cnt = recv(hSocket,&result,RLT_SIZE,0);
            if(recv_cnt == -1)
                ErrorHandling("read() error!");
            recv_len += recv_cnt;
        }
        
        printf("Operation result %d\n", result);
    }
        close(hSocket);
        return 0;
}
 
void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
