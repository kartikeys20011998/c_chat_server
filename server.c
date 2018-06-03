#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<pthread.h>

pthread_t t1,t2;
 char s[100];
 char sendBuff[1000];

void* clnt_read(void *args)
{
 int *connfd;
 connfd=(int*)args;
 void *a;
 while(1)
 {
  read((*connfd),sendBuff,100);
  printf("\nSTUDENT : %s",sendBuff);
  if(strcmp(sendBuff,"_exit_\n")==0)
  { 
   printf("\n BYE");

   return a;
  }
  printf("\n>>>>>");
 }
}

void* clnt_write(void *args)
{
 int *connfd;
 connfd=(int*)args;
 void *a;
 while(1) 
 {
  printf("\n\t\t\t\t\t NOTE: TO EXIT type '_exit_' ");
  printf("\n>>>>>");
  fgets(s,100,stdin);
  write((*connfd),s,100);
  if(strcmp(s,"_exit_\n")==0)
  { 
   printf("\n BYE");
   return a;
  }
  printf("\nTEACHER : %s",s);
 }
 pthread_join(t1,NULL);
}

int main(int argc,char *argv[])
{
 int listen_fd=0,connfd=0;
 struct sockaddr_in serv_addr;
 // creating the socket 
 listen_fd=socket(AF_INET,SOCK_STREAM,0);
 memset(&serv_addr,'0',sizeof(serv_addr));
 memset(&sendBuff,'0',sizeof(sendBuff));
 // initialising the socket
 serv_addr.sin_family=AF_INET;  //setting the family of network address
 serv_addr.sin_port=htons(4300); //setting the port number and changing its data format to the one that is readable to the structure.
 serv_addr.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY same as connecting to 0.0.0.0 or 127.0.0.1
  
 //binding the name to the socket
 bind(listen_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
 
 //listen TO  the incoming connections
 listen(listen_fd,6); // 6 denotes that maximum it can create upto 6 connections
 
 connfd=accept(listen_fd,(struct sockaddr*)NULL,NULL);
//SENDING THE MESSAGE 
// send(connfd,"HELLO BRO I AM THE SERVER",sizeof("HELLO BRO I AM THE SERVER"),0);

pthread_create(&t1,NULL,clnt_read,(void *)(&connfd));
pthread_create(&t2,NULL,clnt_write,(void *)(&connfd));

pthread_join(t2,NULL);
   
/*
while(1)
{
  printf("\n\t\t\t\t\t NOTE: TO EXIT type '_exit_' ");
  printf("\n>>>>>");
  fgets(s,100,stdin);
  write(connfd,s,100);
  if(strcmp(s,"_exit_\n")==0)
  { 
   printf("\n BYE");
   return 1;
  }
  printf("\nTEACHER : %s",s);
  read(connfd,sendBuff,100);
  if(strcmp(sendBuff,"_exit_\n")==0)
  { 
   printf("\n BYE");
   return 1;
  }
  printf("STUDENT : %s",sendBuff);
} 
*/ 

close(listen_fd);   //close the socket

return 1;
 
}
