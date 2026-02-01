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


const int PORT = 8081;
const char* SERVER_IP = "127.0.0.1";
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

    
    int sockfd = 0;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); // Inicializar a cero
    //definir la direccion del servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    const char *message = "Hello server, I am UDP client.";
    //uint8_t cmd[] = {0xAA, 0x02, 0x03, 0x04}; si en lugar de string tuviesemos que mandar bytes
    char buffer[BUFFER_SIZE] = {0};

    SensorData sensorData;
    uint32_t id_local = 1;
    uint16_t type_local = 2;
    uint32_t value_local = 12345;
    sensorData.sensorId = htonl(id_local); // Convertir a orden de bytes de red
    sensorData.type = htons(type_local);   // Convertir a orden de bytes de red
    sensorData.value = htonl(value_local); // Convertir a orden de bytes de red

    char buffer_send[sizeof(SensorData)];
    memcpy(buffer_send, &sensorData, sizeof(SensorData));


    //crear socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cout << "Error al crear el socket UDP" << std::endl;
        //perror("UDP Socket creation error");
        return 1;
    }


    // No hay connect en UDP.
    

    // Enviar mensaje al servidor
    if (sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    {
        perror("Fallo al enviar el mensaje");
    }
    std::cout << "Mensaje enviado al servidor: " << message << std::endl;

    // Enviar 2do mensaje al servidor
    if (sendto(sockfd, (const char *)buffer_send, sizeof(SensorData), MSG_CONFIRM, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    {
        perror("Fallo al enviar el 2do mensaje");
    }
    std::cout << "2do Mensaje enviado al servidor: " << buffer_send[0] << std::endl;


    // Recibir respuesta del servidor
    socklen_t len = sizeof(serv_addr);
    int valread = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE -1, MSG_WAITALL, (struct sockaddr *) &serv_addr, &len);
    
    //int valread = read(sock, buffer, BUFFER_SIZE);
    if (valread < 0)
    {
        perror("Fallo al recibir la respuesta");
    } else {
        buffer[valread] = '\0'; // Asegurar que el buffer está null-terminated
        std::cout << "Mensaje recibido del servidor: " << buffer << std::endl;
    }
    

    // Cerrar el socket
    close(sockfd);


    return 0;
}