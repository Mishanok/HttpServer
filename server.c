//server.c
#include <stdio.h>
#include "processor.h"
#include "listener.h"

#define PORT 8080
int main(int argc, char const *argv[])
{
	int socket = bindSocket(PORT);

	listenRequests(socket);

	return 0;
}
