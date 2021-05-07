#include <stdio.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#define PORT 8080
#define sockaddr struct sockaddr

int recv_file(FILE *fp,int sock_fd){
    int size=500;
    char msg[size];
    while(1){
        if(recv(sock_fd,msg,sizeof(msg),0) <0){
            perror("receiving");
            exit(0);
        }
        if(strcmp(msg,"Gotyou!")==0){
            printf("File transmission done..\n"); 
            break;
            return 0;
        }
        char *conf="received";
        send(sock_fd,msg,sizeof(msg),0);
        fwrite(msg,1,sizeof(msg),fp);
        bzero(msg,size);
    }
}

int my_fun(int sock_fd){
    char msg[500];
    while(1)
    {   
        bzero(msg,500);
      printf("Enter the message to server :");
        // scanf("%[^\n]",msg);
        fgets(msg,sizeof(msg),stdin);
        
        int temp=send(sock_fd,msg,strlen(msg),0);
        if(temp==0){
        printf("Connection closed...\n");
        break;
        } 
        else if(temp== -1)
        {
            printf("Send failed..\n");
            exit(0);
        }
        else{
            printf("Message sent to server..!\n");
        } 

        bzero(msg,500);
        temp=recv(sock_fd,msg,sizeof(msg),0);
        if(strcmp(msg,"OK!")==0){
            FILE *fp;
            fp=fopen("recv.mp4","w");
            recv_file(fp,sock_fd);
        }
        if(temp==0){
            printf("Connection closed...\n");
            break;
        } 
        else if(temp== -1)
        {
            printf("Receive failed..\n");
            exit(0);
        }
        else{
            printf("Message from server :%s\n",msg);
        } 
        
      
        if (strncmp("exit", msg, 4) == 0) { 
            printf("Server Exit...\n"); 
            close(sock_fd);
            break; 
        } 
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
    my_server.sin_addr.s_addr=inet_addr("127.0.0.1");

      if (connect(sock_fd, (sockaddr*)&my_server, sizeof(my_server)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(1); 
    } 
    else
        printf("connected to the server..\n"); 
  
    my_fun(sock_fd); 
    close(sock_fd); 



}
