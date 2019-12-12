//processor.h
#ifndef PROCESSOR
#define PROCESSOR
int processRequest(int* socket,char * request);
int parse(char * request, char * filepath);
char * createResponse(char * filepath);
#endif
