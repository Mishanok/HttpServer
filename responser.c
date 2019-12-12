//responser.c
#include "responser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define HEADERLENGTH 12

void createResponseFAIL(enum StatusCode code, char * message)
{
	char * header = (char *)malloc(sizeof(char) * (HEADERLENGTH + strlen(message)));
	sprintf(header, "HTTP/1.1 %d %s", code, message);
	response = header;
}

void createResponseOK(char * filepath)
{
	FILE * file = fopen(filepath, "r");

	char buffer[1000];

	char c = fgetc(file);
	int i =0;
	int dotInxex;
	while(c != EOF)
	{
	    buffer[i] = c;
	    c = fgetc(file);
	    i++;
	}
	buffer[i+1] = '\0';

	const char * contentType= getContentType(filepath);
	if(contentType == NULL)
	{
	    createResponseFAIL(NotFound, "NOT FOUND");
	    return;
	}

	int contentLength = strlen(buffer);
	char header[150];
	int bytes = snprintf(header,150, "HTTP/1.1 200 OK\n");
	bytes+= snprintf(header + bytes, 150, "Content-Type: %s\nContent-Length: %d\n\n\0", contentType, contentLength);

	response = (char *)malloc(sizeof(char) * (contentLength + strlen(header)));

	bytes = snprintf(response, 150, "%s", header);
	bytes += snprintf(response + bytes, 1000, "%s", buffer);
}

static const char * getContentType(char * filepath)
{
	char * token = strtok(filepath, ".");
	char * buffer = (char *)malloc(sizeof(char) * 100);
	enum ContentType type;
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
	    type  = Unknown;

	return getStrFromType(type);
}

static const char * getStrFromType(enum ContentType type)
{
	switch(type)
	{
	case Text:
	    return "text/plain; charset=UTF-8";
	    break;
	case HTML:
	    return "text/html; charset=UTF-8";
	    break;
	case ImageJPEG:
	    return "image/jpeg";
	    break;
	case ImagePNG:
	    return "image/png";
	    break;
	default:
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
	printf("-------------------------REQUEST-------------------\n");
	printf("%s", response);
	free(response);
	return 0;
}
