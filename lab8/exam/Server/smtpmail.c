#include <stdio.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netdb.h>
#include<unistd.h>
#define backlog 5
#define sockaddr struct sockaddr
int my_fun(int);

char us_name[128];
int send_msg(char *msg,int sock_fd){
  int x;
  x=send(sock_fd,msg,strlen(msg),0);
  if(x==-1){
    perror("send");
    return -1;
  }
  return 0;
}

// int checkUser(char *user){
//   const char s[2] = ",";
//   // printf("%ld %s",strlen(username),username);
//   char command[500],*name;
//   FILE *fptr = fopen("logincred.txt","r");
//   // printf("file opened\n");
//   int n=0;
//   while(fscanf(fptr, "%[^\n]", command) != EOF&&n<4){
//     fgetc(fptr);
//     // printf("%s\n",command);
//     name = strtok(command, s);
//     // printf("%ld %s",strlen(name),name);
//     ++n;
//     if(strcmp(user,name)==0){
//       strcpy(password,strtok(NULL,s));
//       strcpy(username,user);
//      return 1;
//    } 
//   }
//   return 0;
// }

int auth_user(char *username,char *password){
   FILE *fp = fopen("logincred.txt", "r");
    
    int i, j, k, state = 0;
    char str[512], user[128], pass[128];
    while(fscanf(fp, "%s", str) != -1) {
        bzero(user, sizeof(user));
        bzero(pass, sizeof(pass));

        // Checking Username
        for(i = 0, j = 0; ; i++, j++){
            if(str[j] == ',') {
                user[i++] = '\r';
                user[i++] = '\n';
                user[i++] = '\0';
                j++;
                break;
            }
            user[i] = str[j];
        }

        // printf("%s\r %d\n", user, strcmp(username, user));

        if(strcmp(user, username))
            continue;

        // Checking Password
        for(i = 0; ; i++, j++){
            if(str[j] == '\0') {
                pass[i++] = '\r';
                pass[i++] = '\n';
                pass[i++] = '\0';
                break;
            }
            pass[i] = str[j];
        }

        // printf("%s\r %d\n", pass, strcmp(password, pass));

        if(!strcmp(pass, password)) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}
void extract_name(char *dest, char *src) {
    for(; *src != '<'; src++);
    src++;
    for(; *src != '@'; src++, dest++)
        *dest = *src;
    dest++;
    *dest = '\0';
}

void append_msg(FILE *fp, char *msg) {
    for(; *msg != '\0'; msg++)
        fprintf(fp, "%c", *msg);
    fclose(fp);
}

int authenticate(int sock_fd){
  char buffer[1024];
  char username[128],password[128];
bzero(buffer, sizeof(buffer));
                recv(sock_fd, buffer, sizeof(buffer) - 1, 0);

                // 334 Username
                bzero(buffer, sizeof(buffer));
                strcpy(buffer, "334 Username\r\n");
                send_msg(buffer,sock_fd);

                // User
                bzero(username, sizeof(username));
                recv(sock_fd, username, sizeof(username) - 1, 0);

                // 334 Password
                bzero(buffer, sizeof(buffer));
                strcpy(buffer, "334 Password\r\n");
                send_msg(buffer,sock_fd);

                // Pass
                bzero(password, sizeof(password));
                recv(sock_fd, password, sizeof(password) - 1, 0);

                if(auth_user(username, password)) {
                    // 235 Authenttication Succeeded
                    bzero(buffer, sizeof(buffer));
                    bzero(us_name,128);
                    strcpy(us_name,username);
                    strcpy(buffer, "235 Authentication Succeeded\r\n");
                    send_msg(buffer,sock_fd);

                    return 0;
                }
                else {
                    // 535 Authenttication Credential Invalid 
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, "535 Authentication Credentials Invalid\r\n");
                    send_msg(buffer,sock_fd);
                    return -1;
                    
                }
                return 0;
  
}

int check(int conn_fd, char *str, int temp) {
    if(temp == 0) {
        close(conn_fd);
        printf("[-]CONNECTION(%d) CLOSED.................................\n", conn_fd);
        return 1;
    }
    else if(temp == -1) {
        perror(str);
        close(conn_fd);
        printf("[-]CONNECTION(%d) CLOSED.................................\n", conn_fd);
        return 1;
    }
    return 0;
}
int main(int argc,char *argv[]){
    struct sockaddr_in my_server,cli;
    int PORT=atoi(argv[1]);

    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd ==-1){
        printf("Socket creation failed..\n") ;
        exit(1);
        }
    printf("Socket created successfully..\n");

    my_server.sin_family=AF_INET;
    my_server.sin_addr.s_addr=INADDR_ANY;
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

    // my_fun(conn_fd);
    char buffer[1024];
    // 220
            bzero(buffer, sizeof(buffer));
            strcpy(buffer, "220 localhost service ready\r\n");
            send_msg(buffer,conn_fd);
     // EHLO
            bzero(buffer, sizeof(buffer));
            recv(conn_fd, buffer, sizeof(buffer), 0);

    // 250 OK
            bzero(buffer, sizeof(buffer));
            strcpy(buffer, "250 OK\r\n");
            send_msg(buffer,conn_fd);

    int p=authenticate(conn_fd);
    while(p==-1){
      p=authenticate(conn_fd);
    }
   
    
            char from[128], to[128], msg[1024];
            char directory[128];
            FILE *fp = NULL;
            int temp;
    
     while(1) {
                // Send || Quit
                bzero(buffer, sizeof(buffer));
                recv(conn_fd, buffer, sizeof(buffer), 0);
                if(!strcmp(buffer, "Quit\r\n"))
                    break;

                // MAIL FROM
                else if(!strcmp(buffer,"Send\r\n")){
                bzero(from, sizeof(from));
                temp = recv(conn_fd, from, sizeof(from) - 1, 0);
                if(check(conn_fd, "Receive", temp))
                    break;

                if(strncmp(from, "MAIL FROM", 9))
                    continue;

                printf("FROM: %s", from);

                // 250 OK
                bzero(buffer, sizeof(buffer));
                strcpy(buffer, "250 OK\r\n");
                temp = send(conn_fd, buffer, strlen(buffer), 0);

                // RCPT TO
                bzero(to, sizeof(to));
                temp = recv(conn_fd, to, sizeof(to) - 1, 0);
                if(check(conn_fd, "Receive", temp))
                    break;

                printf("TO: %s", to);

                // 250 OK
                bzero(buffer, sizeof(buffer));
                strcpy(buffer, "250 Accepted\r\n");
                temp = send(conn_fd, buffer, strlen(buffer), 0);

                // DATA
                bzero(buffer, sizeof(buffer));
                temp = recv(conn_fd, buffer, sizeof(buffer), 0);

                // 354 Start Mail Input
                bzero(buffer, sizeof(buffer));
                strcpy(buffer, "354 Start Mail Input\r\n");
                temp = send(conn_fd, buffer, strlen(buffer), 0);

                // MESSAGE
                bzero(msg, sizeof(msg));
                temp = recv(conn_fd, msg, sizeof(msg), 0);

                // Extracting the "directory" name from "to"
                bzero(directory, sizeof(directory));
                extract_name(directory, to);

                bzero(buffer, sizeof(buffer));
                sprintf(buffer, "%s/mymailbox.mail", directory);

                // Trying to append the message
                fp = fopen(buffer, "a");
                if(fp == NULL) {
                    //553 or 554 for transaction failure (DOUBT)
                    printf("FILE NOT FOUND\n");
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, "553\r\n");
                    temp = send(conn_fd, buffer, sizeof(buffer), 0);
                }
                else {
                    append_msg(fp, msg);

                    //250 OK
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, "250 OK\r\n");
                    temp = send(conn_fd, buffer, sizeof(buffer), 0);
                } 
            }
    else if(!strcmp(buffer,"Manage\r\n")){
                char path[512];
                char us[128];
                strcpy(us,us_name);
                us[strlen(us)-2]='\0';
                strcpy(path,us);
                // path[strlen(path)-1]="\0";
                strcat(path,"/mymailbox.mail");
                FILE *fp1=fopen(path,"r");
             
                char inboxList[10][512];
                int i=0;
                while(i<10){
                    inboxList[i][0]='!';
                    i++;
                }
                char buf[100], buf1[100], buf2[100], buf3[100];
	char *token1, *token2, *token3, *token4, *token5, *token6;
	i = 0;
	while(1) {
		if(fgets(buf, 100, fp1) == NULL)
			break;
		if(strncmp(buf, "From:", 5) == 0) {
			token1 = strtok(buf, ": ");
			token2 = strtok(NULL, ": ");
			strcpy(buf1, token2);
		}
		else if(strncmp(buf, "Subject:", 8) == 0) {
			token3 = strtok(buf, ": ");
			token4 = strtok(NULL, ":");
			strcpy(buf2, token4);
		}
		else if(strncmp(buf, "Received:", 9) == 0) {
			token5 = strtok(buf, ": ");
			token6 = strtok(NULL, ":");
			strcpy(buf3, token6);
		}
		else if(strncmp(buf, ".", 1) == 0) {
			bzero(inboxList[i], 256);
			sprintf(inboxList[i], "%s %s %s", buf1, buf2, buf3);
			i++;
		}
	}
    fclose(fp1);
    fp1=fopen(path,"r");
	send(conn_fd, &inboxList, sizeof(inboxList), 0);
    bzero(buffer, sizeof(buffer));
    temp = recv(conn_fd, buffer, sizeof(buffer), 0);
    if(strncmp(buffer,"inv",3)==0){
        bzero(buffer, sizeof(buffer));
    temp = recv(conn_fd, buffer, sizeof(buffer), 0);
    }
    if(strncmp(buffer,"inv",3)){
    //  printf("%s-%d\n",buffer,strlen(buffer));
                 char f_inboxList[10][512];
                i=0;
                while(i<10){
                    f_inboxList[i][0]='!';
                    i++;
                }
//  printf("%s\n",path);
    char buf0[100],buf4[100];       
    bzero(buf,100);bzero(buf1,100);bzero(buf2,100);bzero(buf3,100);bzero(buf4,100);
    bzero(buf0,100);

    char txt[1024];char *token7,*token8;
    bzero(txt,1024);

	i = 0;
	while(1) {
		if(fgets(buf, 100, fp1) == NULL)
			break;
		if(strncmp(buf, "From:", 5) == 0) {
			token1 = strtok(buf, ": ");
			token2 = strtok(NULL, ": ");
			strcpy(buf1,token1);
            strcat(buf1,":");
            strcat(buf1,token2);
		}
        if(strncmp(buf, "To:", 3) == 0) {
			token7 = strtok(buf, ": ");
			token8 = strtok(NULL, ": ");
			strcpy(buf0, token7);
            strcat(buf0,":");
            strcat(buf0,token8);
		}
		else if(strncmp(buf, "Subject:", 8) == 0) {
			token3 = strtok(buf, ": ");
			token4 = strtok(NULL, ":");
			strcpy(buf2, token3);
            strcat(buf2,":");
            strcat(buf2,token4);
		}
		else if(strncmp(buf, "Received:", 9) == 0) {
			token5 = strtok(buf, ": ");
			token6 = strtok(NULL, ":");
			strcpy(buf3, token5);
            strcat(buf3,":");
            strcat(buf3,token6);
		}
		else if(strncmp(buf, ".", 1) == 0) {
			bzero(f_inboxList[i], 256);
			sprintf(f_inboxList[i], "%s %s %s %s \n%s", buf1,buf0, buf2, buf3,buf4);
			i++;
		}
        else{
            strcpy(buf4,buf);
        }
        // printf("%s",buf);
	}
    send(conn_fd, &f_inboxList, sizeof(f_inboxList), 0);
    fclose(fp1);
    bzero(buf,sizeof(buf));
    }
     }
     }
            close(conn_fd);
            // printf("[-]CONNECTION(%d) CLOSED GRACEFULLY......................\n", conn_fd); 
    close(sock_fd);

}
