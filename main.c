#include "include/webc.h"
#include "include/controller.h"
#include "include/view.h"
#include <stdio.h>

/* funciones encargadas de renderizar lo que
 * el usuario ve en el navegador */

string index () {
    return get_content_file("layouts/index.html");
}

string login () {
    return get_content_file("layouts/login.html");
}

string imagenes () {
    return NULL;
}

string post_login () {
    string error = "error";
    string message = "No hay datos en el formulario";
    string response_json = get_content_file("layouts/login.json");
    if (isset("POST")) {
        error = "success";
        message = "Bienvenido Juaneto";
        string user = get_of_response("username\":\"", '"');
        string pass = get_of_response("password\":\"", '"');
        if (user == NULL) {
            error = "error";
            message = "No se encontro ningun usuario";
        } else if (pass == NULL) {
            error = "error";
            message = "No se encontro ninguna contraceña";
        } else if (strcmp(user, "Juaneto") != 0) {
            error = "error";
            message = "Nombre de usuario incorrecto";
        } else if (strcmp(pass, "12345678") != 0) {
            error = "error";
            message = "Contraceña incorrecta";
        }
    }
    string total_response = (string)malloc(strlen(response_json) + 1);
    string_cat(total_response, response_json, error, message);
    return total_response;
}

int main () {
    controller control;//controlador de las rutas
    view views;//controlador de la vista de las rutas
    control.port = DEFAULT_PORT;//3000
    control.listen = DEFAULT_LISTEN;//10
    prepare_routers(&control);//establece las routers en NULL
    prepare_layouts(&views);//establece layout y mime_type en NULL
    control.routers[0] = "GET /";
    control.routers[1] = "GET /login";
    control.routers[2] = "POST /login";
    control.routers[3] = "GET /assets/test.gif";
    control.routers[4] = "GET /favicon.png";
    views.layout[0] = index;
    views.layout[1] = login;
    views.layout[2] = post_login;
    views.layout[3] = imagenes;
    views.layout[4] = imagenes;//las imagenes o archivos apuntan a una misma funcion, esto con el fin de evitar que no se envie la respuesta
    views.mime_type[0] = HTML;
    views.mime_type[1] = HTML;
    views.mime_type[2] = JSON;
    views.mime_type[3] = GIF;
    views.mime_type[4] = PNG;
    create_server(&control);
    while(true) {
        listen_server(&control);
        Controller(&control, &views);//manejo de las peticiones
        puts(GET_RESPONSE());
    }
    return 0;
}

