//responser.h
#ifndef RESPONSER
#define RESPONSER
enum StatusCode {NotFound = 404, OK = 200, BadRequest = 400, NotImplemented = 501};

enum ContentType {Text = 10, HTML = 11, ImageJPEG = 12, ImagePNG = 13, Unknown = 0};

static void createResponseFAIL(enum StatusCode code, char* message);
static void createResponseOK(char * filepath);
int sendResponse(int * socket, enum StatusCode status, char * filepath);

static const char* getContentType(char* filepath);
static const char* getStrFromType(enum ContentType type);
static void * getFileContent(char* filepath, enum ContentType type);
#endif
