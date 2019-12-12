//listener.h
#ifndef LISTENER
#define LISTENER
int bindSocket(int port);
void listenRequests(int socket);
static void processConnection();
#endif
