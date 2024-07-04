// Creación y manejo de un servidor usando paths en C (SERVIDOR SECUENCIAL)
// Para convertirlo en un servidor con Threats o con forks, remplazo el paso 7.

#include <stdio.h>

// BEGIN TODO Paso1:
// Incluir las librerías necesarias para sockets
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "request_handler.h"
// END TODO Paso1

// BEGIN TODO Paso2:
// Incluir las librerías necesarias para sockets
#include "shared.h"

#define PORT 8080
#define BUFFER_SIZE 1024
// END TODO Paso2

void handle_request(int new_socket) {

    char buffer[BUFFER_SIZE] = {0};
    read(new_socket, buffer, BUFFER_SIZE);

    // Extract the request path
    char method[BUFFER_SIZE];
    char path[BUFFER_SIZE];
    sscanf(buffer, "%s %s", method, path);
    printf("Method: %s\n", method);
    printf("Path: %s\n", path);

    // Determine the response based on the path
    const char *html_content;
    if (strcmp(path, "/") == 0) {
        html_content = "<html><body><h1>Home</h1></body></html>";
    } else if (strcmp(path, "/about") == 0) {
        html_content = "<html><body><h1>About</h1></body></html>";
    } else {
        html_content = "<html><body><h1>Not Found</h1></body></html>";
    }

    // Calculate the content length
    int content_length = strlen(html_content);

    // Create the response
    char response[BUFFER_SIZE];
    const char *mimeType = "text/html";
    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\n"
             "Content-Type: %s\n"
             "Content-Length: %d\n"
             "\n"
             "%s",
             mimeType, content_length, html_content);

    // Send the response
    printf(response);
    write(new_socket, response, strlen(response));
}

void requestHandler() {

    // BEGIN TODO Paso3:
    // Crear el socket

    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error("socket failed");
    }
    // END TODO Paso3

    // BEGIN TODO Paso4:
    // Asociar el socket a un puerto e IP
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        error("setsockopt");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        error("bind failed");
    }
    // END TODO Paso4

    // BEGIN TODO Paso5:
    // Escuchar por conexiones entrantes

    if (listen(server_fd, 3) < 0) {
        error("listen");
    }

    printf("Server listening on port %d\n", PORT);
    // END TODO Paso5

    while (1) {
    // BEGIN TODO Paso6:
    // Aceptar conexiones entrantes

    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        error("accept");
    }
    printf("Got a connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    // END TODO Paso6

    // BEGIN TODO Paso7:
    handle_request(new_socket);
    // END TODO Paso7

    // BEGIN TODO Paso8:
    close(new_socket);
    // END TODO Paso8
    }
}
