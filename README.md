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
