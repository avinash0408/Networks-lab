#include <stdio.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netdb.h>
#include<unistd.h>
#define sockaddr struct sockaddr
#include<time.h>

char username[1024],pass[1024];
int send_msg(char *msg,int sock_fd){
  int x;
  x=send(sock_fd,msg,strlen(msg),0);
  if(x==-1){
    perror("send");
    return -1;
  }
  return 0;
}

// int auth_name(int sock_fd){
//   printf("Username :");
//   fgets(username,sizeof(username),stdin);
//   username[strlen(username)-1]='\0';
//   // send_msg(username,sock_fd);
//   char buff[1024];
//   bzero(buff,1024);
//   if(recv(sock_fd,buff,sizeof(buff),0)==-1){
//     perror("receive");
//     return -2;
//   }
//   // printf("%s %d\n",buff,strlen(buff));
//   if(strcmp(buff,"301")==0){
//     return -1;
//   }
//   return 0;

// }

// int auth_pass(int sock_fd){
//   printf("Password :");
//   fgets(pass,sizeof(pass),stdin);
//   // pass[strlen(pass)-1]='\0';
//   send_msg(pass,sock_fd);
//   char buff[1024];
//   bzero(buff,1024);
//   if(recv(sock_fd,buff,sizeof(buff),0)==-1){
//     perror("receive");
//     return -2;
//   }
//   // printf("%s %d\n",buff,strlen(buff));
//   if(strcmp(buff,"310")==0){
//     return -1;
//   }
//   return 0;
  
// }

int authenication(int sock_fd){
  char buffer[1024];
    bzero(buffer, sizeof(buffer));
        strcpy(buffer, "AUTH LOGIN\r\n");
        send_msg(buffer,sock_fd);

        // 334 Username: 
        bzero(buffer, sizeof(buffer));
        recv(sock_fd, buffer, sizeof(buffer), 0);
        printf("USERNAME: ");

        // User
        bzero(buffer, sizeof(buffer));
        scanf("%s", buffer);
        bzero(username,1024);
        strcpy(username,buffer);
        strcat(buffer, "\r\n");
        send_msg(buffer,sock_fd);

        // 334 Password 
        bzero(buffer, sizeof(buffer));
        recv(sock_fd, buffer, sizeof(buffer), 0);
        printf("PASSWORD: ");

        // Pass
        bzero(buffer, sizeof(buffer));
        scanf("%s", buffer);
        strcat(buffer, "\r\n");
        send_msg(buffer,sock_fd);

        // 235 Authentication Succeeded || 535 Authentication Credentials Invalid
        bzero(buffer, sizeof(buffer));
        recv(sock_fd, buffer, sizeof(buffer), 0);
        printf("SERVER: %s", buffer);
        if(strncmp(buffer,"235",3)!=0) return -1;
        return 0;
}
int search(char *str) {
    for(int i = 0; i < strlen(str); i++) 
        if(str[i] == '@')
            return 1;

    return 0;
   
    // if(strncmp(username,str,l)==0 && str[l]=='@')
    // return 1;
    // return 0;
}

int main(int argc,char *argv[]){
    struct sockaddr_in my_server;
  int PORT=atoi(argv[1]);
    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
      if(sock_fd ==-1){
        printf("Socket creation failed..\n") ;
        exit(1);
        }
    // printf("Socket created successfully..\n");
    my_server.sin_family=AF_INET;
    my_server.sin_port=htons(PORT);
    my_server.sin_addr.s_addr=INADDR_ANY;

      if (connect(sock_fd, (sockaddr*)&my_server, sizeof(my_server)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(1); 
    }
    char buffer[1024];
     // 220
    recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
    printf("SERVER: %s", buffer);

    // EHLO
    bzero(buffer, sizeof(buffer));
    strcpy(buffer, "EHLO\r\n");

    send_msg(buffer,sock_fd);

    // send(sock_fd, buffer, strlen(buffer), 0);

    // 250 OK
    bzero(buffer, sizeof(buffer));
     recv(sock_fd, buffer, sizeof(buffer), 0);

   
  
    // my_fun(sock_fd);
    // int p;
    // p=auth_name(sock_fd);
    //  while(p==-1){
    //   printf("Invalid Username. Try again..!\n");
    //   p=auth_name(sock_fd);
    // } 
    // p=auth_pass(sock_fd);
    //  while(p==-1){
    //   printf("Wrong password. Try again..!\n");
    //   p=auth_pass(sock_fd);
    // }
    int p;
    do{
      p=authenication(sock_fd);
    }while(p==-1);

  
  //  while(1){
  //   printf("---------------------------------------------------------------\n");
  //   printf("1. Send Mail\n2. Quit\n");
  //   int t;
  //   char mail[1024];
  //   printf("Your option:");
  //   scanf("%d",&t);
  //   if(t!=1 || t!=2){
  //     printf("Invalid command...!");
  //     continue;
  //   }
  //   if(t==2){
  //     break;
  //   }
  //   char line[80];
  //   if(t==1){
  //     printf("**************\n");
  //   bzero(mail,1024);
  //   while(1){
  //     bzero(line,80);
  //   fgets(line,sizeof(line),stdin);
  //   line[strlen(line)-1]='\0';
  //   strcat(mail,line);
  //   strcat(mail,"\r\n");
  //   if(strcmp(line,".")==0) break;

  //   }
  //    printf("%s",mail);
  //   }
  //  }
    char from[128], to[128], subject[128], line[128];
    char msg[1024];
    char buffer_1[16], buffer_2[16], buffer_3[16],buffer_4[128],buffer_5[1024];
    char *re_msg[1024];
    char bef[128];
    
  int temp;
while(1) {
   printf("---------------------------------------------------------------\n");
        printf("1)Send - To send mail\n2)Quit - Close the application\n");
        bzero(buffer, sizeof(buffer));
        scanf("%s", buffer);

        temp = strlen(buffer);
        buffer[temp] = '\r';
        buffer[temp + 1] = '\n';
        temp = send(sock_fd, buffer, strlen(buffer), 0);

        if(strcmp(buffer, "Quit\r\n") == 0)
            break;
        else if(strcmp(buffer, "Send\r\n") != 0) {
            printf("Invalid Command\n");
            continue;
        }

        //To read newline character from scanf
        getchar();
        printf("Enter your mail\n");
        bzero(bef,128);
        strcpy(bef,"Received: ");
        bzero(msg, sizeof(msg));
        bzero(line, sizeof(line));
        struct tm* ptr;
        time_t lt;
        lt=time(NULL);
        ptr=localtime(&lt);
        char time[128];
        strcpy(time,asctime(ptr));
        strcat(bef,time);
        temp=strlen(time);
        // strcat(time,"\r\n");
        int j=0;
        for(int i = 0; fgets(line, 80, stdin) && i < 80; i++, bzero(line, sizeof(line))) {
            temp = strlen(line);
            line[temp - 1] = '\r';
            line[temp] = '\n';
            if(j==3){
              strcat(msg,bef);
            }
            strcat(msg, line);
            j=j+1;

            if(!strncmp(line, ".\r\n", 3))
                break;
        }

        bzero(from, sizeof(from));
        bzero(to, sizeof(to));
        sscanf(msg, "%s %s\r\n%s %s\r\n%s", buffer_1, from, buffer_2, to, buffer_3);
        //  printf("%s\n",msg);
        // printf("%s-%d\n",buffer_1,strlen(buffer_1));
        // printf("%s-%d\n",from,strlen(from));
        // printf("%s-%d\n",buffer_2,strlen(buffer_2));
        // printf("%s-%d\n",to,strlen(to));
        //  printf("%s-%d\n",buffer_4,strlen(buffer_4));
        //  printf("%s-%d\n",buffer_5,strlen(buffer_5));
       
        




        if(!strcmp(buffer_1, "From:") && !strcmp(buffer_2, "To:") && !strcmp(buffer_3, "Subject:") && search(from) && search(to) &&!strncmp(username, from, strlen(username))) {
          
            // MAIL FROM
            bzero(buffer, sizeof(buffer));
            sprintf(buffer, "MAIL FROM: <%s>\r\n", from);
            // printf("<%s>\r\n", buffer);
            temp = send(sock_fd, buffer, strlen(buffer), 0);

            // 250 OK
            bzero(buffer, sizeof(buffer));
            temp = recv(sock_fd, buffer, sizeof(buffer), 0);
            // printf("<%s>\r\n", buffer);

            // RCPT TO
            bzero(buffer, sizeof(buffer));
            sprintf(buffer, "RCPT TO: <%s>\r\n", to);
            temp = send(sock_fd, buffer, strlen(buffer), 0);
            // printf("<%s>\r\n", buffer);

            // 250 OK
            bzero(buffer, sizeof(buffer));
            temp = recv(sock_fd, buffer, sizeof(buffer), 0);
            // printf("<%s>\r\n", buffer);

            // DATA
            bzero(buffer, sizeof(buffer));
            strcpy(buffer, "DATA\r\n");
            temp = send(sock_fd, buffer, strlen(buffer), 0);
            // printf("<%s>\r\n", buffer);

            // 354 Start Mail Input
            bzero(buffer, sizeof(buffer));
            temp = recv(sock_fd, buffer, sizeof(buffer), 0);
            // printf("<%s>\r\n", buffer);

            // MESSAGE
            temp = send(sock_fd, msg, strlen(msg), 0);

            // 250 OK
            bzero(buffer, sizeof(buffer));
            temp = recv(sock_fd, buffer, sizeof(buffer), 0);
            // printf("<%s>\r\n", buffer);
            
            if(!strncmp(buffer, "250", 3))
                printf("Mail sent successfully\n");
            else if(!strncmp(buffer, "553", 3))
                printf("To address not found..\n");
        }
        else {
            printf("Incorrect Format\n");
            bzero(buffer, sizeof(buffer));
            strcpy(buffer, "Invalid Message Format\r\n");
            temp = send(sock_fd, buffer, strlen(buffer), 0);
        }
    }

    
    close(sock_fd); 

}