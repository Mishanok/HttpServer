//responser.c
#include "responser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define HEADERLENGTH 12
#define BUFFLEN 1024
#define HEADER_OK_LEN 150

enum ContentType type;
int sizeOfType;
static char * response = NULL;
void* fileContent = NULL;

void createResponseFAIL(enum StatusCode code, char * message)
{
	char * header = (char *)malloc(sizeof(char) * (HEADERLENGTH + strlen(message)));
	sprintf(header, "HTTP/1.1 %d %s", code, message);
	response = header;
}

void createResponseOK(char * filepath)
{
	char* filePathCopy = (char *)malloc(sizeof(char) * strlen(filepath));
	strcpy(filePathCopy, filepath);
	const char * contentType= getContentType(filePathCopy);
	if(contentType == NULL)
	{
	    createResponseFAIL(NotFound, "NOT FOUND");
	    return;
	}

	void * buffer = getFileContent(filepath, type);

	int contentLength = sizeof(buffer);
	char header[HEADER_OK_LEN];
	int bytes = snprintf(header,HEADER_OK_LEN, "HTTP/1.1 200 OK\n");
	bytes+= snprintf(header + bytes, HEADER_OK_LEN, "Content-Type: %s\nContent-Length: %d\n\n\0", contentType, contentLength);

	response = (char *)malloc(sizeof(char) * (contentLength + strlen(header)));
	fileContent = buffer;

	bytes = snprintf(response, HEADER_OK_LEN, "%s", header);
}

static const char * getContentType(char * filepath)
{
	char * token = strtok(filepath, ".");
	char * buffer = (char *)malloc(sizeof(char) * 100);
	do
	{
	    strcpy(buffer, token);
	    token = strtok(NULL, ".");
	}while(token != NULL);

	if(strcmp(buffer, "html") == 0)
	    type  = HTML;
	else if(strcmp(buffer, "txt") == 0)
	    type  = Text;
	else if(strcmp(buffer, "jpeg") ==0 || strcmp(buffer, "jpg") == 0)
	    type  = ImageJPEG;
	else if(strcmp(buffer, "png") == 0)
	    type  = ImagePNG;
	else
	    type  = Text;

	return getStrFromType(type);
}

static const char * getStrFromType(enum ContentType contType)
{
	switch(type)
	{
	case Text:
	    sizeOfType = sizeof(char);
	    return "text/plain; charset=UTF-8";
	    break;
	case HTML:
	    sizeOfType = sizeof(char);
	    return "text/html; charset=UTF-8";
	    break;
	case ImageJPEG:
	    sizeOfType = 1;
	    return "image/jpeg";
	    break;
	case ImagePNG:
	    sizeOfType = 1;
	    return "image/png";
	    break;
	default:
	    sizeOfType = 0;
	    return NULL;
	    break;
	}
}

int sendResponse(int* socket, enum StatusCode status, char* filepath)
{
	switch(status)
	{
	case NotFound:
	    createResponseFAIL(status, "NON FOUND");
	    break;
	case BadRequest:
	    createResponseFAIL(status, "Bad Request");
	    break;
	case NotImplemented:
	    createResponseFAIL(status, "Not implemented");
	    break;
	case OK:
	    createResponseOK(filepath);
	    break;
	}

	write(*socket, response, strlen(response));
	if(fileContent != NULL)
	    write(*socket, fileContent, sizeof(fileContent));
	printf("-------------------------REQUEST-------------------\n");
	printf("%s", response);
	free(response);
	return 0;
}

static void* getFileContent(char* filepath, enum ContentType contentType)
{
	FILE * file = fopen(filepath, "rb");

	long size = 0;

	fseek(file, 0L, SEEK_END);
	size = ftell(file);

	void * buffer = malloc(size);
	fread(buffer, sizeOfType, size / sizeOfType, file);
	fclose(file);

	return buffer;
}
