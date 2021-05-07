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

int f=1;
void send_msg(char *msg,int socket_fd){
    // printf("%s %ld",msg,strlen(msg));
    int value=send(socket_fd,msg,strlen(msg)+1,0);
    if(value<0){
        perror("sending");
        exit(0);
    }

}
int recv_file(FILE *fp,int sock_fd){
    int size=500;
    char msg[size];
    // printf("file contents\n");
    while(1){
        if(recv(sock_fd,msg,sizeof(msg),0) <0){
            perror("receiving");
            exit(0);
        }
        if(strcmp(msg,"111")==0){
            // printf("File transmission done..\n"); 
            send_msg("f_done",sock_fd);
            fclose(fp);
            break;
            return 0;
        }
        fprintf(fp,"%s",msg);
        // printf("%s\n",msg);
        bzero(msg,size);
    }
}

int recv_msg(int socket_fd){
    char message[500];
    int value = recv(socket_fd,message,sizeof(message),0);
    // printf("%ld",strlen(message));
    if(value <0){
        perror("receive");
        exit(0);
    }
    while(1){
    if(strcmp(message,"200") == 0){
      printf("connection established...\n");
      return 0;
    }else if(strcmp(message,"300") == 0){
      printf("Correct Username; Need password...\n");
      return 0;
    } else if(strcmp(message,"301") == 0){
      printf("InCorrect Username...\n");
      return 0;
    } else if(strcmp(message,"305") == 0){
      printf("User Authenticated with password...\n");
      return 0;
    } else if(strcmp(message,"310") == 0){
      printf("Incorrect password...\n");
      return 0;
    } else if(strcmp(message,"505") == 0){
      printf("Command not supported...\n");
      return 0;
    } 
     else if(strcmp(message,"ff_done") == 0){
      printf("file transmission done...\n");
      return 0;
    }
      else if(strcmp(message,"f_done") == 0){
      printf("file sent to server...\n");
      return 0;
    }
    
    else if(strcmp(message,"000") == 0){
      printf("New file created...\n");
      return 0;
    }
     else if(strcmp(message,"f_dir") == 0){
      return 0;
    }
    else if(strcmp(message,"OVER")==0){
      return 0;
    } if(strcmp(message,"100") == 0){
      printf("Success...\n");
      return 0;
    }if(strcmp(message,"0") == 0){
      printf("Failed...\n");
      return 0;
    }else{
      printf("%s\n",message);
    }
    }

}
int sendfile(FILE *fp,int sock_fd){
    int size=500;
    char msg[500]={0};
    int byte;
    while(fgets(msg,size,fp)!=NULL){
         if (send(sock_fd, msg,sizeof(msg), 0) == -1) {
      perror("File sending..");
      exit(1);
    }
    bzero(msg,size);
    }
    
}

// int sendfile(FILE *fp,int sock_fd){
   
//     int size=500;
//     char msg[500]={0};
//     int byte;
//     while(byte=fread(msg,1,size,fp)>=1){
//          if (send(sock_fd, msg,sizeof(msg), 0) == -1) {
//       perror("File sending..");
//       exit(1);
//     }
//     bzero(msg,size);
//     char *rec[500];
//     while(recv(sock_fd,rec,sizeof(rec),0)<=0){};
//     }
// }
int recv_dir(int socket_fd){
  printf("Server's directory :\n");
  while(1){
     char message[500];
    int value = recv(socket_fd,message,sizeof(message),0);
    // printf("%ld",strlen(message));
    if(value <0){
        perror("receive");
        exit(0);
    }
    else if(strcmp(message,"stop_dir")==0)
    break;
    else
    printf("%s\n",message);
  }
}

int main(){
    char s[2]=" ";
     printf("Enter START to start the connection\n");
    char start[1024];
    while(1){
        fgets(start,sizeof(start),stdin);
        start[strlen(start)-1]='\0';
        if(strcmp(start,"START")==0)
        break;
        // bzero(start,strlen(start));
        printf("505\n");

    }
    printf("Connection Started..\n");
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
    while(1)
    {  
        bzero(start,1024);
    fgets(start,sizeof(start),stdin);
    start[strlen(start)-1]='\0';
     send_msg(start,sock_fd);
    if(strncmp(start,"QUIT",4)==0){
        close(sock_fd);
        printf("Connection closed..\n");
        return 0;
    }
    else if(strncmp(start,"GetFile",7)==0){
        char client_message[500];
        char name[500];
        strcpy(client_message,start);
        strcpy(name, strtok(client_message, s));
        strcpy(name,strtok(NULL,s));
        printf("%s",name);
        FILE *fpw=fopen(name,"w");
        recv_file(fpw,sock_fd);
    }
    else if(strncmp(start,"ListDir",7)==0){
      recv_dir(sock_fd);
        
    }
      else if(strncmp(start,"StoreFile",7)==0){
        char client_message[500];
        char name[500];
        strcpy(client_message,start);
        strcpy(name, strtok(client_message, s));
        strcpy(name,strtok(NULL,s));
        // printf("%s",name);
        FILE *fpr=fopen(name,"r");
        // send_msg("c2s_transfer",sock_fd);
        if(fpr){
        sendfile(fpr,sock_fd);
        send_msg("111",sock_fd);
        }
    }
   
    bzero(start,1024);
     recv_msg(sock_fd);
     }
    close(sock_fd); 



}