#include <stdio.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<dirent.h>
#define PORT 8080
#define backlog 5
#define sockaddr struct sockaddr
int my_fun(int);
char username[500], password[500];

int sendtoclient(char *code,int sock_fd){
        if (send(sock_fd, code,strlen(code)+1, 0) == -1) {
      perror("code sending..");
      exit(1);
    }
}
int sendfile(int sock_fd,FILE *fp){
    
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
             printf("file received from Client..\n");
            sendtoclient("f_done",sock_fd);
            fclose(fp);
            break;
            return 0;
        }
        fprintf(fp,"%s",msg);
        // printf("%s\n",msg);
        bzero(msg,size);
    }
}


// int recv_file(FILE *fp,int sock_fd){
//     int size=500;
//     char msg[size];
//     while(1){
//         if(recv(sock_fd,msg,sizeof(msg),0) <0){
//             perror("receiving");
//             exit(0);
//         }
//         if(strcmp(msg,"Gotyou!")==0){
//             printf("File transmission done..\n"); 
//             break;
//             return 0;
//         }
//         char *conf="received";
//         send(sock_fd,msg,sizeof(msg),0);
//         fwrite(msg,1,sizeof(msg),fp);
//         bzero(msg,size);
//     }
// }

int checkUser(char *username){
  const char s[2] = ",";
  // printf("%ld %s",strlen(username),username);
  char command[500],*name;
  FILE *fptr = fopen("logincred.txt","r");
  // printf("file opened\n");
  int n=0;
  while(fscanf(fptr, "%[^\n]", command) != EOF&&n<4){
    fgetc(fptr);
    // printf("%s\n",command);
    name = strtok(command, s);
    // printf("%ld %s",strlen(name),name);
    ++n;
    if(strcmp(username,name)==0){
      strcpy(password,strtok(NULL,s));
     return 1;
   } 
  }
  return 0;
}

int checkUserPassword(char *password1){
  // printf("Password: %s, Password1: %s\n",password,password1);
  if(strcmp(password,password1)==0){
    return 1;
  } return 0;
  
}
int send_dir(int sock_fd){
   DIR *folder;
    struct dirent *entry;
    int files = 0;

    folder = opendir(".");
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return(1);
    }

    while( (entry=readdir(folder)) )
    {
        files++;
        printf("%s\n",entry->d_name);
        sendtoclient(entry->d_name,sock_fd);
    }
    sendtoclient("stop_dir",sock_fd);
    closedir(folder);

    return(0);
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
while(1){
    my_fun(conn_fd);
}
    close(sock_fd);

}


int my_fun(int socket_fd){
    char client_message[500];
  recv(socket_fd,client_message,sizeof(client_message),0);
  printf("client message: %s\n",client_message);
  const char s[2] = " ";
  char command[500],name[500];
   strcpy(name, strtok(client_message, s));
   strcpy(command,name);
  if(strcmp(command,"USERN") == 0){
    strcpy(name ,strtok(NULL, s));
      // printf("%s\n %s\n",command,name);
      if(checkUser(name)){
        sendtoclient("300",socket_fd);
        strcpy(username,name);
      }else{
        sendtoclient("301",socket_fd);
      }
  }   
  else  if(strcmp(command,"StoreFile") == 0){
    strcpy(name ,strtok(NULL, s));
      // printf("%s\n %s\n",command,name);
      FILE *fpw=fopen(name,"w");
      recv_file(fpw,socket_fd);

  }
    else  if(strcmp(command,"dir_done") == 0){
      sendtoclient("f_dir",socket_fd);

  }
    else if(strcmp(command, "QUIT")==0){
	    close(socket_fd);
	    printf("client connection ended ...\n");
	    exit(0);
    }
     else if(strcmp(command, "f_done")==0){
       sendtoclient("ff_done",socket_fd);
    }
   else if(strcmp(command,"PASSWD")==0){
      strcpy(name ,strtok(NULL, s));
      if(checkUserPassword(name)){
        sendtoclient("305",socket_fd);
      }else{
        sendtoclient("310",socket_fd);
      }
     }
       else if(strcmp(command,"ListDir")==0){
          send_dir(socket_fd);
          sendtoclient("f_dir",socket_fd);
     }
     else if(strcmp(command,"CreateFile")==0){
       strcpy(name,strtok(NULL,s));
       FILE *fnew=fopen(name,"w");
       fclose(fnew);
       sendtoclient("000",socket_fd);
     }
     else if(strcmp(command,"GetFile")==0){
       strcpy(name,strtok(NULL,s));
       FILE *fg=fopen(name,"r");
       if(fg){
         printf("sending\n");
         sendfile(socket_fd,fg);
         sendtoclient("111",socket_fd);
       }

       } 
     //  sendfile(fg,socket_fd);
     
     else {
       sendtoclient("505",socket_fd);
     }

   
   return 0; 

}