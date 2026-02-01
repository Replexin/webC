#ifndef VIEW_H
#define VIEW_H

void prepare_layouts (view * v){
    for (int i = 0; i < MAX_VIEWS; i++) {                 v->layout[i] = NULL;
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

static void buffer_inicializar(Buffer* buf, size_t capacidad_inicial) {
    buf->datas = malloc(capacidad_inicial);
    buf->size = capacidad_inicial;
    buf->len = 0;
    buf->datas[0] = '\0';
}

static void buffer_agregar(Buffer * buf, const char * texto, size_t len) {
    while (buf->len + len >= buf->size) {
        buf->size *= 2;
        buf->datas = realloc(buf->datas, buf->size);
    }
    memcpy(buf->datas + buf->len, texto, len);
    buf->len += len;
    buf->datas[buf->len] = '\0';
}

static void buffer_agregar_char(Buffer * buf, char c) {
    buffer_agregar(buf, &c, 1);
}

static char * escape_html(const char * texto) {
    if (!texto) return strdup("");

    size_t len = strlen(texto);
    size_t capacidad = len * 2;
    char * resultado = malloc(capacidad);
    size_t pos = 0;

    for (size_t i = 0; i < len; i++) {
        const char * reemplazo = NULL;
        size_t tam_reemplazo = 0;

        switch (texto[i]) {
            case '<':
                reemplazo = "&lt;";
                tam_reemplazo = 4;
                break;
            case '>':
                reemplazo = "&gt;";
                tam_reemplazo = 4;
                break;
            case '&':
                reemplazo = "&amp;";
                tam_reemplazo = 5;
                break;
            case '"':
                reemplazo = "&quot;";
                tam_reemplazo = 6;
                break;
            case '\'':
                reemplazo = "&#39;";
                tam_reemplazo = 5;
                break;
            default:
                if (pos >= capacidad - 1) {
                    capacidad *= 2;
                    resultado = realloc(resultado, capacidad);
                }
                resultado[pos++] = texto[i];
                continue;
        }

        while (pos + tam_reemplazo >= capacidad) {
            capacidad *= 2;
            resultado = realloc(resultado, capacidad);
        }
        memcpy(resultado + pos, reemplazo, tam_reemplazo);
        pos += tam_reemplazo;
    }

    resultado[pos] = '\0';
    return resultado;
}

static char * leer_archivo(const char * archivo) {
    FILE* fp = fopen(archivo, "r");
    if (!fp) {
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long tamano = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char * contenido = malloc(tamano + 1);
    fread(contenido, 1, tamano, fp);
    contenido[tamano] = '\0';
    fclose(fp);

    return contenido;
}

static char * formatear_valor(Template * valor, const char * formato) {
    char buffer[256];

    if (formato && strcmp(formato, "precie") == 0) {
        if (valor->type == TYPE_FLOAT) {
            snprintf(buffer, sizeof(buffer), "$%.2f", *(float*)valor->value);
        } else if (valor->type == TYPE_INT) {
            snprintf(buffer, sizeof(buffer), "$%d.00", *(int*)valor->value);
        } else {
            return strdup("");
        }
        return strdup(buffer);
    }

    if (formato && strcmp(formato, "date") == 0) {
        if (valor->type == TYPE_DATE) {
            time_t * t = (time_t *)valor->value;
            struct tm * tm_info = localtime(t);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y", tm_info);
            return strdup(buffer);
        }
        return strdup("");
    }

    if (formato && strcmp(formato, "hour") == 0) {
        if (valor->type == TYPE_DATE) {
            time_t * t = (time_t *)valor->value;
            struct tm * tm_info = localtime(t);
            strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);
            return strdup(buffer);
        }
        return strdup("");
    }

    if (formato && strcmp(formato, "full_time") == 0) {
        if (valor->type == TYPE_DATE) {
            time_t * t = (time_t *)valor->value;
            struct tm * tm_info = localtime(t);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);
            return strdup(buffer);
        }
        return strdup("");
    }

    switch (valor->type) {
        case TYPE_STRING:
            return strdup((char *)valor->value);
        case TYPE_INT:
            snprintf(buffer, sizeof(buffer), "%d", *(int*)valor->value);
            return strdup(buffer);
        case TYPE_FLOAT:
            snprintf(buffer, sizeof(buffer), "%.2f", *(float*)valor->value);
            return strdup(buffer);
        case TYPE_DATE:
            {
                time_t * t = (time_t*)valor->value;
                struct tm * tm_info = localtime(t);
                strftime(buffer, sizeof(buffer), "%d/%m/%Y", tm_info);
                return strdup(buffer);
            }
        default:
            return strdup("");
    }
}

static int es_verdadero(Template * valor) {
    switch (valor->type) {
        case TYPE_STRING:
            {
                char * str = (char*)valor->value;
                return str && strlen(str) > 0 && strcmp(str, "0") != 0;
            }
        case TYPE_INT:
            return *(int*)valor->value != 0;
        case TYPE_FLOAT:
            return *(float*)valor->value != 0.0f;
        case TYPE_DATE:
            return valor->value != NULL;
        default:
            return 0;
    }
}

static char * procesar_plantilla(const char * contenido, Template * valores, const char * ruta_base);

static char * procesar_include(const char* archivo, Template * valores, const char* ruta_base) {
    char ruta_completa[512];

    if (ruta_base && archivo[0] != '/') {
        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", ruta_base, archivo);
    } else {
        snprintf(ruta_completa, sizeof(ruta_completa), "%s", archivo);
    }

    char * contenido_include = leer_archivo(ruta_completa);
    if (!contenido_include) {
        return strdup("");
    }

    char * resultado = procesar_plantilla(contenido_include, valores, ruta_base);
    free(contenido_include);

    return resultado;
}

static char * procesar_plantilla(const char * contenido, Template * valores, const char * ruta_base) {
    Buffer resultado;
    buffer_inicializar(&resultado, strlen(contenido) * 2);

    char* cursor = (char*)contenido;
    int nivel_if = 0;
    int saltar_hasta_endif = 0;

    while (*cursor) {
        if (strncmp(cursor, "<#", 2) == 0) {
            char* inicio = cursor + 2;
            while (*inicio == ' ') inicio++;

            char* fin = strstr(inicio, "#>");
            if (!fin) {
                buffer_agregar_char(&resultado, *cursor++);
                continue;
            }

            size_t longitud_comando = fin - inicio;
            char comando[256];
            if (longitud_comando >= sizeof(comando)) {
                cursor = fin + 2;
                continue;
            }

            strncpy(comando, inicio, longitud_comando);
            comando[longitud_comando] = '\0';

            char* fin_comando = comando + strlen(comando) - 1;
            while (fin_comando > comando && *fin_comando == ' ') {
                *fin_comando-- = '\0';
            }

            if (strncmp(comando, "if ", 3) == 0) {
                nivel_if++;

                if (!saltar_hasta_endif) {
                    int indice = atoi(comando + 3);
                    if (!es_verdadero(&valores[indice])) {
                        saltar_hasta_endif = nivel_if;
                    }
                }

                cursor = fin + 2;
                continue;
            }

            if (strcmp(comando, "endif") == 0) {
                if (saltar_hasta_endif == nivel_if) {
                    saltar_hasta_endif = 0;
                }
                nivel_if--;
                cursor = fin + 2;
                continue;
            }

            if (saltar_hasta_endif) {
                cursor = fin + 2;
                continue;
            }

            if (strncmp(comando, "include ", 8) == 0) {
                char * nombre_archivo = comando + 8;
                while (*nombre_archivo == ' ') nombre_archivo++;

                char * incluido = procesar_include(nombre_archivo, valores, ruta_base);
                buffer_agregar(&resultado, incluido, strlen(incluido));
                free(incluido);

                cursor = fin + 2;
                continue;
            }

            int sin_escape = 0;
            if (*inicio == '!') {
                sin_escape = 1;
                inicio++;
                while (*inicio == ' ') inicio++;
            }

            char* pipe = strchr(comando, '|');
            char* formato = NULL;
            int indice;

            if (pipe) {
                *pipe = '\0';
                formato = pipe + 1;
                while (*formato == ' ') formato++;
                char* fin_formato = formato + strlen(formato) - 1;
                while (fin_formato > formato && *fin_formato == ' ') {
                    *fin_formato-- = '\0';
                }
                indice = atoi(comando + (sin_escape ? 1 : 0));
            } else {
                indice = atoi(inicio);
            }

            char* valor_formateado = formatear_valor(&valores[indice], formato);

            if (sin_escape) {
                buffer_agregar(&resultado, valor_formateado, strlen(valor_formateado));
                free(valor_formateado);
            } else {
                char* valor_escapado = escape_html(valor_formateado);
                buffer_agregar(&resultado, valor_escapado, strlen(valor_escapado));
                free(valor_formateado);
                free(valor_escapado);
            }
            cursor = fin + 2;
            continue;
        }
        if (!saltar_hasta_endif) {
            buffer_agregar_char(&resultado, *cursor);
        }
        cursor++;
    }
    return resultado.datas;
}

char * chtml(const char * archivo, Template * valores) {
    char* contenido = leer_archivo(archivo);
    if (!contenido) {
        return NULL;
    }

    char* directorio = strdup(archivo);
    char* ultima_barra = strrchr(directorio, '/');
    if (ultima_barra) {
        *ultima_barra = '\0';
    } else {
        free(directorio);
        directorio = strdup(".");
    }
    char* resultado = procesar_plantilla(contenido, valores, directorio);
    free(contenido);
    free(directorio);
    return resultado;
}

void chtml_free(char* html) {
    free(html);
}

#endif