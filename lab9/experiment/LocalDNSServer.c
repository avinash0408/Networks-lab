#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netdb.h>
#include <arpa/inet.h> 
#include<unistd.h>
#define sockaddr struct sockaddr


char* find_ser(char *line){
    char dl[3]="= ";
    char *token;
    token=strtok(line,dl);
    token=strtok(NULL,dl);
    if(strncmp(token,"nam",3)==0){
        token=strtok(NULL,dl);
        return "";
    }
    token[strlen(token)-1]='\0';
    return token;
}
char fin[1024];
char domain[1024];
char type[10];
char cache_rep[2048];
char* parse(char *domain){
    system(": > out.txt");
    char command[512];
    strcpy(command,"nslookup -type=ns ");
    strcat(command,domain);
    strcat(command," >> out.txt");
    system(command);
    FILE *fp=fopen("out.txt","r");

    char buff[1024];
    int len=strlen(domain);
    int i=0;
    char ch;
      while(fgets(buff,1024,fp)) {
         if(strncmp(buff,domain,strlen(domain))==0){
             char server1[1024];
             strcpy(server1,find_ser(buff));
             strcpy(fin,server1);
             break;
         }
       
     }
     fclose(fp);
    
}
void Type_domain(char *line){
    char dl[2]="=";
    char *token;
    token=strtok(line,dl);
    token=strtok(NULL,dl);
    // printf("%s ",token);
    bzero(type,sizeof(type));
    bzero(domain,sizeof(domain));
    sscanf(token,"%s %s",type,domain);
    //  printf("%s-%d\n%s-%d\n",type,strlen(type),domain,strlen(domain));

}

int check_cache(){
    FILE *fp=fopen("cache.txt","r");
    char check[512];
    strcpy(check,domain);
    strcat(check,"-");
    strcat(check,type);
    char buff[1024];
    bzero(cache_rep,2048);
    while(fgets(buff,1024,fp)){
        if(strncmp(buff,check,strlen(check))==0){
            while(1){
                bzero(buff,1024);
                fgets(buff,1024,fp);
                if(strncmp(buff,"---",3)==0) break;
                strcat(cache_rep,buff);
            }
            return 1;
        }
    }
return 0;
}

void cache(char *text){
    FILE *fp=fopen("cache.txt","a");
    fprintf(fp,"%s-%s\n",domain,type);
    fprintf(fp,"%s\n",text);
    fprintf(fp,"--------------------------\n");
    fclose(fp);
}

int main(int argc,char *argv[]){
    int PORT=atoi(argv[1]);
    struct sockaddr_in my_addr,cli;
    int sock_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(sock_fd<0){
        printf("Socket creation failed..\n");
        exit(1);
    }
    my_addr.sin_family = AF_INET; // IPv4 
    my_addr.sin_addr.s_addr = INADDR_ANY; 
    my_addr.sin_port = htons(PORT); 
    if(bind(sock_fd,(sockaddr*)&my_addr,sizeof(my_addr))<0){
        printf("bind failed..\n");
        exit(1);
    }
    int len,n;
    len=sizeof(cli);
    char buff[1024];
    while(1){
    bzero(buff,1024);
    n=recvfrom(sock_fd,(char *)buff,1024,MSG_WAITALL,(sockaddr*)&cli,&len);
    if(strncmp(buff,"quit",4)==0) exit(0);

    Type_domain(buff);
     printf("Domain: %s\nType: %s\n\n",domain,type);
     if(check_cache()==1){
            printf("Answer found in cache..\n-------------\n");
            sendto(sock_fd, (char *)cache_rep, strlen(cache_rep),  
        MSG_CONFIRM, (sockaddr *) &cli, len); 
            
     }
    else{
    char split[10][1024];
    strcpy(split[0],domain);
    char dumb[1024];
    strcpy(dumb,domain);
    char dl[2]=".";
    char *token;
    token=strtok(dumb,dl);
    int i=1;
    while(token!=NULL){
        strcpy(split[i++],token);
        token=strtok(NULL,dl);
    }
    n=i;
    char step_resolve[n][1024];
    strcpy(step_resolve[0],split[n-1]);
    int j=1;
    for(i=n-2;i>0;i--){
        strcpy(step_resolve[j],split[i]);
        strcat(step_resolve[j],".");
        strcat(step_resolve[j],step_resolve[j-1]);
        j=j+1;
    }
     n=n-1;
    for(i=0;i<n;i++){
        if(i==0) parse(".");
        parse(step_resolve[i]);
    }
    char result_cmd[1024];
    // if(strcmp(type,"a")==0){
    strcpy(result_cmd,"nslookup -type=");
    strcat(result_cmd,type);
    strcat(result_cmd," ");
    strcat(result_cmd,step_resolve[n-1]);
    strcat(result_cmd," ");
    strcat(result_cmd,fin);
    system(": > out.txt");
    strcat(result_cmd,">> out.txt");
    // printf("command:%s",result_cmd);
     system(result_cmd);
    // }
        char text[2048];
        bzero(text,2048);
        char buff1[1024];
        FILE *fp=fopen("out.txt","r");
         while(fgets(buff1,1024,fp)) {
             strcat(text,buff1);
         }
    // printf("%s",text);
      sendto(sock_fd, (char *)text, strlen(text),  
        MSG_CONFIRM, (sockaddr *) &cli, len); 
    fclose(fp);

    cache(text);
    printf("******Written to cache\n");
    printf("-------------\n");
   
    }
    }
    close(sock_fd);

}