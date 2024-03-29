//listener.c
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "listener.h"
#include "processor.h"

int server_fd, new_socket;
struct sockaddr_in address;
int addrlen = sizeof(address);


int bindSocket(int port)
{
	// Creating socket file descriptor
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
	    perror("In socket");
	    exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	memset(address.sin_zero, '\0', sizeof address.sin_zero);


	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
	    perror("In bind");
	    exit(EXIT_FAILURE);
	}

	return server_fd;

}

void listenRequests(int socket)
{
	if(listen(server_fd, 10) < 0)
	{
	    perror("In listen");
	    exit(EXIT_FAILURE);
	}
	while(1)
	{
	    processConnection();
	}

}

static void processConnection()
{
	int new_sokcet;
	long valread;
	printf("\n+++++++++++ Waiting for new connection+++++++++\n\n");
	if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
	{
	    perror("In-accept");
	    return;
	}

	char buffer[30000] = {0};
	valread = read(new_socket, buffer, 30000);
	printf("%s\n", buffer);
	processRequest(&new_socket, buffer);

	close(new_socket);
}
