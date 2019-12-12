//processor.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processor.h"
#include "responser.h"
#include <unistd.h>

int processRequest(int* socket, char * request)
{
	char *filepath = (char * )malloc(sizeof(char) * 100);
	if(parse(request, filepath) == 1)
	{
	    sendResponse(socket, BadRequest, NULL);
	    return 1;
	}

	if(access(filepath, F_OK))
	{ //file doesn`t exist
	    sendResponse(socket ,NotFound, NULL);
	    return 1;
	}
	else if(strcmp(filepath, "/") == 0)
	    sendResponse(socket, OK, "/index.html");
	else
	    sendResponse(socket, OK, filepath);

	if(filepath != NULL)
	    free(filepath);

	return 0;
}

int parse(char * request, char * filepath)
{
	int tokNum = 0;
	char * line = strtok(request, "\n");

	char* token = strtok(line, " ");
	while(token != NULL)
	{
		if(strcmp(token, "GET") != 0 && tokNum == 0)
		{
			printf("Olny GET is available!");
			return 1;
		}

		if(tokNum == 1)
		{
		    strcpy(filepath, token + sizeof(char));
		    break;
		}

		tokNum++;
		token = strtok(NULL, " ");
	}

	return 0;
}
