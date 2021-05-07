#include <stdio.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netdb.h>
#include<unistd.h>
#define PORT 8080
#define sockaddr struct sockaddr

int my_fun(int sock_fd){
    char msg[1024];
    // while(1)
    // {   
    //     printf("Message to server:");
    //     gets(msg);
    //     if(send(sock_fd,msg,sizeof(msg),0) == -1)
    //     {
    //         printf("Send failed..\n");
    //         break;
    //     }
    //     bzero(msg,1024);
    //     if(recv(sock_fd,msg,sizeof(msg),0) == -1)
    //     {
    //         printf("Receive failed..\n");
    //         break;
    //     }
    //     printf("Message from server: %s\n",msg);
        
      
    //     if (strncmp("exit", msg, 4) == 0) { 
    //         printf("Server Exit...\n"); 
    //         break; 
    //     } 
    // }
        printf("Enter the message to server :");
        char greet[1024];
        scanf("%[^\n]",greet);
        
        int temp=send(sock_fd,greet,strlen(greet),0);
        if(temp==0){
        printf("Connection closed...\n");
        } 
        else if(temp== -1)
        {
            printf("Send failed..\n");
            exit(0);
        }
        else{
            printf("Message sent to server..!\n");
        } 

        bzero(msg,1024);
        temp=recv(sock_fd,msg,sizeof(msg),0);
        if(temp==0){
            printf("Connection closed...\n");
        } 
        else if(temp== -1)
        {
            printf("Receive failed..\n");
            exit(0);
        }
        else{
            printf("Message from server :%s\n",msg);
        }   
        
}

int main(){
    struct sockaddr_in my_server;

    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
      if(sock_fd ==-1){
        printf("Socket creation failed..\n") ;
        exit(1);
        }
    printf("Socket created successfully..\n");
    my_server.sin_family=AF_INET;
    my_server.sin_port=htons(PORT);
    my_server.sin_addr.s_addr=INADDR_ANY;

      if (connect(sock_fd, (sockaddr*)&my_server, sizeof(my_server)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(1); 
    } 
    else
        printf("connected to the server..\n"); 
  
    my_fun(sock_fd); 
    close(sock_fd); 



}