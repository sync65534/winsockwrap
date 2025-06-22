# winsockwrap
wrapper for winsock2.h focused om reducing manual error checking and improving readability

Nowhere near done, will add linux socket functionality in a seperate repo

example usage:
without winsockwrap:
```cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
int main(){

    // Standard WSA bullshit
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) { 
        printf("Failed to initialize Winsock: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    int serverSck = socket(AF_INET, SOCK_STREAM, 0);    // Used to create a TCP socket, like the gateway to be used 
    if (serverSck == INVALID_SOCKET){   // Socket errchk
        std::cerr << "socket fucking failed:\t" << WSAGetLastError();
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    sockaddr_in serverAddr;  // Established a refrence for a connection made by anyone, on TCP port 8080
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSck, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0){    // Binds our refrence to the previous TCP gateway, with encorperated error checking
        std::cerr << "bind fucking failed\t" << WSAGetLastError();
        WSACleanup();
        exit(EXIT_FAILURE);
    };

    if (listen(serverSck, 5) != 0){ // Listens for incoming connections with a max pending connection length of 5
        std::cerr << "listen fucking failed\t" << WSAGetLastError();
        WSACleanup();
        exit(EXIT_FAILURE);
    }  

    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr)  ;
    int clientSck = accept(serverSck, (sockaddr*)&clientAddr, &clientAddrLen);  // Accepts the first connection, fills in clientaddr and clientaddrlen
    if (clientSck == INVALID_SOCKET){
        std::cerr << "accept fucking failed\t" << WSAGetLastError();
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    int recvLen = recv(clientSck, buffer, sizeof(buffer) - 1, 0);
    if (recvLen == SOCKET_ERROR){
        std::cerr << "recv fucking failed\t" << WSAGetLastError();
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    buffer[recvLen] = '\0'; // Null-terminate safely


    printf("Address:\t%s\nPort:\t%d\nMessage:\t%s\n",
    inet_ntoa(clientAddr.sin_addr),
    ntohs(clientAddr.sin_port),
    buffer
    );
}
```

WITH winsockwrap
```cpp
#include <iostream>
#include <winsockwrap.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main(){
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) { 
        printf("Failed to initialize Winsock: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    sckman server;
    server.Ssocket(AF_INET, SOCK_STREAM, 0);
    server.addr.sin_family = AF_INET;
    server.addr.sin_port = htons(8080);
    server.addr.sin_addr.s_addr = INADDR_ANY;
    server.addrlen = sizeof(server.addr);
    server.Sbind();

    printf("Established socket\nHost\t%s\nPort\t%d\n",
    inet_ntoa(server.addr.sin_addr),
    ntohs(server.addr.sin_port)
    );
    server.Slisten(5);

    sckman client;
    client.fatal_on_error = true;
    client.socket_fd = server.Saccept((sockaddr*)&client.addr, &client.addrlen);
    client.addr = client.addr;
    client.addrlen = client.addrlen;
    char buf[1024];
    int recvLen = client.Srecv(buf, sizeof(buf), 0);

    buf[recvLen] = '\0'; // Null-terminate safely
    printf("Addr:\t%s\nPort:\t%d\nMsg:\t%s\n",
    inet_ntoa(client.addr.sin_addr),
    ntohs(client.addr.sin_port),
    buf
    );
}
```

