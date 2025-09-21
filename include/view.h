#ifndef VIEW_H
#define VIEW_H

void prepare_layouts (view * v){
    for (int i = 0; i < MAX_VIEWS; i++) {
        v->layout[i] = NULL;
        v->mime_type[i] = NULL;
    }
}

size_t f_read (string ptr, string name_file, int size_file) {
    FILE * fp = fopen(name_file, "rb");
    if (!fp) {
        perror("f_read()");
        return ERROR;
    }
    return fread(ptr, sizeof(char), size_file, fp);
}

off_t fsize(String name_file) {
    struct stat st;
    if (stat(name_file, &st) == 0) return st.st_size;
    fprintf(stderr, "fsize(): Cannot determine size of %s: %s\n", name_file, strerror(errno));
    return ERROR;
}

string itos (int integer){
    int len = snprintf(NULL, 0, "%d", integer);
    string str = malloc(len + 1);
    snprintf(str, len + 1, "%d", integer);
    return str;
}

string ftos (double floating){
    int len = snprintf(NULL, 0, "%.2f", floating);
    string str = malloc(len + 1);
    snprintf(str, len + 1, "%.2f", floating);
    return str;
}

string get_content_file (String rute_file) {
    off_t size_file = fsize(rute_file);
    if (size_file <= 0) return NULL;
    string content_file = (string)malloc(size_file + 1);
    if (!content_file) return NULL;
    f_read(content_file, rute_file, size_file);
    content_file[size_file] = '\0';
    return content_file;
}

#endif

