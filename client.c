#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
 
pthread_t t1,t2;
 char recvBuff[1024]; //read buffer
 char s[1024]; //write buffer
int inet_pton(int,const char *,void *);


void* serv_read(void *args)
{
 void *a;
 
 int *sock_fd;
 sock_fd=(int *)args;
 while(1)
 {
  read((*sock_fd),recvBuff,100);
  printf("\n TEACHER : %s",recvBuff); 
  if(strcmp(recvBuff,"_exit_\n")==0)
   {
    printf("\n BYE"); 
    return a;
   }
   printf("\n>>>>>");  
 }
 
}

void* serv_write(void *args)
{ 
int *sock_fd;
void *a;
sock_fd=(int*)args;

while(1)
{
  printf("\t\t\t\t\t NOTE: TO EXIT type '_exit_' ");	
  printf("\n>>>>>");
  fgets(s,100,stdin);
  write((*sock_fd),s,100);	
  if(strcmp(s,"_exit_\n")==0)
  {
   printf("\n BYE");
   return a;
  }
  printf("STUDENT: %s",s); 
} 

pthread_join(t1,NULL);
}

int main(int argc,char *argv[])
{
 int sock_fd;
 struct sockaddr_in serv_addr;
 int conn_status;


 if(argc!=2 && argc<2)
 {
  printf("\n USAGE: %s <ip of server> \n",argv[0]);
  return 1; 
 }
 else
 if(argc>2)
 {
  printf("INVALID NUMBER OF ARGUMENTS");
  return 1;
 }
 memset(recvBuff,'0',sizeof(recvBuff));
 //creating the socket
 if((sock_fd=socket(AF_INET,SOCK_STREAM,0))<0)
 {
  printf("\n ERROR IN CREATING THE SOCKET");
  return 1;
 } 
 memset(&serv_addr,'0',sizeof(serv_addr));
 //creating the address of the socket
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_port= htons(4300);
 if((inet_pton(AF_INET,argv[1],&serv_addr.sin_addr)) <= 0) 
 { 
  printf("\n inet_pton error occurred \n");
  return 1;
 }

//connection establishment 
 conn_status = connect(sock_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

//checking connection status
 if(conn_status==-1)
 { 
  printf("CONNECTION FAILED");
  return 1;
 }

 printf("\nCONNECTION SUCCESSFULL");

 pthread_create(&t1,NULL,serv_read,(void *)(&sock_fd)); //thread for server text reading 

 pthread_create(&t2,NULL,serv_write,(void *)(&sock_fd)); // thread for sending text to the server

 pthread_join(t2,NULL); //waiting for one of the threads

/*
while(1)
{
  read(sock_fd,recvBuff,100);
  if(strcmp(recvBuff,"_exit_\n")==0)
  {
   printf("\n BYE"); 
   return 1;
  }
  printf("\nTEACHER : %s",recvBuff); 
  printf("\n\t\t\t\t\t NOTE: TO EXIT type '_exit_' ");	
  printf("\n>>>>>");
  fgets(s,100,stdin);
  write(sock_fd,s,100);
  if(strcmp(s,"_exit_\n")==0)
  {
   printf("\n BYE");
   return 1;
  }
  printf("\nSTUDENT: %s",s); 
 }  */
 /*recieving the data from the server 
   recv(sock_fd,s,sizeof(s),0);
   printing the server response
   printf("\n THE SERVER SENT THE FOLLOWING DATA : %s ",s); 
 */
 

 close(sock_fd);  //closing the socket
 
 return 1;

}

