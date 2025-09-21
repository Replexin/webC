#ifndef WEBC_H
#define WEBC_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
//#include <sqlite3.h>
//#include <curl/curl.h>
#include <stdbool.h>

typedef char * string;
typedef const char * String;

#define BUFFER_SIZE 1096
char buffer[BUFFER_SIZE];

#define GET_RESPONSE() buffer
#define MAX_ROUTERS 1024
#define MAX_VIEWS 1024
#define MAX_STRING 1024
#define ERROR 500
#define OK 200
#define HTML "text/html"
#define PNG "image/png"
#define JPG "image/jpeg"
#define GIF "image/gif"
#define BMP "image/bmp"
#define WEBP "image/webp"
#define SVG "image/svg+xml"
#define ICON "image/x-icon"
#define JS "application/javascript"
#define XML "application/xml"
#define JSON "application/json"
#define MP4 "video/mp4"
#define WEBM "video/webm"
#define OGG "video/ogg"
#define AVI "video/x-msvideo"
#define MP3 "audio/mp3"
#define WAV "audio/wav"
#define PDF "application/pdf"
#define ZIP "application/zip"
#define TTF "font/ttf"
#define QUICKTIME "video/quicktime"
#define CSS "text/css"
#define DEFAULT_PORT 3000
#define DEFAULT_LISTEN 10
#define DEFAULT_BUFFER_FILE 6046
#define DEFAULT_URL "127.0.0.1"
#define ERROR_EMPTY_VIEW "<center><h1>View '%s' is empty. this return null</h1></center>"

typedef struct {
    int addrlen;
    int server_fd, new_socket, valread;
    int server_socket, client_socket, errors, is_error, port, listen, buffer_file;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    string routers[MAX_ROUTERS];
} controller;

typedef struct {
    string (*layout[MAX_VIEWS])();
    string mime_type[MAX_VIEWS];
} view;

typedef struct {
} model;

void string_cat (string result, String format, ...){
    va_list args;
    va_start(args, format);
    va_list args_copy;
    va_copy(args_copy, args);
    int length = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    vsnprintf(result, length + 1, format, args);
    va_end(args);
}

#endif

