# webC
Simple libreria para crear servidores webs en C de forma muy sencilla
# Todo es una pagina
Si exactamente todo, si tu pagina muestra una imagen esa es una pagina
si muestra codigo js, css, video, etc. Todo ello necesita de una 'pagina'
que no son más que funciones que devuelven la vista al navegador ¿Ventaja? que no es necesario crear una función por cada recurso de imagenes o videos con cada vista apunten a la misma función el mime y el controller se encargara del resto.
## MVC
Esta libreria es un proyecto simple y con el objetivo de experimentar y tambien con el objetivo de crear una libreria en C para crear servidores webs con el modelo MVC(Modelo, Vista, Controlador)
inspirado de frameworks como rails de ruby.
De momento solo se esta trabajando en el C y V el modelo ira con el tiempo.
Actualmente faltan agregar muchas cosas lo principal ya esta se puede crear servidores con la struct controller y se puede enviar la respuesta de las routers con las views.
## Controller
Esta struct aparte de tener las variables para la creación del servidor web
tambien tiene el arreglo routers donde se define las rutas a trabajar
```
//creación de un servidor
controller con;
con.port = DEFAULT_PORT;
con.listen = DEFAULT_LISTEN;
prepare_routers(&con);//Se establece en NULL
con.routers[0] = "GET /";
create_server(&con);
while(true) {
    listen_server(&con);
    Controller(&con, /*&view*/);
}
```
¿Por qué antes de agrgar la ruta se establece en NULL?
Esto es simplemente para evitar errores con valores no inicializados y solo usar hasta los que estan inicializados.
Routers solo trabaja con las cabeceras de la respuesta: GET, POST...
## View
La view como el mismo nombre lo indica
es la vista lo que se devuelve como
respuesta a una petición de una ruta.
```
//función que devuelve la vista de tipo string
string index () {
    return get_content_file("layouts/index.html");
}
//crear la view
view views;
prepare_layouts(&views);//lo mismo que controller
views.layout[0] = index;//se establece la referecia a la función index que se ejecutara cada vez se aceda a su ruta
views.mime_type[0] = HTML;//se establece el mime de la respuesta
//
//
//en la función Controller
Controller(/*&control*/, &views);
```
## Model
Nada aun...
Hay un ejemplo de un servidor sencillo en el archivo *main.c*
en el repositorio.
## Funciónes para manipular la respuesta
Aun no.
## CHTML
# Sistema de Plantillas CHTML

Sistema minimalista de plantillas para C con escape automático, condicionales, includes y formateo. Los valores son el indice de el arreglo pasado.
## Características

### 1. Sustitución básica
```
<# 0 #>  - Muestra el valor del índice 0 con escape HTML automático
<#! 0 #> - Muestra el valor sin escape
```

### 2. Condicionales
```
<# if 0 #>
    Contenido que se muestra si el valor 0 es verdadero
<# endif #>
```

Valores verdaderos:
- Strings no vacíos y diferentes de "0"
- Enteros diferentes de 0
- Floats diferentes de 0.0
- Dates no nulas

### 3.Se puede incluir codigo html desde otro lado
```
<# include header.html #>
<# include footer.html #>
```

Los includes son relativos al directorio de la plantilla principal.

### 4. Formateo
```
<# 0 | precie #>    - Formatea como $123.45
<# 0 | date #>     - Formatea DATE como DD/MM/YYYY
<# 0 | hour #>      - Formatea hora como HH:MM:SS
<# 0 | full_time #>  - Formatea DATE y hora completas
```

## Tipos de valores

```
typedef enum {
    TYPE_STRING,  // char*
    TYPE_INT,     // int*
    TYPE_FLOAT,   // float*
    TYPE_DATE    // time_t*
} TypeValue;

typedef struct {
    void * value;
    TypeValue type;
} Template;
```

## Uso

```
//En la función callback
string ejemplo () {
    int edad = 25;
    float precio = 1599.99;
    time_t ahora = time(NULL);

    Template t[] = {
        {.value = "Juan Pérez", .type = TYPE_STRING},
        {.value = &edad, .type = TYPE_INT},
        {.value = "Activo", .type = TYPE_STRING},
        {.value = &precio, .type = TYPE_FLOAT},
        {.value = &ahora, .type = TYPE_DATE},
        {.value = "<script>alert('Hola mundo')</script>", .type = TYPE_STRING},
        {.value = "1", .type = TYPE_STRING},
        {.value = "0", .type = TYPE_STRING}
    };
    char * r = chtml("layouts/plantilla.html", t);
    if (r) {
        return r;
    }
    return NULL;
}
```
En plantilla.html
```
<!DOCTYPE html>
<html>
<head>
    <title>Perfil de <# 0 #></title>
</head>
<body>
    <# include header.html #>

    <h1>Informacion del Usuario</h1>

    <p>Nombre: <# 0 #></p>
    <p>Edad: <# 1 #> años</p>
    <p>Estado: <# 2 #></p>
    <p>Precio del producto: <# 3 | precie #></p>
    <p>Fecha de registro: <# 4 | date #></p>
    <p>Hora exacta: <# 4 | hour #></p>
    <p>Fecha completa: <# 4 | full_time #></p>

    <h2>Escape HTML automatico</h2>
    <p>Texto escapado: <# 5 #></p>
    <p>Texto sin escapar: <#! 5 #></p>

    <h2>Condicionales</h2>

    <# if 6 #>
    <div class="activo">
        <p>El usuario está activo y verificado</p>
    </div>
    <# endif #>

    <# if 7 #>
    <div class="inactivo">
        <p>Este mensaje no se mostrara porque el valor es "0"</p>
    </div>
    <# endif #>

    <# include footer.html #>
</body>
</html>
```

## Seguridad basica
- Escape HTML automático por defecto previene XSS
- Solo usa <#! #> cuando quieras insertar html desde el servidor
- Los condicionales no ejecutan código arbitrario
- Los includes solo permiten archivos, no URLs
## Compilar
Esta libreria esta hecha simplemente con un fichero C, 
sus cabeceras y una linea de comando(por ahora) **clang main.c -o webc**
