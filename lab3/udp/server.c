#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT 8080 
#define sockaddr struct sockaddr

int main(){
    struct sockaddr_in my_addr,cli;
    int sock_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(sock_fd<0){
        printf("Socket creation failed..\n");
        exit(1);
    }
    printf("Socket created successfully..\n");
    my_addr.sin_family = AF_INET; // IPv4 
    my_addr.sin_addr.s_addr = INADDR_ANY; 
    my_addr.sin_port = htons(PORT); 
    if(bind(sock_fd,(sockaddr*)&my_addr,sizeof(my_addr))<0){
        printf("bind failed..\n");
        exit(1);
    }
    printf("Bind successful..\n");
    int len,n;
    len=sizeof(cli);
    char buff[1024];
    n=recvfrom(sock_fd,(char *)buff,1024,MSG_WAITALL,(sockaddr*)&cli,&len);
    buff[n]='\0';
    if(n==0)
    printf("CONNECTION CLOSED..\n");
    else if(n==-1){
        printf("Error while receiving..\n");
    }
    else{
         printf("Message from client :%s\n",buff);
    }
   
    char *from_ser="Hey from server";
   n= sendto(sock_fd, (const char *)from_ser, strlen(from_ser),  
        MSG_CONFIRM, (sockaddr *) &cli, len); 
     if(n==0)
    printf("CONNECTION CLOSED..\n");
    else if(n==-1){
        printf("Error while sending..\n");
    }
    else{
         printf("Message Sent to client..\n");
    }
}
