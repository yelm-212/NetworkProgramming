//
//  Server.c
//  socket
//
//  Created by Jho on 2020/04/13.
//  Copyright © Jho All rights reserved.
//
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define BUF_SIZE 1024
#define OPSZ 4
 
void ErrorHandling(char* message);
int calculate(int opnum, int opends[], char op);
 
int main()
{
    int hServSock, hClntSock; //서버 및 클라이언트 소켓
    char message[BUF_SIZE];
    int  i;
    
    int result, opendCnt, recv_cnt,recv_len; //수신한 데이터의 길이의 제어변수
    socklen_t szClntAddr; //클라이언트 주소 구조체의 바이트 크기
    struct sockaddr_in servAddr,clntAddr; // 서버용 소켓, 클라이언트용 소켓
   
    hServSock=socket(PF_INET, SOCK_STREAM, 0);//소켓 개통
    if(hServSock<0)
        ErrorHandling("socket() error");
  
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family=AF_INET;//주소체계 지정
    servAddr.sin_addr.s_addr=htonl(INADDR_ANY);//IP주소 리드
    servAddr.sin_port=htons(35124); //포트 번호 htons->호스트 바이트 순서를 short타입 네트워크바이트 순서로 변환
    
    if(bind(hServSock, (struct sockaddr*) &servAddr, sizeof(servAddr))<0) //주소 할당
        ErrorHandling("bind() error");
    
    if(listen(hServSock, 3)<0) //대기 큐 5개
        ErrorHandling("listen() error");
 
    szClntAddr=sizeof(struct sockaddr_in);
    
    for(i = 0; i < 5; i++)
    {
        hClntSock = accept(hServSock, (struct sockaddr*)&clntAddr, &szClntAddr);
        if(hClntSock == -1)
            ErrorHandling("accept() error");
        else
            printf("Connected client %d \n", i+1);
        int flag;
            //client에게 지속적인 서비를 해주기 위한 while문
        while(1)
        {
            flag = 0; //flag 를 이용하여 while문 종료조건 부여
            opendCnt = 0; // 피연산자의 수를 받아 오기 위한 int
            recv_len = 0; // buffer로 부터 읽어온 길이를 판단하기 위한 int
            //buffer로 부터 피연산자의 수만 얻기위해 1byte만 읽음
            read(hClntSock, &opendCnt, 1);
        
        	// client로부터 요청 데이터의 크기는 4bytes(int)+피연산자수+1(연산자)
            while((opendCnt * OPSZ + 1) > recv_len)
            {
                recv_cnt = recv(hClntSock, &message[recv_len], BUF_SIZE,0);
                recv_len += recv_cnt;
                
                if(recv_cnt == -1 || recv_cnt ==0)
                {
                    flag = 1;
                    break;
                }
            }
            
             if (flag ==1)
              break;
            
            //client가 비정상 졸료 또는 close 호출했다면 while을 완전 종료
            //수신한 데이터를 통해 연산함수 호출하고 결과값 전
            result = calculate(opendCnt, (int*)message, message[recv_len - 1]);
            write(hClntSock, (char*)&result, sizeof(result));
            printf("result : %d \n",result);
        }
        printf("Client %d is disconnected \n", i+1);
        close(hClntSock);
    }
    close(hServSock);
}
 
int calculate(int opnum, int opnds[], char op)
       {
           int output = opnds[0], i;
           switch(op)
           {
               case '+':
                   for(i = 1; i < opnum; i++) output += opnds[i];
                   break;
               case '-':
                   for(i = 1; i < opnum; i++) output -= opnds[i];
                   break;
               case '*':
                   for(i = 1; i < opnum; i++) output *= opnds[i];
                   break;
           }
           return output;
       }
 
void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
 