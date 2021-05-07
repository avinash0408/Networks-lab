#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT 1234
  
int main(){
    int sock_fd; 
    char buff[2048]; 
    char from_client[1024] ;
    struct sockaddr_in servaddr; 
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sock_fd < 0 ) { 
        printf("socket creation failed..\n"); 
        exit(1); 
    } 
    // printf("Socket created successfully..\n");
    memset(&servaddr, 0, sizeof(servaddr)); 
      

    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int len; 
    len=sizeof(servaddr);
    while(1){
     printf("Enter your Query:");
    fgets(from_client,sizeof(from_client),stdin);
    from_client[strlen(from_client)]='\0';
    sendto(sock_fd, (const char *)from_client, strlen(from_client), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    if(strncmp(from_client,"quit",4)==0) break;
          
    recvfrom(sock_fd, (char *)buff,2048,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len);  
    printf("%s\n-------\n",buff);
    bzero(buff,sizeof(buff));
    
    }
  
    close(sock_fd); 
    return 0; 
}