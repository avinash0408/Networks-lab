#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT 8080
  
int main(){
    int sock_fd; 
    char buff[1024]; 
    char from_client[1024] ;
    struct sockaddr_in servaddr; 
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sock_fd < 0 ) { 
        printf("socket creation failed..\n"); 
        exit(1); 
    } 
    printf("Socket created successfully..\n");
    memset(&servaddr, 0, sizeof(servaddr)); 
      

    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int n, len; 
    len=sizeof(servaddr);
     printf("Enter the message to server :");
        scanf("%[^\n]",from_client);
    n=sendto(sock_fd, (const char *)from_client, strlen(from_client), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    if(n==0)
    printf("CONNECTION CLOSED..\n");
    else if(n==-1){
        printf("Error while sending..\n");
    }
    else{
         printf("Message Sent to server..\n");
    }
          
    n = recvfrom(sock_fd, (char *)buff, 1024,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    buff[n] = '\0'; 
    if(n==0)
    printf("CONNECTION CLOSED..\n");
    else if(n==-1){
        printf("Error while receiving..\n");
    }
    else{
         printf("Message from server :%s\n",buff);
    }
  
    close(sock_fd); 
    return 0; 
}