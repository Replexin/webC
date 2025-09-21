#ifndef CONTROLLER_H
#define CONTROLLER_H

int create_server (controller * server){
    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_socket < 0){
        perror("create_server()");
        return ERROR;
    }
    server->server_addr.sin_family = AF_INET;
    server->server_addr.sin_addr.s_addr = INADDR_ANY;
    server->server_addr.sin_port = htons(server->port);
    if (bind(server->server_socket, (struct sockaddr *)&server->server_addr, sizeof(server->server_addr)) < 0){
        perror("create_server()");
        close(server->client_socket);
        return ERROR;
    }
    if (listen(server->server_socket, server->listen) < 0){
        perror("create_server()");
        close(server->client_socket);
        return ERROR;
    }
    return OK;
}

int listen_server (controller * server){
    server->client_addr_len = sizeof(server->client_addr);
    server->client_socket = accept(server->server_socket, (struct sockaddr *)&server->client_addr, &server->client_addr_len);
    if (server->client_socket < 0){
        perror("listen_server()");
        close(server->client_socket);
        return ERROR;
    }
    return OK;
}

void send_view(int client_socket, const char *header, const char *content_type, const char *content, size_t content_length){
    char response[BUFFER_SIZE];
    int response_length = snprintf(response, sizeof(response),
                                   "%s\r\n"
                                   "Content-Type: %s\r\n"
                                   "Content-Length: %zu\r\n"
                                   "Connection: close\r\n"
                                   "\r\n",
                                   header, content_type, content_length);
    write(client_socket, response, response_length);
    write(client_socket, content, content_length);
}


void send_file(int client_socket, const char * header, const char * content_type, const char * file_path){
    FILE *file = fopen(file_path, "rb");
    if (file == NULL){
        perror("send_file()");
        return;
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_content = malloc(file_size);
    if (file_content == NULL){
        perror("send_file()");
        fclose(file);
        return;
    }
    fread(file_content, 1, file_size, file);
    fclose(file);
    send_view(client_socket, header, content_type, file_content, file_size);
    free(file_content);
}

void prepare_routers (controller * c) {
    for (int i = 0; i < MAX_ROUTERS; i++) c->routers[i] = NULL;
}

string search_in_text (String text, String word, char limit){
    string getting = strstr(text, word);
    if (getting != NULL){
        size_t finalPosition = getting - text + strlen(word);
        String limitChar = strchr(text + finalPosition, limit);
        if (limitChar != NULL){
            size_t lon = limitChar - (text + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, text + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    }
    else return NULL;
}

int Controller (controller * c, view * v) {
    int bytes_read;
    //string error404 = "<center><h1>El recurso solicitado no ha sido encontrado.</h1></center>";
    bytes_read = read(c->client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0){
        perror("Controller()");
        close(c->client_socket);
        return ERROR;
    }
    buffer[bytes_read] = '\0';
    for (int i = 0; i < MAX_ROUTERS; i++) {
        if (c->routers[i] == NULL || v->layout[i] == NULL || v->mime_type[i] == NULL) return OK;
        char router[MAX_STRING];
        snprintf(router, MAX_STRING, "%s HTTP/1.1", c->routers[i]);
        if (strncmp(buffer, router, strlen(router)) == 0) {
            if (strcmp(v->mime_type[i], PNG) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", PNG, path);
                free(path);
            } else if (strcmp(v->mime_type[i], GIF) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", GIF, path);
                free(path);
            } else if (strcmp(v->mime_type[i], MP4) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", MP4, path);
                free(path);
            } else if (strcmp(v->mime_type[i], JPG) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", JPG, path);
                free(path);
            } else if (strcmp(v->mime_type[i], BMP) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", BMP, path);
                free(path);
            } else if (strcmp(v->mime_type[i], WEBP) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", WEBP, path);
                free(path);
            } else if (strcmp(v->mime_type[i], SVG) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", SVG, path);
                free(path);
            } else if (strcmp(v->mime_type[i], ICON) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", ICON, path);
                free(path);
            } else if (strcmp(v->mime_type[i], WEBM) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", WEBM, path);
                free(path);
            } else if (strcmp(v->mime_type[i], OGG) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", OGG, path);
                free(path);
            } else if (strcmp(v->mime_type[i], AVI) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", AVI, path);
                free(path);
            } else if (strcmp(v->mime_type[i], MP3) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", MP3, path);
                free(path);
            } else if (strcmp(v->mime_type[i], WAV) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", WAV, path);
                free(path);
            } else if (strcmp(v->mime_type[i], PDF) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", PDF, path);
                free(path);
            } else if (strcmp(v->mime_type[i], ZIP) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", ZIP, path);
                free(path);
            } else if (strcmp(v->mime_type[i], TTF) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", TTF, path);
                free(path);
            } else if (strcmp(v->mime_type[i], QUICKTIME) == 0) {
                string path = search_in_text(c->routers[i], "/", '\0');
                send_file(c->client_socket, "HTTP/1.1 200 OK", QUICKTIME, path);
                free(path);
            }  else {
                string view = v->layout[i]();
                if (!view) {
                    char view_error[200];
                    string_cat(view_error, ERROR_EMPTY_VIEW, c->routers[i]);
                    send_view(c->client_socket, "HTTP/1.1 403 Forbidden", HTML, view_error, strlen(view_error));
                    return ERROR;
                }
                send_view(c->client_socket, "HTTP/1.1 200 OK", v->mime_type[i], view, strlen(view));
                free(view);
            }
        } /*else {
            send_view(c->client_socket, "HTTP/1.1 404 Not Found", "text/html", error404, strlen(error404));
        }*/
    }
    close(c->client_socket);
    return OK;
}

string get_file (String file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL){
        perror("get_file()");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    string file_content = malloc(file_size);
    if (file_content == NULL){
        perror("get_file()");
        fclose(file);
        return NULL;
    }
    fread(file_content, 1, file_size, file);
    fclose(file);
    return file_content;
}

bool isset (String word_response){
    string local_response = GET_RESPONSE();
    string local_response_copy = strdup(word_response);
    if (local_response_copy == NULL) false;
    string line = strtok(local_response_copy, "\n");
    while (line != NULL) {
        if (!strstr(line, local_response)) {
            free(local_response_copy);
            return true;
        }
        line = strtok(NULL, "\n");
    }
    free(local_response_copy);
    return false;
}

string get_of_response (String search, char limit){
    string local_response = GET_RESPONSE();
    string geting = strstr(local_response, search);
    if (geting != NULL){
        size_t finalPosition = geting - local_response + strlen(search);
        String charLimit = strchr(local_response + finalPosition, limit);
        if (charLimit != NULL){
            size_t lon = charLimit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        } else return NULL;
    } else return NULL;
}

string host (){
    string word = "Host: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string user_agent (){
    string word = "User-Agent: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string accept_lenguage (){
    string word = "Accept-Language: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string accept_encoding (){
    string word = "Accept-Encoding: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string connection (){
    string word = "Connection: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string referer (){
    string word = "Referer: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string upgrade_insecure_requests (){
    string word = "Upgrade-Insecure-Requests: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string dest (){
    string word = "Sec-Fetch-Dest: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string mode (){
    string word = "Sec-Fetch-Mode: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string fetch_user (){
    string word = "Sec-Fetch-User: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string priority (){
    string word = "Priority: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string Accept (){
    string word = "Accept: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string fetch_site (){
    string word = "Sec-Fetch-Site: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string GET (){
    string search = "GET ";
    char limit = 'H';
    string local_response = GET_RESPONSE();
    string geting = strstr(local_response, search);
    if (geting != NULL){
        size_t finalPosition = geting - local_response + strlen(search);
        String charLimit = strchr(local_response + finalPosition, limit);
        if (charLimit != NULL){
            size_t lon = charLimit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        } else return NULL;
    } else return NULL;
}

string POST (){
    string search = "POST ";
    char limit = 'H';
    string local_response = GET_RESPONSE();
    string geting = strstr(local_response, search);
    if (geting != NULL){
        size_t finalPosition = geting - local_response + strlen(search);
        String charLimit = strchr(local_response + finalPosition, limit);
        if (charLimit != NULL){
            size_t lon = charLimit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        } else return NULL;
    } else return NULL;
}

string idempotency_key (){
    string word = "Idempotency-Key:";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string content_length (){
    string word = "Content-Length: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string origin (){
    string word = "Origin:";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string content_type (){
    string word = "Content-Type: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string user_plataform (){
    string word = "sec-ch-ua-platform: \"";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}


string pragma (){
    string word = "Pragma: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string cache_control (){
    string word = "Cache-Control: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string sec_ch_ua (){
    string word = "sec-ch-ua: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string requested_with (){
    string word = "X-Requested-With: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

string get_cookie (){
    string word = "Cookie: ";
    char limitChar = '\0';
    string local_response = GET_RESPONSE();
    string getting = strstr(local_response, word);
    if (getting != NULL){
        size_t finalPosition = getting - local_response + strlen(word);
        String limit = strchr(local_response + finalPosition, limitChar);
        if (limit != NULL){
            size_t lon = limit - (local_response + finalPosition);
            string substring = (string)malloc(lon + 1);
            strncpy(substring, local_response + finalPosition, lon);
            substring[lon] = '\0';
            return substring;
        }
        else return NULL;
    } else return NULL;
}

#endif

