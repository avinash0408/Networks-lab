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
#define backlog 5
#define sockaddr struct sockaddr
int my_fun(int);

int sendfile(int sock_fd){
    FILE *fp;
    fp=fopen("send.mp4","r");
    int size=500;
    char msg[500]={0};
    int byte;
    while(byte=fread(msg,1,size,fp)>=1){
         if (send(sock_fd, msg,sizeof(msg), 0) == -1) {
      perror("File sending..");
      exit(1);
    }
    bzero(msg,size);
    char *rec[500];
    while(recv(sock_fd,rec,sizeof(rec),0)<=0){};
    }
}
int main(){
    struct sockaddr_in my_server,cli;

    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd ==-1){
        printf("Socket creation failed..\n") ;
        exit(1);
        }
    printf("Socket created successfully..\n");

    my_server.sin_family=AF_INET;
    my_server.sin_addr.s_addr=inet_addr("127.0.0.1");
    my_server.sin_port=htons(PORT);

    if((bind(sock_fd,(sockaddr*)&my_server,sizeof(my_server)))<0){
        printf("bind failed..\n"); 
        exit(1); 
    }
    printf("socket binded successfully..\n");

    if((listen(sock_fd,backlog)) != 0) { 
        printf("Listen failed..\n") ; 
        exit(1);
    }
    printf("Listening to client..\n");
    int len=sizeof(cli);
    int conn_fd=accept(sock_fd,(sockaddr*)&cli,&len);
    if(conn_fd < 0){
        printf("accept failed...\n"); 
        exit(1);
    }
    printf("Server accepted the client..\n");

    my_fun(conn_fd);
    close(sock_fd);

}

int my_fun(int sock_fd){
    char msg[500];
    //for continous communication

    while(1)
    {
        char greet[500];
        sprintf(greet,"%s","Hello clien.!");
    bzero(msg,500);
    int temp=recv(sock_fd,msg,sizeof(msg),0);
    msg[strlen(msg)-1]='\0';
    if(temp==0){
        printf("Connection closed...\n");
    } 
    else if(temp== -1)
        {
            printf("Receive failed..\n");
            exit(0);
        }
        printf("Message from client: %s\n",msg);
           if (strncmp("exit", msg, 4) == 0) { 
            printf("Server Exit...\n"); 
            close(sock_fd);
            break; 
        }
        if(strcmp("GivemeyourVideo",msg)==0){
            bzero(greet,500);
           // printf("heheheh");
           sprintf(greet,"%s","OK!");
             if (send(sock_fd, greet,strlen(greet), 0) == -1) {
            perror("File sending..");
            exit(1);
                }
                bzero(greet,500);
            sendfile(sock_fd);
              sprintf(greet,"%s","Gotyou!");

        } 
        bzero(msg,500);
        
        temp=send(sock_fd,greet,strlen(greet),0);
        if(temp==0){
        printf("Connection closed...\n");
        } 
        else if(temp== -1)
        {
            printf("Send failed..\n");
            exit(0);
        }
        else{
            printf("Message sent to client..!\n");
        }     
        if (strncmp("exit", msg, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
     
    }

}
