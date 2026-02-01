#include <iostream>
#include <vector>
#include <string>
#include <cstring> //strlen, memcpy, memset

#include <sys/socket.h> //funciones socket (bind, listen etc)
#include <netinet/in.h> //estructura sockaddr_in
#include <arpa/inet.h>  //conversion de direcciones inet_addr
#include <unistd.h>     //funcion POSIX close()


#include "util.h"
#include "protocol.h"

using namespace std;

const int PORT = 8081; //UDP
const int BUFFER_SIZE = 1024;

int main()
{
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;

    int a = 5;
    int b = 3;
    int resultado = sumar(a, b);
    
    std::cout << "El resultado de " << a << " + " << b << " es: " << resultado << std::endl;


    //crear socket del servidor
    int server_fd = 0; //file descriptor del socket
    char buffer[BUFFER_SIZE] = {0};
    SensorData sensorData;
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    //AF_INET -> IPv4, SOCK_DGRAM -> UDP, 0 -> protocolo por defecto (UDP para SOCK_DGRAM)
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { // == -1
        perror("Fallo en la creacion del socket");
        return 1;
    }

    // Opcional: Reutilizar el puerto inmediatamente después de cerrar
    int opt = 1; //para opciones del socket discretas (e.g. SO_REUSEADDR o SO_REUSEPORT), se pone 1 o 0 para activar o desactivar
    // SOL_SOCKET especificar opciones generales del socket
    // Otras opciones son:
        // IPPROTO_TCP para opciones específicas de TCP
        // IPPROTO_IP para opciones específicas de IP
        // IPPROTO_UDP para opciones específicas de UDP
        // IPPROTO_IPV6 para opciones específicas de IPv6
        // IPPROTO_RAW para opciones específicas de RAW
        // AF_UNIX para opciones específicas de sockets Unix Domain
        // AF_PACKET para opciones específicas de sockets de bajo nivel (ethernet frames)
    // SO_REUSEADDR permite reutilizar la dirección local inmediatamente después de cerrar el socket
    // SO_REUSEPORT permite que múltiples sockets escuchen en el mismo puerto (si el sistema lo soporta)

    //ejemplo especificar timeout para recv o send:
    //struct timeval tv = {.tv_sec = 5, .tv_usec = 0}; //5 segundos
    //setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    //ejemplo especificar tamaño del buffer de recepción:
    //int rcvbuf_size = 4 * 1024; //4KB
    //setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, sizeof(rcvbuf_size));

    //Obtener opciones del socker:
    //int current_rcvbuf_size;
    //socklen_t optlen = sizeof(current_rcvbuf_size);
    //getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &current_rcvbuf_size, &optlen);
    //si especificamos un tamaño de buffer, al obtener las opciones del socket puede que sea mayor por el overhead que añade el sistema operativo

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
    }

    // 2. Definir la dirección del servidor (IP y Puerto)
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Escucha en todas las interfaces (incl. 127.0.0.1)
    server_addr.sin_port = htons(PORT);

    // 3. Enlazar el socket a la dirección y puerto
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Fallo en el bind");
        close(server_fd);
        return 1;
    }

    std::cout << "Servidor UDP escuchando en el puerto " << PORT << "...\n";

    // 4. Recibir mensajes de los clientes
    socklen_t client_len = sizeof(client_addr); 

    int n = recvfrom(server_fd, (char *)buffer, BUFFER_SIZE - 1, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_len);
    if (n < 0) {
        perror("Fallo en recvfrom");
    }
    else if (n == 30) { //tamaño del primer mensaje
        buffer[n] = '\0'; // Asegurar que el buffer está null-terminated
        std::cout << "Mensaje recibido de " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " - " << buffer << "\n";
    }
    else {
        memcpy(&sensorData, buffer, sizeof(sensorData));
        std::cout << "2do Mensaje recibido de tamaño " << n << "\n";
    }
    
    
    n = recvfrom(server_fd, (char *)buffer, BUFFER_SIZE - 1, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_len);
    if (n < 0) {
        perror("Fallo en recvfrom");
    }   
    else if (n == 30) {
        buffer[n] = '\0'; // Asegurar que el buffer está null-terminated
        std::cout << "Mensaje recibido de " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " - " << buffer << "\n";
    }
    else {
        memcpy(&sensorData, buffer, sizeof(sensorData));
        std::cout << "2do Mensaje recibido de tamaño " << n << "\n";
    }
    // 5. Enviar respuesta al cliente

    const char *hello = "Hola Cliente, UDP - Mensaje Recibido!";
    sendto(server_fd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&client_addr, client_len);
    std::cout << "Mensaje de respuesta enviado.\n";

    // 8. Cerrar sockets
    close(server_fd);





    return 0;
}