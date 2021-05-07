#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/select.h>

#define PORT 8080

int my_func(int sock_fd, char *str, int return_val, fd_set *main_fds) {
	
	if(return_val == 0) {
		printf("Connection closed for: %d\n", sock_fd);
		FD_CLR(sock_fd, main_fds);
		close(sock_fd);
		return 0;
	}
	
	else if(return_val == -1) {
		perror(str);
		FD_CLR(sock_fd, main_fds);
		close(sock_fd);
		return 0;
	}
	
	return 1;
}

int main() {
	int server_fd;
	int high;

	fd_set read_fds;
	fd_set main_fds;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(server_fd == -1) {
		perror("Socket");
		exit(0);
	}

	printf("Socket Created Successfully.\n");
	high = server_fd;

	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	int serv_addr_len = sizeof(serv_addr);
	int client_addr_len = sizeof(client_addr);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET, "127.0.0.2", &(serv_addr.sin_addr)) <= 0) {
		perror("Address Conversion");
		exit(0);
	}

	if(bind(server_fd, (struct sockaddr *)&serv_addr, serv_addr_len) == -1) {
		perror("Bind");
		exit(0);
	}

	printf("Binded Successfully...\n");

	if(listen(server_fd, 5) == -1) {
		perror("Listen");
		exit(0);
	}

	printf("Listening...\n");

	FD_ZERO(&read_fds);
	FD_ZERO(&main_fds);

	int temp, i, j, sum, new_conn;
	char buffer_1[1024], buffer_2[1024];
	char client[10][256];

	FD_SET(server_fd, &main_fds);
	printf("------------------------------------------------------\n");
	 while(1){
		read_fds = main_fds;

		if(select(high + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("Select");
			exit(0);
		}

		for(i = 0; i < high + 1; i++) {
			if(FD_ISSET(i, &read_fds)) {

				if(i == server_fd) {
					new_conn = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

					if(new_conn == -1) 
						perror("Accept");
					else {
						printf("New Connection : %d\n", new_conn);
						FD_SET(new_conn, &main_fds);
						strcpy(buffer_1, "Welcome to Group Chat\nTo Leave the Chat enter \"exit\"\nActive clients: ");
						temp = send(new_conn, buffer_1, strlen(buffer_1) + 1, 0);
						if(my_func(new_conn, "Send", temp, &main_fds)==0)
							continue;

						temp = recv(new_conn, buffer_1, sizeof(buffer_1), 0);
						if(my_func(new_conn, "Receive", temp, &main_fds)==0)
							continue;

						sum = 0;
						for(j = 0; j < high + 1; j++) {
							if((j == server_fd) || (j == new_conn))
								continue;

							if(FD_ISSET(j, &main_fds)) {

								sum += 1;
								sprintf(buffer_1, "%s(%d)", client[j], j);
								
								temp = send(new_conn, buffer_1, strlen(buffer_1) + 1, 0);
								if(my_func(new_conn, "Send", temp, &main_fds)==0)
									continue;

								temp = recv(new_conn, buffer_1, sizeof(buffer_1), 0);
								if(my_func(new_conn, "Receive", temp, &main_fds)==0)
									continue;
							}
						} 
						
						if(sum == 0) {
							strcpy(buffer_1, "None");
							temp = send(new_conn, buffer_1, strlen(buffer_1) + 1, 0);
							if(my_func(new_conn, "Send", temp, &main_fds)==0)
								continue;

							temp = recv(new_conn, buffer_1, sizeof(buffer_1), 0);
							if(my_func(new_conn, "Receive", temp, &main_fds)==0)
								continue;
						}

						strcpy(buffer_1, "Enter Your Name:");
						temp = send(new_conn, buffer_1, strlen(buffer_1) + 1, 0);
						if(my_func(new_conn, "Send", temp, &main_fds)==0)
							continue;

						temp = recv(new_conn, buffer_1, sizeof(buffer_1), 0);
						if(my_func(new_conn, "Receive", temp, &main_fds)==0)
							continue;

						temp = recv(new_conn, buffer_1, sizeof(buffer_1), 0);
						if(my_func(new_conn, "Receive", temp, &main_fds))
							strcpy(client[new_conn], buffer_1);
						
						if(FD_ISSET(new_conn, &main_fds) && high < new_conn)
							high = new_conn;

						printf("A new Client has entered..\n");

						sprintf(buffer_1, "SERVER: %s(%d) joined the chat", client[new_conn], new_conn);
						for(j = 0; j < high + 1; j++) {
							if(j == server_fd || j == new_conn)
								continue;
							if(FD_ISSET(j, &main_fds)) {
								temp = send(j, buffer_1, strlen(buffer_1) + 1, 0);
								printf("Message sent: \"%s\"\n", buffer_1);
								if(my_func(new_conn, "Send", temp, &main_fds)==0)
									continue;
							}
						}
					}
				}

				else {
					temp = recv(i, buffer_1, sizeof(buffer_1), 0);

					if(my_func(i, "Receive", temp, &main_fds)){
						printf("%s from %d\n", buffer_1, i);

						for(j = 0; j < high + 1; j++) {
							if(j == server_fd || j == i)
								continue;

							if(FD_ISSET(j, &main_fds)) {
								sprintf(buffer_2, "%s(%d): %s", client[i], i, buffer_1);
								temp = send(j, buffer_2, strlen(buffer_2) + 1, 0);
								my_func(j, "Send", temp, &main_fds);
							}
						}

						if(strcmp(buffer_1, "exit") == 0) {
							sprintf(buffer_1, "SERVER: %s(%d) left the chat", client[i], i);
							for(j = 0; j < high + 1; j++) {
								if(j == server_fd || j == i)
									continue;
								if(FD_ISSET(j, &main_fds)) {
									temp = send(j, buffer_1, strlen(buffer_1) + 1, 0);
									printf("Message Sent: %s\n", buffer_1);
									if(my_func(new_conn, "Send", temp, &main_fds)==0)
										continue;
								}
							}

							close(i);
							FD_CLR(i, &main_fds);
						}
					}	
				}	
			}
		}
	}
	FD_ZERO(&read_fds);
}

