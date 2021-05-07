#include <stdio.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 8080
#define backlog 5
#define sockaddr struct sockaddr
int my_fun(int);
int main(){
    struct sockaddr_in my_server,cli;

    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd ==-1){
        printf("Socket creation failed..\n") ;
        exit(1);
        }
    printf("Socket created successfully..\n");

    my_server.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.4",&(my_server.sin_addr));
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
    close(conn_fd);

}

int my_fun(int sock_fd){
    char msg[1024];
   
         
        if(recv(sock_fd,msg,sizeof(msg),0) == -1)
        {
            printf("Receive failed..\n");
            return -1;
        }
        printf("Message from client: %s\n",msg);
        bzero(msg,1024);
       
       
        char res[]="HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"Server: HTTP Server\r\n"
"\r\n"
"<html>\n"
"<head><title>HTTP Server</title></head>\n"
"<body><h2>Welcome To Networks Lab!</h2></body>\n"
"</html>";
 if(send(sock_fd,res,sizeof(res),0) == -1)
        {
            printf("Send failed..\n");
            return -1;
        }
       
       return 0; 
       
    }
       



